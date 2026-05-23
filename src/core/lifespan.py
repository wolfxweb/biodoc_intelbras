import os
from contextlib import asynccontextmanager

from fastapi import FastAPI

from src.core.logging import logger
from src.services.biodoc_client import BiodocClient
from src.services.defense_ia_client import (
    DefenseIAClient,
    DefenseIASettings,
)


def build_defense_client_from_env() -> DefenseIAClient:
    api_mode = os.getenv("DEFENSE_IA_API_MODE", "brms").lower()
    if api_mode not in ("brms", "legacy"):
        api_mode = "brms"
    settings = DefenseIASettings(
        server_url=os.getenv("DEFENSE_IA_SERVER_URL", ""),
        username=os.getenv("DEFENSE_IA_USERNAME", ""),
        password=os.getenv("DEFENSE_IA_PASSWORD", ""),
        api_mode=api_mode,  # type: ignore[arg-type]
        client_type=os.getenv("DEFENSE_IA_CLIENT_TYPE", "WINPC_V2"),
        public_key=os.getenv("DEFENSE_IA_PUBLIC_KEY", ""),
        use_server_public_key=os.getenv("DEFENSE_IA_USE_SERVER_PUBLICKEY", "false").lower()
        in ("1", "true", "yes"),
        private_key=os.getenv("DEFENSE_IA_PRIVATE_KEY", ""),
        user_type=os.getenv("DEFENSE_IA_USER_TYPE", "0"),
        org_code=os.getenv("DEFENSE_IA_ORG_CODE", "001"),
        keep_alive_interval_seconds=float(
            os.getenv("DEFENSE_IA_KEEP_ALIVE_SECONDS", "20")
        ),
        timeout_seconds=float(os.getenv("DEFENSE_IA_TIMEOUT_SECONDS", "10")),
    )
    return DefenseIAClient(settings=settings)


def build_biodoc_client_from_env() -> BiodocClient:
    return BiodocClient(
        base_url=os.getenv("BIODOC_API_URL", "https://api.biodoc.com.br/api"),
        token_api=os.getenv("BIODOC_TOKEN_API", ""),
        timeout_seconds=float(os.getenv("DEFENSE_IA_TIMEOUT_SECONDS", "10")),
    )


@asynccontextmanager
async def lifespan(app: FastAPI):
    logger.info("Starting BIODOC-Intelbras Middleware API")

    app.state.defense_client = build_defense_client_from_env()
    if app.state.defense_client.settings.enabled:
        await app.state.defense_client.start()
        logger.info(
            "Defense IA client started (token: %s)",
            "ok" if app.state.defense_client.is_ready else "pending — retrying in background",
        )
    else:
        logger.warning("Defense IA client disabled: missing environment settings")

    app.state.biodoc_client = build_biodoc_client_from_env()
    await app.state.biodoc_client.start()
    biodoc_configured = bool(os.getenv("BIODOC_TOKEN_API"))
    logger.info(
        "BioDoc client started (api_url=%s, configured=%s, ambiente=%s)",
        os.getenv("BIODOC_API_URL", ""),
        biodoc_configured,
        os.getenv("BIODOC_AMBIENTE", "sandbox"),
    )

    try:
        yield
    finally:
        await app.state.biodoc_client.close()
        await app.state.defense_client.close()
        logger.info("Stopping BIODOC-Intelbras Middleware API")
