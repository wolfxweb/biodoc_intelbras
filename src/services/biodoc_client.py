"""Cliente async para a API BioDoc — consulta de beneficiários."""

from __future__ import annotations

from dataclasses import dataclass

import httpx

from src.core.logging import logger


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

        data = body.get("data", {})
        if not isinstance(data, dict):
            raise BiodocAPIUnavailableError(
                "Resposta da API BioDoc sem campo 'data' válido"
            )

        return CardMainImageData(
            name=str(data.get("name") or ""),
            card=str(data.get("card") or card),
            status=bool(data.get("status", False)),
            image=data.get("image") or None,
        )

    async def get_integration_log(self, reference_id: str) -> IntegrationLogData:
        """
        GET /integrations/log/{reference_id}

        Endpoint de webhook: retorna os dados do log de integracao pelo LogID
        recebido no webhook BioDoc. Inclui o `reguiredName` que e mapeado para
        `accessGroupId` no Defense IA via DEFENSE_IA_ACCESS_GROUP_MAP.

        Levanta BiodocAPIError em caso de falha.
        """
        if self._client is None:
            raise BiodocAPIUnavailableError("BiodocClient não iniciado")

        logger.debug("[BIODOC OUT] GET /integrations/log/%s", reference_id)
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

        logger.debug(
            "[BIODOC IN] GET /integrations/log/%s status=%d",
            reference_id,
            response.status_code,
        )

        if response.status_code == 401:
            raise BiodocAPIUnauthorizedError("BIODOC_TOKEN_API recusado pela API BioDoc")

        if not response.is_success:
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

        if not isinstance(body, dict):
            raise BiodocAPIUnavailableError(
                "Resposta da API BioDoc em formato inesperado (esperado objeto JSON)"
            )

        id_card = str(body.get("id_Card") or "")
        if not id_card:
            raise BiodocAPIUnavailableError(
                f"Resposta da API BioDoc sem campo 'id_Card' (reference_id={reference_id})"
            )

        return IntegrationLogData(
            id=body.get("id"),
            id_card=id_card,
            name=str(body.get("name") or ""),
            status=int(body.get("status") or 0),
            main_image=body.get("mainImage") or None,
            path=body.get("path") or None,
            required_name=body.get("reguiredName") or None,
        )
