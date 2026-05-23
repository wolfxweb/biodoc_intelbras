"""
Helper compartilhado para sincronizar um SyncRequest no Defense IA
e mapear as exceções do cliente para HTTPException do FastAPI.

Usado por:
  - src/api/routes/sync.py  (rota POST /v1/person/sync)
  - src/services/biodoc_webhook_service.py  (fluxo webhook BioDoc)
"""

from __future__ import annotations

from fastapi import HTTPException, status

from src.api.schemas import SyncRequest
from src.core.logging import logger
from src.services.defense_ia_client import (
    DefenseIAArgumentError,
    DefenseIAClient,
    DefenseIAError,
    DefenseIANotReadyError,
    DefenseIAUnavailableError,
    defense_error_detail_public,
)


async def sync_to_defense(
    sync_request: SyncRequest,
    defense_client: DefenseIAClient,
    *,
    log_context: str = "",
) -> None:
    """
    Envia um SyncRequest para o Defense IA e converte erros em HTTPException.

    Args:
        sync_request: payload validado (source, operation, external_id, person, biometrics)
        defense_client: cliente Intelbras já iniciado
        log_context: prefixo opcional para mensagens de log (ex: "[WEBHOOK] LogID=abc")
    """
    prefix = f"{log_context} " if log_context else ""

    if defense_client.settings.enabled and not defense_client.is_ready:
        raise HTTPException(
            status_code=status.HTTP_503_SERVICE_UNAVAILABLE,
            detail="Defense IA não conectado",
        )

    try:
        await defense_client.sync_person(sync_request)
    except DefenseIAArgumentError as exc:
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
        raise HTTPException(
            status_code=status.HTTP_502_BAD_GATEWAY,
            detail=defense_error_detail_public(exc),
        ) from exc
