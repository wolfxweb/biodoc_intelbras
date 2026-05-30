import json
from typing import Annotated

from fastapi import APIRouter, Depends, HTTPException, Request, status

from src.api.biodoc_redirect_query import parse_biodoc_redirect_params
from src.api.webhook_audit_middleware import _redact_query_string
from src.api.dependencies import (
    get_biodoc_client,
    get_defense_client,
    require_biodoc_webhook_token,
)
from src.api.schemas_biodoc import BiodocWebhookPayload, BiodocWebhookResponse
from src.core.logging import logger
from src.services.biodoc_client import BiodocClient
from src.services.biodoc_webhook_service import (
    parse_redirect_response_success,
    process_biodoc_webhook,
    process_biodoc_webhook_by_card,
)
from src.services.defense_ia_client import DefenseIAClient

router = APIRouter(
    prefix="/webhook",
    tags=["webhook"],
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


def _format_redirect_query_for_log(
    *,
    card: str | None,
    reference_id: str | None,
    response: str | None,
    message: str | None,
    date: str | None,
    justify_id: str | None,
    id_transaction: str | None,
    name: str | None,
) -> str:
    payload = {
        "card": card,
        "reference_Id": reference_id,
        "response": response,
        "message": message,
        "date": date,
        "justifyId": justify_id,
        "idTransaction": id_transaction,
        "name": name,
        "token": "***REDACTED***",
    }
    return json.dumps(payload, ensure_ascii=False)


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
    client_host = (
        request.headers.get("x-real-ip")
        or request.headers.get("x-forwarded-for", "").split(",")[0].strip()
        or (request.client.host if request.client else "?")
    )

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
        "[WEBHOOK] resumo reference_Id=%s id_Log=%s success=%s status=%s percentage=%s operador=%s",
        payload.reference_Id,
        payload.id_Log,
        payload.success,
        payload.status,
        payload.percentage,
        payload.operador,
    )

    result = await process_biodoc_webhook(payload, biodoc_client, defense_client)
    return BiodocWebhookResponse(**result)


@router.get(
    "/biodoc/redirect",
    response_model=BiodocWebhookResponse,
    summary="Redirect BioDoc — GET com query string",
    description=(
        "Rota para callbacks via redirect do navegador após verificação biométrica. "
        "Sem validação de token. "
        "Informe `reference_Id` (fluxo oficial) ou `card` (consulta mainimage). "
        "Parâmetros extras (`date`, `message`, `response`, etc.) são apenas logados."
    ),
)
async def webhook_biodoc_redirect(
    request: Request,
    biodoc_client: Annotated[BiodocClient, Depends(get_biodoc_client)],
    defense_client: Annotated[DefenseIAClient, Depends(get_defense_client)],
) -> BiodocWebhookResponse:
    try:
        params = parse_biodoc_redirect_params(request.url.query)
    except ValueError as exc:
        logger.warning(
            "[WEBHOOK REDIRECT] query inválida: %s | erro: %s",
            _redact_query_string(request.url.query),
            exc,
        )
        raise HTTPException(
            status_code=status.HTTP_422_UNPROCESSABLE_CONTENT,
            detail=str(exc),
        ) from exc

    client_host = (
        request.headers.get("x-real-ip")
        or request.headers.get("x-forwarded-for", "").split(",")[0].strip()
        or (request.client.host if request.client else "?")
    )
    logger.info(
        "[WEBHOOK REDIRECT] GET /webhook/biodoc/redirect <- %s query=%s",
        client_host,
        _format_redirect_query_for_log(
            card=params.card,
            reference_id=params.reference_id,
            response=params.response,
            message=params.message,
            date=params.date,
            justify_id=params.justify_id,
            id_transaction=params.id_transaction,
            name=params.name,
        ),
    )

    if not parse_redirect_response_success(params.response):
        logger.warning(
            "[WEBHOOK REDIRECT] response=%r indica falha — ignorando sync, retornando 200 para BioDoc",
            params.response,
        )
        return BiodocWebhookResponse(
            status="ignored",
            external_id=params.card or "?",
            defense_sync="skipped",
        )

    if params.reference_id:
        payload = BiodocWebhookPayload(
            id_Log=params.id_log,
            percentage=params.percentage,
            success=True,
            status=params.biodoc_status or 2,
            message=params.message,
            url=params.url,
            reference_Id=params.reference_id,
        )
        result = await process_biodoc_webhook(payload, biodoc_client, defense_client)
        return BiodocWebhookResponse(**result)

    if not params.card:
        raise HTTPException(
            status_code=status.HTTP_422_UNPROCESSABLE_CONTENT,
            detail="Informe 'card' ou 'reference_Id' na query do redirect",
        )

    result = await process_biodoc_webhook_by_card(
        params.card,
        biodoc_client,
        defense_client,
        event_date=params.date,
    )
    return BiodocWebhookResponse(**result)
