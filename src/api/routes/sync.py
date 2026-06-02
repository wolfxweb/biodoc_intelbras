from typing import Annotated

from fastapi import APIRouter, Depends

from src.api.dependencies import get_defense_client, require_admin_token
from src.api.schemas import ManualSyncRequest, SyncResponse
from src.core.logging import logger
from src.services.defense_ia_client import DefenseIAClient
from src.services.defense_sync import sync_to_defense

router = APIRouter(
    prefix="/v1/person",
    tags=["Intelbras Defense — cadastro direto"],
    dependencies=[Depends(require_admin_token)],
)


@router.post(
    "/sync",
    response_model=SyncResponse,
    summary="Cadastro de visitante no Intelbras Defense",
    description=(
        "**Cadastro direto de visitante no Intelbras Defense.** "
        "Cada chamada cria uma nova visita; portas de acesso são resolvidas "
        "automaticamente pelo middleware.\n\n"
        "**Autenticação:** header `Authorization: Bearer <ADMIN_API_TOKEN>`\n\n"
        "**Campos do body:**\n\n"
        "| Campo | Obrigatório | Uso |\n"
        "|-------|-------------|-----|\n"
        "| `source` | Sim | Sistema de origem. Use `biodoc`. |\n"
        "| `operation` | Sim | Sempre `upsert` (nova visita a cada chamada). |\n"
        "| `external_id` | Sim | Rastreio no Defense (`remark`). Alfanumérico, máx. 30 chars. "
        "Ex.: ID do cartão BioDoc. |\n"
        "| `person.full_name` | Sim | Nome completo do visitante. |\n"
        "| `person.document` | Sim | CPF ou outro documento. |\n"
        "| `biometrics` | Não | Omita para cadastro sem foto. |\n"
        "| `biometrics.face_image_base64` | Não | JPEG/PNG em base64 (mín. 1 KB). |\n"
        "| `defense.org_code` | Sim | Sub-organização no Defense. Ex.: `001021`. "
        "Liste códigos com `scripts/list_person_orgs.py`. |\n\n"
        "**Quando usar:** integração direta (ERP, script, homologação), reenvio ou "
        "correção manual."
    ),
)
async def sync_person(
    payload: ManualSyncRequest,
    defense_client: Annotated[DefenseIAClient, Depends(get_defense_client)],
) -> SyncResponse:
    face_info = "none"
    if payload.biometrics and payload.biometrics.face_image_base64:
        face_info = f"<IMG:{len(payload.biometrics.face_image_base64)}_chars>"
    logger.debug(
        "[API IN] source=%s operation=%s external_id=%s org_code=%s "
        "person=%s biometrics.face=%s",
        payload.source,
        payload.operation,
        payload.external_id,
        payload.defense.org_code,
        payload.person.model_dump(),
        face_info,
    )

    sync_result = await sync_to_defense(
        payload,
        defense_client,
        sync_target="visitor",
        org_code=payload.defense.org_code,
        log_context=f"source={payload.source} external_id={payload.external_id}",
    )

    logger.info(
        "Defense IA sync succeeded source=%s external_id=%s visitor_id=%s person_id=%s",
        payload.source,
        payload.external_id,
        sync_result.get("visitor_id"),
        sync_result.get("person_id"),
    )
    return SyncResponse(
        status="success",
        message="Visitante registrado no Intelbras Defense com sucesso",
        visitor_id=sync_result.get("visitor_id"),
        person_id=sync_result.get("person_id"),
    )
