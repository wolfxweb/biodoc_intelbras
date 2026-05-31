import os
from typing import Annotated

from fastapi import Depends, HTTPException, Request, status
from fastapi.security import HTTPAuthorizationCredentials, HTTPBearer

from src.core.logging import logger
from src.core.security import verify_admin_token
from src.services.biodoc_client import BiodocClient
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
    credentials: Annotated[
        HTTPAuthorizationCredentials | None,
        Depends(integration_bearer),
    ] = None,
) -> None:
    auth = resolve_authorization_header(credentials)
    token = extract_bearer_token(auth)
    if not verify_admin_token(token):
        raise HTTPException(
            status_code=status.HTTP_401_UNAUTHORIZED,
            detail="Token administrativo inválido",
        )


async def get_defense_client(request: Request) -> DefenseIAClient:
    return request.app.state.defense_client


async def get_biodoc_client(request: Request) -> BiodocClient:
    return request.app.state.biodoc_client


def verify_biodoc_webhook_token_value(token: str) -> None:
    """Valida token Bearer do webhook BioDoc."""
    import secrets

    expected = os.getenv("BIODOC_WEBHOOK_TOKEN", "")
    if not expected:
        raise HTTPException(
            status_code=status.HTTP_500_INTERNAL_SERVER_ERROR,
            detail="BIODOC_WEBHOOK_TOKEN não configurado no servidor",
        )
    if not token or not secrets.compare_digest(token, expected):
        logger.warning("[WEBHOOK IN] token do webhook recusado (ausente ou inválido)")
        raise HTTPException(
            status_code=status.HTTP_401_UNAUTHORIZED,
            detail="Token do webhook BioDoc inválido",
        )


async def require_biodoc_webhook_token(
    credentials: Annotated[
        HTTPAuthorizationCredentials | None,
        Depends(integration_bearer),
    ] = None,
) -> None:
    """Valida Bearer quando presente. Fluxo urlWebhook do BioDoc pode vir sem Authorization."""
    auth = resolve_authorization_header(credentials)
    if not auth:
        logger.warning(
            "[WEBHOOK IN] POST /webhook/biodoc sem Authorization — "
            "aceito (fluxo urlWebhook BioDoc); configure token no painel para produção"
        )
        return
    try:
        token = extract_bearer_token(auth)
    except HTTPException:
        logger.warning(
            "[WEBHOOK IN] Authorization mal formatado no POST /webhook/biodoc"
        )
        raise
    verify_biodoc_webhook_token_value(token)
