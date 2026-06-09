import httpx
import pytest

from src.services.biodoc_client import (
    BiodocClient,
    _extract_local_name_from_detail,
    _resolve_local_name_from_log,
)


@pytest.mark.asyncio
async def test_get_integration_log_uses_reguired_name_not_json_local_token():
    def handler(request: httpx.Request) -> httpx.Response:
        assert request.url.path.endswith("/integrations/log/ref-123")
        return httpx.Response(
            200,
            json={
                "data": {
                    "id": "ref-123",
                    "status": 2,
                    "id_Card": "00271368992672000",
                    "name": "CARLOS EDUARDO LOBO",
                    "mainImage": "https://example.com/face.png",
                    "path": "https://example.com/capture.png",
                    "reguiredName": "Refeitorio",
                    "detail": "{\"operador\":\"VIVER\"}",
                    "json": {
                        "Operador": "VIVER",
                        "Local Token": "CHU - ESPAÇO VIVER BEM",
                    },
                }
            },
        )

    client = BiodocClient(
        base_url="https://api.sandbox.biodoc.com.br/api",
        token_api="test-token",
    )
    client._client = httpx.AsyncClient(
        base_url="https://api.sandbox.biodoc.com.br/api",
        transport=httpx.MockTransport(handler),
    )

    result = await client.get_integration_log("ref-123")

    assert result.id_card == "00271368992672000"
    assert result.name == "CARLOS EDUARDO LOBO"
    assert result.status == 2
    assert result.required_name == "Refeitorio"
    assert result.operador == "VIVER"
    assert result.local_name == "Refeitorio"
    assert result.local_source == "reguiredName"

    await client.close()


@pytest.mark.asyncio
async def test_get_integration_log_uses_detail_nm_local_when_reguired_name_null():
    def handler(request: httpx.Request) -> httpx.Response:
        return httpx.Response(
            200,
            json={
                "id": 1,
                "status": 2,
                "id_Card": "00271368992672000",
                "name": "CARLOS EDUARDO LOBO",
                "mainImage": "https://example.com/face.png",
                "reguiredName": None,
                "detail": '{"operador":"VIVER","nmLocal":"CHU - ESPAÇO VIVER BEM"}',
            },
        )

    client = BiodocClient(
        base_url="https://api.sandbox.biodoc.com.br/api",
        token_api="test-token",
    )
    client._client = httpx.AsyncClient(
        base_url="https://api.sandbox.biodoc.com.br/api",
        transport=httpx.MockTransport(handler),
    )

    result = await client.get_integration_log("1")

    assert result.local_name == "CHU - ESPAÇO VIVER BEM"
    assert result.local_source == "detail"
    assert result.operador == "VIVER"

    await client.close()


@pytest.mark.asyncio
async def test_get_integration_log_parses_detail_dict():
    def handler(request: httpx.Request) -> httpx.Response:
        return httpx.Response(
            200,
            json={
                "id": 1,
                "status": 2,
                "id_Card": "00271368992672000",
                "name": "CARLOS EDUARDO LOBO",
                "mainImage": "https://example.com/face.png",
                "detail": {"operador": "VIVER"},
            },
        )

    client = BiodocClient(
        base_url="https://api.sandbox.biodoc.com.br/api",
        token_api="test-token",
    )
    client._client = httpx.AsyncClient(
        base_url="https://api.sandbox.biodoc.com.br/api",
        transport=httpx.MockTransport(handler),
    )

    result = await client.get_integration_log("1")

    assert result.operador == "VIVER"
    assert result.local_name is None

    await client.close()


@pytest.mark.asyncio
async def test_get_card_mainimage_accepts_flat_sandbox_payload():
    def handler(request: httpx.Request) -> httpx.Response:
        assert request.url.params["idCard"] == "00271368992672000"
        return httpx.Response(
            200,
            json={
                "cardStatus": 1,
                "name": "CARLOS EDUARDO LOBO",
                "mainImage": "https://example.com/main.png",
            },
        )

    client = BiodocClient(
        base_url="https://api.sandbox.biodoc.com.br/api",
        token_api="test-token",
    )
    client._client = httpx.AsyncClient(
        base_url="https://api.sandbox.biodoc.com.br/api",
        transport=httpx.MockTransport(handler),
    )

    result = await client.get_card_mainimage("00271368992672000")

    assert result.card == "00271368992672000"
    assert result.name == "CARLOS EDUARDO LOBO"
    assert result.status is True
    assert result.image == "https://example.com/main.png"

    await client.close()


@pytest.mark.asyncio
async def test_get_external_audits_parses_data_logs():
    def handler(request: httpx.Request) -> httpx.Response:
        assert request.url.path.endswith("/logs/external-audits")
        assert request.url.params["idCard"] == "00271368992672000"
        assert request.url.params["initialDate"] == "2026-05-30"
        assert request.url.params["endDate"] == "2026-05-30"
        return httpx.Response(
            200,
            json={
                "data": {
                    "logs": [
                        {
                            "id": 677,
                            "id_Card": "00271368992672000",
                            "name": "CARLOS EDUARDO LOBO",
                            "status": "Autenticado",
                            "date": "2026-05-30T14:59:40.067569",
                            "required": 28,
                            "required_Name": "Colaboradores",
                            "description": "verify",
                        }
                    ]
                }
            },
        )

    client = BiodocClient(
        base_url="https://api.sandbox.biodoc.com.br/api",
        token_api="test-token",
    )
    client._client = httpx.AsyncClient(
        base_url="https://api.sandbox.biodoc.com.br/api",
        transport=httpx.MockTransport(handler),
    )

    result = await client.get_external_audits(
        "00271368992672000",
        initial_date="2026-05-30",
        end_date="2026-05-30",
    )

    assert len(result) == 1
    assert result[0].id == 677
    assert result[0].id_card == "00271368992672000"
    assert result[0].required_name == "Colaboradores"

    await client.close()


@pytest.mark.asyncio
async def test_get_external_audits_empty_logs():
    def handler(request: httpx.Request) -> httpx.Response:
        return httpx.Response(200, json={"data": {"logs": []}})

    client = BiodocClient(
        base_url="https://api.sandbox.biodoc.com.br/api",
        token_api="test-token",
    )
    client._client = httpx.AsyncClient(
        base_url="https://api.sandbox.biodoc.com.br/api",
        transport=httpx.MockTransport(handler),
    )

    result = await client.get_external_audits("00271368992672000")

    assert result == []

    await client.close()


def test_extract_operador_from_detail_usuclin():
    from src.services.biodoc_client import _extract_operador_from_detail

    assert _extract_operador_from_detail({"usuclin": "VIVER"}) == "VIVER"


def test_extract_local_name_from_detail_nm_local():
    assert _extract_local_name_from_detail({"nmLocal": "Hospital Central"}) == "Hospital Central"


def test_extract_local_name_from_detail_required_name():
    assert _extract_local_name_from_detail({"RequiredName": "TOKEN123"}) == "TOKEN123"


def test_extract_local_name_from_detail_ignores_operador():
    assert _extract_local_name_from_detail({"operador": "VIVER"}) is None


def test_resolve_local_name_from_log_prefers_reguired_name():
    value, source = _resolve_local_name_from_log(
        {
            "reguiredName": "Refeitorio",
            "detail": '{"nmLocal":"Outro Local"}',
        }
    )
    assert value == "Refeitorio"
    assert source == "reguiredName"
