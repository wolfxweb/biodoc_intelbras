"""Rotas /defense — GET: callback BioDoc (url=); POST: captura Intelbras (log)."""

from typing import Annotated

from fastapi import APIRouter, Depends, HTTPException, Request
from fastapi.responses import HTMLResponse

from src.api.biodoc_success_page import render_unimed_error_page, render_unimed_success_page
from src.api.dependencies import get_biodoc_client, get_defense_client
from src.services.biodoc_client import BiodocClient
from src.services.defense_callback_service import process_defense_biodoc_callback
from src.services.defense_ia_client import DefenseIAClient

router = APIRouter(tags=["defense"])


@router.get(
    "/defense",
    response_model=None,
    response_class=HTMLResponse,
    summary="Callback BioDoc (url=) — sync Defense IA e página de sucesso Unimed",
    description=(
        "Redirect do navegador após verify BioDoc (`url=` na integração). "
        "Sincroniza no Defense IA e exibe página HTML no padrão Unimed Joinville."
    ),
)
async def defense_biodoc_callback(
    request: Request,
    biodoc_client: Annotated[BiodocClient, Depends(get_biodoc_client)],
    defense_client: Annotated[DefenseIAClient, Depends(get_defense_client)],
) -> HTMLResponse:
    params = request.query_params
    card = params.get("card", "").strip()

    try:
        result = await process_defense_biodoc_callback(
            card=card or None,
            response=params.get("response"),
            event_date=params.get("date"),
            operador=params.get("operador"),
            details=params.get("details"),
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
    "/defense",
    summary="Ingress Intelbras — captura POST e log",
    description=(
        "Recebe POST de integrações Intelbras Defense IA. "
        "Grava método, headers, query e body em `log/app.log`. "
        "Sem autenticação. Não processa nem sincroniza dados."
    ),
)
async def defense_intelbras_ingress(request: Request) -> dict[str, str]:
    _ = request
    return {"status": "ok"}
