import asyncio
import hashlib
from dataclasses import dataclass
from typing import Any

import httpx

from src.api.schemas import SyncRequest


class DefenseIAError(Exception):
    pass


class DefenseIAUnavailableError(DefenseIAError):
    pass


class DefenseIAUnauthorizedError(DefenseIAError):
    pass


@dataclass(frozen=True)
class DefenseIASettings:
    server_url: str
    username: str
    password: str
    keep_alive_interval_seconds: float = 20.0
    timeout_seconds: float = 10.0

    @property
    def enabled(self) -> bool:
        return bool(self.server_url and self.username and self.password)


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
        self._temp_signature: Any | None = None
        self._keep_alive_task: asyncio.Task[None] | None = None
        self._auth_lock = asyncio.Lock()

    @property
    def token(self) -> str | None:
        return self._token

    async def start(self) -> None:
        if self._http_client is None:
            self._http_client = httpx.AsyncClient(timeout=self.settings.timeout_seconds)
        if self.settings.enabled:
            await self.login()
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
            token, temp_signature = await self._perform_login()
            self._token = token
            self._temp_signature = temp_signature
            return token

    async def keep_alive_once(self) -> None:
        if not self._token or not self._temp_signature:
            await self.login()
            return

        signature = self._build_token_signature(self._token, self._temp_signature)
        headers = {
            "content-type": "application/json",
            "X-Subject-Token": self._token,
        }
        response = await self._request(
            "POST",
            "/admin/API/accounts/updateToken",
            json={"signature": signature},
            headers=headers,
        )
        if response.status_code == 401:
            await self.login()
            return
        self._raise_for_response(response)

    async def sync_person(self, payload: SyncRequest) -> dict[str, Any]:
        return await self._send_person_with_relogin(payload)

    async def _send_person_with_relogin(self, payload: SyncRequest) -> dict[str, Any]:
        if not self._token:
            await self.login()

        response = await self._send_person_request(payload)
        if response.status_code == 401:
            await self.login()
            response = await self._send_person_request(payload)

        self._raise_for_response(response)
        if not response.content:
            return {}
        return response.json()

    async def _send_person_request(self, payload: SyncRequest) -> httpx.Response:
        headers = {
            "content-type": "application/json",
            "X-Subject-Token": self._token or "",
        }
        person_payload = self.build_person_payload(payload)
        return await self._request(
            "PUT",
            f"/OBMS/accessControl/person/{payload.external_id}",
            json=person_payload,
            headers=headers,
        )

    def build_person_payload(self, payload: SyncRequest) -> dict[str, Any]:
        return {
            "details": {
                "idNum": payload.person.document,
            },
            "baseInfo": {
                "personId": payload.external_id,
                "firstName": payload.person.full_name,
                "lastName": " ",
                "pictureData": payload.biometrics.face_image_base64,
                "status": "0",
            },
            "accessRight": {
                "entranceDeviceCode": [],
            },
        }

    async def _perform_login(self) -> tuple[str, Any]:
        first_response = await self._request(
            "POST",
            "/admin/API/accounts/authorize",
            json={
                "userName": self.settings.username,
                "ipAddress": "",
                "clientType": "WINPC",
            },
            headers={"content-type": "application/json"},
        )
        self._raise_for_response(first_response)
        first_payload = first_response.json()
        realm = first_payload["realm"]
        random_key = first_payload["randomKey"]

        password_md5 = hashlib.md5(self.settings.password.encode("utf-8")).hexdigest()
        username_password = hashlib.md5(
            f"{self.settings.username}{password_md5}".encode("utf-8")
        ).hexdigest()
        temp_signature = hashlib.md5(username_password.encode("utf-8"))
        realm_signature = hashlib.md5(
            f"{self.settings.username}:{realm}:{temp_signature.hexdigest()}".encode(
                "utf-8"
            )
        )
        signature = hashlib.md5(
            f"{realm_signature.hexdigest()}:{random_key}".encode("utf-8")
        ).hexdigest()

        second_response = await self._request(
            "POST",
            "/admin/API/accounts/authorize",
            json={
                "userName": self.settings.username,
                "randomKey": random_key,
                "mac": "",
                "encryptType": "MD5",
                "ipAddress": "",
                "signature": signature,
                "clientType": "WINPC",
            },
            headers={"content-type": "application/json"},
        )
        self._raise_for_response(second_response)
        return second_response.json()["token"], realm_signature

    async def _keep_alive_loop(self) -> None:
        while True:
            await asyncio.sleep(self.settings.keep_alive_interval_seconds)
            try:
                await self.keep_alive_once()
            except DefenseIAError:
                await self.login()

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

    def _raise_for_response(self, response: httpx.Response) -> None:
        if response.status_code == 401:
            raise DefenseIAUnauthorizedError("Token do Defense IA recusado")
        if response.status_code >= 500:
            raise DefenseIAUnavailableError("API do Defense IA indisponível")
        if response.status_code >= 400:
            raise DefenseIAError(f"Defense IA retornou HTTP {response.status_code}")

    @staticmethod
    def _build_token_signature(token: str, temp_signature: Any) -> str:
        return hashlib.md5(
            f"{temp_signature.hexdigest()}:{token}".encode("utf-8")
        ).hexdigest()
