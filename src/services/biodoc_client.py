"""Cliente async para a API BioDoc — consulta de beneficiários."""

from __future__ import annotations

import json
from dataclasses import dataclass

import httpx

from src.core.logging import logger
from src.core.webhook_log import format_biodoc_call


class BiodocAPIError(Exception):
    """Erro genérico na comunicação com a API BioDoc."""


class BiodocAPIUnavailableError(BiodocAPIError):
    """API BioDoc inacessível ou timeout."""


class BiodocAPIUnauthorizedError(BiodocAPIError):
    """Token da API BioDoc recusado."""


@dataclass
class CardMainImageData:
    name: str
    card: str
    status: bool
    image: str | None


@dataclass
class ExternalAuditEntry:
    """Entrada retornada por GET /logs/external-audits."""

    id: int | str | None
    id_card: str
    name: str | None
    status: str | None
    date: str | None
    required: int | None
    required_name: str | None
    description: str | None


@dataclass
class IntegrationLogData:
    """Dados retornados pelo endpoint GET /integrations/log/{reference_id}.

    Campos relevantes para o middleware:
    - id_card: documento/cartao usado para registrar a pessoa no Defense IA.
    - name: nome do beneficiario.
    - status: 1=ativo, 2=ativo (renovacao); demais valores = inativo.
    - main_image / path: URLs alternativas da foto facial (usadas em fallback).
    - required_name: reguiredName do BioDoc (ex.: nome da empresa/local).
      Mapeado para accessGroupId via DEFENSE_IA_ACCESS_GROUP_MAP no .env.
    """

    id: int | None
    id_card: str
    name: str
    status: int
    main_image: str | None
    path: str | None
    required_name: str | None
    operador: str | None = None
    local_token: str | None = None


def _extract_local_token_from_log_record(data: dict[str, object]) -> str | None:
    """Local Token do log BioDoc (ex.: 'CHU - ESPAÇO VIVER BEM')."""
    json_block = data.get("json")
    if not isinstance(json_block, dict):
        return None
    raw = json_block.get("Local Token") or json_block.get("localToken")
    if raw is None:
        return None
    value = str(raw).strip()
    return value or None


def _extract_operador_from_log_record(data: dict[str, object]) -> str | None:
    """Operador pode vir em `detail` (JSON string ou dict) ou em `json.Operador`."""
    detail = data.get("detail")
    if isinstance(detail, dict):
        raw = detail.get("operador") or detail.get("operator") or detail.get("grupo")
        if raw is not None:
            value = str(raw).strip()
            if value:
                return value
    if isinstance(detail, str) and detail.strip():
        try:
            parsed = json.loads(detail)
        except ValueError:
            parsed = None
        if isinstance(parsed, dict):
            raw = parsed.get("operador") or parsed.get("operator") or parsed.get("grupo")
            if raw is not None:
                value = str(raw).strip()
                if value:
                    return value

    json_block = data.get("json")
    if isinstance(json_block, dict):
        raw = json_block.get("Operador") or json_block.get("operador")
        if raw is not None:
            value = str(raw).strip()
            if value:
                return value

    return None


def _unwrap_biodoc_payload(body: object) -> dict[str, object]:
    """BioDoc pode retornar campos na raiz ou dentro de `data`."""
    if not isinstance(body, dict):
        return {}
    nested = body.get("data")
    if isinstance(nested, dict):
        return nested
    return body


def _coerce_active_status(raw: object) -> bool:
    if isinstance(raw, bool):
        return raw
    try:
        return int(raw) in (1, 2)
    except (TypeError, ValueError):
        return bool(raw)


class BiodocClient:
    """
    Wrapper async (httpx) para consulta de beneficiários no BioDoc.

    Uso típico (gerenciado pelo lifespan):
        client = BiodocClient(base_url=..., token_api=...)
        await client.start()
        data = await client.get_card_mainimage("1234567890")
        await client.close()
    """

    def __init__(
        self,
        base_url: str,
        token_api: str,
        timeout_seconds: float = 10.0,
    ) -> None:
        self._base_url = base_url.rstrip("/")
        self._token_api = token_api
        self._timeout = timeout_seconds
        self._client: httpx.AsyncClient | None = None

    async def start(self) -> None:
        self._client = httpx.AsyncClient(
            base_url=self._base_url,
            timeout=self._timeout,
            headers={"Authorization": f"Bearer {self._token_api}"},
        )

    async def close(self) -> None:
        if self._client is not None:
            await self._client.aclose()
            self._client = None

    async def get_card_mainimage(self, card: str) -> CardMainImageData:
        """
        GET /card/integration/mainimage?idCard=<card>

        Retorna os dados do beneficiário ou levanta BiodocAPIError em caso de falha.
        """
        if self._client is None:
            raise BiodocAPIUnavailableError("BiodocClient não iniciado")

        logger.debug("[BIODOC OUT] GET /card/integration/mainimage idCard=%s", card)
        try:
            response = await self._client.get(
                "/card/integration/mainimage",
                params={"idCard": card},
            )
        except httpx.TimeoutException as exc:
            raise BiodocAPIUnavailableError(
                f"Timeout ao consultar BioDoc (idCard={card})"
            ) from exc
        except httpx.RequestError as exc:
            raise BiodocAPIUnavailableError(
                f"Erro de rede ao consultar BioDoc (idCard={card}): {exc}"
            ) from exc

        logger.debug(
            "[BIODOC IN] GET /card/integration/mainimage idCard=%s status=%d",
            card,
            response.status_code,
        )

        if response.status_code == 401:
            raise BiodocAPIUnauthorizedError("BIODOC_TOKEN_API recusado pela API BioDoc")

        if not response.is_success:
            raise BiodocAPIUnavailableError(
                f"API BioDoc retornou status inesperado {response.status_code} "
                f"para idCard={card}"
            )

        try:
            body = response.json()
        except ValueError as exc:
            raise BiodocAPIUnavailableError(
                "Resposta inválida da API BioDoc (JSON malformado)"
            ) from exc

        data = _unwrap_biodoc_payload(body)
        if not data:
            raise BiodocAPIUnavailableError(
                "Resposta da API BioDoc em formato inesperado (objeto vazio)"
            )

        status_raw = data.get("status", data.get("cardStatus"))
        image = data.get("image") or data.get("mainImage") or data.get("base64Image")

        return CardMainImageData(
            name=str(data.get("name") or ""),
            card=str(data.get("card") or data.get("idCard") or card),
            status=_coerce_active_status(status_raw),
            image=str(image) if image else None,
        )

    async def get_external_audits(
        self,
        id_card: str,
        *,
        initial_date: str | None = None,
        end_date: str | None = None,
    ) -> list[ExternalAuditEntry]:
        """
        GET /logs/external-audits?idCard=<card>&initialDate=&endDate=

        Lista auditorias/interações do beneficiário no intervalo informado.
        """
        if self._client is None:
            raise BiodocAPIUnavailableError("BiodocClient não iniciado")

        params: dict[str, str] = {"idCard": id_card}
        if initial_date:
            params["initialDate"] = initial_date
        if end_date:
            params["endDate"] = end_date

        logger.info(
            format_biodoc_call(
                direction="OUT",
                method="GET",
                path="/logs/external-audits",
                fields={
                    "idCard": id_card,
                    "initialDate": initial_date,
                    "endDate": end_date,
                },
            )
        )
        try:
            response = await self._client.get("/logs/external-audits", params=params)
        except httpx.TimeoutException as exc:
            raise BiodocAPIUnavailableError(
                f"Timeout ao consultar BioDoc external-audits (idCard={id_card})"
            ) from exc
        except httpx.RequestError as exc:
            raise BiodocAPIUnavailableError(
                f"Erro de rede ao consultar BioDoc external-audits (idCard={id_card}): {exc}"
            ) from exc

        if response.status_code == 401:
            raise BiodocAPIUnauthorizedError("BIODOC_TOKEN_API recusado pela API BioDoc")

        if not response.is_success:
            logger.warning(
                "[BIODOC IN] body status=%d idCard=%s body=%s",
                response.status_code,
                id_card,
                response.text[:500],
            )
            raise BiodocAPIUnavailableError(
                f"API BioDoc retornou status inesperado {response.status_code} "
                f"para external-audits idCard={id_card}"
            )

        try:
            body = response.json()
        except ValueError as exc:
            raise BiodocAPIUnavailableError(
                "Resposta inválida da API BioDoc (JSON malformado)"
            ) from exc

        data = _unwrap_biodoc_payload(body)
        raw_logs = data.get("logs") if data else None
        if not isinstance(raw_logs, list):
            logger.info(
                format_biodoc_call(
                    direction="IN",
                    method="GET",
                    path="/logs/external-audits",
                    status=response.status_code,
                    fields={"idCard": id_card, "count": 0},
                )
            )
            return []

        entries: list[ExternalAuditEntry] = []
        for item in raw_logs:
            if not isinstance(item, dict):
                continue
            entries.append(
                ExternalAuditEntry(
                    id=item.get("id"),  # type: ignore[arg-type]
                    id_card=str(item.get("id_Card") or item.get("idCard") or id_card),
                    name=str(item.get("name") or "") or None,
                    status=str(item.get("status") or "") or None,
                    date=str(item.get("date") or "") or None,
                    required=item.get("required"),  # type: ignore[arg-type]
                    required_name=str(item.get("required_Name") or item.get("requiredName") or "")
                    or None,
                    description=str(item.get("description") or "") or None,
                )
            )

        logger.info(
            format_biodoc_call(
                direction="IN",
                method="GET",
                path="/logs/external-audits",
                status=response.status_code,
                fields={"idCard": id_card, "count": len(entries)},
            )
        )
        return entries

    async def get_integration_log(self, reference_id: str) -> IntegrationLogData:
        """
        GET /integrations/log/{reference_id}

        Endpoint de webhook: retorna os dados do log de integracao pelo reference_Id
        recebido no webhook BioDoc. Inclui o `reguiredName` que e mapeado para
        `accessGroupId` no Defense IA via DEFENSE_IA_ACCESS_GROUP_MAP.

        Levanta BiodocAPIError em caso de falha.
        """
        if self._client is None:
            raise BiodocAPIUnavailableError("BiodocClient não iniciado")

        logger.info(
            format_biodoc_call(
                direction="OUT",
                method="GET",
                path=f"/integrations/log/{reference_id}",
            )
        )
        try:
            response = await self._client.get(f"/integrations/log/{reference_id}")
        except httpx.TimeoutException as exc:
            raise BiodocAPIUnavailableError(
                f"Timeout ao consultar BioDoc (reference_id={reference_id})"
            ) from exc
        except httpx.RequestError as exc:
            raise BiodocAPIUnavailableError(
                f"Erro de rede ao consultar BioDoc (reference_id={reference_id}): {exc}"
            ) from exc

        if response.status_code == 401:
            logger.warning(
                "[BIODOC IN] body 401 reference_id=%s body=%s",
                reference_id,
                response.text[:500],
            )
            raise BiodocAPIUnauthorizedError("BIODOC_TOKEN_API recusado pela API BioDoc")

        if not response.is_success:
            logger.warning(
                "[BIODOC IN] body status=%d reference_id=%s body=%s",
                response.status_code,
                reference_id,
                response.text[:500],
            )
            raise BiodocAPIUnavailableError(
                f"API BioDoc retornou status inesperado {response.status_code} "
                f"para reference_id={reference_id}"
            )

        try:
            body = response.json()
        except ValueError as exc:
            raise BiodocAPIUnavailableError(
                "Resposta inválida da API BioDoc (JSON malformado)"
            ) from exc

        data = _unwrap_biodoc_payload(body)
        if not data:
            raise BiodocAPIUnavailableError(
                "Resposta da API BioDoc em formato inesperado (esperado objeto JSON)"
            )

        id_card = str(data.get("id_Card") or data.get("idCard") or "")
        if not id_card:
            raise BiodocAPIUnavailableError(
                f"Resposta da API BioDoc sem campo 'id_Card' (reference_id={reference_id})"
            )

        operador = _extract_operador_from_log_record(data)
        local_token = _extract_local_token_from_log_record(data)
        logger.info(
            format_biodoc_call(
                direction="IN",
                method="GET",
                path=f"/integrations/log/{reference_id}",
                status=response.status_code,
                fields={
                    "id_Card": id_card,
                    "name": data.get("name") or data.get("userName"),
                    "status": data.get("status"),
                    "detail": "present" if data.get("detail") else "absent",
                    "operador": operador,
                    "local_token": local_token,
                    "required_name": data.get("requiredName") or data.get("reguiredName"),
                },
            )
        )

        return IntegrationLogData(
            id=data.get("id"),  # type: ignore[arg-type]
            id_card=id_card,
            name=str(data.get("name") or data.get("userName") or ""),
            status=int(data.get("status") or 0),
            main_image=data.get("mainImage") or None,  # type: ignore[arg-type]
            path=data.get("path") or None,  # type: ignore[arg-type]
            required_name=data.get("requiredName") or data.get("reguiredName") or None,  # type: ignore[arg-type]
            operador=operador,
            local_token=local_token,
        )
