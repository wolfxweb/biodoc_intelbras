from typing import Annotated

from fastapi import Depends, Header, HTTPException, Request, status
from fastapi.security import HTTPAuthorizationCredentials, HTTPBearer
from sqlalchemy.orm import Session

from src.core.database import get_db
from src.core.security import verify_admin_token, verify_token
from src.models.integration_source import IntegrationSource
from src.services.defense_ia_client import DefenseIAClient

integration_bearer = HTTPBearer(
    auto_error=False,
    description="Token do middleware (ADMIN_API_TOKEN).",
)


def resolve_authorization_header(
    credentials: HTTPAuthorizationCredentials | None = None,
) -> str | None:
    if credentials is not None:
        return f"Bearer {credentials.credentials}"
    return None


def extract_bearer_token(authorization: str | None) -> str:
    if not authorization or not authorization.strip():
        raise HTTPException(
            status_code=status.HTTP_401_UNAUTHORIZED,
            detail=(
                "Authorization ausente: informe Bearer <ADMIN_API_TOKEN> "
                "(token do middleware/BIODOC, não o token Intelbras Defense)"
            ),
        )
    value = authorization.strip()
    if value.lower().startswith("bearer "):
        return value[7:].strip()
    return value


async def require_admin_token(
    authorization: Annotated[str | None, Header()] = None,
) -> None:
    token = extract_bearer_token(authorization)
    if not verify_admin_token(token):
        raise HTTPException(
            status_code=status.HTTP_401_UNAUTHORIZED,
            detail="Token administrativo inválido",
        )


def validate_integration_source(
    source: str,
    authorization: str | None,
    db: Session,
) -> IntegrationSource:
    integration_source = (
        db.query(IntegrationSource)
        .filter(IntegrationSource.name == source)
        .filter(IntegrationSource.is_active.is_(True))
        .first()
    )
    if integration_source is None:
        raise HTTPException(
            status_code=status.HTTP_400_BAD_REQUEST,
            detail="source não cadastrado",
        )

    token = extract_bearer_token(authorization)
    if not (
        verify_token(token, integration_source.token_hash)
        or verify_admin_token(token)
    ):
        raise HTTPException(
            status_code=status.HTTP_401_UNAUTHORIZED,
            detail="Token ou source inválido",
        )

    return integration_source


async def get_defense_client(request: Request) -> DefenseIAClient:
    return request.app.state.defense_client
