import asyncio
import hashlib
import os
import time
from dataclasses import dataclass
from typing import Any, Literal

import httpx

from src.api.schemas import SyncRequest
from src.core.logging import logger
from src.services.defense_ia_crypto import resolve_login_public_key

API_MODE_BRMS = "brms"
BRMS_PERSON = "/obms/api/v1.1/acs/person"
API_MODE_LEGACY = "legacy"


def brms_person_path(person_id: str) -> str:
    return f"{BRMS_PERSON}/{person_id}"


BRMS_AUTHORIZE = "/brms/api/v1.0/accounts/authorize"
BRMS_KEEPALIVE = "/brms/api/v1.0/accounts/keepalive"
BRMS_UPDATE_TOKEN = "/brms/api/v1.0/accounts/updateToken"
LEGACY_AUTHORIZE = "/admin/API/accounts/authorize"
LEGACY_UPDATE_TOKEN = "/admin/API/accounts/updateToken"

JSON_HEADERS = {"content-type": "application/json;charset=UTF-8"}
SUCCESS_CODES = (None, 0, "0", 1000, "1000")


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
            await self.login()
            return

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
            await self.login()
            return
        self._raise_for_response(response)

    async def sync_person(self, payload: SyncRequest) -> dict[str, Any]:
        if self.settings.enabled and not self._token:
            raise DefenseIANotReadyError("Defense IA não conectado")
        return await self._send_person_with_relogin(payload)

    async def _send_person_with_relogin(self, payload: SyncRequest) -> dict[str, Any]:
        if not self._token:
            await self.login()

        response = await self._upsert_person_request(payload)
        if response.status_code == 401:
            await self.login()
            response = await self._upsert_person_request(payload)

        self._raise_for_response(response)
        if not response.content:
            return {}
        return response.json()

    async def _upsert_person_request(self, payload: SyncRequest) -> httpx.Response:
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
            person_payload = self.build_person_payload(payload)
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
        person_payload = self.build_person_payload(payload)
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
    ) -> dict[str, Any]:
        face = self._resolve_face_base64(payload)
        if face:
            face_pictures: list[str] = [face]
        elif existing_face_pictures is not None:
            face_pictures = list(existing_face_pictures)
        else:
            face_pictures = []

        if self.settings.is_brms:
            return {
                "baseInfo": {
                    "personId": payload.external_id,
                    "lastName": "",
                    "firstName": payload.person.full_name,
                    "gender": "0",
                    "orgCode": self.settings.org_code,
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
                    "enableAccessGroup": "0",
                    "accessGroupIds": [],
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
                try:
                    await self.login()
                except DefenseIAError as exc:
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
