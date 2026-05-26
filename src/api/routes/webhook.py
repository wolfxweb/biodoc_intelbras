import json
from typing import Annotated

from fastapi import APIRouter, Depends, Request

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


_REDACTED_HEADERS = {"authorization", "cookie", "set-cookie", "x-api-key"}


def _format_headers_for_log(request: Request) -> str:
    """Serializa headers ofuscando os sensíveis para auditoria do POST."""
    safe = {}
    for name, value in request.headers.items():
        if name.lower() in _REDACTED_HEADERS:
            safe[name] = "***REDACTED***"
        else:
            safe[name] = value
    return json.dumps(safe, ensure_ascii=False)


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
    request: Request,
    payload: BiodocWebhookPayload,
    biodoc_client: Annotated[BiodocClient, Depends(get_biodoc_client)],
    defense_client: Annotated[DefenseIAClient, Depends(get_defense_client)],
) -> BiodocWebhookResponse:
    client_host = request.client.host if request.client else "?"

    logger.info(
        "[WEBHOOK] POST /webhook/biodoc <- %s headers=%s",
        client_host,
        _format_headers_for_log(request),
    )
    logger.info(
        "[WEBHOOK] BODY recebido: %s",
        payload.model_dump_json(),
    )
    logger.info(
        "[WEBHOOK] resumo reference_Id=%s id_Log=%s success=%s status=%s percentage=%s",
        payload.reference_Id,
        payload.id_Log,
        payload.success,
        payload.status,
        payload.percentage,
    )

    result = await process_biodoc_webhook(payload, biodoc_client, defense_client)
    return BiodocWebhookResponse(**result)
