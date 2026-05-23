from unittest.mock import AsyncMock

import httpx
import pytest

from src.main import app


@pytest.mark.asyncio
async def test_status_reports_defense_and_biodoc(
    api_client: httpx.AsyncClient,
    defense_client_mock: AsyncMock,
    monkeypatch: pytest.MonkeyPatch,
):
    app.state.defense_client = defense_client_mock
    monkeypatch.setenv("BIODOC_TOKEN_API", "test-token")
    monkeypatch.setenv("BIODOC_API_URL", "https://api.sandbox.biodoc.com.br/api")
    monkeypatch.setenv("BIODOC_AMBIENTE", "sandbox")

    response = await api_client.get("/status")

    assert response.status_code == 200
    body = response.json()
    assert body["middleware"] == "ok"
    assert body["defense_ia"]["enabled"] is True
    assert body["defense_ia"]["connected"] is True
    assert body["defense_ia"]["api_mode"] == "brms"
    assert body["biodoc"]["configured"] is True
    assert body["biodoc"]["ambiente"] == "sandbox"
