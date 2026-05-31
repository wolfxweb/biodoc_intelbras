from typing import Annotated

from fastapi import APIRouter, Depends

from src.api.dependencies import get_defense_client, require_admin_token
from src.api.schemas import SyncRequest, SyncResponse
from src.core.logging import logger
from src.services.defense_ia_client import DefenseIAClient
from src.services.defense_sync import sync_to_defense

router = APIRouter(
    prefix="/v1/person",
    tags=["Defense IA — cadastro manual"],
    dependencies=[Depends(require_admin_token)],
)


@router.post(
    "/sync",
    response_model=SyncResponse,
    summary="Enviar usuário para o Intelbras Defense IA (cadastro/atualização)",
    description=(
        "**Cadastro manual de pessoa no Defense IA** — use esta rota para enviar ou "
        "atualizar um usuário diretamente, sem passar pelo fluxo BioDoc.\n\n"
        "O middleware faz **upsert** no Defense: cria a pessoa se não existir ou "
        "atualiza se o `external_id` já estiver cadastrado (`personId` no Defense).\n\n"
        "**Autenticação:** `Authorization: Bearer <ADMIN_API_TOKEN>`\n\n"
        "**Quando usar:**\n"
        "- Integração direta (ERP, script, teste) enviando nome, documento e foto\n"
        "- Reenvio ou correção manual de cadastro no Defense\n\n"
        "**Fluxo BioDoc (não use esta rota):**\n"
        "- Após verify BioDoc no navegador → `GET /defense` (sync automático)\n\n"
        "Com `biometrics.face_image_base64`, a foto facial é enviada ou substituída "
        "no Defense. Sem foto, cadastra ou atualiza somente os dados da pessoa."
    ),
)
async def sync_person(
    payload: SyncRequest,
    defense_client: Annotated[DefenseIAClient, Depends(get_defense_client)],
) -> SyncResponse:
    face_info = "none"
    if payload.biometrics and payload.biometrics.face_image_base64:
        face_info = f"<IMG:{len(payload.biometrics.face_image_base64)}_chars>"
    logger.debug(
        "[API IN] source=%s operation=%s external_id=%s person=%s biometrics.face=%s",
        payload.source,
        payload.operation,
        payload.external_id,
        payload.person.model_dump(),
        face_info,
    )

    await sync_to_defense(
        payload,
        defense_client,
        log_context=f"source={payload.source} external_id={payload.external_id}",
    )

    logger.info(
        "Defense IA sync succeeded source=%s external_id=%s",
        payload.source,
        payload.external_id,
    )
    return SyncResponse(
        status="success",
        message="Usuário enviado ao Intelbras Defense IA com sucesso",
    )
