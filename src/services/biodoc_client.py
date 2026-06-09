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
    local_name: str | None = None
    local_source: str | None = None


_DETAIL_LOCAL_KEYS = (
    "nmLocal",
    "RequiredName",
    "requiredName",
    "local",
    "prestNome",
    "prestador",
)


def _parse_detail_block(detail: object) -> dict[str, object]:
    """Normaliza `detail` do log BioDoc (dict ou string JSON)."""
    if isinstance(detail, dict):
        return detail
    if isinstance(detail, str) and detail.strip():
        try:
            parsed = json.loads(detail)
        except ValueError:
            return {}
        if isinstance(parsed, dict):
            return parsed
    return {}


def _extract_operador_from_detail(detail: object) -> str | None:
    """Grupo/regra de acesso a partir do bloco detail (doc BioDoc Detail).

    Chaves oficiais: operador, usuclin — https://docs.biodoc.com.br/detail/
    """
    block = _parse_detail_block(detail)
    for key in ("operador", "usuclin", "org_code", "orgCode", "operator", "grupo"):
        raw = block.get(key)
        if raw is not None:
            value = str(raw).strip()
            if value:
                return value
    return None


def _extract_local_name_from_detail(detail: object) -> str | None:
    """Nome do local a partir do bloco detail (doc BioDoc Detail)."""
    block = _parse_detail_block(detail)
    for key in _DETAIL_LOCAL_KEYS:
        raw = block.get(key)
        if raw is not None:
            value = str(raw).strip()
            if value:
                return value
    return None


def _resolve_local_name_from_log(data: dict[str, object]) -> tuple[str | None, str | None]:
    """Resolve local: reguiredName (API) → campos do detail (nmLocal, RequiredName, ...)."""
    raw_required = data.get("requiredName") or data.get("reguiredName")
    if raw_required is not None:
        value = str(raw_required).strip()
        if value:
            return value, "reguiredName"

    from_detail = _extract_local_name_from_detail(data.get("detail"))
    if from_detail:
        return from_detail, "detail"

    return None, None


def _extract_operador_from_log_record(data: dict[str, object]) -> str | None:
    """Operador pode vir em `detail` (JSON string ou dict) ou em `json.Operador`."""
    from_detail = _extract_operador_from_detail(data.get("detail"))
    if from_detail:
        return from_detail

    json_block = data.get("json")
    if isinstance(json_block, dict):
        raw = json_block.get("Operador") or json_block.get("operador")
        if raw is not None:
            value = str(raw).strip()
            if value:
                return value

    return None


def _integration_log_log_fields(
    *,
    reference_id: str,
    data: dict[str, object],
    operador: str | None,
) -> dict[str, object]:
    """Campos explícitos do GET /integrations/log para o log (inclui detail.operador)."""
    detail_block = _parse_detail_block(data.get("detail"))
    json_block = data.get("json") if isinstance(data.get("json"), dict) else {}
    return {
        "reference_Id": reference_id,
        "id": data.get("id"),
        "id_Card": data.get("id_Card") or data.get("idCard"),
        "name": data.get("name") or data.get("userName"),
        "status": data.get("status"),
        "date": data.get("date"),
        "reguiredName": data.get("reguiredName") or data.get("requiredName"),
        "description": data.get("description"),
        "observation": data.get("observation"),
        "auditor": data.get("auditor"),
        "id_Company": data.get("id_Company"),
        "containsHistory": data.get("containsHistory"),
        "detail.operador": operador
        or detail_block.get("operador")
        or detail_block.get("usuclin"),
        "detail.usuclin": detail_block.get("usuclin"),
        "detail": detail_block if detail_block else data.get("detail"),
        "json.Operador": json_block.get("Operador") or json_block.get("operador"),
    }


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

        logger.info(
            format_biodoc_call(
                direction="OUT",
                method="GET",
                path="/card/integration/mainimage",
                fields={"idCard": card},
            )
        )
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

        if response.status_code == 401:
            logger.warning(
                format_biodoc_call(
                    direction="IN",
                    method="GET",
                    path="/card/integration/mainimage",
                    status=response.status_code,
                    fields={"idCard": card},
                    response_body=_unwrap_biodoc_payload(response.json())
                    if response.headers.get("content-type", "").startswith("application/json")
                    else response.text[:500],
                )
            )
            raise BiodocAPIUnauthorizedError("BIODOC_TOKEN_API recusado pela API BioDoc")

        if not response.is_success:
            logger.warning(
                format_biodoc_call(
                    direction="IN",
                    method="GET",
                    path="/card/integration/mainimage",
                    status=response.status_code,
                    fields={"idCard": card},
                    response_body=response.text[:500],
                )
            )
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
            logger.warning(
                format_biodoc_call(
                    direction="IN",
                    method="GET",
                    path="/card/integration/mainimage",
                    status=response.status_code,
                    fields={"idCard": card},
                    response_body=body,
                )
            )
            raise BiodocAPIUnavailableError(
                "Resposta da API BioDoc em formato inesperado (objeto vazio)"
            )

        status_raw = data.get("status", data.get("cardStatus"))
        image = data.get("image") or data.get("mainImage") or data.get("base64Image")

        logger.info(
            format_biodoc_call(
                direction="IN",
                method="GET",
                path="/card/integration/mainimage",
                status=response.status_code,
                fields={
                    "idCard": card,
                    "name": data.get("name"),
                    "status": status_raw,
                },
                response_body=data,
            )
        )

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
                format_biodoc_call(
                    direction="IN",
                    method="GET",
                    path="/logs/external-audits",
                    status=response.status_code,
                    fields={"idCard": id_card},
                    response_body=response.text[:500],
                )
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
                    response_body=body,
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
                response_body=body,
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
                format_biodoc_call(
                    direction="IN",
                    method="GET",
                    path=f"/integrations/log/{reference_id}",
                    status=response.status_code,
                    fields={"reference_Id": reference_id},
                    response_body=response.text[:2000],
                )
            )
            raise BiodocAPIUnauthorizedError("BIODOC_TOKEN_API recusado pela API BioDoc")

        if not response.is_success:
            logger.warning(
                format_biodoc_call(
                    direction="IN",
                    method="GET",
                    path=f"/integrations/log/{reference_id}",
                    status=response.status_code,
                    fields={"reference_Id": reference_id},
                    response_body=response.text[:2000],
                )
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
        local_name, local_source = _resolve_local_name_from_log(data)
        required_name_raw = data.get("requiredName") or data.get("reguiredName")
        logger.info(
            format_biodoc_call(
                direction="IN",
                method="GET",
                path=f"/integrations/log/{reference_id}",
                status=response.status_code,
                fields=_integration_log_log_fields(
                    reference_id=reference_id,
                    data=data,
                    operador=operador,
                ),
                response_body=data,
            )
        )

        return IntegrationLogData(
            id=data.get("id"),  # type: ignore[arg-type]
            id_card=id_card,
            name=str(data.get("name") or data.get("userName") or ""),
            status=int(data.get("status") or 0),
            main_image=data.get("mainImage") or None,  # type: ignore[arg-type]
            path=data.get("path") or None,  # type: ignore[arg-type]
            required_name=required_name_raw or None,  # type: ignore[arg-type]
            operador=operador,
            local_name=local_name,
            local_source=local_source,
        )
