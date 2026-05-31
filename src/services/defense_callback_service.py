"""Callback GET /defense — verify BioDoc (parâmetro url=) → BioDoc API → Defense IA."""

from __future__ import annotations

import json

from fastapi import HTTPException, status

from src.core.logging import logger
from src.services.biodoc_client import BiodocClient
from src.services.biodoc_webhook_service import process_biodoc_webhook_by_card
from src.services.defense_ia_client import DefenseIAClient


def _callback_response_success(response: str | None) -> bool:
    if response is None or not str(response).strip():
        return True
    normalized = str(response).strip().lower()
    if normalized in ("200", "201", "204", "true", "1", "ok", "success"):
        return True
    if normalized in ("false", "0", "fail", "error", "null", "undefined"):
        return False
    try:
        code = int(normalized)
        return 200 <= code < 300
    except ValueError:
        return False


def _operador_from_details_raw(details: str | None) -> str | None:
    if not details or not details.strip():
        return None
    text = details.strip()
    if text.lower() in ("null", "undefined"):
        return None
    try:
        parsed = json.loads(text)
    except json.JSONDecodeError:
        return None
    if not isinstance(parsed, dict):
        return None
    for key in ("operador", "operator", "grupo"):
        raw = parsed.get(key)
        if raw is not None and str(raw).strip():
            return str(raw).strip()
    return None


async def process_defense_biodoc_callback(
    *,
    card: str | None,
    response: str | None,
    event_date: str | None,
    operador: str | None,
    details: str | None,
    biodoc_client: BiodocClient,
    defense_client: DefenseIAClient,
) -> dict:
    card_value = (card or "").strip()
    if not card_value:
        raise HTTPException(
            status_code=status.HTTP_422_UNPROCESSABLE_CONTENT,
            detail="Parâmetro 'card' ausente na query",
        )

    if not _callback_response_success(response):
        logger.warning(
            "[DEFENSE] card=%s response=%r — verify não concluída, sync ignorado",
            card_value,
            response,
        )
        return {
            "status": "ignored",
            "external_id": card_value,
            "defense_sync": "skipped",
        }

    effective_operador = operador or _operador_from_details_raw(details)
    logger.info(
        "[DEFENSE] card=%s date=%r — iniciando external-audits → Defense IA",
        card_value,
        event_date,
    )

    return await process_biodoc_webhook_by_card(
        card_value,
        biodoc_client,
        defense_client,
        event_date=event_date,
        required_name=effective_operador,
    )
