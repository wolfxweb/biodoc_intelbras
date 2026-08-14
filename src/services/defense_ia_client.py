import asyncio
import difflib
import hashlib
import json
import os
import time
import unicodedata
from collections import Counter
from dataclasses import dataclass, field
from typing import Any, Literal

import httpx

from src.api.schemas import SyncRequest
from src.core.logging import keep_alive_logger, logger
from src.services.defense_ia_crypto import resolve_login_public_key

API_MODE_BRMS = "brms"
BRMS_PERSON = "/obms/api/v1.1/acs/person"
BRMS_VISITOR = "/obms/api/v1.0/visitors/visitor"
BRMS_VISITOR_PAGE = "/obms/api/v1.0/visitors/visitor/page"
API_MODE_LEGACY = "legacy"


def brms_person_path(person_id: str) -> str:
    return f"{BRMS_PERSON}/{person_id}"


def brms_visitor_path(visitor_id: str) -> str:
    return f"{BRMS_VISITOR}/{visitor_id}"


BRMS_AUTHORIZE = "/brms/api/v1.0/accounts/authorize"
BRMS_KEEPALIVE = "/brms/api/v1.0/accounts/keepalive"
BRMS_UPDATE_TOKEN = "/brms/api/v1.0/accounts/updateToken"
LEGACY_AUTHORIZE = "/admin/API/accounts/authorize"
LEGACY_UPDATE_TOKEN = "/admin/API/accounts/updateToken"

JSON_HEADERS = {"content-type": "application/json;charset=UTF-8"}
SUCCESS_CODES = (None, 0, "0", 1000, "1000")
# Defense rejeita cadastros em rajada (HTTP 200 + code no body).
RATE_LIMIT_CODES = (142016, "142016")
VISITOR_RATE_LIMIT_RETRIES = 4
VISITOR_RATE_LIMIT_BASE_DELAY_SECONDS = 1.5
# Defense devolve HTTP 200 + code 1001 em PUT redundante; GET confirma estado desejado.
IDEMPOTENT_MUTATION_CODES = (1001, "1001")

# Listagem de sub-organizações (grupos de pessoas) no Defense IA BRMS 3.x.
BRMS_PERSON_GROUP_LIST = "/obms/api/v1.1/acs/person-group/list"
BRMS_PERSON_DELETE_BATCH = "/obms/api/v1.1/acs/person/delete/batch"
BRMS_VISITOR_CONFIG = "/brms/api/v1.1/config/visitor"
BRMS_DEVICE_ORG_TREE = "/brms/api/v1.0/tree/deviceOrg"
BRMS_ACCESS_GROUP_LIST = "/obms/api/v1.1/acs/access-group/list"
BRMS_ACCESS_GROUP_DETAIL = "/obms/api/v1.1/acs/access-group/{group_id}"
BRMS_DOOR_GROUP_LIST = "/obms/api/v1.0/accessControl/doorGroupList"
PERSON_GROUPS_CACHE_TTL_SECONDS = 1800.0
DEVICE_ORG_CACHE_TTL_SECONDS = 300.0
VISITED_NAME_CHANNEL_CACHE_TTL_SECONDS = 3600.0
ACCESS_RULE_CHANNEL_CACHE_TTL_SECONDS = 3600.0
VISITOR_PAGE_LOOKBACK_DAYS = 730
VISITOR_VALIDITY_SECONDS = 48 * 3600
SESSION_CONFLICT_RETRY_SECONDS = 600.0  # 10 min enquanto sessão API estiver ocupada
ACS_CHANNEL_TYPE = "7"
SYNC_TARGET_PERSON = "person"
SYNC_TARGET_VISITOR = "visitor"


def _normalize_org_lookup_key(name: str) -> str:
    return name.strip().casefold()


def _normalize_visited_name_key(name: str) -> str:
    text = name.strip()
    if not text:
        return ""
    normalized = unicodedata.normalize("NFKD", text)
    without_accents = "".join(
        char for char in normalized if not unicodedata.combining(char)
    )
    collapsed = " ".join(without_accents.split())
    return collapsed.casefold()


def _visited_names_fuzzy_match(search_key: str, candidate_key: str) -> bool:
    """True when host names differ only by typo/acento leve (ex.: recpcao vs recepcao)."""
    if not search_key or not candidate_key or search_key == candidate_key:
        return search_key == candidate_key
    search_tokens = search_key.split()
    candidate_tokens = candidate_key.split()
    if not search_tokens or not candidate_tokens:
        return False
    if search_tokens[-1] != candidate_tokens[-1]:
        return False
    ratio = difflib.SequenceMatcher(None, search_key, candidate_key).ratio()
    return ratio >= 0.85


def parse_visitor_channel_map(raw: str) -> dict[str, list[str]]:
    """Parse DEFENSE_IA_VISITOR_CHANNEL_MAP (JSON orgCode -> acsChannelIds)."""
    text = raw.strip()
    if not text:
        return {}
    try:
        parsed = json.loads(text)
    except json.JSONDecodeError as exc:
        raise ValueError(f"DEFENSE_IA_VISITOR_CHANNEL_MAP JSON inválido: {exc}") from exc
    if not isinstance(parsed, dict):
        raise ValueError("DEFENSE_IA_VISITOR_CHANNEL_MAP deve ser um objeto JSON")
    result: dict[str, list[str]] = {}
    for key, value in parsed.items():
        org = str(key).strip()
        if not org:
            continue
        if isinstance(value, str):
            channels = [value.strip()] if value.strip() else []
        elif isinstance(value, list):
            channels = [str(item).strip() for item in value if str(item).strip()]
        else:
            channels = []
        if channels:
            result[org] = channels
    return result


def parse_visitor_channel_default(raw: str) -> list[str]:
    """Parse DEFENSE_IA_VISITOR_CHANNEL_DEFAULT (CSV de acsChannelIds)."""
    return [part.strip() for part in raw.split(",") if part.strip()]


def extract_sync_result_ids(body: dict[str, Any] | None) -> dict[str, str | None]:
    """Extrai visitorId e personId da resposta do Defense IA."""
    if not body:
        return {"visitor_id": None, "person_id": None}
    data = body.get("data", body)
    if not isinstance(data, dict):
        return {"visitor_id": None, "person_id": None}
    visitor_id = data.get("visitorId")
    person_id = data.get("personId")
    return {
        "visitor_id": str(visitor_id).strip() if visitor_id not in (None, "") else None,
        "person_id": str(person_id).strip() if person_id not in (None, "") else None,
    }


def extract_org_code_from_person_body(body: dict[str, Any] | None) -> str | None:
    if not body:
        return None
    node = body.get("data", body)
    if not isinstance(node, dict):
        return None
    base = node.get("baseInfo")
    if isinstance(base, dict):
        code = base.get("orgCode")
        if code:
            return str(code).strip()
    code = node.get("orgCode")
    return str(code).strip() if code else None


def _org_codes_match(actual: str | None, expected: str | None) -> bool:
    if not actual or not expected:
        return False
    return actual.strip() == expected.strip()


def _parse_org_name_code_pairs(body: object) -> dict[str, str]:
    """Extrai {orgName_lower: orgCode} de respostas variadas do Defense IA."""
    orgs: dict[str, str] = {}
    if not isinstance(body, dict):
        return orgs

    data = body.get("data", body)
    items: list[object] = []
    if isinstance(data, list):
        items = data
    elif isinstance(data, dict):
        for key in ("list", "pageData", "groups", "records", "data", "groupList", "results"):
            candidate = data.get(key)
            if isinstance(candidate, list):
                items = candidate
                break

    for entry in items:
        if not isinstance(entry, dict):
            continue
        code = entry.get("orgCode") or entry.get("groupCode") or entry.get("code")
        name = entry.get("orgName") or entry.get("groupName") or entry.get("name")
        if not code or not name:
            base = entry.get("baseInfo")
            if isinstance(base, dict):
                code = code or base.get("orgCode") or base.get("groupCode")
                name = name or base.get("orgName") or base.get("groupName")
        if not code or not name:
            continue
        lookup_key = _normalize_org_lookup_key(str(name))
        if lookup_key and lookup_key not in orgs:
            orgs[lookup_key] = str(code)
    return orgs


def _parse_org_code_to_name(body: object) -> dict[str, str]:
    """Extrai {orgCode: orgName} de respostas do person-group/list."""
    names: dict[str, str] = {}
    if not isinstance(body, dict):
        return names

    data = body.get("data", body)
    items: list[object] = []
    if isinstance(data, list):
        items = data
    elif isinstance(data, dict):
        for key in ("list", "pageData", "groups", "records", "data", "groupList", "results"):
            candidate = data.get(key)
            if isinstance(candidate, list):
                items = candidate
                break

    for entry in items:
        if not isinstance(entry, dict):
            continue
        code = entry.get("orgCode") or entry.get("groupCode") or entry.get("code")
        name = entry.get("orgName") or entry.get("groupName") or entry.get("name")
        if not code or not name:
            base = entry.get("baseInfo")
            if isinstance(base, dict):
                code = code or base.get("orgCode") or base.get("groupCode")
                name = name or base.get("orgName") or base.get("groupName")
        if code and name:
            names[str(code).strip()] = str(name).strip()
    return names


def _node_own_channels(node: dict[str, Any]) -> list[str]:
    """Coleta apenas channel IDs do nó, sem descendentes."""
    channels: list[str] = []
    seen: set[str] = set()
    for item in node.get("channel") or []:
        if not isinstance(item, dict):
            continue
        channel_id = str(item.get("id") or "").strip()
        if not channel_id or channel_id in seen:
            continue
        seen.add(channel_id)
        channels.append(channel_id)
    return channels


def _extract_department_channels(department: dict[str, Any]) -> list[str]:
    """Coleta channel IDs do nó e de todos os descendentes (fluxo até as folhas).

    Deduplica preservando a ordem de visita (nó atual, depois filhos).
    """
    seen: set[str] = set()
    channels: list[str] = []

    def _walk(node: dict[str, Any]) -> None:
        for channel_id in _node_own_channels(node):
            if channel_id in seen:
                continue
            seen.add(channel_id)
            channels.append(channel_id)
        sub = node.get("departments") or node.get("deparments") or []
        if not isinstance(sub, list):
            return
        for child in sub:
            if isinstance(child, dict):
                _walk(child)

    _walk(department)
    return channels


def _iter_device_org_nodes(departments: list[object]):
    """Percorre deviceOrg plano ou aninhado, yield de cada departamento dict."""
    for department in departments:
        if not isinstance(department, dict):
            continue
        yield department
        sub = department.get("departments") or department.get("deparments") or []
        if isinstance(sub, list):
            yield from _iter_device_org_nodes(sub)


def _index_device_org_by_code(
    departments: list[object],
) -> dict[str, dict[str, Any]]:
    """Índice code -> nó (primeiro ganha se houver duplicata)."""
    index: dict[str, dict[str, Any]] = {}
    for node in _iter_device_org_nodes(departments):
        code = str(node.get("code") or "").strip()
        if code and code not in index:
            index[code] = node
    return index


def _ancestor_org_codes(code: str) -> list[str]:
    """Códigos ancestrais do fluxo (pai → …), excluindo a raiz do site.

    Blocos de 3 dígitos. A raiz (``001`` / Local Atual / Current Site) fica de
    fora — portas genéricas (passarela etc.) não entram no fluxo CHU/Bloco.

    Ex.: ``001002003`` → ``[\"001002\"]`` (sem ``001``).
    """
    raw = (code or "").strip()
    if not raw.isdigit() or len(raw) <= 3:
        return []
    ancestors: list[str] = []
    current = raw
    while len(current) > 3:
        current = current[:-3]
        if not current:
            break
        # len == 3 → raiz do site (Local Atual); não faz parte do fluxo do ramo
        if len(current) <= 3:
            break
        ancestors.append(current)
    ancestors.reverse()
    return ancestors


def _find_device_org_node(
    departments: list[object],
    *,
    org_code: str | None = None,
    org_name_key: str | None = None,
) -> dict[str, Any] | None:
    for node in _iter_device_org_nodes(departments):
        code = str(node.get("code") or "").strip()
        # Mesma normalização da busca (acentos/espaços) — Recepçao ≈ recepcao
        name_key = _normalize_visited_name_key(str(node.get("name") or ""))
        if org_code and code == org_code:
            return node
        if org_name_key and name_key == org_name_key:
            return node
    return None


_SITE_ROOT_NAME_KEYS = frozenset(
    {
        "current site",
        "local atual",
    }
)


def _is_site_root_node(node: dict[str, Any]) -> bool:
    """True for Local Atual / Current Site (passarela) — fora do fluxo do ramo."""
    code = str(node.get("code") or "").strip()
    if code.isdigit() and len(code) <= 3:
        return True
    name_key = _normalize_visited_name_key(str(node.get("name") or ""))
    return name_key in _SITE_ROOT_NAME_KEYS


def _node_matches_org(
    node: dict[str, Any],
    *,
    org_code: str | None = None,
    org_name_key: str | None = None,
) -> bool:
    code = str(node.get("code") or "").strip()
    name_key = _normalize_visited_name_key(str(node.get("name") or ""))
    if org_code and code == org_code:
        return True
    if org_name_key and name_key == org_name_key:
        return True
    return False


def _find_device_org_path(
    departments: list[object],
    *,
    org_code: str | None = None,
    org_name_key: str | None = None,
) -> list[dict[str, Any]]:
    """Caminho JSON da raiz até o nó (inclusive), ou [] se não achar."""

    def _dfs(
        nodes: list[object], trail: list[dict[str, Any]]
    ) -> list[dict[str, Any]] | None:
        for node in nodes:
            if not isinstance(node, dict):
                continue
            next_trail = trail + [node]
            if _node_matches_org(
                node, org_code=org_code, org_name_key=org_name_key
            ):
                return next_trail
            sub = node.get("departments") or node.get("deparments") or []
            if isinstance(sub, list) and sub:
                found = _dfs(sub, next_trail)
                if found is not None:
                    return found
        return None

    return _dfs(departments, []) or []


def _dedupe_channels(*groups: list[str]) -> list[str]:
    seen: set[str] = set()
    merged: list[str] = []
    for group in groups:
        for channel_id in group:
            if channel_id in seen:
                continue
            seen.add(channel_id)
            merged.append(channel_id)
    return merged


def _find_device_org_channels(
    departments: list[object],
    *,
    org_code: str | None = None,
    org_name_key: str | None = None,
    ancestor_channels: list[str] | None = None,
) -> list[str]:
    """Agrega canais do fluxo: pastas no caminho (soltos) + nó + descendentes.

    Ancestrais vêm de duas fontes (dedupe):
    - pais no nesting JSON (qualquer pasta com ``channel[]`` no caminho)
    - prefixo de ``code`` (deviceOrg plano Unimed)

    Exclui Local Atual / Current Site (``001``). Irmãos fora do caminho não entram.
    """
    path = _find_device_org_path(
        departments, org_code=org_code, org_name_key=org_name_key
    )
    match = path[-1] if path else _find_device_org_node(
        departments, org_code=org_code, org_name_key=org_name_key
    )
    if match is None:
        return []

    ancestor_ids: list[str] = list(ancestor_channels or [])
    # Pais do caminho JSON (dispositivos soltos na pasta), exceto raiz do site
    for parent in path[:-1]:
        if _is_site_root_node(parent):
            continue
        ancestor_ids.extend(_node_own_channels(parent))

    code = str(match.get("code") or "").strip()
    by_code = _index_device_org_by_code(departments) if code.isdigit() else {}
    if code.isdigit():
        for anc_code in _ancestor_org_codes(code):
            ancestor = by_code.get(anc_code)
            if ancestor is not None and not _is_site_root_node(ancestor):
                ancestor_ids.extend(_node_own_channels(ancestor))

    match_ids = _extract_department_channels(match)
    descendant_ids: list[str] = []
    if code.isdigit():
        for other_code, other_node in sorted(by_code.items()):
            if other_code == code:
                continue
            if other_code.startswith(code) and len(other_code) > len(code):
                descendant_ids.extend(_node_own_channels(other_node))

    return _dedupe_channels(ancestor_ids, match_ids, descendant_ids)


class DefenseIAError(Exception):
    pass


class DefenseIAUnavailableError(DefenseIAError):
    pass


class DefenseIAUnauthorizedError(DefenseIAError):
    pass


class DefenseIANotReadyError(DefenseIAError):
    """Defense habilitado no .env mas sem sessão (login falhou ou ainda não executado)."""


class DefenseIAArgumentError(DefenseIAError):
    """Argumento inválido enviado ao Defense IA (ex: imagem facial ilegível, código 8044)."""


@dataclass(frozen=True)
class DefenseIASettings:
    server_url: str
    username: str
    password: str
    api_mode: Literal["brms", "legacy"] = API_MODE_BRMS
    client_type: str = "WINPC_V2"
    public_key: str = ""
    use_server_public_key: bool = False
    private_key: str = ""
    user_type: str = "0"
    org_code: str = "001"
    keep_alive_interval_seconds: float = 20.0
    timeout_seconds: float = 10.0
    visited_person_id: str = ""
    sync_target: Literal["person", "visitor"] = SYNC_TARGET_VISITOR
    visitor_status: str = "1"
    visited_name: str = ""
    visited_org_name: str = ""

    @property
    def enabled(self) -> bool:
        return bool(self.server_url and self.username and self.password)

    @property
    def is_brms(self) -> bool:
        return self.api_mode == API_MODE_BRMS

    @property
    def is_visitor_sync(self) -> bool:
        return self.sync_target == SYNC_TARGET_VISITOR


def extract_face_pictures_from_person_body(body: dict[str, Any]) -> list[str]:
    node = body.get("data", body)
    if not isinstance(node, dict):
        return []
    base = node.get("baseInfo")
    if not isinstance(base, dict):
        base = node
    faces = base.get("facePictures")
    if isinstance(faces, list):
        return [str(item) for item in faces if item]
    if isinstance(faces, str) and faces:
        return [faces]
    return []


class DefenseIAClient:
    def __init__(
        self,
        settings: DefenseIASettings,
        http_client: httpx.AsyncClient | None = None,
    ) -> None:
        self.settings = settings
        self._http_client = http_client
        self._owns_http_client = http_client is None
        self._token: str | None = None
        self._dollar_signature: str | None = None
        self._keep_alive_task: asyncio.Task[None] | None = None
        self._auth_lock = asyncio.Lock()
        self._person_groups_cache: dict[str, str] | None = None
        self._person_groups_by_code: dict[str, str] | None = None
        self._person_groups_cache_at: float = 0.0
        self._person_groups_lock = asyncio.Lock()
        self._person_groups_available: bool | None = None
        self._device_org_departments: list[dict[str, Any]] | None = None
        self._device_org_cache_at: float = 0.0
        self._device_org_lock = asyncio.Lock()
        self._visited_channel_cache: dict[str, tuple[list[str], float]] = {}
        self._visited_channel_lock = asyncio.Lock()
        self._access_rule_channel_cache: dict[str, tuple[list[str], float]] = {}
        self._access_rule_channel_lock = asyncio.Lock()
        # Serializa POST de visitante para reduzir 142016 (Too frequently).
        self._visitor_write_lock = asyncio.Lock()
        # Keepalive em intervalo maior enquanto a sessão API estiver ocupada (2004).
        self._session_conflict = False

    @property
    def token(self) -> str | None:
        return self._token

    @property
    def is_ready(self) -> bool:
        if not self.settings.enabled:
            return True
        return self._token is not None

    def _clear_auth_session(self) -> None:
        self._token = None
        self._dollar_signature = None

    async def start(self) -> None:
        if self._http_client is None:
            self._http_client = httpx.AsyncClient(timeout=self.settings.timeout_seconds)
        if self.settings.enabled:
            try:
                await self.login()
            except DefenseIAError as exc:
                keep_alive_logger.warning(
                    "[KEEP_ALIVE] startup login failed, will retry in background: %s",
                    exc,
                )
            self._keep_alive_task = asyncio.create_task(self._keep_alive_loop())

    async def close(self) -> None:
        if self._keep_alive_task:
            self._keep_alive_task.cancel()
            try:
                await self._keep_alive_task
            except asyncio.CancelledError:
                pass
        if self._owns_http_client and self._http_client:
            await self._http_client.aclose()

    async def login(self) -> str:
        async with self._auth_lock:
            if not self._http_client:
                self._http_client = httpx.AsyncClient(timeout=self.settings.timeout_seconds)
            try:
                if self.settings.is_brms:
                    token, dollar_signature = await self._perform_brms_login()
                else:
                    token, dollar_signature = await self._perform_legacy_login()
            except DefenseIAError as exc:
                if is_session_conflict_error(exc):
                    self._clear_auth_session()
                    self._session_conflict = True
                raise
            self._token = token
            self._dollar_signature = dollar_signature
            self._session_conflict = False
            return token

    async def keep_alive_once(self) -> None:
        if not self._token or not self._dollar_signature:
            keep_alive_logger.info("[KEEP_ALIVE] token ausente, fazendo login")
            await self.login()
            keep_alive_logger.info("[KEEP_ALIVE] login concluido")
            return

        token_preview = self._token[:8] + "..." if self._token else "none"
        keep_alive_logger.debug("[KEEP_ALIVE] ping enviado token=%s", token_preview)

        headers = self._auth_headers()
        if self.settings.is_brms:
            response = await self._request(
                "PUT",
                BRMS_KEEPALIVE,
                json={"token": self._token},
                headers=headers,
            )
        else:
            signature = self._build_update_token_signature(self._token, self._dollar_signature)
            response = await self._request(
                "POST",
                LEGACY_UPDATE_TOKEN,
                json={"signature": signature},
                headers=headers,
            )

        if response.status_code == 401:
            keep_alive_logger.warning(
                "[KEEP_ALIVE] token recusado (401), fazendo re-login"
            )
            await self.login()
            keep_alive_logger.info("[KEEP_ALIVE] re-login bem-sucedido, novo token obtido")
            return

        self._raise_for_response(response)
        keep_alive_logger.debug("[KEEP_ALIVE] ok status=%d", response.status_code)

    async def sync_person(
        self, payload: SyncRequest, org_code: str | None = None
    ) -> dict[str, Any]:
        if self.settings.enabled and not self._token:
            raise DefenseIANotReadyError("Defense IA não conectado")
        return await self._send_person_with_relogin(payload, org_code)

    async def warmup_org_cache(self) -> None:
        """Pré-carrega cache de orgs em background (evita ~3 min no 1º webhook)."""
        if not self.settings.enabled:
            return
        try:
            if not self._token:
                await self.login()
            orgs, source = await self.list_available_orgs(force_refresh=True)
            logger.info(
                "[DEFENSE_IA] cache de orgs aquecido: %d entrada(s) (fonte=%s)",
                len(orgs),
                source,
            )
        except DefenseIAError as exc:
            logger.warning("[DEFENSE_IA] falha ao aquecer cache de orgs: %s", exc)

    async def _send_person_with_relogin(
        self, payload: SyncRequest, org_code: str | None
    ) -> dict[str, Any]:
        if not self._token:
            await self.login()

        response = await self._upsert_person_request(payload, org_code)
        if response.status_code == 401:
            await self.login()
            response = await self._upsert_person_request(payload, org_code)

        if not self._brms_mutation_ok(response):
            expected_org = (org_code or self.settings.org_code or "001").strip()
            if await self._recover_idempotent_mutation_failure(
                response,
                payload.external_id,
                expected_org,
            ):
                logger.warning(
                    "[DEFENSE_IA] mutação retornou code=%s para external_id=%s "
                    "mas orgCode=%s já está aplicado — tratado como sucesso idempotente",
                    self._response_json_code(response),
                    payload.external_id,
                    expected_org,
                )
                return {}

        self._raise_for_response(response)
        if not response.content:
            return {}
        return response.json()

    async def list_person_groups(self, force_refresh: bool = False) -> dict[str, str]:
        """Retorna {orgName_lower: orgCode} via GET /acs/person-group/list."""
        if self._person_groups_available is False and not force_refresh:
            return {}

        now = time.time()
        if (
            not force_refresh
            and self._person_groups_cache is not None
            and (now - self._person_groups_cache_at) < PERSON_GROUPS_CACHE_TTL_SECONDS
        ):
            return self._person_groups_cache

        async with self._person_groups_lock:
            now = time.time()
            if (
                not force_refresh
                and self._person_groups_cache is not None
                and (now - self._person_groups_cache_at) < PERSON_GROUPS_CACHE_TTL_SECONDS
            ):
                return self._person_groups_cache

            if not self._token:
                await self.login()

            response = await self._request(
                "GET",
                BRMS_PERSON_GROUP_LIST,
                headers=self._auth_headers(),
            )
            if response.status_code == 401:
                await self.login()
                response = await self._request(
                    "GET",
                    BRMS_PERSON_GROUP_LIST,
                    headers=self._auth_headers(),
                )

            if response.status_code in (404, 405):
                self._person_groups_available = False
                logger.warning(
                    "[DEFENSE_IA] person-group/list indisponível (HTTP %d) — "
                    "nomes de grupo não serão resolvidos até o endpoint responder",
                    response.status_code,
                )
                return {}

            if not response.is_success:
                raise DefenseIAError(
                    f"Falha ao listar person-group (HTTP {response.status_code})"
                )

            try:
                body = response.json()
            except ValueError as exc:
                raise DefenseIAError(
                    "Resposta inválida ao listar person-group do Defense IA"
                ) from exc

            orgs = _parse_org_name_code_pairs(body)
            self._person_groups_by_code = _parse_org_code_to_name(body)
            self._person_groups_available = True
            self._person_groups_cache = orgs
            self._person_groups_cache_at = time.time()
            logger.info(
                "[DEFENSE_IA] cache de grupos atualizado (person-group/list): %d entrada(s)",
                len(orgs),
            )
            if not orgs:
                sample = json.dumps(body, ensure_ascii=False)[:500]
                logger.info(
                    "[DEFENSE_IA] person-group/list retornou 0 grupos — amostra: %s",
                    sample,
                )
            return orgs

    async def list_available_orgs(
        self, force_refresh: bool = False
    ) -> tuple[dict[str, str], str]:
        """Lista orgs via GET /acs/person-group/list (cache TTL 30 min)."""
        if not force_refresh:
            groups = await self.list_person_groups()
            if groups:
                return groups, "person-group/list"

        groups = await self.list_person_groups(force_refresh=True)
        return groups, "person-group/list"

    async def resolve_org_code(self, name: str | None) -> str | None:
        """Resolve o orgCode no Defense IA a partir do nome do grupo (operador/reguiredName).

        Aceita dois formatos:

        1. Código direto (apenas dígitos, ex.: `"001015001"`): retorna como
           está, sem chamar a API.
        2. Nome amigável (ex.: `"Corb"`): lookup case-insensitive no cache de
           grupos obtido via `person-group/list` (aquecido no boot).

        Em caso de miss no cache, **não** força re-fetch para evitar sobrecarga
        em rajadas de webhooks com nomes desconhecidos. Para capturar grupos
        criados após o boot, use `await client.list_available_orgs(force_refresh=True)`
        (ou rode o script `scripts/list_person_orgs.py --refresh`).

        Retorna `None` se o nome for vazio ou não casar com nenhum grupo —
        o chamador decide o fallback (ex.: `DEFENSE_IA_ORG_CODE`).
        """
        if not name:
            return None
        key = name.strip()
        if not key:
            return None
        # Atalho: se o painel BioDoc estiver configurado com o orgCode direto,
        # economizamos a varredura completa de pessoas.
        if key.isdigit():
            return key
        orgs, source = await self.list_available_orgs()
        resolved = orgs.get(_normalize_org_lookup_key(key))
        if resolved:
            logger.debug(
                "[DEFENSE_IA] resolve_org_code %r -> %s (fonte=%s)",
                key,
                resolved,
                source,
            )
        return resolved

    async def resolve_org_name_by_code(self, org_code: str | None) -> str | None:
        code = (org_code or self.settings.org_code or "001").strip()
        await self.list_person_groups()
        if self._person_groups_by_code:
            name = self._person_groups_by_code.get(code)
            if name:
                return name
        return None

    async def list_device_org_departments(
        self, force_refresh: bool = False
    ) -> list[dict[str, Any]]:
        now = time.time()
        if (
            not force_refresh
            and self._device_org_departments is not None
            and (now - self._device_org_cache_at) < DEVICE_ORG_CACHE_TTL_SECONDS
        ):
            return self._device_org_departments

        async with self._device_org_lock:
            now = time.time()
            if (
                not force_refresh
                and self._device_org_departments is not None
                and (now - self._device_org_cache_at) < DEVICE_ORG_CACHE_TTL_SECONDS
            ):
                return self._device_org_departments

            if not self._token:
                await self.login()

            response = await self._request(
                "GET",
                f"{BRMS_DEVICE_ORG_TREE}?channelTypes={ACS_CHANNEL_TYPE}",
                headers=self._auth_headers(),
            )
            if response.status_code == 401:
                await self.login()
                response = await self._request(
                    "GET",
                    f"{BRMS_DEVICE_ORG_TREE}?channelTypes={ACS_CHANNEL_TYPE}",
                    headers=self._auth_headers(),
                )

            departments: list[dict[str, Any]] = []
            if response.is_success and response.content:
                try:
                    body = response.json()
                except ValueError:
                    body = {}
                data = body.get("data", body) if isinstance(body, dict) else {}
                raw = data.get("departments") if isinstance(data, dict) else None
                if isinstance(raw, list):
                    departments = [d for d in raw if isinstance(d, dict)]

            self._device_org_departments = departments
            self._device_org_cache_at = time.time()
            logger.debug(
                "[DEFENSE_IA] deviceOrg cache atualizado: %d departamento(s) raiz",
                len(departments),
            )
            return departments

    async def sync_visitor(
        self,
        payload: SyncRequest,
        visited_name: str | None = None,
        *,
        access_rule_name: str | None = None,
        entrance_ids: list[str] | None = None,
    ) -> dict[str, Any]:
        """Cadastra visitante no Defense IA 3.x (regra de acesso → visitedName/portas)."""
        if self.settings.enabled and not self._token:
            raise DefenseIANotReadyError("Defense IA não conectado")
        rule = (access_rule_name or "").strip()
        host = (visited_name or self.settings.visited_name or "").strip()
        if not host and not rule:
            raise DefenseIAArgumentError(
                "Regra de acesso ausente — informe defense.org_code "
                "com o nome da regra cadastrada no painel Defense"
            )
        post_visited_name = host or rule
        if entrance_ids is None:
            entrance_ids = await self.resolve_visitor_entrance_ids(
                access_rule_name=rule or post_visited_name,
                visited_name=host or post_visited_name,
            )
        return await self._send_visitor_with_relogin(
            payload,
            entrance_ids,
            visited_name=post_visited_name,
        )

    async def _send_visitor_with_relogin(
        self,
        payload: SyncRequest,
        entrance_ids: list[str],
        *,
        visited_name: str,
    ) -> dict[str, Any]:
        if not self._token:
            await self.login()

        async with self._visitor_write_lock:
            last_error: DefenseIAError | None = None
            for attempt in range(VISITOR_RATE_LIMIT_RETRIES):
                response = await self._upsert_visitor_request(
                    payload,
                    entrance_ids,
                    visited_name=visited_name,
                )
                if response.status_code == 401:
                    await self.login()
                    response = await self._upsert_visitor_request(
                        payload,
                        entrance_ids,
                        visited_name=visited_name,
                    )

                if self._response_is_rate_limited(response):
                    delay = VISITOR_RATE_LIMIT_BASE_DELAY_SECONDS * (2**attempt)
                    logger.warning(
                        "[DEFENSE_IA] rate limit 142016 ao cadastrar visitante "
                        "(tentativa %d/%d) — aguardando %.1fs",
                        attempt + 1,
                        VISITOR_RATE_LIMIT_RETRIES,
                        delay,
                    )
                    last_error = DefenseIAError(
                        "Defense IA retornou código 142016: "
                        "Too frequently operation, Please try again later"
                    )
                    await asyncio.sleep(delay)
                    continue

                self._raise_for_response(response)
                if not response.content:
                    return {}
                return response.json()

            if last_error is not None:
                raise last_error
            raise DefenseIAError(
                "Defense IA rate limit 142016 após várias tentativas"
            )

    async def _upsert_visitor_request(
        self,
        payload: SyncRequest,
        entrance_ids: list[str],
        *,
        visited_name: str,
    ) -> httpx.Response:
        # O Defense IA gera visitorId automaticamente; cada evento BioDoc = nova visita.
        headers = self._auth_headers()
        visitor_payload = self.build_visitor_payload(
            payload,
            entrance_ids=entrance_ids,
            visited_name=visited_name,
        )
        logger.debug(
            "[DEFENSE_IA OUT] method=POST url=%s payload=%s",
            BRMS_VISITOR,
            self._sanitize_payload_for_log(visitor_payload),
        )
        response = await self._request(
            "POST", BRMS_VISITOR, json=visitor_payload, headers=headers
        )
        logger.debug(
            "[DEFENSE_IA IN] method=POST url=%s status=%d body=%s",
            BRMS_VISITOR,
            response.status_code,
            response.text[:500],
        )
        return response

    async def _fetch_brms_visitor(self, visitor_id: str) -> dict[str, Any] | None:
        response = await self._request(
            "GET",
            brms_visitor_path(visitor_id),
            headers=self._auth_headers(),
        )
        if response.status_code == 404:
            return None
        if response.status_code == 401:
            raise DefenseIAUnauthorizedError("Token do Defense IA recusado")
        if response.status_code >= 500:
            raise DefenseIAUnavailableError("API do Defense IA indisponível")
        if not response.is_success:
            return None
        if not response.content:
            return {}
        try:
            body = response.json()
        except ValueError:
            return {}
        if not isinstance(body, dict):
            return {}
        code = body.get("code")
        if code in (404, "404", 1004, "1004"):
            return None
        desc = str(body.get("desc") or body.get("message") or "").lower()
        if "not exist" in desc or "not found" in desc or "nao exist" in desc:
            return None
        if code in (None, 0, "0", 1000, "1000"):
            return body
        return None

    @staticmethod
    def _extract_visitor_face_pictures(visitor_body: dict[str, Any]) -> list[str]:
        node = visitor_body.get("data", visitor_body)
        if not isinstance(node, dict):
            node = visitor_body
        auth = node.get("authInfo")
        if not isinstance(auth, dict):
            return []
        faces = auth.get("facePictures") or auth.get("facePicture") or []
        if isinstance(faces, str):
            return [faces] if faces else []
        if isinstance(faces, list):
            return [str(f) for f in faces if f]
        return []

    def build_visitor_payload(
        self,
        payload: SyncRequest,
        *,
        entrance_ids: list[str],
        visited_name: str,
        existing_face_pictures: list[str] | None = None,
    ) -> dict[str, Any]:
        face = self._resolve_face_base64(payload)
        if face:
            face_pictures: list[str] = [face]
        elif existing_face_pictures is not None:
            face_pictures = list(existing_face_pictures)
        else:
            face_pictures = []

        now = int(time.time())
        # acsChannelIds usa formato "channelId$type$0$0" (ex.: "1000032$7$0$0")
        acs_channel_ids = entrance_ids

        body: dict[str, Any] = {
            "status": self.settings.visitor_status,
            "visitorName": payload.person.full_name,
            "idType": "6",
            "idNum": payload.person.document or None,
            "expectArrivalTime": "0",
            "arrivalTime": str(now),
            "expectLeaveTime": str(now + VISITOR_VALIDITY_SECONDS),
            "leaveTime": "0",
            "reason": "BioDoc",
            "remark": payload.external_id,
            "plateNo": "",
            "authInfo": {
                "facePictures": face_pictures,
            },
            "rightInfo": {
                "acsChannelIds": acs_channel_ids,
                "vtoChannelIds": [],
                "positionIds": [],
                "liftChannels": [],
            },
        }
        body["visitedName"] = visited_name
        logger.info(
            "[DEFENSE_IA] visitante visitedName=%r acsChannelIds=%s",
            visited_name,
            acs_channel_ids,
        )
        if self.settings.visited_person_id:
            body["visitedPersonId"] = self.settings.visited_person_id
        return body

    @staticmethod
    def _extract_acs_channel_ids_from_visitor_body(body: dict[str, Any]) -> list[str]:
        node = body.get("data", body)
        if not isinstance(node, dict):
            node = body
        right = node.get("rightInfo")
        if not isinstance(right, dict):
            return []
        channels = right.get("acsChannelIds") or []
        if not isinstance(channels, list):
            return []
        return [str(item).strip() for item in channels if str(item).strip()]

    @staticmethod
    def _extract_visitor_page_items(body: dict[str, Any]) -> list[dict[str, Any]]:
        data = body.get("data", body)
        if not isinstance(data, dict):
            return []
        for key in ("pageData", "list", "records", "results"):
            raw = data.get(key)
            if isinstance(raw, list):
                return [item for item in raw if isinstance(item, dict)]
        return []

    def _visitor_page_params(self, key: str) -> dict[str, str]:
        from src.services.defense_visitor import visitor_page_query

        return visitor_page_query(
            key=key,
            page=1,
            page_size=100,
            lookback_days=VISITOR_PAGE_LOOKBACK_DAYS,
        )

    async def _fetch_visitor_page(self, key: str) -> list[dict[str, Any]]:
        if not self._token:
            await self.login()
        params = self._visitor_page_params(key)
        response = await self._request(
            "GET",
            BRMS_VISITOR_PAGE,
            params=params,
            headers=self._auth_headers(),
        )
        if response.status_code == 401:
            await self.login()
            response = await self._request(
                "GET",
                BRMS_VISITOR_PAGE,
                params=params,
                headers=self._auth_headers(),
            )
        if not response.is_success or not response.content:
            return []
        try:
            body = response.json()
        except ValueError:
            return []
        if not isinstance(body, dict):
            return []
        return self._extract_visitor_page_items(body)

    @staticmethod
    def _extract_acs_channel_ids_from_page_item(item: dict[str, Any]) -> list[str]:
        right = item.get("rightInfo")
        if not isinstance(right, dict):
            return []
        channels = right.get("acsChannelIds") or []
        if not isinstance(channels, list):
            return []
        return [str(item_id).strip() for item_id in channels if str(item_id).strip()]

    async def resolve_visitor_entrance_ids(
        self,
        *,
        access_rule_name: str,
        visited_name: str,
    ) -> list[str]:
        """Regra (6.2.8) → deviceOrg (6.2.4) → cópia por host (6.2.10)."""
        rule = (access_rule_name or "").strip()
        host = (visited_name or "").strip()

        if rule:
            channels = await self.resolve_channels_by_access_group_name(rule)
            if channels:
                return channels

        for lookup in (rule, host):
            if not lookup:
                continue
            channels = await self.resolve_channels_by_device_org_name(
                lookup, force_refresh=True
            )
            if channels:
                return channels

        lookup_host = host or rule
        if lookup_host:
            channels = await self.resolve_channels_by_visited_name(lookup_host)
            if channels:
                return channels

        logger.warning(
            "[DEFENSE_IA] sem portas para regra=%r host=%r — acsChannelIds=[] "
            "(direito padrão global)",
            rule or None,
            host or None,
        )
        return []

    async def resolve_channels_by_device_org_name(
        self, org_name: str, *, force_refresh: bool = False
    ) -> list[str]:
        """Portas vinculadas ao departamento na árvore deviceOrg (6.2.4).

        No deploy Unimed a API access-group (6.2.8) retorna 404; regras como
        ``Refeitorio`` aparecem em ``deviceOrg`` com ``channel[].id``.

        ``force_refresh`` atualiza a árvore (evita cache sem pastas/soltos recentes).
        """
        raw = (org_name or "").strip()
        if not raw:
            return []

        departments = await self.list_device_org_departments(
            force_refresh=force_refresh
        )
        if not departments:
            return []

        if raw.isdigit():
            channels = _find_device_org_channels(departments, org_code=raw)
        else:
            name_key = _normalize_visited_name_key(raw)
            channels = _find_device_org_channels(departments, org_name_key=name_key)

        if channels:
            match = _find_device_org_node(
                departments,
                org_code=raw if raw.isdigit() else None,
                org_name_key=None if raw.isdigit() else _normalize_visited_name_key(raw),
            )
            match_code = str((match or {}).get("code") or "")
            logger.info(
                "[DEFENSE_IA] acsChannelIds via deviceOrg %r code=%r: %s",
                raw,
                match_code or None,
                channels,
            )
        return channels

    async def resolve_channels_by_access_group_name(
        self, access_group_name: str
    ) -> list[str]:
        """Expande regra de acesso (accessGroupName) em acsChannelIds quando a API ACS responde."""
        raw = (access_group_name or "").strip()
        if not raw:
            return []

        normalized_key = _normalize_visited_name_key(raw)
        now = time.time()
        cached = self._access_rule_channel_cache.get(normalized_key)
        if cached and (now - cached[1]) < ACCESS_RULE_CHANNEL_CACHE_TTL_SECONDS:
            return list(cached[0])

        async with self._access_rule_channel_lock:
            now = time.time()
            cached = self._access_rule_channel_cache.get(normalized_key)
            if cached and (now - cached[1]) < ACCESS_RULE_CHANNEL_CACHE_TTL_SECONDS:
                return list(cached[0])

            resolved = await self._fetch_channels_for_access_group_name(raw)
            if resolved:
                logger.info(
                    "[DEFENSE_IA] acsChannelIds via regra de acesso %r: %s",
                    raw,
                    resolved,
                )
            else:
                logger.debug(
                    "[DEFENSE_IA] regra de acesso %r sem canais via API "
                    "(404 ou nome não encontrado)",
                    raw,
                )

            self._access_rule_channel_cache[normalized_key] = (resolved, time.time())
            return list(resolved)

    async def _fetch_channels_for_access_group_name(
        self, access_group_name: str
    ) -> list[str]:
        if not self._token:
            await self.login()

        response = await self._request(
            "GET",
            BRMS_ACCESS_GROUP_LIST,
            headers=self._auth_headers(),
        )
        if response.status_code == 401:
            await self.login()
            response = await self._request(
                "GET",
                BRMS_ACCESS_GROUP_LIST,
                headers=self._auth_headers(),
            )
        if response.status_code == 404:
            return []
        if not response.is_success or not response.content:
            return []

        try:
            body = response.json()
        except ValueError:
            return []
        if not isinstance(body, dict):
            return []

        target_key = _normalize_visited_name_key(access_group_name)
        group_id: str | None = None
        for entry in self._extract_access_group_list_items(body):
            name = str(
                entry.get("accessGroupName")
                or entry.get("groupName")
                or entry.get("name")
                or ""
            )
            if _normalize_visited_name_key(name) != target_key:
                continue
            gid = entry.get("id") or entry.get("accessGroupId") or entry.get("groupId")
            if gid is not None:
                group_id = str(gid).strip()
                break

        if not group_id:
            return []

        detail = await self._fetch_access_group_detail(group_id)
        if not detail:
            return []

        door_group_ids = detail.get("doorGroupIds") or []
        if not isinstance(door_group_ids, list):
            return []
        ids = [str(item).strip() for item in door_group_ids if str(item).strip()]
        if not ids:
            return []

        return await self._expand_door_groups_to_acs_channel_ids(ids)

    @staticmethod
    def _extract_access_group_list_items(body: dict[str, Any]) -> list[dict[str, Any]]:
        data = body.get("data", body)
        if isinstance(data, list):
            return [item for item in data if isinstance(item, dict)]
        if not isinstance(data, dict):
            return []
        for key in ("list", "pageData", "records", "results", "data"):
            raw = data.get(key)
            if isinstance(raw, list):
                return [item for item in raw if isinstance(item, dict)]
        return []

    async def _fetch_access_group_detail(self, group_id: str) -> dict[str, Any] | None:
        path = BRMS_ACCESS_GROUP_DETAIL.format(group_id=group_id)
        response = await self._request("GET", path, headers=self._auth_headers())
        if response.status_code in (401,):
            await self.login()
            response = await self._request("GET", path, headers=self._auth_headers())
        if response.status_code == 404 or not response.is_success or not response.content:
            return None
        try:
            body = response.json()
        except ValueError:
            return None
        if not isinstance(body, dict):
            return None
        data = body.get("data", body)
        return data if isinstance(data, dict) else None

    async def _expand_door_groups_to_acs_channel_ids(
        self, door_group_ids: list[str]
    ) -> list[str]:
        if not door_group_ids:
            return []

        response = await self._request(
            "GET",
            BRMS_DOOR_GROUP_LIST,
            params={"doorGroupIds": ",".join(door_group_ids)},
            headers=self._auth_headers(),
        )
        if response.status_code == 401:
            await self.login()
            response = await self._request(
                "GET",
                BRMS_DOOR_GROUP_LIST,
                params={"doorGroupIds": ",".join(door_group_ids)},
                headers=self._auth_headers(),
            )
        if response.status_code == 404 or not response.is_success or not response.content:
            return []

        try:
            body = response.json()
        except ValueError:
            return []

        channels: list[str] = []
        data = body.get("data", body)
        groups: list[object] = []
        if isinstance(data, list):
            groups = data
        elif isinstance(data, dict):
            for key in ("list", "pageData", "doorGroups", "records", "results"):
                raw = data.get(key)
                if isinstance(raw, list):
                    groups = raw
                    break

        for group in groups:
            if not isinstance(group, dict):
                continue
            raw_channels = group.get("channelIds") or group.get("channels") or []
            if not isinstance(raw_channels, list):
                continue
            for channel_id in raw_channels:
                text = str(channel_id).strip()
                if not text:
                    continue
                if "$" in text:
                    channels.append(text)
                else:
                    channels.append(f"{text}${ACS_CHANNEL_TYPE}$0$0")

        seen: set[str] = set()
        unique: list[str] = []
        for channel in channels:
            if channel not in seen:
                seen.add(channel)
                unique.append(channel)
        return unique

    async def resolve_channels_by_visited_name(self, visited_name: str) -> list[str]:
        """Copia acsChannelIds de visitantes existentes com o mesmo host (API 6.2.10)."""
        raw = (visited_name or "").strip()
        if not raw:
            logger.warning("[DEFENSE_IA] visited_name vazio — acsChannelIds=[]")
            return []

        normalized_key = _normalize_visited_name_key(raw)
        now = time.time()
        cached = self._visited_channel_cache.get(normalized_key)
        if cached and (now - cached[1]) < VISITED_NAME_CHANNEL_CACHE_TTL_SECONDS:
            return list(cached[0])

        async with self._visited_channel_lock:
            now = time.time()
            cached = self._visited_channel_cache.get(normalized_key)
            if cached and (now - cached[1]) < VISITED_NAME_CHANNEL_CACHE_TTL_SECONDS:
                return list(cached[0])

            page_items = await self._fetch_visitor_page(raw)
            channel_sets: list[tuple[str, ...]] = []

            for item in page_items:
                page_visited = str(item.get("visitedName") or "")
                page_key = _normalize_visited_name_key(page_visited)
                if page_key != normalized_key:
                    continue
                channels = await self._channels_for_visitor_page_item(item)
                if channels:
                    channel_sets.append(tuple(channels))

            if not channel_sets:
                for item in page_items:
                    page_visited = str(item.get("visitedName") or "")
                    page_key = _normalize_visited_name_key(page_visited)
                    if not _visited_names_fuzzy_match(normalized_key, page_key):
                        continue
                    channels = await self._channels_for_visitor_page_item(item)
                    if channels:
                        channel_sets.append(tuple(channels))
                if channel_sets:
                    logger.warning(
                        "[DEFENSE_IA] host %r sem match exato — usando visitante "
                        "similar no Defense (typo/variante de nome)",
                        raw,
                    )

            if not channel_sets:
                tokens = normalized_key.split()
                if len(tokens) >= 2:
                    broader_items = await self._fetch_visitor_page(tokens[-1])
                    for item in broader_items:
                        page_key = _normalize_visited_name_key(
                            str(item.get("visitedName") or "")
                        )
                        if page_key == normalized_key:
                            continue
                        if not _visited_names_fuzzy_match(normalized_key, page_key):
                            continue
                        channels = await self._channels_for_visitor_page_item(item)
                        if channels:
                            channel_sets.append(tuple(channels))
                    if channel_sets:
                        logger.warning(
                            "[DEFENSE_IA] host %r sem referência exata — portas copiadas "
                            "de visitante similar (busca ampla por %r)",
                            raw,
                            tokens[-1],
                        )

            if not channel_sets:
                logger.warning(
                    "[DEFENSE_IA] host visitante %r sem referência no Defense — "
                    "acsChannelIds=[] (direito padrão global)",
                    raw,
                )
                resolved: list[str] = []
            else:
                counts = Counter(channel_sets)
                best_count = counts.most_common(1)[0][1]
                top_sets = [channels for channels, count in counts.items() if count == best_count]
                if len(top_sets) > 1:
                    logger.warning(
                        "[DEFENSE_IA] host %r com empate entre %d conjuntos de portas — "
                        "usando o primeiro",
                        raw,
                        len(top_sets),
                    )
                resolved = list(top_sets[0])
                logger.info(
                    "[DEFENSE_IA] acsChannelIds via visitante host=%r: %s",
                    raw,
                    resolved,
                )

            self._visited_channel_cache[normalized_key] = (resolved, time.time())
            return list(resolved)

    async def _channels_for_visitor_page_item(
        self, item: dict[str, Any]
    ) -> list[str]:
        channels = self._extract_acs_channel_ids_from_page_item(item)
        if channels:
            return channels
        visitor_id = item.get("id") or item.get("visitorId")
        if not visitor_id:
            return []
        detail = await self._fetch_brms_visitor(str(visitor_id))
        if not detail:
            return []
        return self._extract_acs_channel_ids_from_visitor_body(detail)

    async def _fetch_visitor_config(self) -> dict[str, Any]:
        if not self._token:
            await self.login()
        response = await self._request(
            "GET",
            BRMS_VISITOR_CONFIG,
            headers=self._auth_headers(),
        )
        if response.status_code == 401:
            await self.login()
            response = await self._request(
                "GET",
                BRMS_VISITOR_CONFIG,
                headers=self._auth_headers(),
            )
        if not response.is_success or not response.content:
            return {}
        try:
            body = response.json()
        except ValueError:
            return {}
        if not isinstance(body, dict):
            return {}
        data = body.get("data", body)
        return data if isinstance(data, dict) else {}

    async def _upsert_person_request(
        self, payload: SyncRequest, org_code: str | None
    ) -> httpx.Response:
        headers = self._auth_headers()
        if self.settings.is_brms:
            person_id = payload.external_id
            existing_body = await self._fetch_brms_person(person_id)
            has_face_in_request = bool(self._resolve_face_base64(payload))
            if existing_body is not None:
                preserve_faces = None
                if not has_face_in_request:
                    preserve_faces = extract_face_pictures_from_person_body(existing_body)
                person_payload = self.build_person_payload(
                    payload,
                    existing_face_pictures=preserve_faces,
                    org_code=org_code,
                )
                url = brms_person_path(person_id)
                resolved = person_payload.get("baseInfo", {}).get("orgCode", org_code)
                previous_org = extract_org_code_from_person_body(existing_body)
                if previous_org and resolved and previous_org != resolved:
                    logger.info(
                        "[DEFENSE_IA] movendo pessoa external_id=%s de orgCode=%s para %s",
                        payload.external_id,
                        previous_org,
                        resolved,
                    )
                logger.info(
                    "[DEFENSE_IA] sync person external_id=%s orgCode=%s",
                    payload.external_id,
                    resolved,
                )
                logger.debug(
                    "[DEFENSE_IA OUT] method=PUT url=%s payload=%s",
                    url,
                    self._sanitize_payload_for_log(person_payload),
                )
                response = await self._request("PUT", url, json=person_payload, headers=headers)
                logger.debug(
                    "[DEFENSE_IA IN] method=PUT url=%s status=%d body=%s",
                    url,
                    response.status_code,
                    response.text[:500],
                )
                if self._brms_mutation_ok(response) and previous_org and resolved and previous_org != resolved:
                    response = await self._ensure_org_code_applied(
                        person_id=person_id,
                        person_payload=person_payload,
                        target_org_code=resolved,
                        previous_org_code=previous_org,
                        headers=headers,
                        last_response=response,
                    )
                if self._brms_mutation_ok(response):
                    await self._log_verified_org_code(person_id, resolved)
                return response
            person_payload = self.build_person_payload(payload, org_code=org_code)
            resolved = person_payload.get("baseInfo", {}).get("orgCode", org_code)
            logger.info(
                "[DEFENSE_IA] sync person external_id=%s orgCode=%s",
                payload.external_id,
                resolved,
            )
            logger.debug(
                "[DEFENSE_IA OUT] method=POST url=%s payload=%s",
                BRMS_PERSON,
                self._sanitize_payload_for_log(person_payload),
            )
            response = await self._request("POST", BRMS_PERSON, json=person_payload, headers=headers)
            logger.debug(
                "[DEFENSE_IA IN] method=POST url=%s status=%d body=%s",
                BRMS_PERSON,
                response.status_code,
                response.text[:500],
            )
            if self._brms_mutation_ok(response):
                await self._log_verified_org_code(person_id, resolved)
            return response
        person_payload = self.build_person_payload(payload, org_code=org_code)
        url = f"/OBMS/accessControl/person/{payload.external_id}"
        logger.debug(
            "[DEFENSE_IA OUT] method=PUT url=%s payload=%s",
            url,
            self._sanitize_payload_for_log(person_payload),
        )
        response = await self._request("PUT", url, json=person_payload, headers=headers)
        logger.debug(
            "[DEFENSE_IA IN] method=PUT url=%s status=%d body=%s",
            url,
            response.status_code,
            response.text[:500],
        )
        return response

    async def _log_verified_org_code(
        self, person_id: str, expected_org_code: str | None
    ) -> None:
        if not expected_org_code:
            return
        verified_body = await self._fetch_brms_person(person_id)
        verified_org = extract_org_code_from_person_body(verified_body)
        if verified_org:
            logger.info(
                "[DEFENSE_IA] verificado orgCode=%s para external_id=%s",
                verified_org,
                person_id,
            )

    async def _ensure_org_code_applied(
        self,
        *,
        person_id: str,
        person_payload: dict[str, Any],
        target_org_code: str,
        previous_org_code: str,
        headers: dict[str, str],
        last_response: httpx.Response,
    ) -> httpx.Response:
        verified_body = await self._fetch_brms_person(person_id)
        verified_org = extract_org_code_from_person_body(verified_body)
        if _org_codes_match(verified_org, target_org_code):
            return last_response
        logger.warning(
            "[DEFENSE_IA] orgCode não alterou no PUT (esperado=%s, atual=%s, anterior=%s) "
            "— recriando via delete+POST external_id=%s",
            target_org_code,
            verified_org,
            previous_org_code,
            person_id,
        )
        delete_response = await self._request(
            "POST",
            BRMS_PERSON_DELETE_BATCH,
            json={"personIds": [person_id]},
            headers=headers,
        )
        if not delete_response.is_success:
            logger.warning(
                "[DEFENSE_IA] delete batch falhou status=%d — mantendo resposta do PUT",
                delete_response.status_code,
            )
            return last_response
        create_response = await self._request(
            "POST",
            BRMS_PERSON,
            json=person_payload,
            headers=headers,
        )
        logger.debug(
            "[DEFENSE_IA IN] method=POST url=%s status=%d body=%s (recriação pós-delete)",
            BRMS_PERSON,
            create_response.status_code,
            create_response.text[:500],
        )
        return create_response

    async def _fetch_brms_person(self, person_id: str) -> dict[str, Any] | None:
        response = await self._request(
            "GET",
            brms_person_path(person_id),
            headers=self._auth_headers(),
        )
        if response.status_code == 404:
            return None
        if response.status_code == 401:
            raise DefenseIAUnauthorizedError("Token do Defense IA recusado")
        if response.status_code >= 500:
            raise DefenseIAUnavailableError("API do Defense IA indisponível")
        if not response.is_success:
            return None
        if not response.content:
            return {}
        try:
            body = response.json()
        except ValueError:
            return {}
        if not isinstance(body, dict):
            return {}
        code = body.get("code")
        if code in (404, "404", 1004, "1004"):
            return None
        desc = str(body.get("desc") or body.get("message") or "").lower()
        if "not exist" in desc or "not found" in desc or "nao exist" in desc:
            return None
        if code in (None, 0, "0", 1000, "1000"):
            return body
        return None

    async def _brms_person_exists(self, person_id: str) -> bool:
        return await self._fetch_brms_person(person_id) is not None

    @staticmethod
    def _resolve_face_base64(payload: SyncRequest) -> str:
        if payload.biometrics is None:
            return ""
        raw = payload.biometrics.face_image_base64
        if raw is None:
            return ""
        return raw.strip()

    @staticmethod
    def _sanitize_payload_for_log(person_payload: dict[str, Any]) -> dict[str, Any]:
        """Clona o payload substituindo imagens base64 por marcadores de tamanho."""
        import copy
        sanitized = copy.deepcopy(person_payload)
        for section in (sanitized, sanitized.get("baseInfo", {})):
            faces = section.get("facePictures")
            if isinstance(faces, list):
                section["facePictures"] = [
                    f"<IMG:{len(f)}_chars>" if isinstance(f, str) and f else f
                    for f in faces
                ]
        return sanitized

    def build_person_payload(
        self,
        payload: SyncRequest,
        *,
        existing_face_pictures: list[str] | None = None,
        org_code: str | None = None,
    ) -> dict[str, Any]:
        face = self._resolve_face_base64(payload)
        if face:
            face_pictures: list[str] = [face]
        elif existing_face_pictures is not None:
            face_pictures = list(existing_face_pictures)
        else:
            face_pictures = []

        # `orgCode` decide a sub-organização da pessoa no painel Defense IA;
        # cada sub-org já tem portas vinculadas no painel desktop, então o
        # acesso a portas é gerenciado por essa associação (não por
        # accessGroupIds, que este servidor ignora silenciosamente).
        candidate = (org_code or "").strip()
        resolved_org_code = candidate or self.settings.org_code or "001"

        if self.settings.is_brms:
            return {
                "baseInfo": {
                    "personId": payload.external_id,
                    "lastName": "",
                    "firstName": payload.person.full_name,
                    "gender": "0",
                    "orgCode": resolved_org_code,
                    "source": "0",
                    "facePictures": face_pictures,
                },
                "extensionInfo": {
                    "idType": "6",
                    "idNo": payload.person.document,
                    "nationalityId": "9999",
                    "birthday": "2001-01-01",
                },
                "authenticationInfo": {
                    "startTime": str(int(time.time())),
                    "endTime": str(int(time.time()) + 86400 * 365 * 10),
                },
                "residentInfo": {
                    "houseHolder": "0",
                },
                "accessInfo": {
                    "accessType": "0",
                    "guestUseTimes": "0",
                    "allowLoginDevice": "0",
                },
                "faceComparisonInfo": {
                    "enableFaceComparisonGroup": "0",
                    "faceComparisonGroupId": "",
                },
                "entranceInfo": {
                    "enableParkingSpace": "0",
                    "parkingSpaceNum": "0",
                    "enableEntranceGroup": "0",
                    "vehicles": [],
                },
            }
        return {
            "details": {
                "idNum": payload.person.document,
            },
            "baseInfo": {
                "personId": payload.external_id,
                "firstName": payload.person.full_name,
                "lastName": " ",
                "pictureData": face,
                "status": "0",
            },
            "accessRight": {
                "entranceDeviceCode": [],
            },
        }

    async def _perform_brms_login(self) -> tuple[str, str]:
        first_response = await self._request(
            "POST",
            BRMS_AUTHORIZE,
            json={
                "userName": self.settings.username,
                "ipAddress": "",
                "clientType": self.settings.client_type,
            },
            headers=JSON_HEADERS,
        )
        challenge = self._parse_authorize_challenge(first_response)
        realm = challenge.get("realm", "")
        random_key = challenge["randomKey"]
        encrypt_type = challenge.get("encryptType", "MD5")

        signature, dollar_signature = build_auth_signatures(
            self.settings.username,
            self.settings.password,
            realm,
            random_key,
        )

        try:
            login_public_key = resolve_login_public_key(
                self.settings.public_key,
                self.settings.use_server_public_key,
                challenge,
            )
        except ValueError as exc:
            raise DefenseIAError(str(exc)) from exc

        second_response = await self._request(
            "POST",
            BRMS_AUTHORIZE,
            json={
                "userName": self.settings.username,
                "randomKey": random_key,
                "mac": "",
                "signature": signature,
                "encryptType": encrypt_type,
                "ipAddress": "",
                "clientType": self.settings.client_type,
                "userType": self.settings.user_type,
                "publicKey": login_public_key,
            },
            headers=JSON_HEADERS,
        )
        self._raise_for_response(second_response)
        return extract_token(second_response.json()), dollar_signature

    async def _perform_legacy_login(self) -> tuple[str, str]:
        first_response = await self._request(
            "POST",
            LEGACY_AUTHORIZE,
            json={
                "userName": self.settings.username,
                "ipAddress": "",
                "clientType": self.settings.client_type,
            },
            headers=JSON_HEADERS,
        )
        challenge = self._parse_authorize_challenge(first_response)
        realm = challenge.get("realm", "")
        if not realm:
            raise DefenseIAError(
                "Resposta legacy sem realm; use DEFENSE_IA_API_MODE=brms para Defense IA 3.x"
            )
        random_key = challenge["randomKey"]
        signature, dollar_signature = build_auth_signatures(
            self.settings.username,
            self.settings.password,
            realm,
            random_key,
        )

        second_response = await self._request(
            "POST",
            LEGACY_AUTHORIZE,
            json={
                "userName": self.settings.username,
                "randomKey": random_key,
                "mac": "",
                "encryptType": "MD5",
                "ipAddress": "",
                "signature": signature,
                "clientType": self.settings.client_type,
            },
            headers=JSON_HEADERS,
        )
        self._raise_for_response(second_response)
        return extract_token(second_response.json()), dollar_signature

    def _parse_authorize_challenge(self, response: httpx.Response) -> dict[str, Any]:
        if response.status_code not in (200, 401):
            self._raise_for_response(response)
        try:
            payload: dict[str, Any] = response.json()
        except ValueError as exc:
            raise DefenseIAError("Defense IA retornou resposta inválida no authorize") from exc

        if payload.get("randomKey"):
            return payload

        code = payload.get("code")
        desc = payload.get("desc") or payload.get("message") or ""
        raise DefenseIAError(
            f"Defense IA authorize sem randomKey (code={code}): {desc}".strip()
        )

    async def _keep_alive_loop(self) -> None:
        while True:
            interval = (
                SESSION_CONFLICT_RETRY_SECONDS
                if self._session_conflict
                else self.settings.keep_alive_interval_seconds
            )
            await asyncio.sleep(interval)
            try:
                await self.keep_alive_once()
                self._session_conflict = False
            except DefenseIAError as exc:
                if is_session_conflict_error(exc):
                    self._clear_auth_session()
                    self._session_conflict = True
                    keep_alive_logger.warning(
                        "[KEEP_ALIVE] sessão ocupada (2004); nova tentativa em %d min",
                        int(SESSION_CONFLICT_RETRY_SECONDS // 60),
                    )
                    continue
                keep_alive_logger.warning("[KEEP_ALIVE] falha no ping, tentando re-login")
                try:
                    await self.login()
                    self._session_conflict = False
                    keep_alive_logger.info("[KEEP_ALIVE] re-login bem-sucedido apos falha")
                except DefenseIAError as login_exc:
                    if is_session_conflict_error(login_exc):
                        self._clear_auth_session()
                        self._session_conflict = True
                        keep_alive_logger.warning(
                            "[KEEP_ALIVE] sessão ocupada (2004); nova tentativa em %d min",
                            int(SESSION_CONFLICT_RETRY_SECONDS // 60),
                        )
                    else:
                        keep_alive_logger.warning(
                            "[KEEP_ALIVE] re-login falhou: %s", login_exc
                        )

    async def _request(self, method: str, path: str, **kwargs: Any) -> httpx.Response:
        if not self._http_client:
            raise DefenseIAUnavailableError("HTTP client não inicializado")
        try:
            return await self._http_client.request(
                method,
                f"{self.settings.server_url.rstrip('/')}{path}",
                **kwargs,
            )
        except httpx.HTTPError as exc:
            raise DefenseIAUnavailableError("API do Defense IA indisponível") from exc

    def _auth_headers(self) -> dict[str, str]:
        return {
            **JSON_HEADERS,
            "X-Subject-Token": self._token or "",
            "Time-Zone": "America/Sao_Paulo",
            "Accept-Language": "pt",
        }

    async def _recover_idempotent_mutation_failure(
        self,
        response: httpx.Response,
        person_id: str,
        expected_org_code: str,
    ) -> bool:
        """PUT/POST com code 1001 mas pessoa já no orgCode esperado (update redundante)."""
        code = self._response_json_code(response)
        if code not in IDEMPOTENT_MUTATION_CODES:
            return False
        verified_body = await self._fetch_brms_person(person_id)
        verified_org = extract_org_code_from_person_body(verified_body)
        if _org_codes_match(verified_org, expected_org_code):
            return True
        logger.warning(
            "[DEFENSE_IA] code=%s mas orgCode diverge: esperado=%s atual=%s external_id=%s",
            code,
            expected_org_code,
            verified_org,
            person_id,
        )
        return False

    @staticmethod
    def _response_json_code(response: httpx.Response) -> Any:
        if not response.content:
            return None
        try:
            payload = response.json()
        except ValueError:
            return None
        if isinstance(payload, dict):
            return payload.get("code")
        return None

    @staticmethod
    def _response_is_rate_limited(response: httpx.Response) -> bool:
        if not response.content:
            return False
        try:
            payload = response.json()
        except ValueError:
            return False
        if not isinstance(payload, dict):
            return False
        code = payload.get("code")
        if code in RATE_LIMIT_CODES:
            return True
        desc = str(payload.get("desc") or payload.get("message") or "").lower()
        return "too frequently" in desc

    @staticmethod
    def _brms_mutation_ok(response: httpx.Response) -> bool:
        """HTTP 2xx **e** code JSON 1000 (Defense usa 200 + code no body)."""
        if not response.is_success:
            return False
        return DefenseIAClient._response_json_code(response) in SUCCESS_CODES

    def _raise_for_response(self, response: httpx.Response) -> None:
        if response.content:
            try:
                payload = response.json()
            except ValueError:
                payload = None
            if isinstance(payload, dict):
                code = payload.get("code")
                if code not in SUCCESS_CODES:
                    desc = payload.get("desc") or payload.get("message") or ""
                    detail = payload.get("data")
                    extra = ""
                    if detail not in (None, "", {}):
                        extra = f" | data={detail}"
                    msg = f"Defense IA retornou código {code}: {desc}{extra}".strip()
                    if code in (8044, "8044"):
                        raise DefenseIAArgumentError(msg)
                    if code in FACE_SIZE_LIMIT_CODES or _looks_like_face_size_limit(
                        f"{code} {desc}"
                    ):
                        raise DefenseIAArgumentError(FACE_SIZE_LIMIT_PUBLIC_DETAIL)
                    raise DefenseIAError(msg)
        if response.status_code == 401:
            raise DefenseIAUnauthorizedError("Token do Defense IA recusado")
        if response.status_code >= 500:
            raise DefenseIAUnavailableError("API do Defense IA indisponível")
        if response.status_code >= 400:
            raise DefenseIAError(f"Defense IA retornou HTTP {response.status_code}")

    @staticmethod
    def _build_update_token_signature(token: str, dollar_signature: str) -> str:
        return hashlib.md5(f"{dollar_signature}:{token}".encode("utf-8")).hexdigest()


def build_auth_signatures(
    username: str, password: str, realm: str, random_key: str
) -> tuple[str, str]:
    """Assinatura MD5 Defense IA 3.x (login-example-JS.docx). Retorna (signature, $$signature)."""
    inner = hashlib.md5(password.encode("utf-8"))
    inner = hashlib.md5(f"{username}{inner.hexdigest()}".encode("utf-8"))
    inner = hashlib.md5(inner.hexdigest().encode("utf-8"))
    dollar_signature = hashlib.md5(
        f"{username}:{realm}:{inner.hexdigest()}".encode("utf-8")
    ).hexdigest()
    signature = hashlib.md5(f"{dollar_signature}:{random_key}".encode("utf-8")).hexdigest()
    return signature, dollar_signature


def extract_token(payload: dict[str, Any]) -> str:
    token = payload.get("token")
    if token:
        return str(token)
    data = payload.get("data")
    if isinstance(data, dict) and data.get("token"):
        return str(data["token"])
    raise DefenseIAError("Defense IA authorize não retornou token")


FACE_SIZE_LIMIT_CODES = (8079, "8079")
FACE_SIZE_LIMIT_PUBLIC_DETAIL = (
    "Foto facial excede o limite do Defense IA (máximo 100 KB). "
    "Comprima a imagem (preferencialmente JPEG) e envie novamente."
)
SESSION_CONFLICT_PUBLIC_DETAIL = (
    "Defense IA ocupado: a conta da API já está logada em outro lugar "
    "(painel Windows ou outra instância do middleware). "
    "Desconecte essa sessão e tente novamente. "
    "O sistema tenta reconectar sozinho a cada 10 minutos."
)


def _looks_like_face_size_limit(text: str) -> bool:
    lowered = text.casefold()
    return (
        "8079" in lowered
        or "face size over limit" in lowered
        or "person face size" in lowered
    )


def is_face_size_limit_error(exc: BaseException) -> bool:
    return _looks_like_face_size_limit(str(exc))


def _looks_like_session_conflict(text: str) -> bool:
    lowered = text.casefold()
    return "2004" in lowered or "the user has logged in" in lowered


def is_session_conflict_error(exc: BaseException) -> bool:
    return _looks_like_session_conflict(str(exc))


def defense_error_detail_public(exc: DefenseIAError) -> str:
    if is_face_size_limit_error(exc):
        return FACE_SIZE_LIMIT_PUBLIC_DETAIL
    if is_session_conflict_error(exc):
        return SESSION_CONFLICT_PUBLIC_DETAIL
    if os.getenv("DEFENSE_IA_EXPOSE_ERROR", "false").lower() in ("1", "true", "yes"):
        return str(exc)
    return "API do Defense IA indisponível"
