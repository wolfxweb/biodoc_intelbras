from unittest.mock import AsyncMock

import httpx
import pytest

from src.main import app
from src.services.defense_ia_client import DefenseIASettings


@pytest.mark.asyncio
async def test_integration_status():
    mock_defense = AsyncMock()
    mock_defense.settings = DefenseIASettings(
        server_url="http://defense.test",
        username="u",
        password="p",
        api_mode="brms",
    )
    mock_defense.is_ready = True

    transport = httpx.ASGITransport(app=app)
    async with httpx.AsyncClient(
        transport=transport,
        base_url="http://testserver",
    ) as client:
        app.state.defense_client = mock_defense
        response = await client.get("/status")

    assert response.status_code == 200
    body = response.json()
    assert body["middleware"] == "ok"
    assert "defense_ia" in body
    assert "enabled" in body["defense_ia"]
    assert "connected" in body["defense_ia"]
    assert "api_mode" in body["defense_ia"]
