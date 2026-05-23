from typing import Annotated

from fastapi import APIRouter, Depends

from src.api.dependencies import get_defense_client, require_admin_token
from src.api.schemas import SyncRequest, SyncResponse
from src.core.logging import logger
from src.services.defense_ia_client import DefenseIAClient
from src.services.defense_sync import sync_to_defense

router = APIRouter(
    prefix="/v1/person",
    tags=["person"],
    dependencies=[Depends(require_admin_token)],
)


@router.post(
    "/sync",
    response_model=SyncResponse,
    summary="Sincronizar pessoa (upsert)",
    description=(
        "Cria ou atualiza pessoa no Intelbras Defense IA (`external_id` = personId). "
        "Use `Authorization: Bearer <ADMIN_API_TOKEN>` e `source` da lista permitida. "
        "Se `biometrics.face_image_base64` for enviado, a foto é criada/atualizada."
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
    return SyncResponse(status="success", message="Dados sincronizados com sucesso")
