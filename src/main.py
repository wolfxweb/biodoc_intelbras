import os
from pathlib import Path

from dotenv import load_dotenv

if os.getenv("SKIP_DOTENV") != "1":
    load_dotenv(Path(__file__).resolve().parent.parent / ".env")

from src.core.lifespan import lifespan
from src.api.routes.sync import router as sync_router
from src.api.routes.webhook import router as webhook_router
from fastapi import FastAPI, Request
from fastapi.middleware.cors import CORSMiddleware

app = FastAPI(title="BIODOC-Intelbras Middleware", lifespan=lifespan)
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_methods=["*"],
    allow_headers=["*"],
)
app.include_router(sync_router)
app.include_router(webhook_router)


@app.get(
    "/status",
    tags=["status"],
    summary="Status do middleware e integrações",
    description=(
        "Retorna se a API está no ar e o estado das integrações "
        "(Intelbras Defense IA e cliente BioDoc configurado no startup)."
    ),
)
async def integration_status(request: Request) -> dict:
    defense = request.app.state.defense_client
    defense_enabled = defense.settings.enabled
    biodoc_configured = bool(os.getenv("BIODOC_TOKEN_API", "").strip())
    return {
        "middleware": "ok",
        "defense_ia": {
            "enabled": defense_enabled,
            "connected": defense.is_ready if defense_enabled else False,
            "api_mode": defense.settings.api_mode,
        },
        "biodoc": {
            "api_url": os.getenv("BIODOC_API_URL", ""),
            "configured": biodoc_configured,
            "ambiente": os.getenv("BIODOC_AMBIENTE", "sandbox"),
        },
    }
