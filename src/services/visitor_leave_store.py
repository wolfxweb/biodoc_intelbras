"""Persistência SQLite das baixas de visita e da URL de destino."""

from __future__ import annotations

import json
from dataclasses import dataclass
from datetime import datetime, timedelta
from pathlib import Path
from typing import Any
from zoneinfo import ZoneInfo

from sqlalchemy.exc import IntegrityError
from sqlalchemy.orm import Session

from src.core.database import Base, SessionLocal, engine
from src.core.logging import LOG_DIR, logger
from src.models.integration_source import IntegrationSource  # noqa: F401
from src.models.visitor_leave import (
    WEBHOOK_TOKEN_SETTING_KEY,
    WEBHOOK_URL_SETTING_KEY,
    AppSetting,
    VisitorLeaveEvent,
)

try:
    _LOCAL_TZ = ZoneInfo("America/Sao_Paulo")
except Exception:  # pragma: no cover
    from datetime import timezone

    _LOCAL_TZ = timezone.utc


_FORWARD_COLUMNS = (
    ("forward_attempts", "INTEGER NOT NULL DEFAULT 0"),
    ("last_error", "TEXT"),
    ("last_status_code", "INTEGER"),
    ("last_attempt_at", "VARCHAR(64)"),
)
_ERROR_MAX_LEN = 500


@dataclass(frozen=True)
class PendingLeave:
    visitor_id: str
    leave_time: int
    payload_json: str
    logged_at: str | None


def init_visit_leave_db() -> None:
    Base.metadata.create_all(bind=engine)
    _ensure_forward_columns()


def _ensure_forward_columns() -> None:
    with engine.begin() as conn:
        rows = conn.exec_driver_sql("PRAGMA table_info(visitor_leave_events)").fetchall()
        if not rows:
            return
        existing = {row[1] for row in rows}
        for name, ddl in _FORWARD_COLUMNS:
            if name not in existing:
                conn.exec_driver_sql(
                    f"ALTER TABLE visitor_leave_events ADD COLUMN {name} {ddl}"
                )


def _clock() -> datetime:
    return datetime.now(_LOCAL_TZ)


def _parse_logged_at(value: str | None) -> datetime | None:
    text = (value or "").strip()
    if not text:
        return None
    try:
        parsed = datetime.fromisoformat(text)
    except ValueError:
        return None
    if parsed.tzinfo is None:
        return parsed.replace(tzinfo=_LOCAL_TZ)
    return parsed


def within_retry_window(
    logged_at: str | None,
    *,
    window_hours: float,
    now: datetime | None = None,
) -> bool:
    if window_hours <= 0:
        return True
    started = _parse_logged_at(logged_at)
    if started is None:
        return True
    current = now or _clock()
    return current - started <= timedelta(hours=window_hours)


def _as_int(value: Any) -> int | None:
    if value is None:
        return None
    text = str(value).strip()
    if not text.isdigit():
        return None
    number = int(text)
    return number if number > 0 else None


def persist_visitor_leave(
    payload: dict[str, Any],
    *,
    forwarded: bool = False,
    db: Session | None = None,
) -> bool:
    visitor_id = str(payload.get("visitorId") or "").strip()
    leave_time = _as_int(payload.get("leaveTime"))
    if not visitor_id or leave_time is None:
        return False

    owns_session = db is None
    session = SessionLocal() if owns_session else db
    assert session is not None
    try:
        exists = (
            session.query(VisitorLeaveEvent)
            .filter(
                VisitorLeaveEvent.visitor_id == visitor_id,
                VisitorLeaveEvent.leave_time == leave_time,
            )
            .first()
        )
        if exists is not None:
            if forwarded and not exists.forwarded:
                exists.forwarded = True
                session.commit()
            return False
        session.add(
            VisitorLeaveEvent(
                visitor_id=visitor_id,
                visitor_name=str(payload.get("visitorName") or "").strip() or None,
                id_num=str(payload.get("idNum") or "").strip() or None,
                visited_name=str(payload.get("visitedName") or "").strip() or None,
                arrival_time=_as_int(payload.get("arrivalTime")),
                leave_time=leave_time,
                logged_at=str(payload.get("loggedAt") or "").strip() or None,
                forwarded=forwarded,
                payload_json=json.dumps(payload, ensure_ascii=False, separators=(",", ":")),
            )
        )
        session.commit()
        return True
    except IntegrityError:
        session.rollback()
        return False
    except Exception:
        session.rollback()
        logger.exception("[VISIT_LEAVE] falha ao gravar baixa no SQLite")
        return False
    finally:
        if owns_session:
            session.close()


def get_setting(key: str, default: str = "", *, db: Session | None = None) -> str:
    owns_session = db is None
    session = SessionLocal() if owns_session else db
    assert session is not None
    try:
        row = session.get(AppSetting, key)
        if row is None:
            return default
        return row.value
    finally:
        if owns_session:
            session.close()


def set_setting(key: str, value: str, *, db: Session | None = None) -> None:
    owns_session = db is None
    session = SessionLocal() if owns_session else db
    assert session is not None
    try:
        row = session.get(AppSetting, key)
        if row is None:
            session.add(AppSetting(key=key, value=value))
        else:
            row.value = value
        session.commit()
    finally:
        if owns_session:
            session.close()


def get_webhook_url(env_fallback: str = "", *, db: Session | None = None) -> str:
    stored = get_setting(WEBHOOK_URL_SETTING_KEY, db=db).strip()
    return stored or env_fallback.strip()


def set_webhook_url(url: str, *, db: Session | None = None) -> None:
    set_setting(WEBHOOK_URL_SETTING_KEY, url.strip(), db=db)


def get_webhook_token(env_fallback: str = "", *, db: Session | None = None) -> str:
    stored = get_setting(WEBHOOK_TOKEN_SETTING_KEY, db=db).strip()
    return stored or env_fallback.strip()


def set_webhook_token(token: str, *, db: Session | None = None) -> None:
    set_setting(WEBHOOK_TOKEN_SETTING_KEY, token.strip(), db=db)


def record_forward_result(
    visitor_id: str,
    leave_time: int,
    *,
    sent: bool,
    error: str | None = None,
    status_code: int | None = None,
    db: Session | None = None,
) -> None:
    """Atualiza o resultado do POST sem alterar o payload original."""
    owns_session = db is None
    session = SessionLocal() if owns_session else db
    assert session is not None
    try:
        row = (
            session.query(VisitorLeaveEvent)
            .filter(
                VisitorLeaveEvent.visitor_id == visitor_id,
                VisitorLeaveEvent.leave_time == leave_time,
            )
            .first()
        )
        if row is None:
            return
        row.last_attempt_at = _clock().isoformat(timespec="seconds")
        if sent:
            row.forwarded = True
            row.last_status_code = status_code
        else:
            row.forwarded = False
            row.forward_attempts = int(row.forward_attempts or 0) + 1
            text = (error or "falha sem detalhe").strip()
            row.last_error = text[:_ERROR_MAX_LEN]
            row.last_status_code = status_code
        session.commit()
    except Exception:
        session.rollback()
        logger.exception("[VISIT_LEAVE] falha ao gravar resultado do envio")
    finally:
        if owns_session:
            session.close()


def list_pending_leaves(
    *,
    limit: int,
    window_hours: float,
    now: datetime | None = None,
) -> list[PendingLeave]:
    """Baixas não enviadas, mais antigas primeiro, dentro da janela de reenvio."""
    current = now or _clock()
    session = SessionLocal()
    try:
        rows = (
            session.query(VisitorLeaveEvent)
            .filter(VisitorLeaveEvent.forwarded.is_(False))
            .order_by(VisitorLeaveEvent.leave_time.asc())
            .all()
        )
        pending: list[PendingLeave] = []
        for row in rows:
            if not within_retry_window(row.logged_at, window_hours=window_hours, now=current):
                continue
            pending.append(
                PendingLeave(
                    visitor_id=row.visitor_id,
                    leave_time=row.leave_time,
                    payload_json=row.payload_json,
                    logged_at=row.logged_at,
                )
            )
            if len(pending) >= limit:
                break
        return pending
    finally:
        session.close()


def import_visitor_leave_logs_if_empty(*, log_dir: str | Path | None = None) -> int:
    session = SessionLocal()
    try:
        if session.query(VisitorLeaveEvent).first() is not None:
            return 0
        directory = Path(log_dir) if log_dir is not None else Path(LOG_DIR)
        if not directory.is_dir():
            return 0
        imported = 0
        for path in sorted(directory.glob("visitor_leave.log*")):
            if path.suffix == ".tmp" or not path.is_file():
                continue
            try:
                lines = path.read_text(encoding="utf-8").splitlines()
            except OSError:
                continue
            for line in lines:
                raw = line.strip()
                if not raw:
                    continue
                try:
                    payload = json.loads(raw)
                except json.JSONDecodeError:
                    continue
                if not isinstance(payload, dict) or payload.get("event") != "visitor_leave":
                    continue
                if persist_visitor_leave(payload, forwarded=False, db=session):
                    imported += 1
        logger.info("[VISIT_LEAVE] importou %s linhas do log para SQLite", imported)
        return imported
    finally:
        session.close()


def format_unix(value: int | None) -> str:
    if not value:
        return "—"
    return datetime.fromtimestamp(value, _LOCAL_TZ).strftime("%d/%m/%Y %H:%M")
