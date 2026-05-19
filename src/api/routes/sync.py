from typing import Annotated

from fastapi import APIRouter, Depends, HTTPException, status

from src.api.dependencies import get_defense_client, require_admin_token
from src.api.schemas import SyncRequest, SyncResponse
from src.core.logging import logger
from src.services.defense_ia_client import (
    DefenseIAArgumentError,
    DefenseIAClient,
    DefenseIAError,
    DefenseIANotReadyError,
    DefenseIAUnavailableError,
    defense_error_detail_public,
)

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
    if defense_client.settings.enabled and not defense_client.is_ready:
        raise HTTPException(
            status_code=status.HTTP_503_SERVICE_UNAVAILABLE,
            detail="Defense IA não conectado no startup",
        )

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

    try:
        await defense_client.sync_person(payload)
    except DefenseIAArgumentError as exc:
        logger.warning(
            "Argumento inválido source=%s external_id=%s: %s",
            payload.source,
            payload.external_id,
            exc,
        )
        raise HTTPException(
            status_code=status.HTTP_422_UNPROCESSABLE_ENTITY,
            detail="Biometria inválida: imagem facial não reconhecida pelo Defense IA",
        ) from exc
    except DefenseIANotReadyError as exc:
        raise HTTPException(
            status_code=status.HTTP_503_SERVICE_UNAVAILABLE,
            detail=str(exc),
        ) from exc
    except DefenseIAUnavailableError as exc:
        logger.exception(
            "Defense IA sync failed source=%s external_id=%s error=%s",
            payload.source,
            payload.external_id,
            exc,
        )
        raise HTTPException(
            status_code=status.HTTP_502_BAD_GATEWAY,
            detail=defense_error_detail_public(exc),
        ) from exc
    except DefenseIAError as exc:
        logger.exception(
            "Defense IA sync failed source=%s external_id=%s error=%s",
            payload.source,
            payload.external_id,
            exc,
        )
        raise HTTPException(
            status_code=status.HTTP_502_BAD_GATEWAY,
            detail=defense_error_detail_public(exc),
        ) from exc

    logger.info(
        "Defense IA sync succeeded source=%s external_id=%s",
        payload.source,
        payload.external_id,
    )
    return SyncResponse(status="success", message="Dados sincronizados com sucesso")
