import os

os.environ.setdefault("SKIP_DOTENV", "1")

import httpx
import pytest
from httpx import ASGITransport

from src.main import app


@pytest.mark.asyncio
async def test_webhook_audit_logs_post_biodoc_ingress(
    caplog: pytest.LogCaptureFixture,
):
    import logging

    caplog.set_level(logging.INFO, logger="biodoc_intelbras")
    transport = ASGITransport(app=app)
    payload = {"event": "access", "deviceId": "cam-01"}

    async with httpx.AsyncClient(transport=transport, base_url="http://testserver") as client:
        response = await client.post("/webhook/biodoc", json=payload)

    assert response.status_code == 200
    inbound = [r.message for r in caplog.records if "[WEBHOOK IN]" in r.message]
    assert any("POST /webhook/biodoc" in m for m in inbound)
    assert any("body:" in m for m in inbound)


@pytest.mark.asyncio
async def test_webhook_audit_logs_get_biodoc_callback(
    caplog: pytest.LogCaptureFixture,
):
    import logging
    from unittest.mock import AsyncMock, patch

    from src.api.dependencies import get_biodoc_client, get_defense_client

    caplog.set_level(logging.INFO, logger="biodoc_intelbras")

    async def override_biodoc() -> AsyncMock:
        return AsyncMock()

    async def override_defense() -> AsyncMock:
        return AsyncMock()

    app.dependency_overrides[get_biodoc_client] = override_biodoc
    app.dependency_overrides[get_defense_client] = override_defense

    transport = ASGITransport(app=app)
    try:
        with patch(
            "src.services.defense_callback_service.process_defense_biodoc_callback",
            return_value={"status": "ignored", "external_id": "123"},
        ):
            async with httpx.AsyncClient(transport=transport, base_url="http://testserver") as client:
                response = await client.get(
                    "/webhook/biodoc",
                    params={"card": "123", "response": "403"},
                )
    finally:
        app.dependency_overrides.clear()

    assert response.status_code == 200
    inbound = [r.message for r in caplog.records if "[WEBHOOK IN]" in r.message]
    assert any("GET /webhook/biodoc" in m for m in inbound)
    assert any("HTTP 200" in m for m in inbound)
