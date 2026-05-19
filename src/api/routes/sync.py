from typing import Annotated

from fastapi import APIRouter, Depends, HTTPException, status
from fastapi.security import HTTPAuthorizationCredentials
from sqlalchemy.orm import Session

from src.api.dependencies import (
    get_defense_client,
    integration_bearer,
    resolve_authorization_header,
    validate_integration_source,
)
from src.api.schemas import SyncRequest, SyncResponse
from src.core.database import get_db
from src.core.logging import logger
from src.services.defense_ia_client import (
    DefenseIAClient,
    DefenseIAError,
    DefenseIANotReadyError,
    DefenseIAUnavailableError,
    defense_error_detail_public,
)

router = APIRouter(prefix="/v1/person", tags=["person"])


@router.post(
    "/sync",
    response_model=SyncResponse,
    summary="Sincronizar pessoa (upsert)",
    description=(
        "Cria ou atualiza pessoa no Intelbras Defense IA (`external_id` = personId). "
        "Use `Authorization: Bearer <ADMIN_API_TOKEN>` e `source` cadastrado. "
        "Se `biometrics.face_image_base64` for enviado, a foto é criada/atualizada."
    ),
)
async def sync_person(
    payload: SyncRequest,
    db: Annotated[Session, Depends(get_db)],
    defense_client: Annotated[DefenseIAClient, Depends(get_defense_client)],
    credentials: Annotated[
        HTTPAuthorizationCredentials | None,
        Depends(integration_bearer),
    ] = None,
) -> SyncResponse:
    auth_header = resolve_authorization_header(credentials)
    validate_integration_source(payload.source, auth_header, db)

    if defense_client.settings.enabled and not defense_client.is_ready:
        raise HTTPException(
            status_code=status.HTTP_503_SERVICE_UNAVAILABLE,
            detail="Defense IA não conectado no startup",
        )

    try:
        await defense_client.sync_person(payload)
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
