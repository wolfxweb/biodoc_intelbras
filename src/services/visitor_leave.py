"""Baixas de visita obtidas exclusivamente pelo histórico oficial do Defense IA."""

from __future__ import annotations

import asyncio
import json
import socket
import time
from dataclasses import dataclass, replace
from datetime import datetime, timezone
from pathlib import Path
from typing import Any
from urllib.parse import urlsplit, urlunsplit
from zoneinfo import ZoneInfo

import httpx

from src.core.logging import logger, visitor_leave_logger
from src.services.visitor_leave_store import (
    get_webhook_token,
    get_webhook_url,
    list_pending_leaves,
    persist_visitor_leave,
    record_forward_result,
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
RETRY_BATCH_LIMIT = 200
RETRY_CONCURRENCY = 5
_retry_cycle_lock = asyncio.Lock()


@dataclass(frozen=True)
class ForwardResult:
    sent: bool
    error: str | None = None
    status_code: int | None = None
    attempted: bool = False


@dataclass(frozen=True)
class VisitLeaveSettings:
    webhook_url: str = ""
    webhook_token: str = ""
    timeout_seconds: float = 10.0
    poll_interval_seconds: float = DEFAULT_POLL_INTERVAL_SECONDS
    state_path: str = "data/visitor_leave_state.json"
    retry_interval_seconds: float = 600.0
    retry_window_hours: float = 6.0
    retry_max_attempts: int = 6

    @property
    def forward_enabled(self) -> bool:
        return bool(self.webhook_url.strip())

    @property
    def poll_enabled(self) -> bool:
        return self.poll_interval_seconds > 0

    @property
    def retry_enabled(self) -> bool:
        return self.retry_interval_seconds > 0


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


def _traefik_ip() -> str | None:
    try:
        infos = socket.getaddrinfo("traefik_traefik", 443)
    except OSError:
        return None
    for info in infos:
        ip = info[4][0]
        if ip:
            return ip
    return None


def _delivery_target(url: str) -> tuple[str, dict[str, str], dict[str, str]]:
    """Mantém a URL e o Host configurados. Só o TCP sai pelo Traefik interno.

    O IP público de vista.wolfx.com.br não responde de dentro do container.
    """
    target = url.strip()
    parsed = urlsplit(target)
    if parsed.hostname != "vista.wolfx.com.br":
        return target, {}, {}
    pin = _traefik_ip()
    if not pin:
        return target, {}, {}
    pinned = urlunsplit((parsed.scheme or "https", pin, parsed.path or "/", parsed.query, ""))
    return pinned, {"Host": parsed.hostname}, {"sni_hostname": parsed.hostname}


def format_stored_forward_error(value: str | None) -> str:
    """Texto curto para a tela, inclusive em erros antigos já gravados."""
    text = (value or "").strip()
    if not text:
        return ""
    lowered = text.lower()
    if (
        "connecttimeout" in lowered
        or "deadline exceeded" in lowered
        or "tempo esgotado ao conectar" in lowered
    ):
        return "Tempo esgotado ao conectar no destino. Ele não chegou a responder."
    if "readtimeout" in lowered or "não respondeu a tempo" in lowered:
        return "O destino conectou, mas não respondeu a tempo."
    if text == "falha sem detalhe":
        return "Falha sem detalhe do destino."
    return text


def _error_text(exc: BaseException) -> str:
    if isinstance(exc, httpx.ConnectTimeout):
        return "Tempo esgotado ao conectar no destino. Ele não chegou a responder."
    if isinstance(exc, httpx.ReadTimeout):
        return "O destino conectou, mas não respondeu a tempo."
    if isinstance(exc, httpx.ConnectError):
        return "Não foi possível abrir conexão com o destino."
    message = str(exc).strip()
    if not message:
        return f"Falha de rede ({type(exc).__name__})."
    return f"{type(exc).__name__}: {message}"[:500]


def _friendly_body(text: str) -> str:
    raw = text.strip()
    if not raw:
        return "resposta sem corpo"
    try:
        data = json.loads(raw)
    except json.JSONDecodeError:
        return raw[:300]
    if isinstance(data, dict):
        detail = data.get("detail")
        if isinstance(detail, str) and detail.strip():
            return detail.strip()[:300]
        if isinstance(detail, list):
            messages: list[str] = []
            for item in detail:
                if not isinstance(item, dict) or not item.get("msg"):
                    continue
                loc = [str(part) for part in item.get("loc") or [] if str(part) != "body"]
                field = ".".join(loc)
                messages.append(f"{field}: {item['msg']}" if field else str(item["msg"]))
            if messages:
                return "; ".join(messages)[:300]
        for key in ("message", "error"):
            value = data.get(key)
            if isinstance(value, str) and value.strip():
                return value.strip()[:300]
    return raw[:300]


def _response_error(response: httpx.Response) -> str:
    return f"HTTP {response.status_code} — {_friendly_body(response.text or '')}"[:500]


def _failure_result(error: str, status_code: int | None = None) -> ForwardResult:
    return ForwardResult(
        sent=False,
        error=error[:500],
        status_code=status_code,
        attempted=True,
    )


async def forward_visitor_leave(
    payload: dict[str, Any],
    settings: VisitLeaveSettings,
) -> ForwardResult:
    """Envia o payload recebido. O retry curto de 1s e 3s conta como um envio."""
    if not settings.forward_enabled:
        return ForwardResult(sent=False, attempted=False)

    headers = {"Content-Type": "application/json;charset=UTF-8"}
    token = settings.webhook_token.strip()
    if token:
        headers["Authorization"] = f"Bearer {token}"

    configured_url = settings.webhook_url.strip()
    target_url, extra_headers, extensions = _delivery_target(configured_url)
    headers.update(extra_headers)
    logger.info("[VISIT_LEAVE] webhook POST %s token=%s", configured_url, "sim" if token else "não")
    delays = (0.0, *WEBHOOK_RETRY_DELAYS_SECONDS)
    async with httpx.AsyncClient(timeout=settings.timeout_seconds) as client:
        for attempt, delay in enumerate(delays, start=1):
            if delay:
                await asyncio.sleep(delay)
            try:
                response = await client.post(
                    target_url,
                    json=payload,
                    headers=headers,
                    extensions=extensions or None,
                )
            except httpx.HTTPError as exc:
                detail = _error_text(exc)
                if attempt < len(delays):
                    logger.warning(
                        "[VISIT_LEAVE] webhook falhou tentativa=%s: %s",
                        attempt,
                        detail,
                    )
                    continue
                logger.warning("[VISIT_LEAVE] webhook falhou definitivamente: %s", detail)
                return _failure_result(detail)

            if response.status_code < 300:
                logger.info("[VISIT_LEAVE] webhook ok HTTP %s", response.status_code)
                return ForwardResult(
                    sent=True,
                    status_code=response.status_code,
                    attempted=True,
                )
            detail = _response_error(response)
            if response.status_code not in {429, 500, 502, 503, 504}:
                logger.warning("[VISIT_LEAVE] webhook %s", detail)
                return _failure_result(detail, response.status_code)
            if attempt < len(delays):
                logger.warning(
                    "[VISIT_LEAVE] webhook HTTP %s; nova tentativa=%s",
                    response.status_code,
                    attempt + 1,
                )
                continue
            logger.warning(
                "[VISIT_LEAVE] webhook falhou após tentativas %s",
                detail,
            )
            return _failure_result(detail, response.status_code)
    return _failure_result("falha sem detalhe")


def _delivery_settings(settings: VisitLeaveSettings) -> VisitLeaveSettings:
    return replace(
        settings,
        webhook_url=get_webhook_url(settings.webhook_url),
        webhook_token=get_webhook_token(settings.webhook_token),
    )


async def resend_stored_leave(
    *,
    visitor_id: str,
    leave_time: int,
    payload_json: str,
    settings: VisitLeaveSettings,
) -> ForwardResult:
    """Reenvia o JSON já gravado, com o mesmo visitorId e leaveTime."""
    delivery = _delivery_settings(settings)
    if not delivery.forward_enabled:
        return ForwardResult(sent=False, attempted=False)
    try:
        payload = json.loads(payload_json)
    except json.JSONDecodeError:
        result = _failure_result("payload_json inválido")
        record_forward_result(
            visitor_id,
            leave_time,
            sent=False,
            error=result.error,
        )
        return result
    if not isinstance(payload, dict):
        result = _failure_result("payload_json inválido")
        record_forward_result(
            visitor_id,
            leave_time,
            sent=False,
            error=result.error,
        )
        return result
    result = await forward_visitor_leave(payload, delivery)
    if result.attempted:
        record_forward_result(
            visitor_id,
            leave_time,
            sent=result.sent,
            error=result.error,
            status_code=result.status_code,
        )
    return result


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
        delivery = _delivery_settings(settings)
        result = await forward_visitor_leave(payload, delivery)
        sent = result.sent if isinstance(result, ForwardResult) else bool(result)
        persist_visitor_leave(payload, forwarded=sent)
        if isinstance(result, ForwardResult) and result.attempted:
            visitor_id = str(payload.get("visitorId") or "").strip()
            leave_time_text = str(payload.get("leaveTime") or "").strip()
            if visitor_id and leave_time_text.isdigit():
                record_forward_result(
                    visitor_id,
                    int(leave_time_text),
                    sent=result.sent,
                    error=result.error,
                    status_code=result.status_code,
                )
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


async def retry_unsent_visitor_leaves(settings: VisitLeaveSettings) -> dict[str, Any]:
    """Reenvia baixas não enviadas dentro da janela. Um ciclo não entra no outro."""
    if _retry_cycle_lock.locked():
        return {"status": "busy", "sent": 0, "failed": 0, "skipped": 0}
    async with _retry_cycle_lock:
        pending = list_pending_leaves(
            limit=RETRY_BATCH_LIMIT,
            window_hours=settings.retry_window_hours,
            max_attempts=settings.retry_max_attempts,
        )
        if not pending:
            return {"status": "ok", "sent": 0, "failed": 0, "skipped": 0}

        semaphore = asyncio.Semaphore(RETRY_CONCURRENCY)

        async def _one(item: Any) -> ForwardResult:
            async with semaphore:
                return await resend_stored_leave(
                    visitor_id=item.visitor_id,
                    leave_time=item.leave_time,
                    payload_json=item.payload_json,
                    settings=settings,
                )

        results = await asyncio.gather(*(_one(item) for item in pending))
        sent = sum(1 for item in results if item.sent)
        failed = sum(1 for item in results if item.attempted and not item.sent)
        skipped = sum(1 for item in results if not item.attempted)
        logger.info(
            "[VISIT_LEAVE] reenvio automático sent=%s failed=%s skipped=%s batch=%s",
            sent,
            failed,
            skipped,
            len(pending),
        )
        return {"status": "ok", "sent": sent, "failed": failed, "skipped": skipped}


async def visitor_leave_retry_loop(settings: VisitLeaveSettings) -> None:
    """Reenvia não enviados no intervalo configurado, por algumas horas."""
    if not settings.retry_enabled:
        logger.info("[VISIT_LEAVE] reenvio automático desligado (VISIT_LEAVE_RETRY_SECONDS=0)")
        return
    logger.info(
        "[VISIT_LEAVE] reenvio automático interval=%ss window=%sh max=%s",
        int(settings.retry_interval_seconds),
        settings.retry_window_hours,
        settings.retry_max_attempts,
    )
    while True:
        try:
            await retry_unsent_visitor_leaves(settings)
        except Exception as exc:
            logger.warning("[VISIT_LEAVE] reenvio automático erro: %s", exc)
        await asyncio.sleep(settings.retry_interval_seconds)
