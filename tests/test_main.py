import pytest
import httpx

from src.main import app


@pytest.mark.asyncio
async def test_health_check():
    transport = httpx.ASGITransport(app=app)
    async with httpx.AsyncClient(
        transport=transport,
        base_url="http://testserver",
    ) as client:
        response = await client.get("/health")

    assert response.status_code == 200
    body = response.json()
    assert body["status"] == "ok"
    assert "defense_ia" in body
    assert "enabled" in body["defense_ia"]
    assert "connected" in body["defense_ia"]
    assert "api_mode" in body["defense_ia"]
