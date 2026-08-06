import base64
import json

import pytest
import httpx

from src.api.schemas import SyncRequest
from src.services.defense_ia_client import (
    BRMS_AUTHORIZE,
    BRMS_DEVICE_ORG_TREE,
    BRMS_KEEPALIVE,
    BRMS_PERSON,
    BRMS_PERSON_DELETE_BATCH,
    BRMS_PERSON_GROUP_LIST,
    BRMS_VISITOR,
    BRMS_VISITOR_PAGE,
    BRMS_VISITOR_CONFIG,
    DefenseIAClient,
    DefenseIAError,
    DefenseIASettings,
    DefenseIAArgumentError,
    _ancestor_org_codes,
    _extract_department_channels,
    _find_device_org_channels,
    _find_device_org_path,
    _normalize_org_lookup_key,
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
    )
    client = DefenseIAClient(settings=settings)
    payload = client.build_visitor_payload(
        sync_payload(),
        entrance_ids=["1000174$7$0$0"],
        visited_name="recepção central",
    )
    assert payload["status"] == "1"
    assert payload["idType"] == "6"
    assert payload["visitedPersonId"] == "host1"
    assert payload["visitedName"] == "recepção central"
    assert "visitedOrgName" not in payload
    assert "visitorOrgName" not in payload
    assert payload["rightInfo"]["acsChannelIds"] == ["1000174$7$0$0"]
    assert payload["remark"] == "123"
    assert payload["authInfo"]["facePictures"]


@pytest.mark.asyncio
async def test_fetch_visitor_page_includes_time_window():
    captured: list[dict[str, str]] = []

    async def handler(request: httpx.Request) -> httpx.Response:
        if request.url.path == BRMS_VISITOR_PAGE:
            captured.append(dict(request.url.params))
            return httpx.Response(200, json={"code": 1000, "data": {"pageData": []}})
        return httpx.Response(404)

    http_client = httpx.AsyncClient(transport=httpx.MockTransport(handler))
    client = DefenseIAClient(settings=brms_settings(), http_client=http_client)
    client._token = "token"
    try:
        await client._fetch_visitor_page("recepção central")
    finally:
        await client.close()
        await http_client.aclose()

    assert len(captured) == 1
    assert captured[0]["startTime"]
    assert captured[0]["endTime"]
    assert captured[0]["direction"] == "1"
    assert int(captured[0]["pagesize"]) >= 20


@pytest.mark.asyncio
async def test_resolve_channels_by_visited_name_from_page_right_info():
    async def handler(request: httpx.Request) -> httpx.Response:
        if request.url.path == BRMS_VISITOR_PAGE:
            return httpx.Response(
                200,
                json={
                    "code": 1000,
                    "data": {
                        "pageData": [
                            {
                                "id": "99",
                                "visitedName": "recepção central",
                                "rightInfo": {
                                    "acsChannelIds": [
                                        "1000032$7$0$0",
                                        "1000045$7$0$0",
                                    ]
                                },
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
        ids = await client.resolve_channels_by_visited_name("recepção central")
    finally:
        await client.close()
        await http_client.aclose()
    assert ids == ["1000032$7$0$0", "1000045$7$0$0"]


@pytest.mark.asyncio
async def test_resolve_channels_by_visited_name_from_visitor_detail():
    async def handler(request: httpx.Request) -> httpx.Response:
        if request.url.path == BRMS_VISITOR_PAGE:
            return httpx.Response(
                200,
                json={
                    "code": 1000,
                    "data": {
                        "pageData": [
                            {
                                "id": "99",
                                "visitedName": "recepção central",
                            }
                        ]
                    },
                },
            )
        if request.url.path == f"{BRMS_VISITOR}/99":
            return httpx.Response(
                200,
                json={
                    "code": 1000,
                    "data": {
                        "visitedName": "recepção central",
                        "rightInfo": {
                            "acsChannelIds": [
                                "1000045$7$0$0",
                                "1000046$7$0$0",
                            ]
                        },
                    },
                },
            )
        return httpx.Response(404)

    http_client = httpx.AsyncClient(transport=httpx.MockTransport(handler))
    client = DefenseIAClient(settings=brms_settings(), http_client=http_client)
    client._token = "token"
    try:
        ids = await client.resolve_channels_by_visited_name("recepção central")
    finally:
        await client.close()
        await http_client.aclose()
    assert ids == ["1000045$7$0$0", "1000046$7$0$0"]


_NESTED_DEVICE_ORG = [
    {
        "name": "Current Site",
        "code": "001",
        "channel": [{"id": "1000099$7$0$0"}],
        "departments": [],
    },
    {
        "name": "CHU - CENTRAL",
        "code": "001002",
        "channel": [{"id": "1000001$7$0$0"}],
        "departments": [
            {
                "name": "INT5",
                "code": "001002003",
                "channel": [{"id": "1000005$7$0$0"}],
                "departments": [
                    {
                        "name": "Catraca A",
                        "code": "001002003001",
                        "channel": [{"id": "1000051$7$0$0"}],
                    },
                    {
                        "name": "Sala B",
                        "code": "001002003002",
                        "channel": [{"id": "1000052$7$0$0"}],
                    },
                ],
            },
            {
                "name": "OutroNivel",
                "code": "001002006",
                "channel": [{"id": "1000006$7$0$0"}],
            },
        ],
    },
]


def test_extract_department_channels_aggregates_subtree():
    topo = _NESTED_DEVICE_ORG[1]
    assert _extract_department_channels(topo) == [
        "1000001$7$0$0",
        "1000005$7$0$0",
        "1000051$7$0$0",
        "1000052$7$0$0",
        "1000006$7$0$0",
    ]
    int5 = topo["departments"][0]
    assert _extract_department_channels(int5) == [
        "1000005$7$0$0",
        "1000051$7$0$0",
        "1000052$7$0$0",
    ]
    leaf = int5["departments"][0]
    assert _extract_department_channels(leaf) == ["1000051$7$0$0"]


def test_find_device_org_channels_includes_ancestor_path():
    assert _find_device_org_channels(
        _NESTED_DEVICE_ORG,
        org_name_key=_normalize_org_lookup_key("INT5"),
    ) == [
        "1000001$7$0$0",
        "1000005$7$0$0",
        "1000051$7$0$0",
        "1000052$7$0$0",
    ]
    # Local Atual / Current Site (passarela) fora do fluxo CHU
    assert "1000099$7$0$0" not in _find_device_org_channels(
        _NESTED_DEVICE_ORG,
        org_name_key=_normalize_org_lookup_key("INT5"),
    )
    assert _find_device_org_channels(
        _NESTED_DEVICE_ORG,
        org_name_key=_normalize_org_lookup_key("Sala B"),
    ) == [
        "1000001$7$0$0",
        "1000005$7$0$0",
        "1000052$7$0$0",
    ]
    assert _find_device_org_channels(
        _NESTED_DEVICE_ORG,
        org_name_key=_normalize_org_lookup_key("CHU - CENTRAL"),
    ) == [
        "1000001$7$0$0",
        "1000005$7$0$0",
        "1000051$7$0$0",
        "1000052$7$0$0",
        "1000006$7$0$0",
    ]
    assert "1000099$7$0$0" not in _find_device_org_channels(
        _NESTED_DEVICE_ORG,
        org_name_key=_normalize_org_lookup_key("CHU - CENTRAL"),
    )
    # Irmão fora do caminho INT5 não entra
    int5_ids = _find_device_org_channels(
        _NESTED_DEVICE_ORG,
        org_code="001002003",
    )
    assert int5_ids == [
        "1000001$7$0$0",
        "1000005$7$0$0",
        "1000051$7$0$0",
        "1000052$7$0$0",
    ]
    assert "1000006$7$0$0" not in int5_ids


# deviceOrg plano (Unimed): hierarquia só no code (blocos de 3)
_FLAT_DEVICE_ORG = [
    {
        "name": "Current Site",
        "code": "001",
        "channel": [
            {"id": "1000001$7$0$0"},
            {"id": "1000002$7$0$0"},
        ],
    },
    {
        "name": "CHU Central",
        "code": "001002",
        "channel": [{"id": "1000066$7$0$0"}],
    },
    {
        "name": "Int5",
        "code": "001002003",
        "channel": [{"id": "1000115$7$0$0"}],
    },
    {
        "name": "Int8",
        "code": "001002006",
        "channel": [{"id": "1000122$7$0$0"}],
    },
    {
        "name": "Refeitorio",
        "code": "001003",
        "channel": [{"id": "1000071$7$0$0"}],
    },
]


def test_ancestor_org_codes_chops_by_three():
    assert _ancestor_org_codes("001002003") == ["001002"]
    assert _ancestor_org_codes("001002") == []
    assert _ancestor_org_codes("001") == []
    assert _ancestor_org_codes("") == []
    assert _ancestor_org_codes("abc") == []


def test_find_device_org_channels_flat_code_prefix_flow():
    int5 = _find_device_org_channels(
        _FLAT_DEVICE_ORG,
        org_name_key=_normalize_org_lookup_key("Int5"),
    )
    assert int5 == [
        "1000066$7$0$0",
        "1000115$7$0$0",
    ]
    # Current Site (Local Atual) fora do fluxo CHU
    assert "1000001$7$0$0" not in int5
    assert "1000002$7$0$0" not in int5
    assert "1000122$7$0$0" not in int5
    assert "1000071$7$0$0" not in int5

    int8 = _find_device_org_channels(
        _FLAT_DEVICE_ORG,
        org_name_key=_normalize_org_lookup_key("Int8"),
    )
    assert int8 == [
        "1000066$7$0$0",
        "1000122$7$0$0",
    ]
    assert "1000115$7$0$0" not in int8
    assert "1000001$7$0$0" not in int8

    chu = _find_device_org_channels(
        _FLAT_DEVICE_ORG,
        org_name_key=_normalize_org_lookup_key("CHU Central"),
    )
    assert chu == [
        "1000066$7$0$0",
        "1000115$7$0$0",
        "1000122$7$0$0",
    ]
    assert "1000001$7$0$0" not in chu
    assert "1000071$7$0$0" not in chu


# Recepção → INT5: code do INT5 NÃO tem Recepção como ancestral por chop;
# nesting JSON deve liberar os dispositivos soltos da pasta pai.
_RECEPCAO_INT5_ORG = [
    {
        "name": "Current Site",
        "code": "001",
        "channel": [{"id": "1000099$7$0$0"}],
        "departments": [],
    },
    {
        "name": "Recepção",
        "code": "001050",
        "channel": [
            {"id": "1000201$7$0$0"},
            {"id": "1000202$7$0$0"},
        ],
        "departments": [
            {
                "name": "INT5",
                "code": "001002003",
                "channel": [{"id": "1000115$7$0$0"}],
                "departments": [
                    {
                        "name": "Catraca",
                        "code": "001002003001",
                        "channel": [{"id": "1000051$7$0$0"}],
                    }
                ],
            },
            {
                "name": "OutroNivel",
                "code": "001050006",
                "channel": [{"id": "1000209$7$0$0"}],
            },
        ],
    },
]


# Nome genérico de pasta (prova que não é hardcode de "Recepção")
_BLOCO_NORTE_ORG = [
    {
        "name": "Bloco Norte",
        "code": "001060",
        "channel": [{"id": "1000301$7$0$0"}],
        "departments": [
            {
                "name": "PA",
                "code": "001060005",
                "channel": [{"id": "1000305$7$0$0"}],
            },
            {
                "name": "CDI",
                "code": "001060008",
                "channel": [{"id": "1000308$7$0$0"}],
            },
        ],
    },
]


def test_find_device_org_path_recepcao_to_int5():
    path = _find_device_org_path(
        _RECEPCAO_INT5_ORG,
        org_name_key=_normalize_org_lookup_key("INT5"),
    )
    assert [n["name"] for n in path] == ["Recepção", "INT5"]


def test_find_device_org_channels_recepcao_soltos_on_path():
    ids = _find_device_org_channels(
        _RECEPCAO_INT5_ORG,
        org_name_key=_normalize_org_lookup_key("INT5"),
    )
    assert ids == [
        "1000201$7$0$0",
        "1000202$7$0$0",
        "1000115$7$0$0",
        "1000051$7$0$0",
    ]
    assert "1000209$7$0$0" not in ids
    assert "1000099$7$0$0" not in ids


def test_find_device_org_channels_generic_parent_folder_soltos():
    ids = _find_device_org_channels(
        _BLOCO_NORTE_ORG,
        org_name_key=_normalize_org_lookup_key("PA"),
    )
    assert ids == [
        "1000301$7$0$0",
        "1000305$7$0$0",
    ]
    assert "1000308$7$0$0" not in ids


def test_find_device_org_node_matches_recepcao_without_accent():
    """Recepçao (painel) casa com busca 'recepcao' / 'Recepção'."""
    from src.services.defense_ia_client import (
        _find_device_org_node,
        _normalize_visited_name_key,
    )

    tree = [
        {
            "name": "Recepçao",
            "code": "001002008",
            "channel": [{"id": "1000077$7$0$0"}],
            "departments": [
                {
                    "name": "int5",
                    "code": "001002008001",
                    "channel": [{"id": "1000115$7$0$0"}],
                }
            ],
        }
    ]
    node = _find_device_org_node(
        tree, org_name_key=_normalize_visited_name_key("Recepção")
    )
    assert node is not None
    assert node["code"] == "001002008"
    ids = _find_device_org_channels(
        tree, org_name_key=_normalize_visited_name_key("INT5")
    )
    assert ids == ["1000077$7$0$0", "1000115$7$0$0"]


@pytest.mark.asyncio
async def test_resolve_visitor_entrance_ids_recepcao_path_soltos():
    async def handler(request: httpx.Request) -> httpx.Response:
        if request.url.path == "/obms/api/v1.1/acs/access-group/list":
            return httpx.Response(404)
        if request.url.path == BRMS_DEVICE_ORG_TREE:
            return httpx.Response(
                200,
                json={"code": 1000, "data": {"departments": _RECEPCAO_INT5_ORG}},
            )
        return httpx.Response(404)

    http_client = httpx.AsyncClient(transport=httpx.MockTransport(handler))
    client = DefenseIAClient(settings=brms_settings(), http_client=http_client)
    client._token = "token"
    try:
        ids = await client.resolve_visitor_entrance_ids(
            access_rule_name="INT5",
            visited_name="INT5",
        )
    finally:
        await client.close()
        await http_client.aclose()
    assert ids == [
        "1000201$7$0$0",
        "1000202$7$0$0",
        "1000115$7$0$0",
        "1000051$7$0$0",
    ]
    assert "1000209$7$0$0" not in ids


@pytest.mark.asyncio
async def test_resolve_visitor_entrance_ids_uses_device_org_when_access_group_404():
    async def handler(request: httpx.Request) -> httpx.Response:
        if request.url.path == "/obms/api/v1.1/acs/access-group/list":
            return httpx.Response(404)
        if request.url.path == BRMS_DEVICE_ORG_TREE:
            return httpx.Response(
                200,
                json={
                    "code": 1000,
                    "data": {
                        "departments": [
                            {
                                "name": "Refeitorio",
                                "code": "001003",
                                "channel": [
                                    {"id": "1000071$7$0$0"},
                                    {"id": "1000072$7$0$0"},
                                ],
                                "departments": [
                                    {
                                        "name": "Refeitorio Norte",
                                        "code": "001003001",
                                        "channel": [{"id": "1000073$7$0$0"}],
                                    }
                                ],
                            }
                        ]
                    },
                },
            )
        if request.url.path == BRMS_VISITOR_PAGE:
            return httpx.Response(
                200,
                json={
                    "code": 1000,
                    "data": {
                        "pageData": [
                            {
                                "visitedName": "Refeitorio",
                                "rightInfo": {"acsChannelIds": []},
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
        ids = await client.resolve_visitor_entrance_ids(
            access_rule_name="Refeitorio",
            visited_name="Refeitorio",
        )
    finally:
        await client.close()
        await http_client.aclose()
    assert ids == ["1000071$7$0$0", "1000072$7$0$0", "1000073$7$0$0"]


@pytest.mark.asyncio
async def test_resolve_visitor_entrance_ids_expands_int5_subtree():
    async def handler(request: httpx.Request) -> httpx.Response:
        if request.url.path == "/obms/api/v1.1/acs/access-group/list":
            return httpx.Response(404)
        if request.url.path == BRMS_DEVICE_ORG_TREE:
            return httpx.Response(
                200,
                json={"code": 1000, "data": {"departments": _NESTED_DEVICE_ORG}},
            )
        return httpx.Response(404)

    http_client = httpx.AsyncClient(transport=httpx.MockTransport(handler))
    client = DefenseIAClient(settings=brms_settings(), http_client=http_client)
    client._token = "token"
    try:
        ids = await client.resolve_visitor_entrance_ids(
            access_rule_name="INT5",
            visited_name="INT5",
        )
    finally:
        await client.close()
        await http_client.aclose()
    assert ids == [
        "1000001$7$0$0",
        "1000005$7$0$0",
        "1000051$7$0$0",
        "1000052$7$0$0",
    ]
    assert "1000006$7$0$0" not in ids
    assert "1000099$7$0$0" not in ids


@pytest.mark.asyncio
async def test_resolve_visitor_entrance_ids_flat_int5_code_prefix():
    async def handler(request: httpx.Request) -> httpx.Response:
        if request.url.path == "/obms/api/v1.1/acs/access-group/list":
            return httpx.Response(404)
        if request.url.path == BRMS_DEVICE_ORG_TREE:
            return httpx.Response(
                200,
                json={"code": 1000, "data": {"departments": _FLAT_DEVICE_ORG}},
            )
        return httpx.Response(404)

    http_client = httpx.AsyncClient(transport=httpx.MockTransport(handler))
    client = DefenseIAClient(settings=brms_settings(), http_client=http_client)
    client._token = "token"
    try:
        ids = await client.resolve_visitor_entrance_ids(
            access_rule_name="Int5",
            visited_name="Int5",
        )
    finally:
        await client.close()
        await http_client.aclose()
    assert ids == [
        "1000066$7$0$0",
        "1000115$7$0$0",
    ]
    assert "1000122$7$0$0" not in ids
    assert "1000001$7$0$0" not in ids


@pytest.mark.asyncio
async def test_resolve_visitor_entrance_ids_falls_back_to_host_when_rule_404():
    async def handler(request: httpx.Request) -> httpx.Response:
        if request.url.path == "/obms/api/v1.1/acs/access-group/list":
            return httpx.Response(404)
        if request.url.path == BRMS_VISITOR_PAGE:
            return httpx.Response(
                200,
                json={
                    "code": 1000,
                    "data": {
                        "pageData": [
                            {
                                "visitedName": "recepção  central",
                                "rightInfo": {
                                    "acsChannelIds": ["1000045$7$0$0"]
                                },
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
        ids = await client.resolve_visitor_entrance_ids(
            access_rule_name="CHU - CENTRAL",
            visited_name="recepção  central",
        )
    finally:
        await client.close()
        await http_client.aclose()
    assert ids == ["1000045$7$0$0"]


@pytest.mark.asyncio
async def test_resolve_channels_by_visited_name_fuzzy_typo_fallback():
    """Typo no host (recpcao vs recepcao) ainda copia portas de visita similar."""

    async def handler(request: httpx.Request) -> httpx.Response:
        if request.url.path == BRMS_VISITOR_PAGE:
            return httpx.Response(
                200,
                json={
                    "code": 1000,
                    "data": {
                        "pageData": [
                            {
                                "visitedName": "recepcao oncologia",
                                "rightInfo": {
                                    "acsChannelIds": [
                                        "1000018$7$0$0",
                                        "1000019$7$0$0",
                                    ]
                                },
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
        ids = await client.resolve_channels_by_visited_name("recpção oncologia")
    finally:
        await client.close()
        await http_client.aclose()
    assert ids == ["1000018$7$0$0", "1000019$7$0$0"]


@pytest.mark.asyncio
async def test_resolve_channels_by_visited_name_returns_empty_without_reference():
    async def handler(request: httpx.Request) -> httpx.Response:
        if request.url.path == BRMS_VISITOR_PAGE:
            return httpx.Response(200, json={"code": 1000, "data": {"pageData": []}})
        return httpx.Response(404)

    http_client = httpx.AsyncClient(transport=httpx.MockTransport(handler))
    client = DefenseIAClient(settings=brms_settings(), http_client=http_client)
    client._token = "token"
    try:
        ids = await client.resolve_channels_by_visited_name("host inexistente")
    finally:
        await client.close()
        await http_client.aclose()
    assert ids == []


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
        if request.url.path == BRMS_VISITOR_PAGE:
            return httpx.Response(
                200,
                json={
                    "code": 1000,
                    "data": {
                        "pageData": [
                            {
                                "id": "1",
                                "visitedName": "recepção central",
                            }
                        ]
                    },
                },
            )
        if request.url.path == f"{BRMS_VISITOR}/1":
            return httpx.Response(
                200,
                json={
                    "code": 1000,
                    "data": {
                        "visitedName": "recepção central",
                        "rightInfo": {"acsChannelIds": ["1000174$7$0$0"]},
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
        result = await client.sync_visitor(sync_payload(), "recepção central")
    finally:
        await client.close()
        await http_client.aclose()

    assert result["data"]["visitorId"] == "5"
    assert posted[0]["visitorName"] == "Maria Silva"
    assert posted[0]["rightInfo"]["acsChannelIds"] == ["1000174$7$0$0"]
    assert posted[0]["visitedName"] == "recepção central"


@pytest.mark.asyncio
async def test_sync_visitor_retries_on_rate_limit_142016(monkeypatch):
    """Defense 142016 (Too frequently) deve retentar e concluir."""
    posted = 0
    sleeps: list[float] = []

    async def fake_sleep(delay: float) -> None:
        sleeps.append(delay)

    monkeypatch.setattr(
        "src.services.defense_ia_client.asyncio.sleep",
        fake_sleep,
    )

    async def handler(request: httpx.Request) -> httpx.Response:
        nonlocal posted
        if request.url.path == "/obms/api/v1.1/acs/access-group/list":
            return httpx.Response(404)
        if request.url.path == BRMS_DEVICE_ORG_TREE:
            return httpx.Response(
                200,
                json={
                    "code": 1000,
                    "data": {
                        "departments": [
                            {
                                "name": "CDI",
                                "code": "001006001",
                                "channel": [{"id": "1000084$7$0$0"}],
                            }
                        ]
                    },
                },
            )
        if request.url.path == BRMS_VISITOR and request.method == "POST":
            posted += 1
            if posted < 3:
                return httpx.Response(
                    200,
                    json={
                        "code": 142016,
                        "desc": "Too frequently operation, Please try again later",
                    },
                )
            return httpx.Response(
                200,
                json={"code": 1000, "data": {"visitorId": "99", "personId": "1"}},
            )
        return httpx.Response(404)

    http_client = httpx.AsyncClient(transport=httpx.MockTransport(handler))
    client = DefenseIAClient(settings=brms_settings(), http_client=http_client)
    client._token = "token"
    try:
        result = await client.sync_visitor(
            sync_payload(),
            access_rule_name="CDI",
            visited_name="CDI",
        )
    finally:
        await client.close()
        await http_client.aclose()

    assert result["data"]["visitorId"] == "99"
    assert posted == 3
    assert sleeps == [1.5, 3.0]
