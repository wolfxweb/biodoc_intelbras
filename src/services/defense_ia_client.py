import asyncio
import hashlib
import json
import os
import time
from dataclasses import dataclass
from typing import Any, Literal

import httpx

from src.api.schemas import SyncRequest
from src.core.logging import keep_alive_logger, logger
from src.services.defense_ia_crypto import resolve_login_public_key

API_MODE_BRMS = "brms"
BRMS_PERSON = "/obms/api/v1.1/acs/person"
BRMS_VISITOR = "/obms/api/v1.0/visitors/visitor"
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

# Listagem de sub-organizações (grupos de pessoas) no Defense IA BRMS 3.x.
BRMS_PERSON_GROUP_LIST = "/obms/api/v1.1/acs/person-group/list"
BRMS_PERSON_DELETE_BATCH = "/obms/api/v1.1/acs/person/delete/batch"
PERSON_GROUPS_CACHE_TTL_SECONDS = 1800.0


def _normalize_org_lookup_key(name: str) -> str:
    return name.strip().casefold()


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
            return str(code)
    code = node.get("orgCode")
    return str(code) if code else None


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

    @property
    def enabled(self) -> bool:
        return bool(self.server_url and self.username and self.password)

    @property
    def is_brms(self) -> bool:
        return self.api_mode == API_MODE_BRMS


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
        self._person_groups_cache_at: float = 0.0
        self._person_groups_lock = asyncio.Lock()
        self._person_groups_available: bool | None = None

    @property
    def token(self) -> str | None:
        return self._token

    @property
    def is_ready(self) -> bool:
        if not self.settings.enabled:
            return True
        return self._token is not None

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
            if self.settings.is_brms:
                token, dollar_signature = await self._perform_brms_login()
            else:
                token, dollar_signature = await self._perform_legacy_login()
            self._token = token
            self._dollar_signature = dollar_signature
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

    async def sync_visitor(
        self, payload: SyncRequest, entrance_ids: list[str]
    ) -> dict[str, Any]:
        """Upsert de visitante no Defense IA 3.x (BRMS). Trata re-login em 401."""
        if self.settings.enabled and not self._token:
            raise DefenseIANotReadyError("Defense IA não conectado")
        return await self._send_visitor_with_relogin(payload, entrance_ids)

    async def _send_visitor_with_relogin(
        self, payload: SyncRequest, entrance_ids: list[str]
    ) -> dict[str, Any]:
        if not self._token:
            await self.login()

        response = await self._upsert_visitor_request(payload, entrance_ids)
        if response.status_code == 401:
            await self.login()
            response = await self._upsert_visitor_request(payload, entrance_ids)

        self._raise_for_response(response)
        if not response.content:
            return {}
        return response.json()

    async def _upsert_visitor_request(
        self, payload: SyncRequest, entrance_ids: list[str]
    ) -> httpx.Response:
        # O Defense IA gera visitorId automaticamente; cada evento BioDoc = nova visita.
        headers = self._auth_headers()
        visitor_payload = self.build_visitor_payload(payload, entrance_ids=entrance_ids)
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
            "visitorName": payload.person.full_name,
            "idType": "0",
            "idNum": payload.person.document or None,
            "arrivalTime": str(now),
            "expectLeaveTime": str(now + 86400 * 365 * 10),
            "reason": "",
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
        # visitedPersonId é obrigatório — ID do usuário receptor no Defense IA
        if self.settings.visited_person_id:
            body["visitedPersonId"] = self.settings.visited_person_id
        return body

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
                if response.is_success and previous_org and resolved and previous_org != resolved:
                    response = await self._ensure_org_code_applied(
                        person_id=person_id,
                        person_payload=person_payload,
                        target_org_code=resolved,
                        previous_org_code=previous_org,
                        headers=headers,
                        last_response=response,
                    )
                if response.is_success:
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
            if response.is_success:
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
        if verified_org == target_org_code:
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
            await asyncio.sleep(self.settings.keep_alive_interval_seconds)
            try:
                await self.keep_alive_once()
            except DefenseIAError:
                keep_alive_logger.warning("[KEEP_ALIVE] falha no ping, tentando re-login")
                try:
                    await self.login()
                    keep_alive_logger.info("[KEEP_ALIVE] re-login bem-sucedido apos falha")
                except DefenseIAError as exc:
                    keep_alive_logger.warning(
                        "[KEEP_ALIVE] re-login falhou: %s", exc
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


def defense_error_detail_public(exc: DefenseIAError) -> str:
    if os.getenv("DEFENSE_IA_EXPOSE_ERROR", "false").lower() in ("1", "true", "yes"):
        return str(exc)
    return "API do Defense IA indisponível"
