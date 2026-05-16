from typing import Annotated

from fastapi import APIRouter, Depends, Header, HTTPException, status
from sqlalchemy.orm import Session

from src.api.dependencies import get_defense_client, validate_integration_source
from src.api.schemas import SyncRequest, SyncResponse
from src.core.database import get_db
from src.core.logging import logger
from src.services.defense_ia_client import DefenseIAClient, DefenseIAError

router = APIRouter(prefix="/v1/person", tags=["person"])


@router.post("/sync", response_model=SyncResponse)
async def sync_person(
    payload: SyncRequest,
    db: Annotated[Session, Depends(get_db)],
    defense_client: Annotated[DefenseIAClient, Depends(get_defense_client)],
    authorization: Annotated[str | None, Header()] = None,
) -> SyncResponse:
    validate_integration_source(payload.source, authorization, db)
    try:
        await defense_client.sync_person(payload)
    except DefenseIAError as exc:
        logger.exception(
            "Defense IA sync failed source=%s external_id=%s error=%s",
            payload.source,
            payload.external_id,
            exc,
        )
        raise HTTPException(
            status_code=status.HTTP_502_BAD_GATEWAY,
            detail="API do Defense IA indisponível",
        ) from exc

    logger.info(
        "Defense IA sync succeeded source=%s external_id=%s",
        payload.source,
        payload.external_id,
    )
    return SyncResponse(status="success", message="Dados sincronizados com sucesso")
