import json

import pytest
import httpx

from src.api.schemas import SyncRequest
from src.services.defense_ia_client import (
    BRMS_AUTHORIZE,
    BRMS_KEEPALIVE,
    BRMS_PERSON,
    DefenseIAClient,
    DefenseIAError,
    DefenseIASettings,
    brms_person_path,
    build_auth_signatures,
    extract_face_pictures_from_person_body,
)


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


def brms_settings(**overrides: object) -> DefenseIASettings:
    defaults = {
        "server_url": "http://defense.test",
        "username": "admin",
        "password": "password",
        "api_mode": "brms",
        "public_key": "MIIB-test-public-key",
    }
    defaults.update(overrides)
    return DefenseIASettings(**defaults)  # type: ignore[arg-type]


@pytest.mark.asyncio
async def test_defense_client_brms_login_keep_alive_and_sync_payload():
    requests: list[httpx.Request] = []
    authorize_hits = 0

    async def handler(request: httpx.Request) -> httpx.Response:
        requests.append(request)
        nonlocal authorize_hits
        if request.url.path == BRMS_AUTHORIZE:
            authorize_hits += 1
            if authorize_hits == 1:
                return httpx.Response(
                    401,
                    json={"realm": "realm", "randomKey": "random", "encryptType": "MD5"},
                )
            return httpx.Response(200, json={"code": 1000, "token": "defense-token"})
        if request.url.path == BRMS_KEEPALIVE:
            return httpx.Response(200, json={"ok": True})
        if request.url.path == BRMS_PERSON and request.method == "POST":
            return httpx.Response(200, json={"code": 1000, "synced": True})
        if request.url.path == brms_person_path("123"):
            if request.method == "GET":
                return httpx.Response(404, json={"code": 404, "desc": "not found"})
            return httpx.Response(404)
        if request.url.path == "/OBMS/accessControl/person/123":
            return httpx.Response(200, json={"synced": True})
        return httpx.Response(404)

    http_client = httpx.AsyncClient(transport=httpx.MockTransport(handler))
    client = DefenseIAClient(settings=brms_settings(), http_client=http_client)

    try:
        token = await client.login()
        await client.keep_alive_once()
        result = await client.sync_person(sync_payload())
    finally:
        await client.close()
        await http_client.aclose()

    assert token == "defense-token"
    assert client.token == "defense-token"
    assert result.get("synced") is True
    assert authorize_hits == 2
    second_body = requests[1].content.decode()
    assert '"userType": "0"' in second_body or '"userType":"0"' in second_body
    assert '"publicKey"' in second_body


@pytest.mark.asyncio
async def test_defense_client_brms_requires_public_key():
    async def handler(request: httpx.Request) -> httpx.Response:
        if request.url.path == BRMS_AUTHORIZE:
            return httpx.Response(
                401,
                json={"realm": "r", "randomKey": "k", "encryptType": "MD5"},
            )
        return httpx.Response(404)

    http_client = httpx.AsyncClient(transport=httpx.MockTransport(handler))
    client = DefenseIAClient(
        settings=brms_settings(public_key="", use_server_public_key=False),
        http_client=http_client,
    )
    try:
        with pytest.raises(DefenseIAError, match="DEFENSE_IA_PUBLIC_KEY|publicKey|generate_defense"):
            await client.login()
    finally:
        await client.close()
        await http_client.aclose()


@pytest.mark.asyncio
async def test_defense_client_brms_uses_server_public_key_when_enabled():
    authorize_hits = 0

    async def handler(request: httpx.Request) -> httpx.Response:
        nonlocal authorize_hits
        if request.url.path == BRMS_AUTHORIZE:
            authorize_hits += 1
            if authorize_hits == 1:
                return httpx.Response(
                    401,
                    json={
                        "realm": "r",
                        "randomKey": "k",
                        "encryptType": "MD5",
                        "publickey": "SERVER-PUBLIC-KEY-B64",
                    },
                )
            body = json.loads(request.content.decode())
            assert body["publicKey"] == "SERVER-PUBLIC-KEY-B64"
            return httpx.Response(200, json={"code": 1000, "token": "t"})
        return httpx.Response(404)

    client = DefenseIAClient(
        settings=brms_settings(public_key="", use_server_public_key=True),
        http_client=httpx.AsyncClient(transport=httpx.MockTransport(handler)),
    )
    await client.login()


@pytest.mark.asyncio
async def test_defense_client_relogin_on_sync_401():
    authorize_count = 0
    sync_count = 0

    async def handler(request: httpx.Request) -> httpx.Response:
        nonlocal authorize_count, sync_count
        if request.url.path == BRMS_AUTHORIZE:
            authorize_count += 1
            if authorize_count % 2 == 1:
                return httpx.Response(
                    401,
                    json={"realm": "realm", "randomKey": "random", "encryptType": "MD5"},
                )
            return httpx.Response(
                200,
                json={"code": 1000, "token": f"defense-token-{authorize_count}"},
            )
        if request.url.path == brms_person_path("123"):
            if request.method == "GET":
                return httpx.Response(404, json={"code": 404})
        if request.url.path == BRMS_PERSON and request.method == "POST":
            sync_count += 1
            if sync_count == 1:
                return httpx.Response(401, json={"error": "expired"})
            return httpx.Response(200, json={"synced": True})
        return httpx.Response(404)

    http_client = httpx.AsyncClient(transport=httpx.MockTransport(handler))
    client = DefenseIAClient(settings=brms_settings(), http_client=http_client)

    try:
        await client.login()
        result = await client.sync_person(sync_payload())
    finally:
        await client.close()
        await http_client.aclose()

    assert result.get("synced") is True
    assert authorize_count == 4
    assert sync_count == 2


def test_build_auth_signatures_matches_doc_example_chain():
    signature, dollar = build_auth_signatures("user", "pass", "realm", "key")
    assert len(signature) == 32
    assert len(dollar) == 32


@pytest.mark.asyncio
async def test_brms_upsert_posts_when_person_missing():
    calls: list[tuple[str, str]] = []
    authorize_hits = 0

    async def handler(request: httpx.Request) -> httpx.Response:
        calls.append((request.method, request.url.path))
        nonlocal authorize_hits
        if request.url.path == BRMS_AUTHORIZE:
            authorize_hits += 1
            if authorize_hits == 1:
                return httpx.Response(
                    401,
                    json={"realm": "r", "randomKey": "k", "encryptType": "MD5"},
                )
            return httpx.Response(200, json={"code": 1000, "token": "t"})
        if request.url.path == brms_person_path("123") and request.method == "GET":
            return httpx.Response(404, json={"code": 404})
        if request.url.path == BRMS_PERSON and request.method == "POST":
            return httpx.Response(200, json={"code": 1000, "created": True})
        return httpx.Response(404)

    http_client = httpx.AsyncClient(transport=httpx.MockTransport(handler))
    client = DefenseIAClient(settings=brms_settings(), http_client=http_client)
    try:
        await client.login()
        result = await client.sync_person(sync_payload())
    finally:
        await client.close()
        await http_client.aclose()

    assert result.get("created") is True
    assert ("GET", brms_person_path("123")) in calls
    assert ("POST", BRMS_PERSON) in calls
    assert not any(m == "PUT" for m, _ in calls)


@pytest.mark.asyncio
async def test_brms_upsert_puts_when_person_exists():
    calls: list[tuple[str, str]] = []
    authorize_hits = 0

    async def handler(request: httpx.Request) -> httpx.Response:
        calls.append((request.method, request.url.path))
        nonlocal authorize_hits
        if request.url.path == BRMS_AUTHORIZE:
            authorize_hits += 1
            if authorize_hits == 1:
                return httpx.Response(
                    401,
                    json={"realm": "r", "randomKey": "k", "encryptType": "MD5"},
                )
            return httpx.Response(200, json={"code": 1000, "token": "t"})
        if request.url.path == brms_person_path("123") and request.method == "GET":
            return httpx.Response(200, json={"code": 1000, "data": {"personId": "123"}})
        if request.url.path == brms_person_path("123") and request.method == "PUT":
            return httpx.Response(200, json={"code": 1000, "updated": True})
        return httpx.Response(404)

    http_client = httpx.AsyncClient(transport=httpx.MockTransport(handler))
    client = DefenseIAClient(settings=brms_settings(), http_client=http_client)
    try:
        await client.login()
        result = await client.sync_person(sync_payload())
    finally:
        await client.close()
        await http_client.aclose()

    assert result.get("updated") is True
    assert ("GET", brms_person_path("123")) in calls
    assert ("PUT", brms_person_path("123")) in calls
    assert not any(m == "POST" and p == BRMS_PERSON for m, p in calls)


def test_build_person_payload_maps_biodoc_fields():
    client = DefenseIAClient(settings=brms_settings())

    payload = client.build_person_payload(sync_payload())

    assert payload["baseInfo"]["personId"] == "123"
    assert payload["baseInfo"]["firstName"] == "Maria Silva"
    assert payload["baseInfo"]["facePictures"] == ["base64-image"]
    assert payload["extensionInfo"]["idNo"] == "12345678900"


def test_build_person_payload_without_face():
    client = DefenseIAClient(settings=brms_settings())
    request = SyncRequest.model_validate(
        {
            "source": "biodoc",
            "operation": "upsert",
            "external_id": "123",
            "person": {"full_name": "Maria Silva", "document": "12345678900"},
        }
    )

    payload = client.build_person_payload(request)

    assert payload["baseInfo"]["facePictures"] == []


def test_build_person_payload_preserves_existing_faces():
    client = DefenseIAClient(settings=brms_settings())
    request = SyncRequest.model_validate(
        {
            "source": "biodoc",
            "operation": "upsert",
            "external_id": "123",
            "person": {"full_name": "Maria Silva", "document": "12345678900"},
        }
    )

    payload = client.build_person_payload(
        request,
        existing_face_pictures=["foto-existente"],
    )

    assert payload["baseInfo"]["facePictures"] == ["foto-existente"]


def test_extract_face_pictures_from_person_body():
    body = {"code": 1000, "data": {"baseInfo": {"facePictures": ["f1", "f2"]}}}
    assert extract_face_pictures_from_person_body(body) == ["f1", "f2"]


@pytest.mark.asyncio
async def test_brms_put_preserves_face_on_update_without_biometrics():
    put_bodies: list[dict] = []
    authorize_hits = 0

    async def handler(request: httpx.Request) -> httpx.Response:
        nonlocal authorize_hits
        if request.url.path == BRMS_AUTHORIZE:
            authorize_hits += 1
            if authorize_hits == 1:
                return httpx.Response(
                    401,
                    json={"realm": "r", "randomKey": "k", "encryptType": "MD5"},
                )
            return httpx.Response(200, json={"code": 1000, "token": "t"})
        if request.url.path == brms_person_path("123") and request.method == "GET":
            return httpx.Response(
                200,
                json={
                    "code": 1000,
                    "data": {"baseInfo": {"facePictures": ["foto-existente"]}},
                },
            )
        if request.url.path == brms_person_path("123") and request.method == "PUT":
            put_bodies.append(json.loads(request.content.decode()))
            return httpx.Response(200, json={"code": 1000, "updated": True})
        return httpx.Response(404)

    request = SyncRequest.model_validate(
        {
            "source": "biodoc",
            "operation": "upsert",
            "external_id": "123",
            "person": {"full_name": "Maria Silva", "document": "12345678900"},
        }
    )

    http_client = httpx.AsyncClient(transport=httpx.MockTransport(handler))
    client = DefenseIAClient(settings=brms_settings(), http_client=http_client)
    try:
        await client.login()
        await client.sync_person(request)
    finally:
        await client.close()
        await http_client.aclose()

    assert len(put_bodies) == 1
    assert put_bodies[0]["baseInfo"]["facePictures"] == ["foto-existente"]
