"""Tela autenticada de baixas de visita em GET /."""

from __future__ import annotations

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
from src.models.visitor_leave import VisitorLeaveEvent
from src.services.visitor_leave_store import format_unix, get_webhook_url, set_webhook_url

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
    page: int = 1,
) -> Response:
    if not _is_authenticated(request):
        return RedirectResponse("/login", status_code=303)

    page = max(page, 1)
    page_size = 50
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

    total = query.count()
    pages = max((total + page_size - 1) // page_size, 1)
    page = min(page, pages)
    rows = (
        query.order_by(VisitorLeaveEvent.leave_time.desc())
        .offset((page - 1) * page_size)
        .limit(page_size)
        .all()
    )
    filter_qs = urlencode(
        {
            k: v
            for k, v in {
                "name": name,
                "setor": setor,
                "from": date_from,
                "to": date_to,
            }.items()
            if v
        }
    )
    env_url = os.getenv("VISIT_LEAVE_WEBHOOK_URL", "").strip()
    return templates.TemplateResponse(
        request,
        "list.html",
        {
            "rows": rows,
            "name": name,
            "setor": setor,
            "date_from": date_from,
            "date_to": date_to,
            "page": page,
            "pages": pages,
            "total": total,
            "filter_qs": filter_qs,
            "webhook_url": get_webhook_url(env_url, db=db),
            "saved": request.query_params.get("saved") == "1",
        },
    )


@router.post("/settings/webhook-url")
async def save_webhook_url(
    request: Request,
    db: Annotated[Session, Depends(get_db)],
    webhook_url: Annotated[str, Form()] = "",
) -> RedirectResponse:
    if not _is_authenticated(request):
        return RedirectResponse("/login", status_code=303)
    set_webhook_url(webhook_url, db=db)
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
