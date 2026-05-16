import os
from contextlib import asynccontextmanager

from fastapi import FastAPI

from src.core.logging import logger
from src.services.defense_ia_client import DefenseIAClient, DefenseIASettings


def build_defense_client_from_env() -> DefenseIAClient:
    settings = DefenseIASettings(
        server_url=os.getenv("DEFENSE_IA_SERVER_URL", ""),
        username=os.getenv("DEFENSE_IA_USERNAME", ""),
        password=os.getenv("DEFENSE_IA_PASSWORD", ""),
        keep_alive_interval_seconds=float(
            os.getenv("DEFENSE_IA_KEEP_ALIVE_SECONDS", "20")
        ),
        timeout_seconds=float(os.getenv("DEFENSE_IA_TIMEOUT_SECONDS", "10")),
    )
    return DefenseIAClient(settings=settings)


@asynccontextmanager
async def lifespan(app: FastAPI):
    logger.info("Starting BIODOC-Intelbras Middleware API")
    app.state.defense_client = build_defense_client_from_env()
    if app.state.defense_client.settings.enabled:
        await app.state.defense_client.start()
        logger.info("Defense IA client started")
    else:
        logger.warning("Defense IA client disabled: missing environment settings")
    try:
        yield
    finally:
        await app.state.defense_client.close()
        logger.info("Stopping BIODOC-Intelbras Middleware API")
