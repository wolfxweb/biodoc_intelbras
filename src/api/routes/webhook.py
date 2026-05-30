from typing import Annotated

from fastapi import APIRouter, Depends, Request

from src.api.dependencies import (
    get_biodoc_client,
    get_defense_client,
    require_biodoc_webhook_token,
)
from src.api.schemas_biodoc import BiodocWebhookPayload, BiodocWebhookResponse
from src.core.logging import logger
from src.core.webhook_log import format_payload_summary
from src.services.biodoc_client import BiodocClient
from src.services.biodoc_webhook_service import (
    process_biodoc_webhook,
    webhook_event_succeeded,
)
from src.services.defense_ia_client import DefenseIAClient

router = APIRouter(
    prefix="/webhook",
    tags=["webhook"],
)


@router.post(
    "/biodoc",
    response_model=BiodocWebhookResponse,
    dependencies=[Depends(require_biodoc_webhook_token)],
    summary="Webhook BioDoc — evento de cadastro/liveness",
    description=(
        "Recebe eventos automáticos do BioDoc após validação de liveness. "
        "Requer `Authorization: Bearer <BIODOC_WEBHOOK_TOKEN>`. "
        "Consulta a API BioDoc, baixa a imagem e sincroniza o beneficiário no Intelbras Defense IA."
    ),
)
async def webhook_biodoc(
    request: Request,
    payload: BiodocWebhookPayload,
    biodoc_client: Annotated[BiodocClient, Depends(get_biodoc_client)],
    defense_client: Annotated[DefenseIAClient, Depends(get_defense_client)],
) -> BiodocWebhookResponse:
    _ = request
    if webhook_event_succeeded(payload):
        logger.info(
            format_payload_summary(
                reference_id=payload.reference_Id,
                id_log=payload.id_Log,
                log_id=payload.logId,
                card=payload.card,
                success=payload.success,
                status=payload.status,
                percentage=payload.percentage,
                operador=payload.operador,
                date=payload.date,
                response_code=payload.response,
            )
        )

    result = await process_biodoc_webhook(payload, biodoc_client, defense_client)
    return BiodocWebhookResponse(**result)
