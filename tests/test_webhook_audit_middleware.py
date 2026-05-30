import os

os.environ.setdefault("SKIP_DOTENV", "1")

import httpx
import pytest
from httpx import ASGITransport

from src.main import app


@pytest.mark.asyncio
async def test_webhook_audit_logs_post_without_authorization(
    caplog: pytest.LogCaptureFixture,
    monkeypatch: pytest.MonkeyPatch,
):
    import logging

    from src.api.dependencies import get_biodoc_client, get_defense_client
    from unittest.mock import AsyncMock

    monkeypatch.setenv("BIODOC_WEBHOOK_TOKEN", "valid-test-token")
    caplog.set_level(logging.INFO, logger="biodoc_intelbras")

    async def override_biodoc() -> AsyncMock:
        return AsyncMock()

    async def override_defense() -> AsyncMock:
        return AsyncMock()

    app.dependency_overrides[get_biodoc_client] = override_biodoc
    app.dependency_overrides[get_defense_client] = override_defense

    transport = ASGITransport(app=app)
    try:
        async with httpx.AsyncClient(transport=transport, base_url="http://testserver") as client:
            response = await client.post(
                "/webhook/biodoc",
                json={"success": True},
            )
    finally:
        app.dependency_overrides.clear()

    # Fluxo urlWebhook BioDoc pode chegar sem Bearer; auth opcional no POST.
    assert response.status_code != 401
    inbound = [r.message for r in caplog.records if "[WEBHOOK IN]" in r.message]
    assert any("POST /webhook/biodoc" in m for m in inbound)
    assert any("body=" in m for m in inbound)
    assert any("sem Authorization" in r.message for r in caplog.records)


@pytest.mark.asyncio
async def test_webhook_audit_logs_invalid_token_returns_401(
    caplog: pytest.LogCaptureFixture,
    monkeypatch: pytest.MonkeyPatch,
):
    import logging

    monkeypatch.setenv("BIODOC_WEBHOOK_TOKEN", "valid-test-token")
    caplog.set_level(logging.INFO, logger="biodoc_intelbras")
    transport = ASGITransport(app=app)
    async with httpx.AsyncClient(transport=transport, base_url="http://testserver") as client:
        response = await client.post(
            "/webhook/biodoc",
            json={"success": True},
            headers={"Authorization": "Bearer token-invalido"},
        )

    assert response.status_code == 401
    inbound = [r.message for r in caplog.records if "[WEBHOOK IN]" in r.message]
    assert any("POST /webhook/biodoc" in m for m in inbound)
    assert any("status=401" in m for m in inbound)
