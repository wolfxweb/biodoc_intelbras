from typing import Annotated

from fastapi import APIRouter, Depends, HTTPException, Request, status
from fastapi.responses import HTMLResponse, RedirectResponse

from src.api.biodoc_redirect_query import (
    _operador_from_details_raw,
    parse_biodoc_redirect_params,
)
from src.api.webhook_audit_middleware import _redact_query_string
from src.api.dependencies import (
    get_biodoc_client,
    get_defense_client,
    require_biodoc_webhook_token,
)
from src.api.schemas_biodoc import BiodocWebhookPayload, BiodocWebhookResponse
from src.core.logging import logger
from src.core.webhook_log import format_fields_block, format_payload_summary
from src.services.biodoc_client import BiodocClient
from src.services.biodoc_webhook_service import (
    parse_redirect_response_success,
    process_biodoc_webhook,
    process_biodoc_webhook_by_card,
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
        format_fields_block(
            f"[WEBHOOK REDIRECT] GET /webhook/biodoc/redirect ← {client_host}",
            {
                "card": params.card,
                "reference_Id": params.reference_id,
                "response": params.response,
                "message": params.message,
                "date": params.date,
                "justifyId": params.justify_id,
                "idTransaction": params.id_transaction,
                "name": params.name,
            },
        )
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
        required_name=params.operador,
    )
    return BiodocWebhookResponse(**result)


@router.get(
    "/biodoc",
    summary="Callback BioDoc no navegador (parâmetro url= na verify)",
    description=(
        "Redirect para página de sucesso após verify. O sync no Defense usa "
        "GET /webhook/sucesso (external-audits → integrations/log)."
    ),
)
async def webhook_biodoc_browser_callback(
    request: Request,
) -> RedirectResponse:
    try:
        params = parse_biodoc_redirect_params(request.url.query)
    except ValueError as exc:
        logger.warning(
            "[WEBHOOK REDIRECT] GET /webhook/biodoc query inválida: %s",
            exc,
        )
        raise HTTPException(
            status_code=status.HTTP_422_UNPROCESSABLE_CONTENT,
            detail=str(exc),
        ) from exc

    if not params.card:
        raise HTTPException(
            status_code=status.HTTP_422_UNPROCESSABLE_CONTENT,
            detail="Parâmetro 'card' ausente na query",
        )

    if not parse_redirect_response_success(params.response):
        logger.warning(
            "[WEBHOOK REDIRECT] GET /webhook/biodoc card=%s response=%r — sem redirect sucesso",
            params.card,
            params.response,
        )
        raise HTTPException(
            status_code=status.HTTP_422_UNPROCESSABLE_CONTENT,
            detail="Verificação BioDoc não concluída com sucesso",
        )

    logger.info(
        "[WEBHOOK REDIRECT] card=%s response=%s — GET /webhook/biodoc → /webhook/sucesso",
        params.card,
        params.response,
    )
    return RedirectResponse(
        url=f"/webhook/sucesso?card={params.card}",
        status_code=status.HTTP_302_FOUND,
    )


@router.get(
    "/sucesso",
    response_class=HTMLResponse,
    summary="Página de sucesso + sync Defense (fluxo url= do BioDoc)",
)
async def webhook_biodoc_sucesso(
    request: Request,
    biodoc_client: Annotated[BiodocClient, Depends(get_biodoc_client)],
    defense_client: Annotated[DefenseIAClient, Depends(get_defense_client)],
) -> HTMLResponse:
    card = request.query_params.get("card", "").strip()
    if not card:
        raise HTTPException(
            status_code=status.HTTP_422_UNPROCESSABLE_CONTENT,
            detail="Parâmetro 'card' ausente",
        )

    client_host = (
        request.headers.get("x-real-ip")
        or request.headers.get("x-forwarded-for", "").split(",")[0].strip()
        or (request.client.host if request.client else "?")
    )
    logger.info("[WEBHOOK SUCESSO] GET /webhook/sucesso card=%s ← %s", card, client_host)

    date = request.query_params.get("date")
    operador = request.query_params.get("operador")
    details = request.query_params.get("details")
    if not operador and details:
        operador = _operador_from_details_raw(details)

    try:
        await process_biodoc_webhook_by_card(
            card,
            biodoc_client,
            defense_client,
            event_date=date,
            required_name=operador,
        )
    except HTTPException as exc:
        logger.warning(
            "[WEBHOOK SUCESSO] card=%s sync Defense falhou: %s",
            card,
            exc.detail,
        )
        return HTMLResponse(
            content=(
                "<!DOCTYPE html><html><body>"
                f"<h1>Verificação registrada</h1>"
                f"<p>Não foi possível sincronizar no Defense: {exc.detail}</p>"
                "</body></html>"
            ),
            status_code=exc.status_code,
        )

    return HTMLResponse(
        content=(
            "<!DOCTYPE html><html><body>"
            "<h1>Verificação concluída</h1>"
            f"<p>Beneficiário {card} sincronizado no Defense IA.</p>"
            "</body></html>"
        ),
        status_code=status.HTTP_200_OK,
    )
