from unittest.mock import AsyncMock

import httpx
import pytest

from src.main import app


@pytest.mark.asyncio
async def test_health_reports_defense_connected(
    api_client: httpx.AsyncClient,
    defense_client_mock: AsyncMock,
):
    app.state.defense_client = defense_client_mock

    response = await api_client.get("/health")

    assert response.status_code == 200
    body = response.json()
    assert body["status"] == "ok"
    assert body["defense_ia"]["enabled"] is True
    assert body["defense_ia"]["connected"] is True
    assert body["defense_ia"]["api_mode"] == "brms"
