"""
Testes unitários do endpoint POST /webhook/biodoc.

Não dependem dos módulos SQLite ausentes (src.core.database, src.models.*).
Cada teste cria seu próprio AsyncClient com overrides de dependências.
"""

import os

os.environ.setdefault("SKIP_DOTENV", "1")

from collections.abc import AsyncGenerator
from unittest.mock import AsyncMock, MagicMock, patch

import httpx
import pytest
import pytest_asyncio

from src.api.dependencies import get_biodoc_client, get_defense_client, require_biodoc_webhook_token
from src.main import app
from src.services.biodoc_client import (
    BiodocAPIUnavailableError,
    BiodocAPIUnauthorizedError,
    BiodocClient,
    CardMainImageData,
)
from src.services.defense_ia_client import (
    DefenseIAArgumentError,
    DefenseIASettings,
    DefenseIAUnavailableError,
)

WEBHOOK_TOKEN = "test-webhook-token"

VALID_PAYLOAD = {
    "confidence": "98",
    "date": "2025-02-04T12:34:56Z",
    "response": 201,
    "message": "Cadastro realizado com sucesso!",
    "card": "1234567890",
    "image": "https://example.com/face.jpg",
    "success": True,
    "LogID": "abc-123",
}

VALID_HEADERS = {"Authorization": f"Bearer {WEBHOOK_TOKEN}"}

_DUMMY_JPEG = b"\xff\xd8\xff" + b"\x00" * 2048


def _make_defense_mock(is_ready: bool = True) -> AsyncMock:
    client = AsyncMock()
    client.sync_person.return_value = {"ok": True}
    client.settings = DefenseIASettings(
        server_url="http://defense.test",
        username="u",
        password="p",
        api_mode="brms",
    )
    client.is_ready = is_ready
    return client


MOCK_USER_NAME = "teste webhook"


def _make_biodoc_mock(
    name: str = MOCK_USER_NAME,
    card: str = "1234567890",
    status: bool = True,
    image: str | None = "https://example.com/face.jpg",
) -> AsyncMock:
    client = AsyncMock(spec=BiodocClient)
    client.get_card_mainimage.return_value = CardMainImageData(
        name=name, card=card, status=status, image=image
    )
    return client


class WebhookFixture:
    """Agrupa o client HTTP e os mocks para inspeção nos testes."""

    def __init__(
        self,
        client: httpx.AsyncClient,
        defense_mock: AsyncMock,
        biodoc_mock: AsyncMock,
    ) -> None:
        self.client = client
        self.defense_mock = defense_mock
        self.biodoc_mock = biodoc_mock


@pytest_asyncio.fixture()
async def webhook_client(monkeypatch: pytest.MonkeyPatch) -> AsyncGenerator[WebhookFixture, None]:
    monkeypatch.setenv("BIODOC_WEBHOOK_TOKEN", WEBHOOK_TOKEN)

    defense_mock = _make_defense_mock()
    biodoc_mock = _make_biodoc_mock()

    app.dependency_overrides[get_defense_client] = lambda: defense_mock
    app.dependency_overrides[get_biodoc_client] = lambda: biodoc_mock

    transport = httpx.ASGITransport(app=app)
    async with httpx.AsyncClient(transport=transport, base_url="http://testserver") as client:
        with patch(
            "src.services.biodoc_webhook_service.download_image_as_base64",
            new=AsyncMock(return_value=__import__("base64").b64encode(_DUMMY_JPEG).decode()),
        ):
            yield WebhookFixture(client=client, defense_mock=defense_mock, biodoc_mock=biodoc_mock)

    app.dependency_overrides.clear()


# ---------------------------------------------------------------------------
# Auth tests
# ---------------------------------------------------------------------------


@pytest.mark.asyncio
async def test_webhook_missing_token(monkeypatch: pytest.MonkeyPatch):
    monkeypatch.setenv("BIODOC_WEBHOOK_TOKEN", WEBHOOK_TOKEN)

    defense_mock = _make_defense_mock()
    biodoc_mock = _make_biodoc_mock()
    app.dependency_overrides[get_defense_client] = lambda: defense_mock
    app.dependency_overrides[get_biodoc_client] = lambda: biodoc_mock

    transport = httpx.ASGITransport(app=app)
    async with httpx.AsyncClient(transport=transport, base_url="http://testserver") as client:
        response = await client.post("/webhook/biodoc", json=VALID_PAYLOAD)

    app.dependency_overrides.clear()
    assert response.status_code == 401
    assert "Authorization" in response.json()["detail"]


@pytest.mark.asyncio
async def test_webhook_wrong_token(monkeypatch: pytest.MonkeyPatch):
    monkeypatch.setenv("BIODOC_WEBHOOK_TOKEN", WEBHOOK_TOKEN)

    defense_mock = _make_defense_mock()
    biodoc_mock = _make_biodoc_mock()
    app.dependency_overrides[get_defense_client] = lambda: defense_mock
    app.dependency_overrides[get_biodoc_client] = lambda: biodoc_mock

    transport = httpx.ASGITransport(app=app)
    async with httpx.AsyncClient(transport=transport, base_url="http://testserver") as client:
        response = await client.post(
            "/webhook/biodoc",
            json=VALID_PAYLOAD,
            headers={"Authorization": "Bearer wrong-token"},
        )

    app.dependency_overrides.clear()
    assert response.status_code == 401
    assert "inválido" in response.json()["detail"]


# ---------------------------------------------------------------------------
# Business validation tests
# ---------------------------------------------------------------------------


@pytest.mark.asyncio
async def test_webhook_success_false_returns_422(webhook_client: WebhookFixture):
    payload = {**VALID_PAYLOAD, "success": False}
    response = await webhook_client.client.post("/webhook/biodoc", json=payload, headers=VALID_HEADERS)
    assert response.status_code == 422
    assert "success=false" in response.json()["detail"]


@pytest.mark.asyncio
async def test_webhook_missing_card_returns_422(webhook_client: WebhookFixture):
    payload = {**VALID_PAYLOAD, "card": None}
    response = await webhook_client.client.post("/webhook/biodoc", json=payload, headers=VALID_HEADERS)
    assert response.status_code == 422
    assert "card" in response.json()["detail"].lower()


@pytest.mark.asyncio
async def test_webhook_card_too_long_returns_422(monkeypatch: pytest.MonkeyPatch):
    monkeypatch.setenv("BIODOC_WEBHOOK_TOKEN", WEBHOOK_TOKEN)

    defense_mock = _make_defense_mock()
    biodoc_mock = _make_biodoc_mock()
    app.dependency_overrides[get_defense_client] = lambda: defense_mock
    app.dependency_overrides[get_biodoc_client] = lambda: biodoc_mock

    transport = httpx.ASGITransport(app=app)
    async with httpx.AsyncClient(transport=transport, base_url="http://testserver") as client:
        payload = {**VALID_PAYLOAD, "card": "X" * 31}
        response = await client.post("/webhook/biodoc", json=payload, headers=VALID_HEADERS)

    app.dependency_overrides.clear()
    assert response.status_code == 422


# ---------------------------------------------------------------------------
# Happy path
# ---------------------------------------------------------------------------


@pytest.mark.asyncio
async def test_webhook_success_flow(webhook_client: WebhookFixture):
    response = await webhook_client.client.post("/webhook/biodoc", json=VALID_PAYLOAD, headers=VALID_HEADERS)
    assert response.status_code == 200
    body = response.json()
    assert body["status"] == "success"
    assert body["external_id"] == VALID_PAYLOAD["card"]
    assert body["defense_sync"] == "ok"


@pytest.mark.asyncio
async def test_webhook_success_calls_defense_sync_with_payload(webhook_client: WebhookFixture):
    """Garante que o fluxo mockado chega até sync_person com o payload esperado."""
    response = await webhook_client.client.post("/webhook/biodoc", json=VALID_PAYLOAD, headers=VALID_HEADERS)
    assert response.status_code == 200

    webhook_client.biodoc_mock.get_card_mainimage.assert_awaited_once_with(VALID_PAYLOAD["card"])
    webhook_client.defense_mock.sync_person.assert_awaited_once()

    sync_req = webhook_client.defense_mock.sync_person.await_args[0][0]
    assert sync_req.source == "biodoc"
    assert sync_req.operation == "upsert"
    assert sync_req.external_id == VALID_PAYLOAD["card"]
    assert sync_req.person.full_name == MOCK_USER_NAME
    assert sync_req.person.document == VALID_PAYLOAD["card"]
    assert sync_req.biometrics.face_image_base64
    assert len(sync_req.biometrics.face_image_base64) > 100


# ---------------------------------------------------------------------------
# BioDoc API error propagation
# ---------------------------------------------------------------------------


@pytest.mark.asyncio
async def test_webhook_biodoc_api_unavailable_returns_502(monkeypatch: pytest.MonkeyPatch):
    monkeypatch.setenv("BIODOC_WEBHOOK_TOKEN", WEBHOOK_TOKEN)

    defense_mock = _make_defense_mock()
    biodoc_mock = AsyncMock(spec=BiodocClient)
    biodoc_mock.get_card_mainimage.side_effect = BiodocAPIUnavailableError("timeout")

    app.dependency_overrides[get_defense_client] = lambda: defense_mock
    app.dependency_overrides[get_biodoc_client] = lambda: biodoc_mock

    transport = httpx.ASGITransport(app=app)
    async with httpx.AsyncClient(transport=transport, base_url="http://testserver") as client:
        response = await client.post("/webhook/biodoc", json=VALID_PAYLOAD, headers=VALID_HEADERS)

    app.dependency_overrides.clear()
    assert response.status_code == 502
    assert "BioDoc" in response.json()["detail"]


@pytest.mark.asyncio
async def test_webhook_biodoc_api_unauthorized_returns_502(monkeypatch: pytest.MonkeyPatch):
    monkeypatch.setenv("BIODOC_WEBHOOK_TOKEN", WEBHOOK_TOKEN)

    defense_mock = _make_defense_mock()
    biodoc_mock = AsyncMock(spec=BiodocClient)
    biodoc_mock.get_card_mainimage.side_effect = BiodocAPIUnauthorizedError("invalid token")

    app.dependency_overrides[get_defense_client] = lambda: defense_mock
    app.dependency_overrides[get_biodoc_client] = lambda: biodoc_mock

    transport = httpx.ASGITransport(app=app)
    async with httpx.AsyncClient(transport=transport, base_url="http://testserver") as client:
        response = await client.post("/webhook/biodoc", json=VALID_PAYLOAD, headers=VALID_HEADERS)

    app.dependency_overrides.clear()
    assert response.status_code == 502
    assert "Credencial" in response.json()["detail"]


@pytest.mark.asyncio
async def test_webhook_beneficiary_inactive_returns_422(monkeypatch: pytest.MonkeyPatch):
    monkeypatch.setenv("BIODOC_WEBHOOK_TOKEN", WEBHOOK_TOKEN)

    defense_mock = _make_defense_mock()
    biodoc_mock = _make_biodoc_mock(status=False)

    app.dependency_overrides[get_defense_client] = lambda: defense_mock
    app.dependency_overrides[get_biodoc_client] = lambda: biodoc_mock

    transport = httpx.ASGITransport(app=app)
    async with httpx.AsyncClient(transport=transport, base_url="http://testserver") as client:
        response = await client.post("/webhook/biodoc", json=VALID_PAYLOAD, headers=VALID_HEADERS)

    app.dependency_overrides.clear()
    assert response.status_code == 422
    assert "inativo" in response.json()["detail"]


@pytest.mark.asyncio
async def test_webhook_no_image_returns_422(monkeypatch: pytest.MonkeyPatch):
    monkeypatch.setenv("BIODOC_WEBHOOK_TOKEN", WEBHOOK_TOKEN)

    defense_mock = _make_defense_mock()
    biodoc_mock = _make_biodoc_mock(image=None)

    app.dependency_overrides[get_defense_client] = lambda: defense_mock
    app.dependency_overrides[get_biodoc_client] = lambda: biodoc_mock

    payload = {**VALID_PAYLOAD, "image": None}
    transport = httpx.ASGITransport(app=app)
    async with httpx.AsyncClient(transport=transport, base_url="http://testserver") as client:
        response = await client.post("/webhook/biodoc", json=payload, headers=VALID_HEADERS)

    app.dependency_overrides.clear()
    assert response.status_code == 422
    assert "imagem" in response.json()["detail"].lower()


# ---------------------------------------------------------------------------
# Defense IA error propagation
# ---------------------------------------------------------------------------


@pytest.mark.asyncio
async def test_webhook_defense_ia_unavailable_returns_502(monkeypatch: pytest.MonkeyPatch):
    monkeypatch.setenv("BIODOC_WEBHOOK_TOKEN", WEBHOOK_TOKEN)

    defense_mock = _make_defense_mock()
    defense_mock.sync_person.side_effect = DefenseIAUnavailableError("offline")
    biodoc_mock = _make_biodoc_mock()

    app.dependency_overrides[get_defense_client] = lambda: defense_mock
    app.dependency_overrides[get_biodoc_client] = lambda: biodoc_mock

    transport = httpx.ASGITransport(app=app)
    async with httpx.AsyncClient(transport=transport, base_url="http://testserver") as client:
        with patch(
            "src.services.biodoc_webhook_service.download_image_as_base64",
            new=AsyncMock(return_value=__import__("base64").b64encode(_DUMMY_JPEG).decode()),
        ):
            response = await client.post("/webhook/biodoc", json=VALID_PAYLOAD, headers=VALID_HEADERS)

    app.dependency_overrides.clear()
    assert response.status_code == 502


@pytest.mark.asyncio
async def test_webhook_defense_ia_bad_image_returns_422(monkeypatch: pytest.MonkeyPatch):
    monkeypatch.setenv("BIODOC_WEBHOOK_TOKEN", WEBHOOK_TOKEN)

    defense_mock = _make_defense_mock()
    defense_mock.sync_person.side_effect = DefenseIAArgumentError("bad image")
    biodoc_mock = _make_biodoc_mock()

    app.dependency_overrides[get_defense_client] = lambda: defense_mock
    app.dependency_overrides[get_biodoc_client] = lambda: biodoc_mock

    transport = httpx.ASGITransport(app=app)
    async with httpx.AsyncClient(transport=transport, base_url="http://testserver") as client:
        with patch(
            "src.services.biodoc_webhook_service.download_image_as_base64",
            new=AsyncMock(return_value=__import__("base64").b64encode(_DUMMY_JPEG).decode()),
        ):
            response = await client.post("/webhook/biodoc", json=VALID_PAYLOAD, headers=VALID_HEADERS)

    app.dependency_overrides.clear()
    assert response.status_code == 422
    assert "Biometria" in response.json()["detail"]
