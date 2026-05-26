import asyncio
import hashlib
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

# Endpoint usado para inferir as sub-organizações cadastradas no Defense IA.
# Não há endpoint público de listagem de orgs neste servidor (BRMS 3.x da Unimed
# retorna 404 em /obms/.../org/list), então a inferência é feita varrendo as
# pessoas existentes e coletando pares (orgCode, orgName).
BRMS_PERSON_PAGE = "/obms/api/v1.1/acs/person/page"
PERSON_ORGS_CACHE_TTL_SECONDS = 1800.0
PERSON_ORGS_PAGE_SIZE = 200


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
        # Cache nome (lower) -> orgCode obtido via varredura de pessoas. Como
        # não existe endpoint listador de orgs neste servidor, evitamos pagar
        # o custo da varredura a cada webhook usando TTL.
        self._person_orgs_cache: dict[str, str] | None = None
        self._person_orgs_cache_at: float = 0.0
        self._person_orgs_lock = asyncio.Lock()

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
                logger.warning(
                    "Defense IA startup login failed, will retry in background: %s", exc
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

    async def list_person_orgs(self, force_refresh: bool = False) -> dict[str, str]:
        """Retorna {orgName_lower: orgCode} das sub-organizações do Defense IA.

        Como o servidor da Unimed não expõe endpoint listador de orgs (todas as
        rotas /org/list, /org/tree, /department/* documentadas retornam 404),
        a única fonte confiável é varrer todas as pessoas e coletar o par
        (orgCode, orgName) que aparece em baseInfo. O resultado é cacheado em
        memória com TTL para evitar paginar 70+ páginas a cada webhook.
        """
        now = time.time()
        if (
            not force_refresh
            and self._person_orgs_cache is not None
            and (now - self._person_orgs_cache_at) < PERSON_ORGS_CACHE_TTL_SECONDS
        ):
            return self._person_orgs_cache

        async with self._person_orgs_lock:
            now = time.time()
            if (
                not force_refresh
                and self._person_orgs_cache is not None
                and (now - self._person_orgs_cache_at) < PERSON_ORGS_CACHE_TTL_SECONDS
            ):
                return self._person_orgs_cache

            if not self._token:
                await self.login()

            orgs: dict[str, str] = {}
            page = 1
            while True:
                response = await self._request(
                    "GET",
                    BRMS_PERSON_PAGE,
                    headers=self._auth_headers(),
                    params={
                        "page": page,
                        "pageSize": PERSON_ORGS_PAGE_SIZE,
                        "orgCode": self.settings.org_code or "001",
                        "containChild": "1",
                    },
                )
                if response.status_code == 401:
                    await self.login()
                    response = await self._request(
                        "GET",
                        BRMS_PERSON_PAGE,
                        headers=self._auth_headers(),
                        params={
                            "page": page,
                            "pageSize": PERSON_ORGS_PAGE_SIZE,
                            "orgCode": self.settings.org_code or "001",
                            "containChild": "1",
                        },
                    )
                if not response.is_success:
                    raise DefenseIAError(
                        f"Falha ao listar sub-orgs (HTTP {response.status_code})"
                    )
                try:
                    body = response.json()
                except ValueError as exc:
                    raise DefenseIAError(
                        "Resposta inválida ao listar sub-orgs do Defense IA"
                    ) from exc
                data = body.get("data") or {}
                people = data.get("pageData") or []
                if not isinstance(people, list) or not people:
                    break
                for entry in people:
                    base = entry.get("baseInfo") if isinstance(entry, dict) else None
                    if not isinstance(base, dict):
                        continue
                    code = base.get("orgCode")
                    name = base.get("orgName")
                    if not code or not name:
                        continue
                    key = str(name).strip().lower()
                    if key and key not in orgs:
                        orgs[key] = str(code)
                if len(people) < PERSON_ORGS_PAGE_SIZE:
                    break
                page += 1

            self._person_orgs_cache = orgs
            self._person_orgs_cache_at = time.time()
            logger.info(
                "[DEFENSE_IA] cache de sub-orgs atualizado: %d entrada(s)",
                len(orgs),
            )
            return orgs

    async def resolve_org_code(self, name: str | None) -> str | None:
        """Resolve o orgCode no Defense IA a partir do `reguiredName` do BioDoc.

        Aceita dois formatos:

        1. Código direto (apenas dígitos, ex.: `"001015001"`): retorna como
           está, sem chamar a API. Esta é a forma recomendada para evitar a
           paginação completa de pessoas (~3 min no servidor da Unimed).
        2. Nome amigável (ex.: `"Corb"`): faz lookup case-insensitive no
           cache de sub-orgs (preenchido na 1ª chamada com varredura completa,
           válido por PERSON_ORGS_CACHE_TTL_SECONDS).

        Em caso de miss no cache, **não** força re-varredura para evitar
        sobrecarga em rajadas de webhooks com nomes desconhecidos. Para
        capturar sub-orgs criadas após o boot, use
        `await client.list_person_orgs(force_refresh=True)` (ou rode o
        script `scripts/list_person_orgs.py --refresh`).

        Retorna `None` se o nome for vazio ou não casar com nenhuma sub-org —
        o chamador decide o fallback.
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
        orgs = await self.list_person_orgs()
        return orgs.get(key.lower())

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
            person_payload = self.build_person_payload(payload, org_code=org_code)
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
                    logger.warning("Defense IA keep-alive re-login failed: %s", exc)

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
