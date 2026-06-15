"""Callback GET /defense — verify BioDoc (parâmetro url=) → BioDoc API → Defense IA."""

from __future__ import annotations

import json

from fastapi import HTTPException, status

from src.core.logging import logger
from src.services.biodoc_client import (
    BiodocClient,
    _extract_operador_from_detail,
)
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
    return _extract_operador_from_detail(parsed)


def _operador_from_plain_query(*values: str | None) -> str | None:
    for raw in values:
        if raw is None:
            continue
        text = str(raw).strip()
        if not text or text.lower() in ("null", "undefined", "none"):
            continue
        return text
    return None


def operador_from_callback_query(
    *,
    details: str | None = None,
    detail: str | None = None,
    org_code: str | None = None,
    operador: str | None = None,
    local: str | None = None,
    local_de_acesso: str | None = None,
    acesso: str | None = None,
) -> str | None:
    """Regra de acesso / porta (Defense visitedName) vinda da URL do redirect.

    Prioridade:
    1. `details` / `detail` (JSON) — operador, usuclin, org_code
    2. Parâmetros diretos: org_code, operador, local, local_de_acesso, acesso

    Exemplo na URL verify (parâmetro url=):
    https://un.wolfx.com.br/biodoc?org_code=VIVER
    """
    for raw in (details, detail):
        value = _operador_from_details_raw(raw)
        if value:
            return value
    return _operador_from_plain_query(
        org_code,
        operador,
        local,
        local_de_acesso,
        acesso,
    )


def _reference_id_from_query(*values: str | None) -> str | None:
    for raw in values:
        if raw is None:
            continue
        text = str(raw).strip()
        if not text or text.lower() in ("null", "undefined", "none"):
            continue
        return text
    return None


def reference_id_from_callback_query(
    *,
    reference_id: str | None = None,
    reference_id_alt: str | None = None,
    reference_id_camel: str | None = None,
    log_id: str | None = None,
    id_log: str | None = None,
    id_transaction: str | None = None,
) -> str | None:
    """Identificador do log BioDoc para GET /integrations/log/{id} → detail.operador."""
    return _reference_id_from_query(
        reference_id,
        reference_id_alt,
        reference_id_camel,
        log_id,
        id_log,
        id_transaction,
    )


async def process_defense_biodoc_callback(
    *,
    card: str | None,
    response: str | None,
    event_date: str | None,
    reference_id: str | None = None,
    log_id: str | None = None,
    id_log: str | None = None,
    id_transaction: str | None = None,
    reference_id_alt: str | None = None,
    reference_id_camel: str | None = None,
    details: str | None = None,
    detail: str | None = None,
    org_code: str | None = None,
    operador: str | None = None,
    local: str | None = None,
    local_de_acesso: str | None = None,
    acesso: str | None = None,
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

    effective_reference_id = reference_id_from_callback_query(
        reference_id=reference_id,
        reference_id_alt=reference_id_alt,
        reference_id_camel=reference_id_camel,
        log_id=log_id,
        id_log=id_log,
        id_transaction=id_transaction,
    )
    details_operador_hint = operador_from_callback_query(
        details=details,
        detail=detail,
        org_code=org_code,
        operador=operador,
        local=local,
        local_de_acesso=local_de_acesso,
        acesso=acesso,
    )
    logger.info(
        "[DEFENSE] card=%s date=%r reference_id=%r operador_requisicao=%r — iniciando sync Defense IA",
        card_value,
        event_date,
        effective_reference_id,
        details_operador_hint,
    )

    return await process_biodoc_webhook_by_card(
        card_value,
        biodoc_client,
        defense_client,
        event_date=event_date,
        reference_id=effective_reference_id,
        details_operador_hint=details_operador_hint,
    )
