"""
Helper compartilhado para sincronizar um SyncRequest no Defense IA
e mapear as exceções do cliente para HTTPException do FastAPI.

Usado por:
  - src/api/routes/sync.py  (rota POST /v1/person/sync)
  - src/services/biodoc_webhook_service.py  (fluxo webhook BioDoc)
"""

from __future__ import annotations

from typing import Any, Literal

from fastapi import HTTPException, status

from src.api.schemas import SyncRequest
from src.core.logging import logger
from src.services.defense_ia_client import (
    FACE_SIZE_LIMIT_PUBLIC_DETAIL,
    SESSION_CONFLICT_PUBLIC_DETAIL,
    DefenseIAArgumentError,
    DefenseIAClient,
    DefenseIAError,
    DefenseIANotReadyError,
    DefenseIAUnavailableError,
    defense_error_detail_public,
    extract_sync_result_ids,
    is_face_size_limit_error,
    is_session_conflict_error,
)


async def sync_to_defense(
    sync_request: SyncRequest,
    defense_client: DefenseIAClient,
    *,
    sync_target: Literal["visitor", "person"] = "visitor",
    org_code: str | None = None,
    access_rule_name: str | None = None,
    visited_name: str | None = None,
    acs_channel_ids: list[str] | None = None,
    log_context: str = "",
) -> dict[str, Any]:
    """
    Envia um SyncRequest para o Defense IA e converte erros em HTTPException.

    Returns:
        Dict com visitor_id/person_id (quando o Defense retornar) e corpo bruto em ``defense_result``.
    """
    prefix = f"{log_context} " if log_context else ""

    if defense_client.settings.enabled and not defense_client.is_ready:
        raise HTTPException(
            status_code=status.HTTP_503_SERVICE_UNAVAILABLE,
            detail="Defense IA não conectado",
        )

    rule = (access_rule_name or org_code or "").strip()
    host = (visited_name or org_code or "").strip()
    if not host:
        host = (defense_client.settings.visited_name or "").strip()
    if not rule and not host:
        rule = host
    resolved_org = org_code or defense_client.settings.org_code or "001"
    post_visited_name = host or rule

    try:
        if sync_target == "visitor":
            logger.info(
                "%ssync visitor external_id=%s access_rule=%s visited_name=%s "
                "acs_channel_ids=%s",
                prefix,
                sync_request.external_id,
                rule or "(n/a)",
                post_visited_name,
                acs_channel_ids if acs_channel_ids is not None else "(auto)",
            )
            defense_result = await defense_client.sync_visitor(
                sync_request,
                host or None,
                access_rule_name=rule or None,
                entrance_ids=acs_channel_ids,
            )
        else:
            logger.info(
                "%ssync person external_id=%s orgCode=%s",
                prefix,
                sync_request.external_id,
                resolved_org,
            )
            defense_result = await defense_client.sync_person(sync_request, resolved_org)
    except DefenseIAArgumentError as exc:
        detail = str(exc)
        if is_face_size_limit_error(exc) or "100 KB" in detail:
            logger.warning(
                "%sfoto facial acima do limite Defense IA external_id=%s: %s",
                prefix,
                sync_request.external_id,
                exc,
            )
            raise HTTPException(
                status_code=status.HTTP_422_UNPROCESSABLE_CONTENT,
                detail=FACE_SIZE_LIMIT_PUBLIC_DETAIL,
            ) from exc
        if "Host visitante" in detail or "acsChannelId" in detail:
            logger.warning(
                "%sconfiguração visitante inválida external_id=%s: %s",
                prefix,
                sync_request.external_id,
                exc,
            )
            raise HTTPException(
                status_code=status.HTTP_422_UNPROCESSABLE_CONTENT,
                detail=detail,
            ) from exc
        logger.warning(
            "%simagem inválida para Defense IA external_id=%s: %s",
            prefix,
            sync_request.external_id,
            exc,
        )
        raise HTTPException(
            status_code=status.HTTP_422_UNPROCESSABLE_CONTENT,
            detail="Biometria inválida: imagem facial não reconhecida pelo Defense IA",
        ) from exc
    except DefenseIANotReadyError as exc:
        raise HTTPException(
            status_code=status.HTTP_503_SERVICE_UNAVAILABLE,
            detail=str(exc),
        ) from exc
    except (DefenseIAUnavailableError, DefenseIAError) as exc:
        logger.exception(
            "%sfalha ao sincronizar external_id=%s no Defense IA: %s",
            prefix,
            sync_request.external_id,
            exc,
        )
        if is_session_conflict_error(exc):
            raise HTTPException(
                status_code=status.HTTP_503_SERVICE_UNAVAILABLE,
                detail=SESSION_CONFLICT_PUBLIC_DETAIL,
            ) from exc
        raise HTTPException(
            status_code=status.HTTP_502_BAD_GATEWAY,
            detail=defense_error_detail_public(exc),
        ) from exc

    ids = extract_sync_result_ids(defense_result)
    return {
        **ids,
        "visited_name": post_visited_name,
        "org_code": rule or post_visited_name,
        "defense_result": defense_result,
    }
