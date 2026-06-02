import base64
import json

import pytest
import httpx

from src.api.schemas import SyncRequest
from src.services.defense_ia_client import (
    BRMS_AUTHORIZE,
    BRMS_KEEPALIVE,
    BRMS_PERSON,
    BRMS_PERSON_DELETE_BATCH,
    BRMS_PERSON_GROUP_LIST,
    BRMS_VISITOR,
    BRMS_VISITOR_CONFIG,
    DefenseIAClient,
    DefenseIAError,
    DefenseIASettings,
    DefenseIAArgumentError,
    brms_person_path,
    build_auth_signatures,
    extract_face_pictures_from_person_body,
    extract_sync_result_ids,
    parse_visitor_channel_default,
    parse_visitor_channel_map,
)


_FAKE_JPEG_B64 = base64.b64encode(b"\xff\xd8\xff" + b"\x00" * 2048).decode()


def sync_payload() -> SyncRequest:
    return SyncRequest.model_validate(
        {
            "source": "biodoc",
            "operation": "upsert",
            "external_id": "123",
            "person": {"full_name": "Maria Silva", "document": "12345678900"},
            "biometrics": {"face_image_base64": _FAKE_JPEG_B64},
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
    assert payload["baseInfo"]["facePictures"] == [_FAKE_JPEG_B64]
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


def test_build_person_payload_uses_settings_org_code_by_default():
    """Sem `org_code` explícito, usa o `org_code` do settings (.env)."""
    client = DefenseIAClient(settings=brms_settings(org_code="001"))

    payload = client.build_person_payload(sync_payload())

    assert payload["baseInfo"]["orgCode"] == "001"
    # Não usamos mais o mecanismo de accessGroupIds neste servidor.
    assert "accessGroupIds" not in payload["accessInfo"]
    assert "enableAccessGroup" not in payload["accessInfo"]
    assert "passageRules" not in payload["accessInfo"]


def test_build_person_payload_with_explicit_org_code():
    """Quando passamos `org_code`, ele sobrescreve o default do settings."""
    client = DefenseIAClient(settings=brms_settings(org_code="001"))

    payload = client.build_person_payload(sync_payload(), org_code="001015001")

    assert payload["baseInfo"]["orgCode"] == "001015001"


def test_build_person_payload_falls_back_to_default_when_org_code_blank():
    """`org_code` vazio (None ou string em branco) cai no default do settings."""
    client = DefenseIAClient(settings=brms_settings(org_code="001"))

    payload_none = client.build_person_payload(sync_payload(), org_code=None)
    payload_blank = client.build_person_payload(sync_payload(), org_code="   ")

    assert payload_none["baseInfo"]["orgCode"] == "001"
    assert payload_blank["baseInfo"]["orgCode"] == "001"


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


def _make_person_group_list_response(
    results: list[dict], *, code: int | str = 1000
) -> httpx.Response:
    return httpx.Response(
        200,
        json={"code": code, "data": {"results": results}},
    )


PERSON_PAGE_PATH = "/obms/api/v1.1/acs/person/page"


@pytest.mark.asyncio
async def test_list_person_groups_caches():
    """GET person-group/list + cache: segunda chamada não refaz HTTP."""
    group_calls = 0
    authorize_hits = 0

    async def handler(request: httpx.Request) -> httpx.Response:
        nonlocal group_calls, authorize_hits
        if request.url.path == BRMS_AUTHORIZE:
            authorize_hits += 1
            if authorize_hits == 1:
                return httpx.Response(
                    401,
                    json={"realm": "r", "randomKey": "k", "encryptType": "MD5"},
                )
            return httpx.Response(200, json={"code": 1000, "token": "t"})
        if request.url.path == BRMS_PERSON_GROUP_LIST:
            group_calls += 1
            return _make_person_group_list_response(
                [
                    {"orgCode": "001", "orgName": "Unimed"},
                    {"orgCode": "001015001", "orgName": "Corb"},
                    {"orgCode": "001008", "orgName": "Colaboradores"},
                ]
            )
        return httpx.Response(404)

    http_client = httpx.AsyncClient(transport=httpx.MockTransport(handler))
    client = DefenseIAClient(settings=brms_settings(), http_client=http_client)
    try:
        await client.login()
        orgs1 = await client.list_person_groups()
        group_calls_first = group_calls
        orgs2 = await client.list_person_groups()
    finally:
        await client.close()
        await http_client.aclose()

    assert orgs1 == {
        "unimed": "001",
        "corb": "001015001",
        "colaboradores": "001008",
    }
    assert orgs2 == orgs1
    assert group_calls == group_calls_first == 1


@pytest.mark.asyncio
async def test_resolve_org_code_returns_digits_directly_without_api_call():
    """Se o BioDoc enviar código puramente numérico, usamos como orgCode sem API."""
    api_calls = 0

    async def handler(request: httpx.Request) -> httpx.Response:
        nonlocal api_calls
        api_calls += 1
        return httpx.Response(404)

    http_client = httpx.AsyncClient(transport=httpx.MockTransport(handler))
    client = DefenseIAClient(settings=brms_settings(), http_client=http_client)
    try:
        assert await client.resolve_org_code("001015001") == "001015001"
        assert await client.resolve_org_code("  001008  ") == "001008"
        assert await client.resolve_org_code("123") == "123"
    finally:
        await client.close()
        await http_client.aclose()

    assert api_calls == 0


@pytest.mark.asyncio
async def test_resolve_org_code_lookup_case_insensitive_and_no_refresh_on_miss():
    """Lookup case-insensitive; miss retorna None sem refetch do person-group/list."""
    group_calls = 0
    authorize_hits = 0

    async def handler(request: httpx.Request) -> httpx.Response:
        nonlocal authorize_hits, group_calls
        if request.url.path == BRMS_AUTHORIZE:
            authorize_hits += 1
            if authorize_hits == 1:
                return httpx.Response(
                    401,
                    json={"realm": "r", "randomKey": "k", "encryptType": "MD5"},
                )
            return httpx.Response(200, json={"code": 1000, "token": "t"})
        if request.url.path == BRMS_PERSON_GROUP_LIST:
            group_calls += 1
            return _make_person_group_list_response(
                [
                    {"orgCode": "001", "orgName": "Unimed"},
                    {"orgCode": "001015001", "orgName": "Corb"},
                ]
            )
        return httpx.Response(404)

    http_client = httpx.AsyncClient(transport=httpx.MockTransport(handler))
    client = DefenseIAClient(settings=brms_settings(), http_client=http_client)
    try:
        await client.login()
        assert await client.resolve_org_code("UNIMED") == "001"
        assert await client.resolve_org_code("Corb") == "001015001"
        assert await client.resolve_org_code("  corb  ") == "001015001"
        assert await client.resolve_org_code("Inexistente") is None
        assert await client.resolve_org_code("OutroDesconhecido") is None
        assert await client.resolve_org_code(None) is None
        assert await client.resolve_org_code("") is None
        assert await client.resolve_org_code("   ") is None
    finally:
        await client.close()
        await http_client.aclose()

    assert group_calls == 1


@pytest.mark.asyncio
async def test_list_person_groups_force_refresh():
    """Permite invalidar o cache explicitamente."""
    group_calls = 0
    authorize_hits = 0

    async def handler(request: httpx.Request) -> httpx.Response:
        nonlocal authorize_hits, group_calls
        if request.url.path == BRMS_AUTHORIZE:
            authorize_hits += 1
            if authorize_hits == 1:
                return httpx.Response(
                    401,
                    json={"realm": "r", "randomKey": "k", "encryptType": "MD5"},
                )
            return httpx.Response(200, json={"code": 1000, "token": "t"})
        if request.url.path == BRMS_PERSON_GROUP_LIST:
            group_calls += 1
            return _make_person_group_list_response(
                [{"orgCode": "001", "orgName": "Unimed"}]
            )
        return httpx.Response(404)

    http_client = httpx.AsyncClient(transport=httpx.MockTransport(handler))
    client = DefenseIAClient(settings=brms_settings(), http_client=http_client)
    try:
        await client.login()
        await client.list_person_groups()
        await client.list_person_groups()  # cache hit
        await client.list_person_groups(force_refresh=True)
    finally:
        await client.close()
        await http_client.aclose()

    assert group_calls == 2


@pytest.mark.asyncio
async def test_list_available_orgs_empty_does_not_call_person_page():
    """Lista vazia de person-group/list não dispara varredura person/page."""
    group_calls = 0
    page_calls = 0
    authorize_hits = 0

    async def handler(request: httpx.Request) -> httpx.Response:
        nonlocal group_calls, page_calls, authorize_hits
        if request.url.path == BRMS_AUTHORIZE:
            authorize_hits += 1
            if authorize_hits == 1:
                return httpx.Response(
                    401,
                    json={"realm": "r", "randomKey": "k", "encryptType": "MD5"},
                )
            return httpx.Response(200, json={"code": 1000, "token": "t"})
        if request.url.path == BRMS_PERSON_GROUP_LIST:
            group_calls += 1
            return httpx.Response(200, json={"code": 1000, "data": {"results": []}})
        if request.url.path == PERSON_PAGE_PATH:
            page_calls += 1
            return httpx.Response(404)
        return httpx.Response(404)

    http_client = httpx.AsyncClient(transport=httpx.MockTransport(handler))
    client = DefenseIAClient(settings=brms_settings(), http_client=http_client)
    try:
        await client.login()
        orgs, source = await client.list_available_orgs(force_refresh=True)
        assert orgs == {}
        assert source == "person-group/list"
        assert await client.resolve_org_code("Corb") is None
    finally:
        await client.close()
        await http_client.aclose()

    assert group_calls >= 1
    assert page_calls == 0


@pytest.mark.asyncio
async def test_resolve_org_code_prefers_person_group_list():
    group_calls = 0
    page_calls = 0
    authorize_hits = 0

    async def handler(request: httpx.Request) -> httpx.Response:
        nonlocal group_calls, page_calls, authorize_hits
        if request.url.path == BRMS_AUTHORIZE:
            authorize_hits += 1
            if authorize_hits == 1:
                return httpx.Response(
                    401,
                    json={"realm": "r", "randomKey": "k", "encryptType": "MD5"},
                )
            return httpx.Response(200, json={"code": 1000, "token": "t"})
        if request.url.path == BRMS_PERSON_GROUP_LIST:
            group_calls += 1
            return httpx.Response(
                200,
                json={
                    "data": [
                        {"orgCode": "001002", "orgName": "CHU Central"},
                        {"orgCode": "001003", "orgName": "Refeitorio"},
                    ]
                },
            )
        if request.url.path == PERSON_PAGE_PATH:
            page_calls += 1
            return httpx.Response(404)
        return httpx.Response(404)

    http_client = httpx.AsyncClient(transport=httpx.MockTransport(handler))
    client = DefenseIAClient(settings=brms_settings(), http_client=http_client)
    try:
        await client.login()
        assert await client.resolve_org_code("Refeitorio") == "001003"
        assert await client.resolve_org_code("chu central") == "001002"
    finally:
        await client.close()
        await http_client.aclose()

    assert group_calls >= 1
    assert page_calls == 0


@pytest.mark.asyncio
async def test_parse_org_name_code_pairs_from_results_key():
    from src.services.defense_ia_client import _parse_org_name_code_pairs

    body = {
        "code": 1000,
        "data": {
            "results": [
                {"orgCode": "001008", "orgName": "Colaboradores"},
                {"orgCode": "001015001", "orgName": "Corb"},
            ]
        },
    }
    orgs = _parse_org_name_code_pairs(body)
    assert orgs["colaboradores"] == "001008"
    assert orgs["corb"] == "001015001"


@pytest.mark.asyncio
async def test_resolve_org_code_from_person_group_results():
    """Resolve nomes via person-group/list (data.results), sem person/page."""
    group_calls = 0
    page_calls = 0
    authorize_hits = 0

    async def handler(request: httpx.Request) -> httpx.Response:
        nonlocal group_calls, page_calls, authorize_hits
        if request.url.path == BRMS_AUTHORIZE:
            authorize_hits += 1
            if authorize_hits == 1:
                return httpx.Response(
                    401,
                    json={"realm": "r", "randomKey": "k", "encryptType": "MD5"},
                )
            return httpx.Response(200, json={"code": 1000, "token": "t"})
        if request.url.path == BRMS_PERSON_GROUP_LIST:
            group_calls += 1
            return httpx.Response(
                200,
                json={
                    "code": 1000,
                    "data": {
                        "results": [
                            {"orgCode": "001008", "orgName": "Colaboradores"},
                            {"orgCode": "001015001", "orgName": "Corb"},
                        ]
                    },
                },
            )
        if request.url.path == PERSON_PAGE_PATH:
            page_calls += 1
            return httpx.Response(404)
        return httpx.Response(404)

    http_client = httpx.AsyncClient(transport=httpx.MockTransport(handler))
    client = DefenseIAClient(settings=brms_settings(), http_client=http_client)
    try:
        await client.login()
        assert await client.resolve_org_code("colaboradores") == "001008"
        assert await client.resolve_org_code("Corb") == "001015001"
    finally:
        await client.close()
        await http_client.aclose()

    assert group_calls >= 1
    assert page_calls == 0


@pytest.mark.asyncio
async def test_sync_person_put_updates_org_code_in_payload():
    put_bodies: list[dict] = []
    get_count = 0
    authorize_hits = 0

    async def handler(request: httpx.Request) -> httpx.Response:
        nonlocal get_count, authorize_hits
        if request.url.path == BRMS_AUTHORIZE:
            authorize_hits += 1
            if authorize_hits == 1:
                return httpx.Response(
                    401,
                    json={"realm": "r", "randomKey": "k", "encryptType": "MD5"},
                )
            return httpx.Response(200, json={"code": 1000, "token": "t"})
        if request.url.path == brms_person_path("123") and request.method == "GET":
            get_count += 1
            if get_count == 1:
                return httpx.Response(
                    200,
                    json={"code": 1000, "data": {"baseInfo": {"orgCode": "001"}}},
                )
            return httpx.Response(
                200,
                json={"code": 1000, "data": {"baseInfo": {"orgCode": "001008"}}},
            )
        if request.url.path == brms_person_path("123") and request.method == "PUT":
            put_bodies.append(json.loads(request.content.decode()))
            return httpx.Response(200, json={"code": 1000})
        return httpx.Response(404)

    request = sync_payload()
    http_client = httpx.AsyncClient(transport=httpx.MockTransport(handler))
    client = DefenseIAClient(settings=brms_settings(), http_client=http_client)
    try:
        await client.login()
        await client.sync_person(request, "001008")
    finally:
        await client.close()
        await http_client.aclose()

    assert len(put_bodies) == 1
    assert put_bodies[0]["baseInfo"]["orgCode"] == "001008"


@pytest.mark.asyncio
async def test_sync_person_delete_recreate_when_put_does_not_change_org():
    put_calls = 0
    post_calls: list[str] = []
    delete_calls = 0
    get_count = 0
    authorize_hits = 0

    async def handler(request: httpx.Request) -> httpx.Response:
        nonlocal put_calls, delete_calls, get_count, authorize_hits
        if request.url.path == BRMS_AUTHORIZE:
            authorize_hits += 1
            if authorize_hits == 1:
                return httpx.Response(
                    401,
                    json={"realm": "r", "randomKey": "k", "encryptType": "MD5"},
                )
            return httpx.Response(200, json={"code": 1000, "token": "t"})
        if request.url.path == brms_person_path("123") and request.method == "GET":
            get_count += 1
            return httpx.Response(
                200,
                json={"code": 1000, "data": {"baseInfo": {"orgCode": "001"}}},
            )
        if request.url.path == brms_person_path("123") and request.method == "PUT":
            put_calls += 1
            return httpx.Response(200, json={"code": 1000})
        if request.url.path == BRMS_PERSON_DELETE_BATCH:
            delete_calls += 1
            return httpx.Response(200, json={"code": 1000})
        if request.url.path == BRMS_PERSON and request.method == "POST":
            post_calls.append(request.url.path)
            return httpx.Response(200, json={"code": 1000, "created": True})
        return httpx.Response(404)

    request = sync_payload()
    http_client = httpx.AsyncClient(transport=httpx.MockTransport(handler))
    client = DefenseIAClient(settings=brms_settings(), http_client=http_client)
    try:
        await client.login()
        await client.sync_person(request, "001008")
    finally:
        await client.close()
        await http_client.aclose()

    assert put_calls == 1
    assert delete_calls == 1
    assert post_calls == [BRMS_PERSON]


def test_brms_mutation_ok_requires_json_success_code():
    ok = httpx.Response(200, json={"code": 1000, "updated": True})
    failed_body = httpx.Response(200, json={"code": 1001, "desc": "Failed"})
    http_err = httpx.Response(502, json={"code": 1001, "desc": "Failed"})

    assert DefenseIAClient._brms_mutation_ok(ok) is True
    assert DefenseIAClient._brms_mutation_ok(failed_body) is False
    assert DefenseIAClient._brms_mutation_ok(http_err) is False


@pytest.mark.asyncio
async def test_brms_upsert_treats_code_1001_as_success_when_org_already_applied():
    """Defense devolve HTTP 200 + code 1001 em PUT redundante; GET confirma orgCode."""
    authorize_hits = 0
    put_calls = 0

    async def handler(request: httpx.Request) -> httpx.Response:
        nonlocal authorize_hits, put_calls
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
                    "data": {"baseInfo": {"personId": "123", "orgCode": "001021"}},
                },
            )
        if request.url.path == brms_person_path("123") and request.method == "PUT":
            put_calls += 1
            return httpx.Response(200, json={"code": 1001, "desc": "Failed"})
        return httpx.Response(404)

    http_client = httpx.AsyncClient(transport=httpx.MockTransport(handler))
    client = DefenseIAClient(settings=brms_settings(), http_client=http_client)
    try:
        await client.login()
        result = await client.sync_person(sync_payload(), "001021")
    finally:
        await client.close()
        await http_client.aclose()

    assert put_calls == 1
    assert result == {}


@pytest.mark.asyncio
async def test_brms_upsert_raises_on_code_1001_when_org_not_applied():
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
                    "data": {"baseInfo": {"personId": "123", "orgCode": "001008"}},
                },
            )
        if request.url.path == brms_person_path("123") and request.method == "PUT":
            return httpx.Response(200, json={"code": 1001, "desc": "Failed"})
        return httpx.Response(404)

    http_client = httpx.AsyncClient(transport=httpx.MockTransport(handler))
    client = DefenseIAClient(settings=brms_settings(), http_client=http_client)
    try:
        await client.login()
        with pytest.raises(DefenseIAError, match="1001"):
            await client.sync_person(sync_payload(), "001021")
    finally:
        await client.close()
        await http_client.aclose()


def test_parse_visitor_channel_map_and_default():
    mapped = parse_visitor_channel_map(
        '{"001021":["1000174$7$0$0"],"001015001":"1000032$7$0$0"}'
    )
    assert mapped == {
        "001021": ["1000174$7$0$0"],
        "001015001": ["1000032$7$0$0"],
    }
    assert parse_visitor_channel_default("a,b, c") == ["a", "b", "c"]


def test_extract_sync_result_ids():
    body = {"code": 1000, "data": {"visitorId": "7", "personId": "999"}}
    assert extract_sync_result_ids(body) == {
        "visitor_id": "7",
        "person_id": "999",
    }


def test_build_visitor_payload_includes_status_and_channels():
    settings = brms_settings(
        visited_person_id="host1",
        visitor_status="1",
        visited_name="Recepcao",
    )
    client = DefenseIAClient(settings=settings)
    payload = client.build_visitor_payload(
        sync_payload(),
        entrance_ids=["1000174$7$0$0"],
        org_code="001021",
        org_name="CHU - ESPAÇO VIVER BEM",
    )
    assert payload["status"] == "1"
    assert payload["idType"] == "6"
    assert payload["visitedPersonId"] == "host1"
    assert payload["visitedName"] == "Recepcao"
    assert payload["visitedOrgName"] == "CHU - ESPAÇO VIVER BEM"
    assert payload["visitorOrgName"] == "CHU - ESPAÇO VIVER BEM"
    assert payload["rightInfo"]["acsChannelIds"] == ["1000174$7$0$0"]
    assert payload["remark"] == "123"
    assert payload["authInfo"]["facePictures"]


@pytest.mark.asyncio
async def test_resolve_visitor_channel_ids_uses_map():
    client = DefenseIAClient(
        settings=brms_settings(
            visitor_channel_map={"001021": ["1000174$7$0$0"]},
        )
    )
    ids = await client.resolve_visitor_channel_ids("001021")
    assert ids == ["1000174$7$0$0"]


@pytest.mark.asyncio
async def test_resolve_visitor_channel_ids_fetches_server_config():
    async def handler(request: httpx.Request) -> httpx.Response:
        if "deviceOrg" in request.url.path:
            return httpx.Response(200, json={"code": 1000, "data": {"departments": []}})
        if request.url.path == BRMS_VISITOR_CONFIG:
            return httpx.Response(
                200,
                json={"code": 1000, "data": {"acsChannelIds": ["1000014$7$0$0"]}},
            )
        return httpx.Response(404)

    http_client = httpx.AsyncClient(transport=httpx.MockTransport(handler))
    client = DefenseIAClient(
        settings=brms_settings(),
        http_client=http_client,
    )
    client._token = "token"
    try:
        ids = await client.resolve_visitor_channel_ids("001999")
    finally:
        await client.close()
        await http_client.aclose()
    assert ids == ["1000014$7$0$0"]


@pytest.mark.asyncio
async def test_resolve_visitor_channel_ids_uses_device_org_tree():
    async def handler(request: httpx.Request) -> httpx.Response:
        if "deviceOrg" in request.url.path:
            return httpx.Response(
                200,
                json={
                    "code": 1000,
                    "data": {
                        "departments": [
                            {
                                "code": "001021",
                                "name": "CHU - ESPAÇO VIVER BEM",
                                "channel": [{"id": "1000032$7$0$0"}],
                            }
                        ]
                    },
                },
            )
        return httpx.Response(404)

    http_client = httpx.AsyncClient(transport=httpx.MockTransport(handler))
    client = DefenseIAClient(settings=brms_settings(), http_client=http_client)
    client._token = "token"
    try:
        ids = await client.resolve_visitor_channel_ids("001021")
    finally:
        await client.close()
        await http_client.aclose()
    assert ids == ["1000032$7$0$0"]


@pytest.mark.asyncio
async def test_resolve_visitor_channel_ids_returns_empty_when_no_sources():
    async def handler(request: httpx.Request) -> httpx.Response:
        if "deviceOrg" in request.url.path:
            return httpx.Response(200, json={"code": 1000, "data": {"departments": []}})
        if request.url.path == BRMS_VISITOR_CONFIG:
            return httpx.Response(200, json={"code": 1000, "data": {}})
        if request.url.path == BRMS_PERSON_GROUP_LIST:
            return httpx.Response(200, json={"code": 1000, "data": {"results": []}})
        return httpx.Response(404)

    http_client = httpx.AsyncClient(transport=httpx.MockTransport(handler))
    client = DefenseIAClient(settings=brms_settings(), http_client=http_client)
    client._token = "token"
    try:
        ids = await client.resolve_visitor_channel_ids("001")
        assert ids == []
    finally:
        await client.close()
        await http_client.aclose()


@pytest.mark.asyncio
async def test_sync_visitor_posts_to_visitor_endpoint():
    posted: list[dict] = []

    async def handler(request: httpx.Request) -> httpx.Response:
        if request.url.path == BRMS_VISITOR and request.method == "POST":
            posted.append(json.loads(request.content.decode()))
            return httpx.Response(
                200,
                json={"code": 1000, "data": {"visitorId": "5", "personId": "888"}},
            )
        if "deviceOrg" in request.url.path:
            return httpx.Response(
                200,
                json={
                    "code": 1000,
                    "data": {
                        "departments": [
                            {"code": "001021", "channel": [{"id": "1000174$7$0$0"}]}
                        ]
                    },
                },
            )
        if request.url.path == BRMS_PERSON_GROUP_LIST:
            return httpx.Response(
                200,
                json={
                    "code": 1000,
                    "data": {
                        "results": [
                            {
                                "orgCode": "001021",
                                "orgName": "CHU - ESPAÇO VIVER BEM",
                            }
                        ]
                    },
                },
            )
        return httpx.Response(404)

    http_client = httpx.AsyncClient(transport=httpx.MockTransport(handler))
    client = DefenseIAClient(
        settings=brms_settings(),
        http_client=http_client,
    )
    client._token = "token"
    try:
        result = await client.sync_visitor(sync_payload(), "001021")
    finally:
        await client.close()
        await http_client.aclose()

    assert result["data"]["visitorId"] == "5"
    assert posted[0]["visitorName"] == "Maria Silva"
    assert posted[0]["rightInfo"]["acsChannelIds"] == ["1000174$7$0$0"]
    assert posted[0]["visitedOrgName"] == "CHU - ESPAÇO VIVER BEM"
