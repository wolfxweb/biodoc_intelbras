"""Baixa de visita: filtra evento ACS, monta JSON, grava log e encaminha URL do .env."""

from __future__ import annotations

import json
import time
from dataclasses import dataclass
from datetime import datetime, timezone
from typing import Any
from zoneinfo import ZoneInfo

import httpx

from src.core.logging import logger, visitor_leave_logger

try:
    _LOCAL_TZ = ZoneInfo("America/Sao_Paulo")
except Exception:  # pragma: no cover
    _LOCAL_TZ = timezone.utc

EXIT_STATUS_VALUES = frozenset({"2", "out", "exit", "leave", "saida", "saída"})
LEFT_VISITOR_STATUS = frozenset({"2", "4"})
NESTED_EVENT_KEYS = ("data", "info", "alarmInfo", "alarm", "content", "params", "record")
LIST_EVENT_KEYS = ("data", "list", "alarms", "records", "events", "pageData")
DEDUP_TTL_SECONDS = 3600.0

_recent_leaves: dict[str, float] = {}


@dataclass(frozen=True)
class VisitLeaveSettings:
    webhook_url: str = ""
    webhook_token: str = ""
    timeout_seconds: float = 10.0

    @property
    def forward_enabled(self) -> bool:
        return bool(self.webhook_url.strip())


def _as_text(value: Any) -> str | None:
    if value is None:
        return None
    text = str(value).strip()
    if not text or text.lower() in ("null", "undefined", "none"):
        return None
    return text


def _first_text(record: dict[str, Any], *keys: str) -> str | None:
    for key in keys:
        found = _as_text(record.get(key))
        if found:
            return found
    return None


def unwrap_event_record(record: dict[str, Any]) -> dict[str, Any]:
    """Achata data/info aninhados sem perder o envelope."""
    flat = dict(record)
    for key in NESTED_EVENT_KEYS:
        nested = record.get(key)
        if isinstance(nested, dict):
            for nested_key, nested_value in nested.items():
                flat.setdefault(nested_key, nested_value)
    return flat


def iter_event_records(body: Any) -> list[dict[str, Any]]:
    if isinstance(body, list):
        return [item for item in body if isinstance(item, dict)]
    if not isinstance(body, dict):
        return []
    for key in LIST_EVENT_KEYS:
        value = body.get(key)
        if isinstance(value, list):
            return [item for item in value if isinstance(item, dict)]
    return [body]


def is_exit_direction(record: dict[str, Any]) -> bool | None:
    """True=saída, False=entrada, None=não dá para afirmar."""
    raw = _first_text(
        record,
        "inOutStatus",
        "inAndOut",
        "enterOrExit",
        "inOrOut",
        "openDirection",
        "direction",
        "iInOutStatus",
    )
    if raw is None:
        return None
    normalized = raw.casefold()
    if normalized in EXIT_STATUS_VALUES or "saida" in normalized or "exit" in normalized:
        return True
    if normalized in {"1", "in", "enter", "entrada"}:
        return False
    return None


def visitor_already_left(visitor: dict[str, Any]) -> bool:
    status = _as_text(visitor.get("status"))
    if status in LEFT_VISITOR_STATUS:
        return True
    leave_time = _as_text(visitor.get("leaveTime"))
    return bool(leave_time and leave_time != "0")


def is_visitor_leave_event(record: dict[str, Any], visitor: dict[str, Any] | None) -> bool:
    if visitor is None:
        return False
    direction = is_exit_direction(record)
    if direction is False:
        return False
    if direction is True:
        return True
    return visitor_already_left(visitor)


def extract_event_lookup_keys(record: dict[str, Any]) -> dict[str, str | None]:
    auth = record.get("authInfo") if isinstance(record.get("authInfo"), dict) else {}
    return {
        "visitor_id": _first_text(record, "visitorId", "id"),
        "person_id": _first_text(record, "personId", "szPersonId"),
        "id_num": _first_text(record, "idNum", "idNo", "szIDNum", "paperNumber"),
        "card_no": _first_text(record, "cardNo", "szCardNum")
        or _as_text(auth.get("cardNo") if isinstance(auth, dict) else None),
        "name": _first_text(record, "visitorName", "personName", "szFirstName", "name"),
        "channel_id": _first_text(record, "channelId", "szChannelCode", "channelCode"),
        "swipe_time": _first_text(record, "swipeTime", "tSwipDate", "alarmTime", "time"),
    }


def _visitor_node(visitor_body: dict[str, Any]) -> dict[str, Any]:
    data = visitor_body.get("data", visitor_body)
    return data if isinstance(data, dict) else visitor_body


def build_leave_payload(
    *,
    visitor: dict[str, Any],
    record: dict[str, Any],
    lookup: dict[str, str | None],
) -> dict[str, Any]:
    node = _visitor_node(visitor)
    auth = node.get("authInfo") if isinstance(node.get("authInfo"), dict) else {}
    logged_at = datetime.now(_LOCAL_TZ).isoformat(timespec="seconds")
    leave_time = (
        _as_text(node.get("leaveTime"))
        or lookup.get("swipe_time")
        or str(int(time.time()))
    )
    return {
        "event": "visitor_leave",
        "loggedAt": logged_at,
        "visitorId": _as_text(node.get("visitorId") or node.get("id")) or lookup.get("visitor_id"),
        "personId": _as_text(node.get("personId")) or lookup.get("person_id"),
        "status": _as_text(node.get("status")) or "2",
        "visitorName": _as_text(node.get("visitorName")) or lookup.get("name"),
        "idNum": _as_text(node.get("idNum")) or lookup.get("id_num"),
        "cardNo": _as_text(auth.get("cardNo")) or lookup.get("card_no"),
        "remark": _as_text(node.get("remark")),
        "visitedName": _as_text(node.get("visitedName")),
        "visitedOrgName": _as_text(node.get("visitedOrgName")),
        "arrivalTime": _as_text(node.get("arrivalTime")),
        "expectLeaveTime": _as_text(node.get("expectLeaveTime")),
        "leaveTime": leave_time if leave_time != "0" else str(int(time.time())),
        "channelId": lookup.get("channel_id"),
        "source": "defense_ia",
    }


def _dedup_key(payload: dict[str, Any]) -> str:
    return "|".join(
        [
            str(payload.get("visitorId") or ""),
            str(payload.get("personId") or ""),
            str(payload.get("leaveTime") or ""),
            str(payload.get("channelId") or ""),
        ]
    )


def _seen_recently(key: str) -> bool:
    now = time.time()
    expired = [item for item, stamp in _recent_leaves.items() if now - stamp > DEDUP_TTL_SECONDS]
    for item in expired:
        _recent_leaves.pop(item, None)
    if key in _recent_leaves:
        return True
    _recent_leaves[key] = now
    return False


def log_visitor_leave(payload: dict[str, Any]) -> None:
    visitor_leave_logger.info(json.dumps(payload, ensure_ascii=False, separators=(",", ":")))


def reset_leave_dedup() -> None:
    _recent_leaves.clear()


async def forward_visitor_leave(
    payload: dict[str, Any],
    settings: VisitLeaveSettings,
) -> bool:
    if not settings.forward_enabled:
        logger.info("[VISIT_LEAVE] webhook URL vazia — só log local")
        return False
    headers = {"Content-Type": "application/json;charset=UTF-8"}
    token = settings.webhook_token.strip()
    if token:
        headers["Authorization"] = f"Bearer {token}"
    try:
        async with httpx.AsyncClient(timeout=settings.timeout_seconds) as client:
            response = await client.post(
                settings.webhook_url.strip(),
                json=payload,
                headers=headers,
            )
    except httpx.HTTPError as exc:
        logger.warning("[VISIT_LEAVE] falha ao enviar webhook: %s", exc)
        return False
    if response.status_code >= 300:
        logger.warning(
            "[VISIT_LEAVE] webhook HTTP %s body=%s",
            response.status_code,
            response.text[:300],
        )
        return False
    logger.info("[VISIT_LEAVE] webhook ok HTTP %s", response.status_code)
    return True


async def process_visitor_leave_body(
    body: Any,
    *,
    defense_client: Any,
    settings: VisitLeaveSettings,
) -> dict[str, Any]:
    processed = 0
    skipped = 0
    forwarded = 0
    for raw in iter_event_records(body):
        record = unwrap_event_record(raw)
        lookup = extract_event_lookup_keys(record)
        visitor = await defense_client.find_visitor_for_leave_event(
            visitor_id=lookup["visitor_id"],
            person_id=lookup["person_id"],
            id_num=lookup["id_num"],
            card_no=lookup["card_no"],
            name=lookup["name"],
        )
        if not is_visitor_leave_event(record, visitor):
            skipped += 1
            continue
        payload = build_leave_payload(visitor=visitor, record=record, lookup=lookup)
        key = _dedup_key(payload)
        if _seen_recently(key):
            logger.info("[VISIT_LEAVE] duplicata ignorada key=%s", key)
            skipped += 1
            continue
        log_visitor_leave(payload)
        if await forward_visitor_leave(payload, settings):
            forwarded += 1
        processed += 1
    return {
        "status": "ok",
        "processed": processed,
        "skipped": skipped,
        "forwarded": forwarded,
    }
