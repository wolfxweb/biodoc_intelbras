"""Auditoria de ingress: GET/POST /defense (BioDoc callback e Intelbras)."""

from __future__ import annotations

import re
from collections.abc import Callable

from starlette.middleware.base import BaseHTTPMiddleware
from starlette.requests import Request
from starlette.responses import Response

from src.core.logging import logger
from src.core.webhook_log import format_inbound_request, format_inbound_response

_MAX_BODY_LOG_BYTES = 64_000
_REDACTED_HEADERS = frozenset(
    {"authorization", "cookie", "set-cookie", "x-api-key", "x-subject-token"}
)
_SENSITIVE_QUERY = re.compile(
    r"(?i)(^|&)(token|authorization|access_token|api_key)=([^&]*)"
)

_CAPTURE_PATH = "/defense"
_LOG_TAG = "[DEFENSE IN]"


def _redact_query_string(query: str) -> str:
    if not query:
        return ""
    normalized = query.lstrip("?").replace("?", "&")
    return _SENSITIVE_QUERY.sub(r"\1\2=***REDACTED***", normalized)


def _format_body_preview(raw: bytes) -> str:
    if not raw:
        return "<empty>"
    clipped = raw[:_MAX_BODY_LOG_BYTES]
    suffix = " ...[truncado]" if len(raw) > _MAX_BODY_LOG_BYTES else ""
    try:
        text = clipped.decode("utf-8")
    except UnicodeDecodeError:
        return f"<binary {len(raw)} bytes>{suffix}"
    return text + suffix


def _format_headers(request: Request) -> dict[str, str]:
    safe: dict[str, str] = {}
    for name, value in request.headers.items():
        if name.lower() in _REDACTED_HEADERS:
            safe[name] = "***REDACTED***"
        else:
            safe[name] = value
    return safe


def _should_audit_request(path: str, method: str) -> bool:
    return path == _CAPTURE_PATH and method.upper() in {"GET", "POST"}


class WebhookAuditMiddleware(BaseHTTPMiddleware):
    """Grava hits em GET/POST /defense ([DEFENSE IN])."""

    async def dispatch(self, request: Request, call_next: Callable) -> Response:
        path = request.url.path
        method = request.method
        if not _should_audit_request(path, method):
            return await call_next(request)

        client = (
            request.headers.get("x-real-ip")
            or request.headers.get("x-forwarded-for", "").split(",")[0].strip()
            or (request.client.host if request.client else "?")
        )
        query = _redact_query_string(request.url.query)

        body = await request.body()

        logger.info(
            format_inbound_request(
                method=method,
                path=path,
                client=client,
                query=query,
                headers=_format_headers(request),
                body_preview=_format_body_preview(body),
                log_tag=_LOG_TAG,
            )
        )

        async def receive() -> dict:
            return {"type": "http.request", "body": body, "more_body": False}

        replay_request = Request(request.scope, receive)
        response = await call_next(replay_request)

        logger.info(
            format_inbound_response(
                method=method,
                path=path,
                client=client,
                status=response.status_code,
                log_tag=_LOG_TAG,
            )
        )
        return response
