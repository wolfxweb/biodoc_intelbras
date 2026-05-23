"""
Orquestrador do fluxo de webhook BioDoc → Defense IA.

Recebe o payload do evento BioDoc, enriquece os dados via API BioDoc,
constrói o SyncRequest e chama o cliente Intelbras.
"""

from __future__ import annotations

from fastapi import HTTPException, status

from src.api.schemas import BiometricData, PersonData, SyncRequest
from src.api.schemas_biodoc import BiodocWebhookPayload
from src.core.logging import logger
from src.services.biodoc_client import (
    BiodocAPIUnavailableError,
    BiodocAPIUnauthorizedError,
    BiodocClient,
)
from src.services.biodoc_image import ImageDownloadError, download_image_as_base64
from src.services.defense_ia_client import DefenseIAClient
from src.services.defense_sync import sync_to_defense


async def process_biodoc_webhook(
    payload: BiodocWebhookPayload,
    biodoc_client: BiodocClient,
    defense_client: DefenseIAClient,
) -> dict:
    """
    Fluxo completo:
    1. Valida sucesso e presença do card
    2. Consulta API BioDoc para obter nome e imagem
    3. Baixa imagem da URL → base64
    4. Faz upsert no Defense IA
    5. Retorna resposta
    """
    log_id = payload.LogID or "?"

    if not payload.success:
        logger.warning(
            "[WEBHOOK] LogID=%s cadastro BioDoc mal-sucedido (success=false), ignorando",
            log_id,
        )
        raise HTTPException(
            status_code=status.HTTP_422_UNPROCESSABLE_CONTENT,
            detail="Cadastro BioDoc não concluído com sucesso (success=false)",
        )

    card = payload.card
    if not card:
        logger.warning("[WEBHOOK] LogID=%s card ausente no payload", log_id)
        raise HTTPException(
            status_code=status.HTTP_422_UNPROCESSABLE_CONTENT,
            detail="Campo 'card' ausente no payload do webhook",
        )

    # Consulta API BioDoc para obter nome/imagem/status do beneficiário
    try:
        biodoc_data = await biodoc_client.get_card_mainimage(card)
    except BiodocAPIUnauthorizedError as exc:
        logger.error("[WEBHOOK] LogID=%s BIODOC_TOKEN_API inválido: %s", log_id, exc)
        raise HTTPException(
            status_code=status.HTTP_502_BAD_GATEWAY,
            detail="Credencial da API BioDoc inválida no servidor",
        ) from exc
    except BiodocAPIUnavailableError as exc:
        logger.error("[WEBHOOK] LogID=%s API BioDoc indisponível: %s", log_id, exc)
        raise HTTPException(
            status_code=status.HTTP_502_BAD_GATEWAY,
            detail="API BioDoc indisponível — tente novamente",
        ) from exc

    if not biodoc_data.status:
        logger.warning(
            "[WEBHOOK] LogID=%s beneficiário card=%s inativo no BioDoc, ignorando",
            log_id,
            card,
        )
        raise HTTPException(
            status_code=status.HTTP_422_UNPROCESSABLE_CONTENT,
            detail="Beneficiário inativo no BioDoc (status=false)",
        )

    # Prioriza a URL da API; fallback para URL do webhook
    image_url = biodoc_data.image or payload.image
    if not image_url:
        logger.warning("[WEBHOOK] LogID=%s sem URL de imagem disponível", log_id)
        raise HTTPException(
            status_code=status.HTTP_422_UNPROCESSABLE_CONTENT,
            detail="Sem imagem disponível para o beneficiário — não é possível sincronizar sem foto",
        )

    try:
        face_b64 = await download_image_as_base64(image_url)
    except ImageDownloadError as exc:
        logger.error("[WEBHOOK] LogID=%s falha ao baixar imagem %s: %s", log_id, image_url, exc)
        raise HTTPException(
            status_code=status.HTTP_422_UNPROCESSABLE_CONTENT,
            detail=f"Falha ao baixar imagem do beneficiário: {exc}",
        ) from exc

    name = biodoc_data.name or card

    sync_request = SyncRequest(
        source="biodoc",
        operation="upsert",
        external_id=card,
        person=PersonData(full_name=name, document=card),
        biometrics=BiometricData(face_image_base64=face_b64),
    )

    logger.debug(
        "[WEBHOOK] LogID=%s card=%s name=%s → Defense IA upsert",
        log_id,
        card,
        name,
    )

    await sync_to_defense(
        sync_request,
        defense_client,
        log_context=f"[WEBHOOK] LogID={log_id}",
    )

    logger.info(
        "[WEBHOOK] LogID=%s card=%s name=%s sincronizado com sucesso no Defense IA",
        log_id,
        card,
        name,
    )
    return {
        "status": "success",
        "external_id": card,
        "defense_sync": "ok",
    }
