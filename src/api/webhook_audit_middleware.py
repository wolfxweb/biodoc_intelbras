"""Auditoria de todas as requisições recebidas em /webhook/* (antes de auth e validação)."""

from __future__ import annotations

import json
import re
from collections.abc import Callable

from starlette.middleware.base import BaseHTTPMiddleware
from starlette.requests import Request
from starlette.responses import Response

from src.core.logging import logger

_MAX_BODY_LOG_BYTES = 64_000
_REDACTED_HEADERS = frozenset(
    {"authorization", "cookie", "set-cookie", "x-api-key", "x-subject-token"}
)
_SENSITIVE_QUERY = re.compile(
    r"(?i)(^|&)(token|authorization|access_token|api_key)=([^&]*)"
)


def _redact_query_string(query: str) -> str:
    if not query:
        return ""
    normalized = query.lstrip("?").replace("?", "&")
    return _SENSITIVE_QUERY.sub(r"\1\2=***REDACTED***", normalized)


def _format_headers(request: Request) -> str:
    safe: dict[str, str] = {}
    for name, value in request.headers.items():
        if name.lower() in _REDACTED_HEADERS:
            safe[name] = "***REDACTED***"
        else:
            safe[name] = value
    return json.dumps(safe, ensure_ascii=False)


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


class WebhookAuditMiddleware(BaseHTTPMiddleware):
    """Grava método, path, query, headers e body de qualquer hit em /webhook."""

    async def dispatch(self, request: Request, call_next: Callable) -> Response:
        path = request.url.path
        if not path.startswith("/webhook"):
            return await call_next(request)

        client = (
            request.headers.get("x-real-ip")
            or request.headers.get("x-forwarded-for", "").split(",")[0].strip()
            or (request.client.host if request.client else "?")
        )
        method = request.method
        query = _redact_query_string(request.url.query)
        headers = _format_headers(request)

        body = await request.body()

        logger.info(
            "[WEBHOOK IN] %s %s <- %s | query=%s | headers=%s | body=%s",
            method,
            path,
            client,
            query or "<empty>",
            headers,
            _format_body_preview(body),
        )

        async def receive() -> dict:
            return {"type": "http.request", "body": body, "more_body": False}

        replay_request = Request(request.scope, receive)
        response = await call_next(replay_request)

        logger.info(
            "[WEBHOOK IN] %s %s <- %s | status=%d",
            method,
            path,
            client,
            response.status_code,
        )
        return response
