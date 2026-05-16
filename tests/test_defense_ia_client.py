import pytest
import httpx

from src.api.schemas import SyncRequest
from src.services.defense_ia_client import DefenseIAClient, DefenseIASettings


def sync_payload() -> SyncRequest:
    return SyncRequest.model_validate(
        {
            "source": "biodoc",
            "operation": "upsert",
            "external_id": "123",
            "person": {"full_name": "Maria Silva", "document": "12345678900"},
            "biometrics": {"face_image_base64": "base64-image"},
        }
    )


@pytest.mark.asyncio
async def test_defense_client_login_keep_alive_and_sync_payload():
    requests: list[httpx.Request] = []

    async def handler(request: httpx.Request) -> httpx.Response:
        requests.append(request)
        if request.url.path == "/admin/API/accounts/authorize" and len(requests) == 1:
            return httpx.Response(200, json={"realm": "realm", "randomKey": "random"})
        if request.url.path == "/admin/API/accounts/authorize":
            return httpx.Response(200, json={"token": "defense-token"})
        if request.url.path == "/admin/API/accounts/updateToken":
            return httpx.Response(200, json={"ok": True})
        if request.url.path == "/OBMS/accessControl/person/123":
            return httpx.Response(200, json={"synced": True})
        return httpx.Response(404)

    transport = httpx.MockTransport(handler)
    http_client = httpx.AsyncClient(transport=transport)
    client = DefenseIAClient(
        settings=DefenseIASettings(
            server_url="http://defense.test",
            username="admin",
            password="password",
        ),
        http_client=http_client,
    )

    try:
        token = await client.login()
        await client.keep_alive_once()
        result = await client.sync_person(sync_payload())
    finally:
        await client.close()
        await http_client.aclose()

    assert token == "defense-token"
    assert client.token == "defense-token"
    assert result == {"synced": True}
    assert any(request.url.path == "/admin/API/accounts/updateToken" for request in requests)


@pytest.mark.asyncio
async def test_defense_client_relogin_on_sync_401():
    authorize_count = 0
    sync_count = 0

    async def handler(request: httpx.Request) -> httpx.Response:
        nonlocal authorize_count, sync_count
        if request.url.path == "/admin/API/accounts/authorize":
            authorize_count += 1
            if authorize_count % 2 == 1:
                return httpx.Response(200, json={"realm": "realm", "randomKey": "random"})
            return httpx.Response(200, json={"token": f"defense-token-{authorize_count}"})
        if request.url.path == "/OBMS/accessControl/person/123":
            sync_count += 1
            if sync_count == 1:
                return httpx.Response(401, json={"error": "expired"})
            return httpx.Response(200, json={"synced": True})
        return httpx.Response(404)

    http_client = httpx.AsyncClient(transport=httpx.MockTransport(handler))
    client = DefenseIAClient(
        settings=DefenseIASettings(
            server_url="http://defense.test",
            username="admin",
            password="password",
        ),
        http_client=http_client,
    )

    try:
        await client.login()
        result = await client.sync_person(sync_payload())
    finally:
        await client.close()
        await http_client.aclose()

    assert result == {"synced": True}
    assert authorize_count == 4
    assert sync_count == 2


def test_build_person_payload_maps_biodoc_fields():
    client = DefenseIAClient(
        settings=DefenseIASettings(
            server_url="http://defense.test",
            username="admin",
            password="password",
        )
    )

    payload = client.build_person_payload(sync_payload())

    assert payload["baseInfo"]["personId"] == "123"
    assert payload["baseInfo"]["firstName"] == "Maria Silva"
    assert payload["baseInfo"]["pictureData"] == "base64-image"
    assert payload["details"]["idNum"] == "12345678900"
