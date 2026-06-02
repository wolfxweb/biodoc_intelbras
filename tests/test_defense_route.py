import os

os.environ.setdefault("SKIP_DOTENV", "1")

from unittest.mock import AsyncMock, patch

import httpx
import pytest
from httpx import ASGITransport

from src.api.dependencies import get_biodoc_client, get_defense_client
from src.main import app
from src.services.biodoc_client import (
    BiodocClient,
    CardMainImageData,
    ExternalAuditEntry,
    IntegrationLogData,
)
from src.services.defense_ia_client import DefenseIASettings

_DUMMY_JPEG = b"\xff\xd8\xff" + b"\x00" * 2048
MOCK_USER_NAME = "CARLOS EDUARDO LOBO"


def _make_defense_mock(resolved_org_code: str = "001021") -> AsyncMock:
    client = AsyncMock()
    client.sync_visitor.return_value = {
        "code": 1000,
        "data": {"visitorId": "42", "personId": "999"},
    }
    client.resolve_org_code.return_value = resolved_org_code
    client.resolve_org_name_by_code.return_value = "CHU - ESPAÇO VIVER BEM"
    client.settings = DefenseIASettings(
        server_url="http://defense.test",
        username="u",
        password="p",
        api_mode="brms",
        org_code="001",
        sync_target="visitor",
    )
    client.is_ready = True
    return client


@pytest.mark.asyncio
async def test_webhook_biodoc_post_logs_payload_and_returns_ok(
    caplog: pytest.LogCaptureFixture,
):
    import logging

    caplog.set_level(logging.INFO, logger="biodoc_intelbras")
    transport = ASGITransport(app=app)
    payload = {"event": "access", "deviceId": "cam-01", "personId": "12345"}

    async with httpx.AsyncClient(transport=transport, base_url="http://testserver") as client:
        response = await client.post("/webhook/biodoc", json=payload)

    assert response.status_code == 200
    assert response.json() == {"status": "ok"}

    inbound = [r.message for r in caplog.records if "[WEBHOOK IN]" in r.message]
    assert any("POST /webhook/biodoc" in m for m in inbound)
    assert any("deviceId" in m for m in inbound)
    assert any("HTTP 200" in m for m in inbound)


@pytest.mark.asyncio
async def test_defense_get_runs_external_audits_and_syncs_defense(
    caplog: pytest.LogCaptureFixture,
):
    import logging

    caplog.set_level(logging.INFO, logger="biodoc_intelbras")

    defense_mock = _make_defense_mock(resolved_org_code="001021")
    biodoc_mock = AsyncMock(spec=BiodocClient)
    biodoc_mock.get_card_mainimage.return_value = CardMainImageData(
        name=MOCK_USER_NAME,
        card="00271368992672000",
        status=True,
        image="https://example.com/face.jpg",
    )
    biodoc_mock.get_external_audits.return_value = [
        ExternalAuditEntry(
            id="d0b22da4-8ebb-4426-bff9-a8ed03bd2876",
            id_card="00271368992672000",
            name=MOCK_USER_NAME,
            status="7",
            date="2026-05-30T16:48:14Z",
            required=28,
            required_name=None,
            description=None,
        )
    ]
    biodoc_mock.get_integration_log.return_value = IntegrationLogData(
        id="d0b22da4-8ebb-4426-bff9-a8ed03bd2876",
        id_card="00271368992672000",
        name=MOCK_USER_NAME,
        status=7,
        main_image="https://example.com/verify-capture.jpg",
        path=None,
        required_name=None,
        operador="VIVER",
        local_token="CHU - ESPAÇO VIVER BEM",
    )
    app.dependency_overrides[get_defense_client] = lambda: defense_mock
    app.dependency_overrides[get_biodoc_client] = lambda: biodoc_mock

    transport = ASGITransport(app=app)
    try:
        with patch(
            "src.services.biodoc_webhook_service.download_image_as_base64",
            new=AsyncMock(return_value=__import__("base64").b64encode(_DUMMY_JPEG).decode()),
        ) as download_mock:
            async with httpx.AsyncClient(transport=transport, base_url="http://testserver") as client:
                response = await client.get(
                    "/webhook/biodoc",
                    params={
                        "card": "00271368992672000",
                        "date": "31/05/2026 15:50:32",
                        "response": "200",
                        "message": "Face Reconhecida",
                    },
                )
    finally:
        app.dependency_overrides.clear()

    assert response.status_code == 200
    assert "text/html" in response.headers.get("content-type", "")
    body = response.text
    assert "Validação realizada com sucesso" in body
    assert "Unimed Joinville" in body
    assert MOCK_USER_NAME in body
    assert "CHU - ESPAÇO VIVER BEM" in body
    assert "Seu cadastro foi adicionado no Defense" in body

    messages = [r.message for r in caplog.records]
    assert any("external-audits escolheu log" in m for m in messages)
    assert any("local_token resolvido" in m for m in messages)
    assert any("imagem resolvida" in m for m in messages)
    assert any("integrations/log" in m for m in messages)
    assert any("verify-capture.jpg" in m for m in messages)
    assert any("orgCode resolvido" in m for m in messages)
    assert any("001021" in m for m in messages)
    download_mock.assert_awaited_once_with("https://example.com/verify-capture.jpg")
    defense_mock.sync_visitor.assert_awaited_once()
    defense_mock.resolve_org_code.assert_awaited_once_with("CHU - ESPAÇO VIVER BEM")
    call_args = defense_mock.sync_visitor.await_args
    assert call_args is not None
    assert call_args[0][1] == "001021"


@pytest.mark.asyncio
async def test_defense_get_failed_response_skips_sync():
    defense_mock = _make_defense_mock()
    biodoc_mock = AsyncMock(spec=BiodocClient)
    app.dependency_overrides[get_defense_client] = lambda: defense_mock
    app.dependency_overrides[get_biodoc_client] = lambda: biodoc_mock

    transport = ASGITransport(app=app)
    try:
        async with httpx.AsyncClient(transport=transport, base_url="http://testserver") as client:
            response = await client.get(
                "/webhook/biodoc",
                params={"card": "123", "response": "500"},
            )
    finally:
        app.dependency_overrides.clear()

    assert response.status_code == 200
    assert "text/html" in response.headers.get("content-type", "")
    assert "Verificação não concluída" in response.text
    defense_mock.sync_visitor.assert_not_called()
