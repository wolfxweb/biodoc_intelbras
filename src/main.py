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


@app.get("/health", tags=["health"])
async def health_check(request: Request) -> dict:
    client = request.app.state.defense_client
    enabled = client.settings.enabled
    return {
        "status": "ok",
        "defense_ia": {
            "enabled": enabled,
            "connected": client.is_ready if enabled else False,
            "api_mode": client.settings.api_mode,
        },
    }
