import base64
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


def _make_person_page_response(
    page_data: list[dict], *, code: int | str = 1000
) -> httpx.Response:
    return httpx.Response(
        200,
        json={"code": code, "data": {"pageData": page_data, "totalCount": str(len(page_data))}},
    )


@pytest.mark.asyncio
async def test_list_person_orgs_pages_and_caches():
    """Paginação completa + cache: segunda chamada não repagina."""
    page_calls: list[dict] = []
    authorize_hits = 0

    page1 = [
        {"baseInfo": {"orgCode": "001", "orgName": "Unimed"}},
        {"baseInfo": {"orgCode": "001015001", "orgName": "Corb"}},
    ]
    page2 = [
        {"baseInfo": {"orgCode": "001008", "orgName": "Colaboradores"}},
        {"baseInfo": {"orgCode": "001015001", "orgName": "Corb"}},  # duplicado, ignorado
    ]

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
        if request.url.path == "/obms/api/v1.1/acs/person/page":
            params = dict(request.url.params)
            page_calls.append(params)
            page_num = int(params.get("page", "1"))
            # Simula pageSize grande: a varredura faz N páginas até voltar < pageSize
            if page_num == 1:
                # Retorna pageSize completo -> avança
                return httpx.Response(
                    200,
                    json={
                        "code": 1000,
                        "data": {"pageData": page1 * 100, "totalCount": "400"},
                    },
                )
            if page_num == 2:
                return httpx.Response(
                    200,
                    json={
                        "code": 1000,
                        "data": {"pageData": page2, "totalCount": "400"},
                    },
                )
            return httpx.Response(
                200, json={"code": 1000, "data": {"pageData": [], "totalCount": "0"}}
            )
        return httpx.Response(404)

    http_client = httpx.AsyncClient(transport=httpx.MockTransport(handler))
    client = DefenseIAClient(settings=brms_settings(), http_client=http_client)
    try:
        await client.login()
        orgs1 = await client.list_person_orgs()
        page_calls_first = list(page_calls)
        orgs2 = await client.list_person_orgs()
    finally:
        await client.close()
        await http_client.aclose()

    assert orgs1 == {
        "unimed": "001",
        "corb": "001015001",
        "colaboradores": "001008",
    }
    assert orgs2 == orgs1
    # Cache: segunda chamada não dispara mais paginação.
    assert page_calls == page_calls_first
    # Confirmamos que paginou ao menos 2 vezes.
    assert any(p.get("page") == "2" for p in page_calls_first)


@pytest.mark.asyncio
async def test_resolve_org_code_returns_digits_directly_without_api_call():
    """Se o BioDoc enviar `reguiredName` puramente numérico, usamos como orgCode."""
    page_calls = 0

    async def handler(request: httpx.Request) -> httpx.Response:
        nonlocal page_calls
        if request.url.path == BRMS_AUTHORIZE:
            return httpx.Response(200, json={"code": 1000, "token": "t"})
        if request.url.path == "/obms/api/v1.1/acs/person/page":
            page_calls += 1
            return httpx.Response(404)
        return httpx.Response(404)

    http_client = httpx.AsyncClient(transport=httpx.MockTransport(handler))
    client = DefenseIAClient(settings=brms_settings(), http_client=http_client)
    try:
        # Não precisa nem fazer login — o atalho de dígitos não toca a API.
        assert await client.resolve_org_code("001015001") == "001015001"
        assert await client.resolve_org_code("  001008  ") == "001008"
        assert await client.resolve_org_code("123") == "123"
    finally:
        await client.close()
        await http_client.aclose()

    assert page_calls == 0


@pytest.mark.asyncio
async def test_resolve_org_code_lookup_case_insensitive_and_no_refresh_on_miss():
    """Lookup case-insensitive; miss retorna None sem refrescar cache.

    O refresh on-miss seria caro demais em produção (paginação completa
    de pessoas leva ~3 min no servidor da Unimed). Quando uma sub-org for
    criada no painel após o boot, o admin chama `list_person_orgs(force_refresh=True)`
    explicitamente.
    """
    refresh_count = 0
    authorize_hits = 0

    async def handler(request: httpx.Request) -> httpx.Response:
        nonlocal authorize_hits, refresh_count
        if request.url.path == BRMS_AUTHORIZE:
            authorize_hits += 1
            if authorize_hits == 1:
                return httpx.Response(
                    401,
                    json={"realm": "r", "randomKey": "k", "encryptType": "MD5"},
                )
            return httpx.Response(200, json={"code": 1000, "token": "t"})
        if request.url.path == "/obms/api/v1.1/acs/person/page":
            page_num = int(dict(request.url.params).get("page", "1"))
            if page_num != 1:
                return httpx.Response(
                    200, json={"code": 1000, "data": {"pageData": []}}
                )
            refresh_count += 1
            page_data = [
                {"baseInfo": {"orgCode": "001", "orgName": "Unimed"}},
                {"baseInfo": {"orgCode": "001015001", "orgName": "Corb"}},
            ]
            return httpx.Response(
                200,
                json={"code": 1000, "data": {"pageData": page_data, "totalCount": "2"}},
            )
        return httpx.Response(404)

    http_client = httpx.AsyncClient(transport=httpx.MockTransport(handler))
    client = DefenseIAClient(settings=brms_settings(), http_client=http_client)
    try:
        await client.login()
        # 1ª resolução paga a varredura única e popula cache
        assert await client.resolve_org_code("UNIMED") == "001"
        # Demais lookups consultam apenas o cache (case-insensitive, trim)
        assert await client.resolve_org_code("Corb") == "001015001"
        assert await client.resolve_org_code("  corb  ") == "001015001"
        # Nome desconhecido NÃO força nova varredura — só cai pra None
        assert await client.resolve_org_code("Inexistente") is None
        assert await client.resolve_org_code("OutroDesconhecido") is None
        # Vazio/None retorna None sem chamar API
        assert await client.resolve_org_code(None) is None
        assert await client.resolve_org_code("") is None
        assert await client.resolve_org_code("   ") is None
    finally:
        await client.close()
        await http_client.aclose()

    assert refresh_count == 1


@pytest.mark.asyncio
async def test_list_person_orgs_force_refresh_repaginates():
    """Permite ao admin invalidar o cache explicitamente."""
    refresh_count = 0
    authorize_hits = 0

    async def handler(request: httpx.Request) -> httpx.Response:
        nonlocal authorize_hits, refresh_count
        if request.url.path == BRMS_AUTHORIZE:
            authorize_hits += 1
            if authorize_hits == 1:
                return httpx.Response(
                    401,
                    json={"realm": "r", "randomKey": "k", "encryptType": "MD5"},
                )
            return httpx.Response(200, json={"code": 1000, "token": "t"})
        if request.url.path == "/obms/api/v1.1/acs/person/page":
            page_num = int(dict(request.url.params).get("page", "1"))
            if page_num != 1:
                return httpx.Response(
                    200, json={"code": 1000, "data": {"pageData": []}}
                )
            refresh_count += 1
            return httpx.Response(
                200,
                json={
                    "code": 1000,
                    "data": {
                        "pageData": [{"baseInfo": {"orgCode": "001", "orgName": "Unimed"}}],
                        "totalCount": "1",
                    },
                },
            )
        return httpx.Response(404)

    http_client = httpx.AsyncClient(transport=httpx.MockTransport(handler))
    client = DefenseIAClient(settings=brms_settings(), http_client=http_client)
    try:
        await client.login()
        await client.list_person_orgs()
        await client.list_person_orgs()  # cache hit
        await client.list_person_orgs(force_refresh=True)
    finally:
        await client.close()
        await http_client.aclose()

    assert refresh_count == 2
