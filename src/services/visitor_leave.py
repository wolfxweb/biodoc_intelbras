"""Baixas de visita obtidas exclusivamente pelo histórico oficial do Defense IA."""

from __future__ import annotations

import asyncio
import json
import time
from dataclasses import dataclass, replace
from datetime import datetime, timezone
from pathlib import Path
from typing import Any
from zoneinfo import ZoneInfo

import httpx

from src.core.logging import logger, visitor_leave_logger
from src.services.visitor_leave_store import (
    get_webhook_token,
    get_webhook_url,
    persist_visitor_leave,
)

try:
    _LOCAL_TZ = ZoneInfo("America/Sao_Paulo")
except Exception:  # pragma: no cover
    _LOCAL_TZ = timezone.utc

FINALIZED_VISITOR_STATUS = "2"
DEFAULT_POLL_INTERVAL_SECONDS = 60.0
# O histórico do Defense filtra startTime/endTime pela chegada da visita, não
# pelo leaveTime. A consulta automática usa 7 dias de chegada; só a chave
# persistida visitorId|leaveTime decide se a baixa é nova.
HISTORY_LOOKBACK_SECONDS = 7 * 86400
INITIAL_LOOKBACK_SECONDS = HISTORY_LOOKBACK_SECONDS
STATE_VERSION = 2
EMITTED_RETENTION_SECONDS = 7 * 86400
WEBHOOK_RETRY_DELAYS_SECONDS = (1.0, 3.0)


@dataclass(frozen=True)
class VisitLeaveSettings:
    webhook_url: str = ""
    webhook_token: str = ""
    timeout_seconds: float = 10.0
    poll_interval_seconds: float = DEFAULT_POLL_INTERVAL_SECONDS
    state_path: str = "data/visitor_leave_state.json"

    @property
    def forward_enabled(self) -> bool:
        return bool(self.webhook_url.strip())

    @property
    def poll_enabled(self) -> bool:
        return self.poll_interval_seconds > 0


@dataclass
class PollState:
    last_poll_ts: int
    emitted: dict[str, int]
    legacy: bool = False


def _as_text(value: Any) -> str | None:
    if value is None:
        return None
    text = str(value).strip()
    if not text or text.lower() in {"null", "undefined", "none"}:
        return None
    return text


def _valid_timestamp(value: Any) -> int | None:
    text = _as_text(value)
    if not text or not text.isdigit():
        return None
    timestamp = int(text)
    return timestamp if timestamp > 0 else None


def is_finalized_visitor(visitor: dict[str, Any]) -> bool:
    """Aceita apenas a baixa oficial: status=2 e leaveTime preenchido."""
    return (
        _as_text(visitor.get("status")) == FINALIZED_VISITOR_STATUS
        and _valid_timestamp(visitor.get("leaveTime")) is not None
    )


def _visitor_id(visitor: dict[str, Any]) -> str | None:
    return _as_text(visitor.get("visitorId") or visitor.get("id"))


def _dedup_key(visitor: dict[str, Any]) -> str | None:
    visitor_id = _visitor_id(visitor)
    leave_time = _valid_timestamp(visitor.get("leaveTime"))
    if not visitor_id or leave_time is None:
        return None
    return f"{visitor_id}|{leave_time}"


def build_leave_payload(visitor: dict[str, Any]) -> dict[str, Any]:
    auth = visitor.get("authInfo") if isinstance(visitor.get("authInfo"), dict) else {}
    return {
        "event": "visitor_leave",
        "loggedAt": datetime.now(_LOCAL_TZ).isoformat(timespec="seconds"),
        "visitorId": _visitor_id(visitor),
        "personId": _as_text(visitor.get("personId")),
        "status": _as_text(visitor.get("status")),
        "visitorName": _as_text(visitor.get("visitorName") or visitor.get("personName")),
        "idNum": _as_text(visitor.get("idNum") or visitor.get("idNo")),
        "cardNo": _as_text(auth.get("cardNo")),
        "remark": _as_text(visitor.get("remark")),
        "visitedName": _as_text(visitor.get("visitedName")),
        "visitedOrgName": _as_text(visitor.get("visitedOrgName")),
        "arrivalTime": _as_text(visitor.get("arrivalTime")),
        "expectLeaveTime": _as_text(visitor.get("expectLeaveTime")),
        "leaveTime": _as_text(visitor.get("leaveTime")),
        "channelId": None,
        "sourceName": None,
        "trigger": "poll",
        "source": "defense_ia",
    }


def log_visitor_leave(payload: dict[str, Any]) -> None:
    visitor_leave_logger.info(
        json.dumps(payload, ensure_ascii=False, separators=(",", ":"))
    )


def _default_poll_state(now: int) -> PollState:
    return PollState(last_poll_ts=now - INITIAL_LOOKBACK_SECONDS, emitted={})


def _load_poll_state(path: str, *, now: int | None = None) -> PollState:
    current = int(time.time()) if now is None else now
    state_file = Path(path)
    if not state_file.is_file():
        return _default_poll_state(current)
    try:
        data = json.loads(state_file.read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError):
        logger.warning("[VISIT_LEAVE] estado inválido; usando lookback de 24h")
        return _default_poll_state(current)
    if not isinstance(data, dict):
        return _default_poll_state(current)

    raw_last_poll = data.get("last_poll_ts")
    if not isinstance(raw_last_poll, (int, float)) or raw_last_poll <= 0:
        return _default_poll_state(current)

    raw_emitted = data.get("emitted")
    emitted: dict[str, int] = {}
    if isinstance(raw_emitted, dict):
        for key, timestamp in raw_emitted.items():
            if isinstance(key, str) and isinstance(timestamp, (int, float)):
                emitted[key] = int(timestamp)

    return PollState(
        last_poll_ts=int(raw_last_poll),
        emitted=emitted,
        legacy=data.get("version") != STATE_VERSION,
    )


def _prune_emitted(emitted: dict[str, int], *, now: int) -> dict[str, int]:
    cutoff = now - EMITTED_RETENTION_SECONDS
    return {key: timestamp for key, timestamp in emitted.items() if timestamp >= cutoff}


def _save_poll_state(path: str, state: PollState) -> None:
    state_file = Path(path)
    state_file.parent.mkdir(parents=True, exist_ok=True)
    temporary = state_file.with_name(f".{state_file.name}.tmp")
    body = {
        "version": STATE_VERSION,
        "last_poll_ts": state.last_poll_ts,
        "emitted": state.emitted,
    }
    temporary.write_text(
        json.dumps(body, ensure_ascii=False, separators=(",", ":")),
        encoding="utf-8",
    )
    temporary.replace(state_file)


async def forward_visitor_leave(
    payload: dict[str, Any],
    settings: VisitLeaveSettings,
) -> bool:
    """Envia somente payload novo; falhas transitórias recebem backoff curto."""
    if not settings.forward_enabled:
        return False

    headers = {"Content-Type": "application/json;charset=UTF-8"}
    token = settings.webhook_token.strip()
    if token:
        headers["Authorization"] = f"Bearer {token}"

    delays = (0.0, *WEBHOOK_RETRY_DELAYS_SECONDS)
    async with httpx.AsyncClient(timeout=settings.timeout_seconds) as client:
        for attempt, delay in enumerate(delays, start=1):
            if delay:
                await asyncio.sleep(delay)
            try:
                response = await client.post(
                    settings.webhook_url.strip(),
                    json=payload,
                    headers=headers,
                )
            except httpx.HTTPError as exc:
                if attempt < len(delays):
                    logger.warning(
                        "[VISIT_LEAVE] webhook falhou tentativa=%s: %s",
                        attempt,
                        exc,
                    )
                    continue
                logger.warning("[VISIT_LEAVE] webhook falhou definitivamente: %s", exc)
                return False

            if response.status_code < 300:
                logger.info("[VISIT_LEAVE] webhook ok HTTP %s", response.status_code)
                return True
            if response.status_code not in {429, 500, 502, 503, 504}:
                logger.warning(
                    "[VISIT_LEAVE] webhook HTTP %s body=%s",
                    response.status_code,
                    response.text[:300],
                )
                return False
            if attempt < len(delays):
                logger.warning(
                    "[VISIT_LEAVE] webhook HTTP %s; nova tentativa=%s",
                    response.status_code,
                    attempt + 1,
                )
                continue
            logger.warning(
                "[VISIT_LEAVE] webhook falhou após tentativas HTTP %s body=%s",
                response.status_code,
                response.text[:300],
            )
            return False
    return False


async def process_visitor_leave_body(
    body: Any,
    *,
    defense_client: Any,
    settings: VisitLeaveSettings,
) -> dict[str, Any]:
    """Compatibilidade da rota antiga: alarmes genéricos nunca geram baixa."""
    _ = body, defense_client, settings
    return {
        "status": "ignored",
        "processed": 0,
        "skipped": 1,
        "forwarded": 0,
    }


async def poll_visitor_leave_history(
    *,
    defense_client: Any,
    settings: VisitLeaveSettings,
) -> dict[str, Any]:
    if not defense_client.settings.enabled:
        return {"status": "disabled", "processed": 0, "skipped": 0, "forwarded": 0}

    now = int(time.time())
    state = _load_poll_state(settings.state_path, now=now)
    query_since = now - HISTORY_LOOKBACK_SECONDS

    try:
        visitors = await defense_client.iter_finalized_visitors_between(
            query_since,
            now + 86400,
        )
    except Exception as exc:
        logger.warning("[VISIT_LEAVE] poll falhou; watermark preservado: %s", exc)
        return {"status": "error", "processed": 0, "skipped": 0, "forwarded": 0}

    processed = 0
    skipped = 0
    forwarded = 0
    fetched = len(visitors)
    state.emitted = _prune_emitted(state.emitted, now=now)

    ordered_visitors = sorted(
        visitors,
        key=lambda item: _valid_timestamp(item.get("leaveTime")) or 0,
    )
    for visitor in ordered_visitors:
        key = _dedup_key(visitor)
        leave_time = _valid_timestamp(visitor.get("leaveTime"))
        if (
            not is_finalized_visitor(visitor)
            or key is None
            or leave_time is None
            or leave_time < query_since
            or key in state.emitted
        ):
            skipped += 1
            continue

        payload = build_leave_payload(visitor)
        log_visitor_leave(payload)
        delivery = replace(
            settings,
            webhook_url=get_webhook_url(settings.webhook_url),
            webhook_token=get_webhook_token(settings.webhook_token),
        )
        sent = await forward_visitor_leave(payload, delivery)
        persist_visitor_leave(payload, forwarded=sent)
        state.emitted[key] = leave_time
        # Persiste após cada emissão para minimizar duplicação em reinícios.
        _save_poll_state(settings.state_path, state)
        processed += 1
        if sent:
            forwarded += 1
        logger.info(
            "[VISIT_LEAVE] registrado trigger=poll visitorId=%s leaveTime=%s",
            payload.get("visitorId"),
            payload.get("leaveTime"),
        )

    state.last_poll_ts = now
    state.legacy = False
    state.emitted = _prune_emitted(state.emitted, now=now)
    _save_poll_state(settings.state_path, state)
    if fetched == 0:
        logger.warning(
            "[VISIT_LEAVE] poll sem histórico status=2 lookback=%ss since=%s",
            HISTORY_LOOKBACK_SECONDS,
            query_since,
        )
    logger.info(
        "[VISIT_LEAVE] poll ok fetched=%s processed=%s skipped=%s forwarded=%s since=%s",
        fetched,
        processed,
        skipped,
        forwarded,
        query_since,
    )
    return {
        "status": "ok",
        "fetched": fetched,
        "processed": processed,
        "skipped": skipped,
        "forwarded": forwarded,
    }


async def visitor_leave_poll_loop(
    defense_client: Any,
    settings: VisitLeaveSettings,
) -> None:
    """Executa imediatamente no startup e depois no intervalo configurado."""
    if not settings.poll_enabled:
        logger.info("[VISIT_LEAVE] poll desabilitado (VISIT_LEAVE_POLL_SECONDS=0)")
        return
    logger.info(
        "[VISIT_LEAVE] poll iniciado interval=%ss state=%s",
        int(settings.poll_interval_seconds),
        settings.state_path,
    )
    while True:
        try:
            await poll_visitor_leave_history(
                defense_client=defense_client,
                settings=settings,
            )
        except Exception as exc:
            logger.warning("[VISIT_LEAVE] poll loop erro: %s", exc)
        await asyncio.sleep(settings.poll_interval_seconds)
