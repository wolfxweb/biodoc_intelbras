"""GET/POST /biodoc — callback BioDoc (url=) e ingress Intelbras (log)."""

from typing import Annotated

from fastapi import APIRouter, Depends, HTTPException, Request
from fastapi.responses import HTMLResponse

from src.api.biodoc_success_page import render_unimed_error_page, render_unimed_success_page
from src.api.dependencies import get_biodoc_client, get_defense_client
from src.core.webhook_query import parse_biodoc_redirect_query
from src.services.biodoc_client import BiodocClient
from src.services.defense_callback_service import process_defense_biodoc_callback
from src.services.defense_ia_client import DefenseIAClient

router = APIRouter(
    tags=["webhook"],
)


@router.get(
    "/biodoc",
    response_model=None,
    response_class=HTMLResponse,
    summary="Callback BioDoc (url=) — sync Defense IA e página de sucesso Unimed",
    description=(
        "Redirect do navegador após verify BioDoc (`url=` na integração). "
        "Sincroniza no Defense IA e exibe página HTML no padrão Unimed Joinville."
    ),
)
async def webhook_biodoc_callback(
    request: Request,
    biodoc_client: Annotated[BiodocClient, Depends(get_biodoc_client)],
    defense_client: Annotated[DefenseIAClient, Depends(get_defense_client)],
) -> HTMLResponse:
    params = parse_biodoc_redirect_query(request.url.query)
    card = params.get("card", "").strip()

    try:
        result = await process_defense_biodoc_callback(
            card=card or None,
            response=params.get("response"),
            event_date=params.get("date"),
            reference_id=params.get("reference_Id"),
            reference_id_alt=params.get("reference_id"),
            reference_id_camel=params.get("referenceId"),
            log_id=params.get("logId"),
            id_log=params.get("id_Log"),
            id_transaction=params.get("idTransaction"),
            details=params.get("details"),
            detail=params.get("detail"),
            org_code=params.get("org_code"),
            operador=params.get("operador"),
            local=params.get("local"),
            local_de_acesso=params.get("local_de_acesso"),
            acesso=params.get("acesso"),
            biodoc_client=biodoc_client,
            defense_client=defense_client,
        )
    except HTTPException as exc:
        return HTMLResponse(
            content=render_unimed_error_page(
                title="Verificação registrada",
                message=str(exc.detail),
            ),
            status_code=exc.status_code,
        )

    if result.get("status") == "ignored":
        return HTMLResponse(
            content=render_unimed_error_page(
                title="Verificação não concluída",
                message="A validação biométrica não foi concluída com sucesso.",
            ),
            status_code=200,
        )

    return HTMLResponse(
        content=render_unimed_success_page(
            card=result.get("external_id") or card,
            name=result.get("name"),
            local_name=result.get("local_name"),
        ),
        status_code=200,
    )


@router.post(
    "/biodoc",
    summary="Ingress Intelbras — captura POST e log",
    description=(
        "Recebe POST de integrações Intelbras Defense IA. "
        "Grava método, headers, query e body em `log/app.log`. "
        "Sem autenticação. Não processa nem sincroniza dados."
    ),
)
async def webhook_biodoc_ingress(request: Request) -> dict[str, str]:
    _ = request
    return {"status": "ok"}
