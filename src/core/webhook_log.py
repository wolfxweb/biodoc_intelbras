"""Formatação legível de logs do fluxo webhook BioDoc (debug)."""

from __future__ import annotations

import json
from typing import Any


def _display(value: object) -> str:
    if value is None:
        return "(null)"
    if isinstance(value, str) and not value.strip():
        return "(empty)"
    return str(value)


def truncate_text(text: str, *, max_len: int = 120) -> str:
    if len(text) <= max_len:
        return text
    return f"{text[: max_len - 3]}..."


def format_fields_block(title: str, fields: dict[str, object]) -> str:
    """Bloco alinhado: título + pares chave/valor (multilinha)."""
    if not fields:
        return title
    width = max(len(k) for k in fields)
    lines = [title]
    for key, raw in fields.items():
        value = _display(raw)
        if key in ("image", "url", "mainImage", "path") and len(value) > 120:
            value = truncate_text(value)
        lines.append(f"  {key:<{width}}  {value}")
    return "\n".join(lines)


def format_json_pretty(raw: str | bytes | dict[str, Any] | None) -> str:
    if raw is None or raw == b"" or raw == "":
        return "(empty)"
    if isinstance(raw, dict):
        obj = raw
    elif isinstance(raw, bytes):
        try:
            obj = json.loads(raw.decode("utf-8"))
        except (UnicodeDecodeError, ValueError):
            return truncate_text(repr(raw), max_len=200)
    else:
        try:
            obj = json.loads(raw)
        except (ValueError, TypeError):
            return truncate_text(str(raw), max_len=200)
    text = json.dumps(obj, ensure_ascii=False, indent=2)
    if len(text) > 4000:
        return truncate_text(text, max_len=4000) + "\n  ...[truncado]"
    return text


def format_inbound_request(
    *,
    method: str,
    path: str,
    client: str,
    query: str,
    headers: dict[str, str] | None = None,
    body_preview: str,
    log_tag: str = "[WEBHOOK IN]",
) -> str:
    query_display = query or "(empty)"
    body_block = format_json_pretty(body_preview) if body_preview != "<empty>" else "(empty)"
    lines = [
        f"{log_tag} {method} {path} ← {client}",
        f"  query: {query_display}",
    ]
    if headers:
        lines.append("  headers:")
        lines.append(indent_block(format_json_pretty(headers), spaces=4))
    lines.append("  body:")
    lines.append(indent_block(body_block, spaces=4))
    return "\n".join(lines)


def format_inbound_response(
    *,
    method: str,
    path: str,
    client: str,
    status: int,
    log_tag: str = "[WEBHOOK IN]",
) -> str:
    return f"{log_tag} {method} {path} ← {client} → HTTP {status}"


def indent_block(text: str, *, spaces: int = 2) -> str:
    prefix = " " * spaces
    return "\n".join(f"{prefix}{line}" if line else "" for line in text.splitlines())


def format_payload_summary(
    *,
    reference_id: object,
    id_log: object,
    log_id: object,
    card: object,
    success: object,
    status: object,
    percentage: object,
    operador: object,
    date: object,
    response_code: object,
) -> str:
    return format_fields_block(
        "[WEBHOOK] payload resumido",
        {
            "card": card,
            "success": success,
            "date": date,
            "reference_Id": reference_id,
            "logId": log_id,
            "id_Log": id_log,
            "status": status,
            "response": response_code,
            "percentage": percentage,
            "operador": operador,
        },
    )


def format_flow_step(step: str, **fields: object) -> str:
    return format_fields_block(f"[WEBHOOK] {step}", fields)


def format_biodoc_call(
    *,
    direction: str,
    method: str,
    path: str,
    status: int | None = None,
    fields: dict[str, object] | None = None,
) -> str:
    title = f"[BIODOC {direction}] {method} {path}"
    if status is not None:
        title = f"{title} → HTTP {status}"
    if not fields:
        return title
    return format_fields_block(title, fields)
