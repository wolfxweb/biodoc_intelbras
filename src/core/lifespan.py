import asyncio
import os
from contextlib import asynccontextmanager

from fastapi import FastAPI

from src.core.logging import keep_alive_logger, logger
from src.services.biodoc_client import BiodocClient
from src.services.defense_ia_client import (
    DefenseIAClient,
    DefenseIASettings,
    SYNC_TARGET_PERSON,
    SYNC_TARGET_VISITOR,
)
from src.services.visitor_leave import (
    VisitLeaveSettings,
    visitor_leave_poll_loop,
)
from src.services.visitor_leave_store import (
    get_webhook_url,
    init_visit_leave_db,
    import_visitor_leave_logs_if_empty,
    set_webhook_url,
)


def _load_sync_target() -> str:
    raw = os.getenv("DEFENSE_IA_SYNC_TARGET", SYNC_TARGET_VISITOR).lower()
    if raw in (SYNC_TARGET_PERSON, SYNC_TARGET_VISITOR):
        return raw
    return SYNC_TARGET_VISITOR


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
        visited_person_id=os.getenv("DEFENSE_IA_VISITED_PERSON_ID", ""),
        sync_target=_load_sync_target(),  # type: ignore[arg-type]
        visitor_status=os.getenv("DEFENSE_IA_VISITOR_STATUS", "1"),
        visited_name=os.getenv("DEFENSE_IA_VISITED_NAME", ""),
        visited_org_name=os.getenv("DEFENSE_IA_VISITED_ORG_NAME", ""),
    )
    return DefenseIAClient(settings=settings)


def build_visit_leave_settings_from_env() -> VisitLeaveSettings:
    poll_raw = os.getenv("VISIT_LEAVE_POLL_SECONDS", "60").strip()
    poll_interval = float(poll_raw) if poll_raw else 0.0
    return VisitLeaveSettings(
        webhook_url=os.getenv("VISIT_LEAVE_WEBHOOK_URL", "").strip(),
        webhook_token=os.getenv("VISIT_LEAVE_WEBHOOK_TOKEN", "").strip(),
        timeout_seconds=float(os.getenv("DEFENSE_IA_TIMEOUT_SECONDS", "10")),
        poll_interval_seconds=poll_interval,
        state_path=os.getenv("VISIT_LEAVE_STATE_PATH", "data/visitor_leave_state.json"),
    )


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
        keep_alive_logger.info(
            "[KEEP_ALIVE] client started (token: %s)",
            "ok" if app.state.defense_client.is_ready else "pending — retrying in background",
        )
        asyncio.create_task(app.state.defense_client.warmup_org_cache())
    else:
        keep_alive_logger.warning(
            "[KEEP_ALIVE] client disabled: missing environment settings"
        )

    app.state.biodoc_client = build_biodoc_client_from_env()
    await app.state.biodoc_client.start()
    init_visit_leave_db()
    import_visitor_leave_logs_if_empty()
    app.state.visit_leave_settings = build_visit_leave_settings_from_env()
    if app.state.visit_leave_settings.webhook_url and not get_webhook_url():
        set_webhook_url(app.state.visit_leave_settings.webhook_url)
    biodoc_configured = bool(os.getenv("BIODOC_TOKEN_API"))
    logger.info(
        "BioDoc client started (api_url=%s, configured=%s, ambiente=%s)",
        os.getenv("BIODOC_API_URL", ""),
        biodoc_configured,
        os.getenv("BIODOC_AMBIENTE", "sandbox"),
    )
    leave_settings = app.state.visit_leave_settings
    logger.info(
        "Visit leave: source=history webhook=%s poll=%ss",
        "on" if bool(get_webhook_url(leave_settings.webhook_url)) else "log-only",
        int(leave_settings.poll_interval_seconds) if leave_settings.poll_enabled else 0,
    )
    if app.state.defense_client.settings.enabled and leave_settings.poll_enabled:
        asyncio.create_task(
            visitor_leave_poll_loop(app.state.defense_client, leave_settings)
        )

    try:
        yield
    finally:
        await app.state.biodoc_client.close()
        await app.state.defense_client.close()
        logger.info("Stopping BIODOC-Intelbras Middleware API")
