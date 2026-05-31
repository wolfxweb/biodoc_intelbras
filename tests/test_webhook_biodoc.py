"""
Testes unitários do endpoint POST /webhook/biodoc.

Não dependem dos módulos SQLite ausentes (src.core.database, src.models.*).
Cada teste cria seu próprio AsyncClient com overrides de dependências.
"""

import os

os.environ.setdefault("SKIP_DOTENV", "1")

from collections.abc import AsyncGenerator
from unittest.mock import AsyncMock, patch

import httpx
import pytest
import pytest_asyncio

from src.api.dependencies import (
    get_biodoc_client,
    get_defense_client,
)
from src.main import app
from src.services.biodoc_client import (
    BiodocAPIUnavailableError,
    BiodocAPIUnauthorizedError,
    BiodocClient,
    CardMainImageData,
    ExternalAuditEntry,
    IntegrationLogData,
)
from src.services.defense_ia_client import (
    DefenseIAArgumentError,
    DefenseIASettings,
    DefenseIAUnavailableError,
)

WEBHOOK_TOKEN = "test-webhook-token"

VALID_PAYLOAD = {
    "id_Log": 1000,
    "percentage": "100%",
    "success": True,
    "status": 2,
    "message": "Sucesso ao realizar autenticação, nível de similaridade 100% e qualidade 100%.",
    "url": "https://example.com/face.jpg",
    "reference_Id": "0c19bfff-9aba-4517-afd7-56e77ea1faeb",
}

VALID_HEADERS = {"Authorization": f"Bearer {WEBHOOK_TOKEN}"}

_DUMMY_JPEG = b"\xff\xd8\xff" + b"\x00" * 2048

MOCK_USER_NAME = "teste webhook"


def _make_defense_mock(
    is_ready: bool = True, resolved_org_code: str | None = None
) -> AsyncMock:
    client = AsyncMock()
    client.sync_person.return_value = {"ok": True}
    client.resolve_org_code.return_value = resolved_org_code
    client.settings = DefenseIASettings(
        server_url="http://defense.test",
        username="u",
        password="p",
        api_mode="brms",
        org_code="001",
    )
    client.is_ready = is_ready
    return client


def _make_biodoc_mock(
    name: str = MOCK_USER_NAME,
    id_card: str = "1234567890",
    status: int = 1,
    main_image: str | None = "https://example.com/face.jpg",
    required_name: str | None = "Empresa Teste",
) -> AsyncMock:
    client = AsyncMock(spec=BiodocClient)
    client.get_integration_log.return_value = IntegrationLogData(
        id=1,
        id_card=id_card,
        name=name,
        status=status,
        main_image=main_image,
        path=None,
        required_name=required_name,
    )
    client.get_card_mainimage.return_value = CardMainImageData(
        name=name,
        card=id_card,
        status=True,
        image=main_image,
    )
    client.get_external_audits.return_value = []
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
async def test_webhook_missing_token_is_accepted(monkeypatch: pytest.MonkeyPatch):
    """Fluxo urlWebhook BioDoc pode POST sem Authorization."""
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
            response = await client.post("/webhook/biodoc", json=VALID_PAYLOAD)

    app.dependency_overrides.clear()
    assert response.status_code == 200
    assert response.json()["status"] == "success"


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
async def test_webhook_success_false_returns_ignored(webhook_client: WebhookFixture):
    payload = {**VALID_PAYLOAD, "success": False}
    response = await webhook_client.client.post("/webhook/biodoc", json=payload, headers=VALID_HEADERS)
    assert response.status_code == 200
    body = response.json()
    assert body["status"] == "ignored"
    assert body["defense_sync"] == "skipped"
    webhook_client.defense_mock.sync_person.assert_not_awaited()


@pytest.mark.asyncio
async def test_webhook_response_403_with_logid_skips_defense(monkeypatch: pytest.MonkeyPatch):
    """Falha de comparação facial (403) não deve acionar Defense mesmo com logId."""
    monkeypatch.setenv("BIODOC_WEBHOOK_TOKEN", WEBHOOK_TOKEN)

    defense_mock = _make_defense_mock()
    biodoc_mock = _make_biodoc_mock()

    app.dependency_overrides[get_defense_client] = lambda: defense_mock
    app.dependency_overrides[get_biodoc_client] = lambda: biodoc_mock

    payload = {
        "success": False,
        "response": 403,
        "code": "ERR_CRD_FACE_COMPARISON",
        "message": "Falha ao comparar as faces. Verifique e tente novamente.",
        "card": "00271368992672000",
        "logId": "fb7aec8c-c72b-41c4-ae89-39543c7c1604",
        "confidence": "0",
        "date": "2026-05-30T14:43:21.201305562Z",
    }

    transport = httpx.ASGITransport(app=app)
    async with httpx.AsyncClient(transport=transport, base_url="http://testserver") as client:
        response = await client.post("/webhook/biodoc", json=payload, headers=VALID_HEADERS)

    app.dependency_overrides.clear()
    assert response.status_code == 200
    body = response.json()
    assert body["status"] == "ignored"
    assert body["defense_sync"] == "skipped"
    biodoc_mock.get_integration_log.assert_not_awaited()
    biodoc_mock.get_external_audits.assert_not_awaited()
    defense_mock.sync_person.assert_not_awaited()


@pytest.mark.asyncio
async def test_webhook_missing_reference_id_returns_422(webhook_client: WebhookFixture):
    payload = {**VALID_PAYLOAD, "reference_Id": None, "id_Log": None}
    response = await webhook_client.client.post("/webhook/biodoc", json=payload, headers=VALID_HEADERS)
    assert response.status_code == 422
    assert "reference_Id" in response.json()["detail"]


# ---------------------------------------------------------------------------
# Happy path
# ---------------------------------------------------------------------------


@pytest.mark.asyncio
async def test_webhook_success_flow(webhook_client: WebhookFixture):
    response = await webhook_client.client.post("/webhook/biodoc", json=VALID_PAYLOAD, headers=VALID_HEADERS)
    assert response.status_code == 200
    body = response.json()
    assert body["status"] == "success"
    # external_id vem do id_Card retornado pelo GET /integrations/log/{reference_Id}
    assert body["external_id"] == "1234567890"
    assert body["defense_sync"] == "ok"


def _extract_org_code_arg(call_args) -> str | None:
    """Extrai o segundo argumento (org_code) passado em sync_person."""
    if len(call_args[0]) > 1:
        return call_args[0][1]
    return call_args[1].get("org_code")


@pytest.mark.asyncio
async def test_webhook_success_calls_defense_sync_with_payload(webhook_client: WebhookFixture):
    """Garante que o fluxo mockado chega até sync_person com o payload esperado."""
    response = await webhook_client.client.post("/webhook/biodoc", json=VALID_PAYLOAD, headers=VALID_HEADERS)
    assert response.status_code == 200

    webhook_client.biodoc_mock.get_integration_log.assert_awaited_once_with(
        VALID_PAYLOAD["reference_Id"]
    )
    webhook_client.defense_mock.sync_person.assert_awaited_once()

    call_args = webhook_client.defense_mock.sync_person.await_args
    sync_req = call_args[0][0]
    assert sync_req.source == "biodoc"
    assert sync_req.operation == "upsert"
    # external_id e document vêm do id_Card mockado em _make_biodoc_mock
    assert sync_req.external_id == "1234567890"
    assert sync_req.person.full_name == MOCK_USER_NAME
    assert sync_req.person.document == "1234567890"
    assert sync_req.biometrics.face_image_base64
    assert len(sync_req.biometrics.face_image_base64) > 100

    # reguiredName não bate com nenhuma sub-org (mock retorna None) -> usa raiz
    assert _extract_org_code_arg(call_args) == "001"


@pytest.mark.asyncio
async def test_webhook_resolves_org_code_from_required_name(monkeypatch: pytest.MonkeyPatch):
    """O reguiredName do BioDoc deve ser resolvido em orgCode via API do Defense IA."""
    monkeypatch.setenv("BIODOC_WEBHOOK_TOKEN", WEBHOOK_TOKEN)

    defense_mock = _make_defense_mock(resolved_org_code="001015001")
    biodoc_mock = _make_biodoc_mock(required_name="Corb")

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
    assert response.status_code == 200

    defense_mock.resolve_org_code.assert_awaited_once_with("Corb")
    assert _extract_org_code_arg(defense_mock.sync_person.await_args) == "001015001"


@pytest.mark.asyncio
async def test_webhook_falls_back_to_root_org_when_required_name_unmapped(
    monkeypatch: pytest.MonkeyPatch,
):
    """Se nenhuma sub-org casa com reguiredName, usa o orgCode raiz do .env."""
    monkeypatch.setenv("BIODOC_WEBHOOK_TOKEN", WEBHOOK_TOKEN)

    defense_mock = _make_defense_mock(resolved_org_code=None)
    biodoc_mock = _make_biodoc_mock(required_name="EmpresaDesconhecida")

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
    assert response.status_code == 200
    assert _extract_org_code_arg(defense_mock.sync_person.await_args) == "001"


@pytest.mark.asyncio
async def test_webhook_skips_resolve_when_required_name_missing(
    monkeypatch: pytest.MonkeyPatch,
):
    """Sem reguiredName, nem chama resolve_org_code — vai direto para o default."""
    monkeypatch.setenv("BIODOC_WEBHOOK_TOKEN", WEBHOOK_TOKEN)

    defense_mock = _make_defense_mock()
    biodoc_mock = _make_biodoc_mock(required_name=None)

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
    assert response.status_code == 200
    defense_mock.resolve_org_code.assert_not_awaited()
    assert _extract_org_code_arg(defense_mock.sync_person.await_args) == "001"


@pytest.mark.asyncio
async def test_webhook_details_operador_is_ignored_for_orgcode(monkeypatch: pytest.MonkeyPatch):
    """operador em details do POST não entra na resolução de orgCode."""
    monkeypatch.setenv("BIODOC_WEBHOOK_TOKEN", WEBHOOK_TOKEN)
    defense_mock = _make_defense_mock(resolved_org_code="001015001")
    biodoc_mock = _make_biodoc_mock(required_name="EmpresaDesconhecida")

    app.dependency_overrides[get_defense_client] = lambda: defense_mock
    app.dependency_overrides[get_biodoc_client] = lambda: biodoc_mock

    payload = {
        **VALID_PAYLOAD,
        "details": {"operador": "VIVER"},
    }

    transport = httpx.ASGITransport(app=app)
    async with httpx.AsyncClient(transport=transport, base_url="http://testserver") as client:
        with patch(
            "src.services.biodoc_webhook_service.download_image_as_base64",
            new=AsyncMock(return_value=__import__("base64").b64encode(_DUMMY_JPEG).decode()),
        ):
            response = await client.post("/webhook/biodoc", json=payload, headers=VALID_HEADERS)

    app.dependency_overrides.clear()
    assert response.status_code == 200
    assert _extract_org_code_arg(defense_mock.sync_person.await_args) == "001015001"
    defense_mock.resolve_org_code.assert_awaited_once_with("EmpresaDesconhecida")


@pytest.mark.asyncio
async def test_webhook_query_operador_is_ignored(monkeypatch: pytest.MonkeyPatch):
    """?operador= na URL do webhook não deve alterar orgCode (fonte: integrations/log)."""
    monkeypatch.setenv("BIODOC_WEBHOOK_TOKEN", WEBHOOK_TOKEN)
    defense_mock = _make_defense_mock(resolved_org_code="001015001")
    biodoc_mock = _make_biodoc_mock(required_name="EmpresaDesconhecida")

    app.dependency_overrides[get_defense_client] = lambda: defense_mock
    app.dependency_overrides[get_biodoc_client] = lambda: biodoc_mock

    transport = httpx.ASGITransport(app=app)
    async with httpx.AsyncClient(transport=transport, base_url="http://testserver") as client:
        with patch(
            "src.services.biodoc_webhook_service.download_image_as_base64",
            new=AsyncMock(return_value=__import__("base64").b64encode(_DUMMY_JPEG).decode()),
        ):
            response = await client.post(
                "/webhook/biodoc?operador=VIVER",
                json=VALID_PAYLOAD,
                headers=VALID_HEADERS,
            )

    app.dependency_overrides.clear()
    assert response.status_code == 200
    assert _extract_org_code_arg(defense_mock.sync_person.await_args) == "001015001"
    defense_mock.resolve_org_code.assert_awaited_once_with("EmpresaDesconhecida")


# ---------------------------------------------------------------------------
# BioDoc API error propagation
# ---------------------------------------------------------------------------


@pytest.mark.asyncio
async def test_webhook_biodoc_api_unavailable_returns_502(monkeypatch: pytest.MonkeyPatch):
    monkeypatch.setenv("BIODOC_WEBHOOK_TOKEN", WEBHOOK_TOKEN)

    defense_mock = _make_defense_mock()
    biodoc_mock = AsyncMock(spec=BiodocClient)
    biodoc_mock.get_integration_log.side_effect = BiodocAPIUnavailableError("timeout")

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
    biodoc_mock.get_integration_log.side_effect = BiodocAPIUnauthorizedError("invalid token")

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
    biodoc_mock = _make_biodoc_mock(status=0)

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
    biodoc_mock = _make_biodoc_mock(main_image=None)

    payload = {**VALID_PAYLOAD, "url": None}
    app.dependency_overrides[get_defense_client] = lambda: defense_mock
    app.dependency_overrides[get_biodoc_client] = lambda: biodoc_mock

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


NEW_FORMAT_PAYLOAD = {
    "success": True,
    "message": "Face Reconhecida",
    "card": "00271368992672000",
    "image": "https://example.com/capture.png",
    "response": 200,
    "confidence": "61.00%",
}


@pytest.mark.asyncio
async def test_webhook_card_image_uses_mainimage_name(monkeypatch: pytest.MonkeyPatch):
    """POST urlWebhook (card+image): nome vem do mainimage, foto do payload."""
    monkeypatch.setenv("BIODOC_WEBHOOK_TOKEN", WEBHOOK_TOKEN)

    defense_mock = _make_defense_mock()
    biodoc_mock = _make_biodoc_mock(name="CARLOS EDUARDO LOBO", id_card="00271368992672000")

    app.dependency_overrides[get_defense_client] = lambda: defense_mock
    app.dependency_overrides[get_biodoc_client] = lambda: biodoc_mock

    transport = httpx.ASGITransport(app=app)
    async with httpx.AsyncClient(transport=transport, base_url="http://testserver") as client:
        with patch(
            "src.services.biodoc_webhook_service.download_image_as_base64",
            new=AsyncMock(return_value=__import__("base64").b64encode(_DUMMY_JPEG).decode()),
        ):
            response = await client.post(
                "/webhook/biodoc",
                json=NEW_FORMAT_PAYLOAD,
                headers=VALID_HEADERS,
            )

    app.dependency_overrides.clear()
    assert response.status_code == 200

    biodoc_mock.get_card_mainimage.assert_awaited_once_with("00271368992672000")
    assert biodoc_mock.get_external_audits.await_count >= 2
    biodoc_mock.get_integration_log.assert_not_awaited()

    sync_req = defense_mock.sync_person.await_args[0][0]
    assert sync_req.person.full_name == "CARLOS EDUARDO LOBO"
    assert sync_req.external_id == "00271368992672000"


@pytest.mark.asyncio
async def test_webhook_log_id_takes_priority_over_card_image(monkeypatch: pytest.MonkeyPatch):
    """logId no payload deve usar /integrations/log (local_token, nome completo)."""
    monkeypatch.setenv("BIODOC_WEBHOOK_TOKEN", WEBHOOK_TOKEN)

    defense_mock = _make_defense_mock()
    biodoc_mock = _make_biodoc_mock(name="CARLOS EDUARDO LOBO", id_card="00271368992672000")
    biodoc_mock.get_integration_log.return_value = IntegrationLogData(
        id="log-uuid",
        id_card="00271368992672000",
        name="CARLOS EDUARDO LOBO",
        status=2,
        main_image="https://example.com/face.jpg",
        path=None,
        required_name=None,
        operador="VIVER",
        local_token="CHU - ESPAÇO VIVER BEM",
    )

    app.dependency_overrides[get_defense_client] = lambda: defense_mock
    app.dependency_overrides[get_biodoc_client] = lambda: biodoc_mock

    payload = {
        **NEW_FORMAT_PAYLOAD,
        "logId": "log-uuid",
    }

    transport = httpx.ASGITransport(app=app)
    async with httpx.AsyncClient(transport=transport, base_url="http://testserver") as client:
        with patch(
            "src.services.biodoc_webhook_service.download_image_as_base64",
            new=AsyncMock(return_value=__import__("base64").b64encode(_DUMMY_JPEG).decode()),
        ):
            response = await client.post(
                "/webhook/biodoc",
                json=payload,
                headers=VALID_HEADERS,
            )

    app.dependency_overrides.clear()
    assert response.status_code == 200
    biodoc_mock.get_integration_log.assert_awaited_once_with("log-uuid")
    biodoc_mock.get_card_mainimage.assert_not_awaited()
    defense_mock.resolve_org_code.assert_awaited_once_with("CHU - ESPAÇO VIVER BEM")


@pytest.mark.asyncio
async def test_webhook_uses_local_token_not_operador(
    monkeypatch: pytest.MonkeyPatch,
):
    """orgCode usa local_token; operador no log é ignorado."""
    monkeypatch.setenv("BIODOC_WEBHOOK_TOKEN", WEBHOOK_TOKEN)

    defense_mock = _make_defense_mock(resolved_org_code="001002")
    biodoc_mock = _make_biodoc_mock(name="CARLOS EDUARDO LOBO", id_card="00271368992672000")
    biodoc_mock.get_integration_log.return_value = IntegrationLogData(
        id="log-uuid",
        id_card="00271368992672000",
        name="CARLOS EDUARDO LOBO",
        status=2,
        main_image="https://example.com/face.jpg",
        path=None,
        required_name=None,
        operador="VIVER",
        local_token="CHU - ESPAÇO VIVER BEM",
    )

    app.dependency_overrides[get_defense_client] = lambda: defense_mock
    app.dependency_overrides[get_biodoc_client] = lambda: biodoc_mock

    payload = {
        **NEW_FORMAT_PAYLOAD,
        "logId": "log-uuid",
    }

    transport = httpx.ASGITransport(app=app)
    async with httpx.AsyncClient(transport=transport, base_url="http://testserver") as client:
        with patch(
            "src.services.biodoc_webhook_service.download_image_as_base64",
            new=AsyncMock(return_value=__import__("base64").b64encode(_DUMMY_JPEG).decode()),
        ):
            response = await client.post(
                "/webhook/biodoc",
                json=payload,
                headers=VALID_HEADERS,
            )

    app.dependency_overrides.clear()
    assert response.status_code == 200
    defense_mock.resolve_org_code.assert_awaited_once_with("CHU - ESPAÇO VIVER BEM")
    assert _extract_org_code_arg(defense_mock.sync_person.await_args) == "001002"


@pytest.mark.asyncio
async def test_webhook_card_image_without_log_id_uses_default_org(
    monkeypatch: pytest.MonkeyPatch,
):
    monkeypatch.setenv("BIODOC_WEBHOOK_TOKEN", WEBHOOK_TOKEN)

    defense_mock = _make_defense_mock(resolved_org_code=None)
    biodoc_mock = _make_biodoc_mock(name="CARLOS EDUARDO LOBO", id_card="00271368992672000")

    app.dependency_overrides[get_defense_client] = lambda: defense_mock
    app.dependency_overrides[get_biodoc_client] = lambda: biodoc_mock

    transport = httpx.ASGITransport(app=app)
    async with httpx.AsyncClient(transport=transport, base_url="http://testserver") as client:
        with patch(
            "src.services.biodoc_webhook_service.download_image_as_base64",
            new=AsyncMock(return_value=__import__("base64").b64encode(_DUMMY_JPEG).decode()),
        ):
            response = await client.post(
                "/webhook/biodoc",
                json=NEW_FORMAT_PAYLOAD,
                headers=VALID_HEADERS,
            )

    app.dependency_overrides.clear()
    assert response.status_code == 200
    biodoc_mock.get_integration_log.assert_not_awaited()
    assert _extract_org_code_arg(defense_mock.sync_person.await_args) == "001"
    defense_mock.resolve_org_code.assert_not_awaited()


@pytest.mark.asyncio
async def test_webhook_card_image_details_operador_resolves_org_when_audits_empty(
    monkeypatch: pytest.MonkeyPatch,
):
    """details.operador do verify (ex. VIVER) no POST quando audits vazio."""
    monkeypatch.setenv("BIODOC_WEBHOOK_TOKEN", WEBHOOK_TOKEN)

    defense_mock = _make_defense_mock(resolved_org_code="001021")
    biodoc_mock = _make_biodoc_mock(name="CARLOS EDUARDO LOBO", id_card="00271368992672000")
    biodoc_mock.get_external_audits.return_value = []

    app.dependency_overrides[get_defense_client] = lambda: defense_mock
    app.dependency_overrides[get_biodoc_client] = lambda: biodoc_mock

    payload = {
        **NEW_FORMAT_PAYLOAD,
        "details": {"operador": "VIVER"},
    }

    transport = httpx.ASGITransport(app=app)
    async with httpx.AsyncClient(transport=transport, base_url="http://testserver") as client:
        with patch(
            "src.services.biodoc_webhook_service.download_image_as_base64",
            new=AsyncMock(return_value=__import__("base64").b64encode(_DUMMY_JPEG).decode()),
        ):
            response = await client.post(
                "/webhook/biodoc",
                json=payload,
                headers=VALID_HEADERS,
            )

    app.dependency_overrides.clear()
    assert response.status_code == 200
    defense_mock.resolve_org_code.assert_awaited_once_with("VIVER")
    assert _extract_org_code_arg(defense_mock.sync_person.await_args) == "001021"


@pytest.mark.asyncio
async def test_webhook_card_image_query_operador_ignored_and_warns_without_log_id(
    monkeypatch: pytest.MonkeyPatch,
    caplog: pytest.LogCaptureFixture,
):
    import logging

    monkeypatch.setenv("BIODOC_WEBHOOK_TOKEN", WEBHOOK_TOKEN)
    caplog.set_level(logging.WARNING, logger="biodoc_intelbras")

    defense_mock = _make_defense_mock(resolved_org_code="001008")
    biodoc_mock = _make_biodoc_mock(name="CARLOS EDUARDO LOBO", id_card="00271368992672000")

    app.dependency_overrides[get_defense_client] = lambda: defense_mock
    app.dependency_overrides[get_biodoc_client] = lambda: biodoc_mock

    transport = httpx.ASGITransport(app=app)
    async with httpx.AsyncClient(transport=transport, base_url="http://testserver") as client:
        with patch(
            "src.services.biodoc_webhook_service.download_image_as_base64",
            new=AsyncMock(return_value=__import__("base64").b64encode(_DUMMY_JPEG).decode()),
        ):
            response = await client.post(
                "/webhook/biodoc?operador=colaboradores",
                json=NEW_FORMAT_PAYLOAD,
                headers=VALID_HEADERS,
            )

    app.dependency_overrides.clear()
    assert response.status_code == 200
    assert any("external-audits não resolveu local_token" in r.message for r in caplog.records)
    biodoc_mock.get_integration_log.assert_not_awaited()
    defense_mock.resolve_org_code.assert_not_awaited()
    assert _extract_org_code_arg(defense_mock.sync_person.await_args) == "001"


@pytest.mark.asyncio
async def test_webhook_card_image_details_query_is_ignored(
    monkeypatch: pytest.MonkeyPatch,
):
    """?details= na URL do webhook não deve alterar orgCode (só verify + integrations/log)."""
    monkeypatch.setenv("BIODOC_WEBHOOK_TOKEN", WEBHOOK_TOKEN)

    defense_mock = _make_defense_mock(resolved_org_code="001008")
    biodoc_mock = _make_biodoc_mock(name="CARLOS EDUARDO LOBO", id_card="00271368992672000")

    app.dependency_overrides[get_defense_client] = lambda: defense_mock
    app.dependency_overrides[get_biodoc_client] = lambda: biodoc_mock

    transport = httpx.ASGITransport(app=app)
    async with httpx.AsyncClient(transport=transport, base_url="http://testserver") as client:
        with patch(
            "src.services.biodoc_webhook_service.download_image_as_base64",
            new=AsyncMock(return_value=__import__("base64").b64encode(_DUMMY_JPEG).decode()),
        ):
            response = await client.post(
                "/webhook/biodoc?details=%7B%22operador%22%3A%22colaboradores%22%7D",
                json=NEW_FORMAT_PAYLOAD,
                headers=VALID_HEADERS,
            )

    app.dependency_overrides.clear()
    assert response.status_code == 200
    defense_mock.resolve_org_code.assert_not_awaited()
    assert _extract_org_code_arg(defense_mock.sync_person.await_args) == "001"


@pytest.mark.asyncio
async def test_external_audits_widens_date_window_when_single_day_empty(
    monkeypatch: pytest.MonkeyPatch,
):
    """BioDoc sandbox: dia único vazio; janela ampliada encontra audit → integrations/log."""
    monkeypatch.setenv("BIODOC_WEBHOOK_TOKEN", WEBHOOK_TOKEN)

    defense_mock = _make_defense_mock(resolved_org_code="001021")
    biodoc_mock = _make_biodoc_mock(name="CARLOS EDUARDO LOBO", id_card="00271368992672000")

    async def audits_side_effect(card, *, initial_date, end_date):
        if initial_date == end_date:
            return []
        return [
            ExternalAuditEntry(
                id="d0b22da4-8ebb-4426-bff9-a8ed03bd2876",
                id_card=card,
                name="CARLOS EDUARDO LOBO",
                status="7",
                date="2026-05-30T16:48:14Z",
                required=28,
                required_name=None,
                description=None,
            )
        ]

    biodoc_mock.get_external_audits.side_effect = audits_side_effect
    biodoc_mock.get_integration_log.return_value = IntegrationLogData(
        id="d0b22da4-8ebb-4426-bff9-a8ed03bd2876",
        id_card="00271368992672000",
        name="CARLOS EDUARDO LOBO",
        status=7,
        main_image="https://example.com/face.jpg",
        path=None,
        required_name=None,
        operador="VIVER",
        local_token="CHU - ESPAÇO VIVER BEM",
    )

    app.dependency_overrides[get_defense_client] = lambda: defense_mock
    app.dependency_overrides[get_biodoc_client] = lambda: biodoc_mock

    payload = {**NEW_FORMAT_PAYLOAD, "date": "2026-05-31T16:22:00Z"}

    transport = httpx.ASGITransport(app=app)
    async with httpx.AsyncClient(transport=transport, base_url="http://testserver") as client:
        with patch(
            "src.services.biodoc_webhook_service.asyncio.sleep",
            new=AsyncMock(),
        ), patch(
            "src.services.biodoc_webhook_service.download_image_as_base64",
            new=AsyncMock(return_value=__import__("base64").b64encode(_DUMMY_JPEG).decode()),
        ):
            response = await client.post(
                "/webhook/biodoc",
                json=payload,
                headers=VALID_HEADERS,
            )

    app.dependency_overrides.clear()
    assert response.status_code == 200
    biodoc_mock.get_integration_log.assert_awaited_once_with(
        "d0b22da4-8ebb-4426-bff9-a8ed03bd2876"
    )
    defense_mock.resolve_org_code.assert_awaited_once_with("CHU - ESPAÇO VIVER BEM")
    assert _extract_org_code_arg(defense_mock.sync_person.await_args) == "001021"


@pytest.mark.asyncio
async def test_webhook_card_image_resolves_local_token_via_external_audits(
    monkeypatch: pytest.MonkeyPatch,
):
    """card+image sem log id: external-audits → integrations/log resolve local_token."""
    monkeypatch.setenv("BIODOC_WEBHOOK_TOKEN", WEBHOOK_TOKEN)

    defense_mock = _make_defense_mock(resolved_org_code="001008")
    biodoc_mock = _make_biodoc_mock(name="CARLOS EDUARDO LOBO", id_card="00271368992672000")
    biodoc_mock.get_external_audits.return_value = [
        ExternalAuditEntry(
            id=677,
            id_card="00271368992672000",
            name="CARLOS EDUARDO LOBO",
            status="Autenticado",
            date="2026-05-28T20:52:35Z",
            required=28,
            required_name="Colaboradores",
            description=None,
        )
    ]
    biodoc_mock.get_integration_log.return_value = IntegrationLogData(
        id=677,
        id_card="00271368992672000",
        name="CARLOS EDUARDO LOBO",
        status=2,
        main_image="https://example.com/face.jpg",
        path=None,
        required_name=None,
        operador="colaboradores",
        local_token="Colaboradores",
    )

    app.dependency_overrides[get_defense_client] = lambda: defense_mock
    app.dependency_overrides[get_biodoc_client] = lambda: biodoc_mock

    payload = {
        **NEW_FORMAT_PAYLOAD,
        "date": "2026-05-28T20:52:35Z",
    }

    transport = httpx.ASGITransport(app=app)
    async with httpx.AsyncClient(transport=transport, base_url="http://testserver") as client:
        with patch(
            "src.services.biodoc_webhook_service.download_image_as_base64",
            new=AsyncMock(return_value=__import__("base64").b64encode(_DUMMY_JPEG).decode()),
        ):
            response = await client.post(
                "/webhook/biodoc",
                json=payload,
                headers=VALID_HEADERS,
            )

    app.dependency_overrides.clear()
    assert response.status_code == 200
    biodoc_mock.get_external_audits.assert_awaited_once()
    biodoc_mock.get_integration_log.assert_awaited_once_with("677")
    defense_mock.resolve_org_code.assert_awaited_once_with("Colaboradores")
    assert _extract_org_code_arg(defense_mock.sync_person.await_args) == "001008"


@pytest.mark.asyncio
async def test_webhook_card_image_external_audits_failure_keeps_fallback(
    monkeypatch: pytest.MonkeyPatch,
):
    monkeypatch.setenv("BIODOC_WEBHOOK_TOKEN", WEBHOOK_TOKEN)

    defense_mock = _make_defense_mock()
    biodoc_mock = _make_biodoc_mock(name="CARLOS EDUARDO LOBO", id_card="00271368992672000")
    biodoc_mock.get_external_audits.side_effect = BiodocAPIUnavailableError("timeout")

    app.dependency_overrides[get_defense_client] = lambda: defense_mock
    app.dependency_overrides[get_biodoc_client] = lambda: biodoc_mock

    transport = httpx.ASGITransport(app=app)
    async with httpx.AsyncClient(transport=transport, base_url="http://testserver") as client:
        with patch(
            "src.services.biodoc_webhook_service.download_image_as_base64",
            new=AsyncMock(return_value=__import__("base64").b64encode(_DUMMY_JPEG).decode()),
        ):
            response = await client.post(
                "/webhook/biodoc",
                json=NEW_FORMAT_PAYLOAD,
                headers=VALID_HEADERS,
            )

    app.dependency_overrides.clear()
    assert response.status_code == 200
    biodoc_mock.get_integration_log.assert_not_awaited()
    defense_mock.resolve_org_code.assert_not_awaited()
    assert _extract_org_code_arg(defense_mock.sync_person.await_args) == "001"


@pytest.mark.asyncio
async def test_webhook_id_log_uses_integration_log(monkeypatch: pytest.MonkeyPatch):
    """id_Log no POST deve acionar GET /integrations/log/{reference_Id}."""
    monkeypatch.setenv("BIODOC_WEBHOOK_TOKEN", WEBHOOK_TOKEN)

    defense_mock = _make_defense_mock(resolved_org_code="001015001")
    biodoc_mock = _make_biodoc_mock()
    biodoc_mock.get_integration_log.return_value = IntegrationLogData(
        id=42,
        id_card="1234567890",
        name=MOCK_USER_NAME,
        status=2,
        main_image="https://example.com/face.jpg",
        path=None,
        required_name=None,
        operador="VIVER",
        local_token="CHU - ESPAÇO VIVER BEM",
    )

    app.dependency_overrides[get_defense_client] = lambda: defense_mock
    app.dependency_overrides[get_biodoc_client] = lambda: biodoc_mock

    payload = {
        "success": True,
        "status": 2,
        "id_Log": 42,
        "url": "https://example.com/face.jpg",
    }

    transport = httpx.ASGITransport(app=app)
    async with httpx.AsyncClient(transport=transport, base_url="http://testserver") as client:
        with patch(
            "src.services.biodoc_webhook_service.download_image_as_base64",
            new=AsyncMock(return_value=__import__("base64").b64encode(_DUMMY_JPEG).decode()),
        ):
            response = await client.post("/webhook/biodoc", json=payload, headers=VALID_HEADERS)

    app.dependency_overrides.clear()
    assert response.status_code == 200
    biodoc_mock.get_integration_log.assert_awaited_once_with("42")
    defense_mock.resolve_org_code.assert_awaited_once_with("CHU - ESPAÇO VIVER BEM")
