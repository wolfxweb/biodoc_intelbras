"""Tela autenticada de baixas de visita em GET /."""

from __future__ import annotations

import asyncio
import os
import secrets
from datetime import datetime
from pathlib import Path
from typing import Annotated
from urllib.parse import urlencode
from zoneinfo import ZoneInfo

from fastapi import APIRouter, Depends, Form, Query, Request
from fastapi.responses import HTMLResponse, RedirectResponse, Response
from fastapi.templating import Jinja2Templates
from itsdangerous import BadSignature, TimestampSigner
from sqlalchemy.orm import Session

from src.api.manuals import get_manual, list_manuals, render_manual
from src.core.database import get_db
from src.core.lifespan import build_visit_leave_settings_from_env
from src.models.visitor_leave import VisitorLeaveEvent
from src.services.visitor_leave import resend_stored_leave
from src.services.visitor_leave_store import (
    format_unix,
    get_webhook_token,
    get_webhook_url,
    set_webhook_token,
    set_webhook_url,
)

router = APIRouter(include_in_schema=False)
templates = Jinja2Templates(directory=str(Path(__file__).resolve().parent.parent / "templates"))
templates.env.filters["unix_sp"] = format_unix

COOKIE_NAME = "visit_leave_ui"
COOKIE_MAX_AGE = 12 * 3600

try:
    _LOCAL_TZ = ZoneInfo("America/Sao_Paulo")
except Exception:  # pragma: no cover
    from datetime import timezone

    _LOCAL_TZ = timezone.utc


def _ui_password() -> str:
    return (
        os.getenv("VISIT_LEAVE_UI_TOKEN", "").strip()
        or os.getenv("ADMIN_API_TOKEN", "").strip()
    )


def _signer() -> TimestampSigner:
    secret = _ui_password() or "visit-leave-ui-dev"
    return TimestampSigner(secret)


def _is_authenticated(request: Request) -> bool:
    raw = request.cookies.get(COOKIE_NAME)
    if not raw:
        return False
    try:
        _signer().unsign(raw, max_age=COOKIE_MAX_AGE)
    except (BadSignature, TypeError):
        return False
    return True


def _day_start(date_text: str) -> int | None:
    text = (date_text or "").strip()
    if not text:
        return None
    try:
        dt = datetime.strptime(text, "%Y-%m-%d").replace(tzinfo=_LOCAL_TZ)
    except ValueError:
        return None
    return int(dt.timestamp())


def _day_end(date_text: str) -> int | None:
    start = _day_start(date_text)
    if start is None:
        return None
    return start + 86400 - 1


def _filtered_leave_query(
    db: Session,
    *,
    name: str,
    setor: str,
    date_from: str,
    date_to: str,
    sent: str,
):
    query = db.query(VisitorLeaveEvent)
    if name.strip():
        query = query.filter(VisitorLeaveEvent.visitor_name.ilike(f"%{name.strip()}%"))
    if setor.strip():
        query = query.filter(VisitorLeaveEvent.visited_name.ilike(f"%{setor.strip()}%"))
    start_ts = _day_start(date_from)
    end_ts = _day_end(date_to)
    if start_ts is not None:
        query = query.filter(VisitorLeaveEvent.leave_time >= start_ts)
    if end_ts is not None:
        query = query.filter(VisitorLeaveEvent.leave_time <= end_ts)
    if sent == "no":
        query = query.filter(VisitorLeaveEvent.forwarded.is_(False))
    elif sent == "yes":
        query = query.filter(VisitorLeaveEvent.forwarded.is_(True))
    elif sent == "retry":
        query = query.filter(VisitorLeaveEvent.forward_attempts > 0)
    return query


def _filter_query(
    *,
    name: str,
    setor: str,
    date_from: str,
    date_to: str,
    sent: str,
) -> str:
    return urlencode(
        {
            key: value
            for key, value in {
                "name": name,
                "setor": setor,
                "from": date_from,
                "to": date_to,
                "sent": sent,
            }.items()
            if value
        }
    )


@router.get("/login", response_class=HTMLResponse, response_model=None)
async def login_form(request: Request) -> Response:
    if _is_authenticated(request):
        return RedirectResponse("/", status_code=303)
    return templates.TemplateResponse(
        request,
        "login.html",
        {"error": request.query_params.get("error") == "1"},
    )


@router.post("/login")
async def login_submit(password: Annotated[str, Form()] = "") -> RedirectResponse:
    expected = _ui_password()
    try:
        valid = bool(expected) and secrets.compare_digest(password, expected)
    except ValueError:
        valid = False
    if not valid:
        return RedirectResponse("/login?error=1", status_code=303)
    token = _signer().sign("ok")
    signed = token.decode("utf-8") if isinstance(token, bytes) else str(token)
    response = RedirectResponse("/", status_code=303)
    response.set_cookie(
        COOKIE_NAME,
        signed,
        httponly=True,
        samesite="lax",
        max_age=COOKIE_MAX_AGE,
        path="/",
    )
    return response


@router.post("/logout")
async def logout() -> RedirectResponse:
    response = RedirectResponse("/login", status_code=303)
    response.delete_cookie(COOKIE_NAME, path="/")
    return response


@router.get("/", response_class=HTMLResponse, response_model=None)
async def visit_leave_list(
    request: Request,
    db: Annotated[Session, Depends(get_db)],
    name: str = "",
    setor: str = "",
    date_from: str = Query("", alias="from"),
    date_to: str = Query("", alias="to"),
    sent: str = "",
    page: int = 1,
) -> Response:
    if not _is_authenticated(request):
        return RedirectResponse("/login", status_code=303)

    if sent not in {"", "no", "yes", "retry"}:
        sent = ""
    page = max(page, 1)
    page_size = 50
    query = _filtered_leave_query(
        db,
        name=name,
        setor=setor,
        date_from=date_from,
        date_to=date_to,
        sent=sent,
    )

    total = query.count()
    pages = max((total + page_size - 1) // page_size, 1)
    page = min(page, pages)
    rows = (
        query.order_by(VisitorLeaveEvent.leave_time.desc())
        .offset((page - 1) * page_size)
        .limit(page_size)
        .all()
    )
    filter_qs = _filter_query(
        name=name,
        setor=setor,
        date_from=date_from,
        date_to=date_to,
        sent=sent,
    )
    env_url = os.getenv("VISIT_LEAVE_WEBHOOK_URL", "").strip()
    env_token = os.getenv("VISIT_LEAVE_WEBHOOK_TOKEN", "").strip()
    return templates.TemplateResponse(
        request,
        "list.html",
        {
            "rows": rows,
            "name": name,
            "setor": setor,
            "date_from": date_from,
            "date_to": date_to,
            "sent": sent,
            "page": page,
            "pages": pages,
            "total": total,
            "filter_qs": filter_qs,
            "webhook_url": get_webhook_url(env_url, db=db),
            "webhook_token": get_webhook_token(env_token, db=db),
            "saved": request.query_params.get("saved") == "1",
            "resend_ok": request.query_params.get("ok"),
            "resend_fail": request.query_params.get("fail"),
            "resend_notice": request.query_params.get("notice") or "",
        },
    )


@router.post("/visit-leave/resend")
async def resend_visit_leaves(
    request: Request,
    db: Annotated[Session, Depends(get_db)],
) -> RedirectResponse:
    if not _is_authenticated(request):
        return RedirectResponse("/login", status_code=303)

    form = await request.form()
    scope = str(form.get("scope") or "selected")
    name = str(form.get("name") or "")
    setor = str(form.get("setor") or "")
    date_from = str(form.get("from") or "")
    date_to = str(form.get("to") or "")
    sent = str(form.get("sent") or "")
    if sent not in {"", "no", "yes", "retry"}:
        sent = ""

    notice = ""
    targets: list[VisitorLeaveEvent] = []
    if scope == "unsent":
        targets = (
            _filtered_leave_query(
                db,
                name=name,
                setor=setor,
                date_from=date_from,
                date_to=date_to,
                sent="no",
            )
            .order_by(VisitorLeaveEvent.leave_time.asc())
            .all()
        )
    else:
        raw_ids = [item for item in form.getlist("ids") if str(item).strip()]
        ids: list[int] = []
        for item in raw_ids:
            text = str(item).strip()
            if text.isdigit():
                ids.append(int(text))
        if not ids:
            notice = "Nenhum registro selecionado."
        else:
            targets = (
                db.query(VisitorLeaveEvent)
                .filter(VisitorLeaveEvent.id.in_(ids))
                .all()
            )

    pending = [row for row in targets if not row.forwarded]
    payloads = [
        (row.visitor_id, row.leave_time, row.payload_json) for row in pending
    ]
    settings = build_visit_leave_settings_from_env()
    sent_count = 0
    failed_count = 0
    if notice:
        pass
    elif payloads and not get_webhook_url(settings.webhook_url, db=db):
        notice = "Informe a URL de destino antes de reenviar."
    else:
        semaphore = asyncio.Semaphore(5)

        async def _one(item: tuple[str, int, str]):
            visitor_id, leave_time, payload_json = item
            async with semaphore:
                return await resend_stored_leave(
                    visitor_id=visitor_id,
                    leave_time=leave_time,
                    payload_json=payload_json,
                    settings=settings,
                )

        results = await asyncio.gather(*(_one(item) for item in payloads))
        sent_count = sum(1 for result in results if result.sent)
        failed_count = sum(1 for result in results if result.attempted and not result.sent)

    params = {
        key: value
        for key, value in {
            "name": name,
            "setor": setor,
            "from": date_from,
            "to": date_to,
            "sent": sent if scope != "unsent" else "no",
            "ok": str(sent_count),
            "fail": str(failed_count),
            "notice": notice,
        }.items()
        if value
    }
    return RedirectResponse(f"/?{urlencode(params)}", status_code=303)


@router.post("/settings/webhook-url")
async def save_webhook_url(
    request: Request,
    db: Annotated[Session, Depends(get_db)],
    webhook_url: Annotated[str, Form()] = "",
    webhook_token: Annotated[str, Form()] = "",
) -> RedirectResponse:
    if not _is_authenticated(request):
        return RedirectResponse("/login", status_code=303)
    set_webhook_url(webhook_url, db=db)
    set_webhook_token(webhook_token, db=db)
    return RedirectResponse("/?saved=1", status_code=303)


@router.get("/manuais", response_class=HTMLResponse, response_model=None)
async def manuals_index(request: Request) -> Response:
    if not _is_authenticated(request):
        return RedirectResponse("/login", status_code=303)
    return templates.TemplateResponse(
        request,
        "manuals.html",
        {"manuals": list_manuals(), "manual": None, "body_html": ""},
    )


@router.get("/manuais/{slug}", response_class=HTMLResponse, response_model=None)
async def manuals_detail(request: Request, slug: str) -> Response:
    if not _is_authenticated(request):
        return RedirectResponse("/login", status_code=303)
    manual = get_manual(slug)
    if manual is None:
        return RedirectResponse("/manuais", status_code=303)
    return templates.TemplateResponse(
        request,
        "manuals.html",
        {
            "manuals": list_manuals(),
            "manual": manual,
            "body_html": render_manual(manual),
        },
    )
