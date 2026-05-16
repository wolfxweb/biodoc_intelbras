from typing import Annotated

from fastapi import APIRouter, Depends, HTTPException, status
from sqlalchemy.exc import IntegrityError
from sqlalchemy.orm import Session

from src.api.dependencies import require_admin_token
from src.api.schemas import (
    IntegrationSourceCreate,
    IntegrationSourceCreated,
    IntegrationSourceRead,
    IntegrationSourceUpdate,
    IntegrationSourceUpdated,
)
from src.core.database import get_db
from src.core.security import generate_integration_key, hash_token
from src.models.integration_source import IntegrationSource

router = APIRouter(
    prefix="/v1/integration-sources",
    tags=["integration-sources"],
    dependencies=[Depends(require_admin_token)],
)


@router.post(
    "",
    response_model=IntegrationSourceCreated,
    status_code=status.HTTP_201_CREATED,
)
async def create_integration_source(
    payload: IntegrationSourceCreate,
    db: Annotated[Session, Depends(get_db)],
) -> IntegrationSourceCreated:
    integration_key = generate_integration_key()
    source = IntegrationSource(
        name=payload.name,
        token_hash=hash_token(integration_key),
        is_active=payload.is_active,
    )
    db.add(source)
    try:
        db.commit()
    except IntegrityError as exc:
        db.rollback()
        raise HTTPException(
            status_code=status.HTTP_409_CONFLICT,
            detail="source já cadastrado",
        ) from exc
    db.refresh(source)
    return IntegrationSourceCreated(
        id=source.id,
        name=source.name,
        is_active=source.is_active,
        integration_key=integration_key,
    )


@router.get("", response_model=list[IntegrationSourceRead])
async def list_integration_sources(
    db: Annotated[Session, Depends(get_db)],
) -> list[IntegrationSource]:
    return db.query(IntegrationSource).order_by(IntegrationSource.id).all()


@router.patch("/{source_id}", response_model=IntegrationSourceUpdated)
async def update_integration_source(
    source_id: int,
    payload: IntegrationSourceUpdate,
    db: Annotated[Session, Depends(get_db)],
) -> IntegrationSourceUpdated:
    source = db.get(IntegrationSource, source_id)
    if source is None:
        raise HTTPException(
            status_code=status.HTTP_404_NOT_FOUND,
            detail="source não cadastrado",
        )

    if payload.name is not None:
        source.name = payload.name
    if payload.is_active is not None:
        source.is_active = payload.is_active

    integration_key = None
    if payload.rotate_token:
        integration_key = generate_integration_key()
        source.token_hash = hash_token(integration_key)

    try:
        db.commit()
    except IntegrityError as exc:
        db.rollback()
        raise HTTPException(
            status_code=status.HTTP_409_CONFLICT,
            detail="source já cadastrado",
        ) from exc
    db.refresh(source)
    return IntegrationSourceUpdated(
        id=source.id,
        name=source.name,
        is_active=source.is_active,
        integration_key=integration_key,
    )


@router.delete("/{source_id}", status_code=status.HTTP_204_NO_CONTENT)
async def delete_integration_source(
    source_id: int,
    db: Annotated[Session, Depends(get_db)],
) -> None:
    source = db.get(IntegrationSource, source_id)
    if source is None:
        raise HTTPException(
            status_code=status.HTTP_404_NOT_FOUND,
            detail="source não cadastrado",
        )
    source.is_active = False
    db.commit()
