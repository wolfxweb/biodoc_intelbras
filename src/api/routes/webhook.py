from typing import Annotated

from fastapi import APIRouter, Depends

from src.api.dependencies import (
    get_biodoc_client,
    get_defense_client,
    require_biodoc_webhook_token,
)
from src.api.schemas_biodoc import BiodocWebhookPayload, BiodocWebhookResponse
from src.core.logging import logger
from src.services.biodoc_client import BiodocClient
from src.services.biodoc_webhook_service import process_biodoc_webhook
from src.services.defense_ia_client import DefenseIAClient

router = APIRouter(
    prefix="/webhook",
    tags=["webhook"],
    dependencies=[Depends(require_biodoc_webhook_token)],
)


@router.post(
    "/biodoc",
    response_model=BiodocWebhookResponse,
    summary="Webhook BioDoc — evento de cadastro/liveness",
    description=(
        "Recebe eventos automáticos do BioDoc após validação de liveness. "
        "Requer `Authorization: Bearer <BIODOC_WEBHOOK_TOKEN>`. "
        "Consulta a API BioDoc, baixa a imagem e sincroniza o beneficiário no Intelbras Defense IA."
    ),
)
async def webhook_biodoc(
    payload: BiodocWebhookPayload,
    biodoc_client: Annotated[BiodocClient, Depends(get_biodoc_client)],
    defense_client: Annotated[DefenseIAClient, Depends(get_defense_client)],
) -> BiodocWebhookResponse:
    logger.info(
        "[WEBHOOK] recebido LogID=%s card=%s success=%s confidence=%s",
        payload.LogID,
        payload.card,
        payload.success,
        payload.confidence,
    )
    result = await process_biodoc_webhook(payload, biodoc_client, defense_client)
    return BiodocWebhookResponse(**result)
