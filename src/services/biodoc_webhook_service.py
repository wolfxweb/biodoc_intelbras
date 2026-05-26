"""
Orquestrador do fluxo de webhook BioDoc → Defense IA.

Recebe o payload do evento BioDoc, enriquece os dados via API BioDoc,
constrói o SyncRequest e chama o cliente Intelbras (cadastro como ACS person).
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
    IntegrationLogData,
)
from src.services.biodoc_image import ImageDownloadError, download_image_as_base64
from src.services.defense_ia_client import (
    DefenseIAArgumentError,
    DefenseIAClient,
    DefenseIAError,
    DefenseIANotReadyError,
    DefenseIAUnavailableError,
    defense_error_detail_public,
)


async def process_biodoc_webhook(
    payload: BiodocWebhookPayload,
    biodoc_client: BiodocClient,
    defense_client: DefenseIAClient,
) -> dict:
    """
    Fluxo completo:
    1. Valida sucesso e presença do reference_Id
    2. Consulta API BioDoc /integrations/log/{reference_Id} para obter id_Card, nome, imagem e status
    3. Verifica se beneficiário está ativo (status 1 ou 2)
    4. Baixa imagem da URL → base64
    5. Resolve `reguiredName` -> `orgCode` no Defense IA (sub-organização)
    6. Faz upsert no Defense IA usando id_Card como external_id
    7. Retorna resposta
    """
    ref_id = payload.reference_Id or "?"

    if not payload.success:
        logger.warning(
            "[WEBHOOK] ref=%s cadastro BioDoc mal-sucedido (success=false), ignorando",
            ref_id,
        )
        raise HTTPException(
            status_code=status.HTTP_422_UNPROCESSABLE_CONTENT,
            detail="Cadastro BioDoc não concluído com sucesso (success=false)",
        )

    if not payload.reference_Id:
        logger.warning("[WEBHOOK] reference_Id ausente no payload")
        raise HTTPException(
            status_code=status.HTTP_422_UNPROCESSABLE_CONTENT,
            detail="Campo 'reference_Id' ausente no payload do webhook",
        )

    # Consulta API BioDoc pelo reference_Id para obter dados completos da interação
    try:
        log_data: IntegrationLogData = await biodoc_client.get_integration_log(
            payload.reference_Id
        )
    except BiodocAPIUnauthorizedError as exc:
        logger.error("[WEBHOOK] ref=%s BIODOC_TOKEN_API inválido: %s", ref_id, exc)
        raise HTTPException(
            status_code=status.HTTP_502_BAD_GATEWAY,
            detail="Credencial da API BioDoc inválida no servidor",
        ) from exc
    except BiodocAPIUnavailableError as exc:
        logger.error("[WEBHOOK] ref=%s API BioDoc indisponível: %s", ref_id, exc)
        raise HTTPException(
            status_code=status.HTTP_502_BAD_GATEWAY,
            detail="API BioDoc indisponível — tente novamente",
        ) from exc

    if log_data.status not in (1, 2):
        logger.warning(
            "[WEBHOOK] ref=%s beneficiário id_Card=%s inativo no BioDoc (status=%d), ignorando",
            ref_id,
            log_data.id_card,
            log_data.status,
        )
        raise HTTPException(
            status_code=status.HTTP_422_UNPROCESSABLE_CONTENT,
            detail=f"Beneficiário inativo no BioDoc (status={log_data.status})",
        )

    # Prioriza mainImage; fallback para path e depois URL do webhook
    image_url = log_data.main_image or log_data.path or payload.url
    if not image_url:
        logger.warning("[WEBHOOK] ref=%s sem URL de imagem disponível", ref_id)
        raise HTTPException(
            status_code=status.HTTP_422_UNPROCESSABLE_CONTENT,
            detail="Sem imagem disponível para o beneficiário — não é possível sincronizar sem foto",
        )

    try:
        face_b64 = await download_image_as_base64(image_url)
    except ImageDownloadError as exc:
        logger.error("[WEBHOOK] ref=%s falha ao baixar imagem %s: %s", ref_id, image_url, exc)
        raise HTTPException(
            status_code=status.HTTP_422_UNPROCESSABLE_CONTENT,
            detail=f"Falha ao baixar imagem do beneficiário: {exc}",
        ) from exc

    card = log_data.id_card
    name = log_data.name or card

    sync_request = SyncRequest(
        source="biodoc",
        operation="upsert",
        external_id=card,
        person=PersonData(full_name=name, document=card),
        biometrics=BiometricData(face_image_base64=face_b64),
    )

    if defense_client.settings.enabled and not defense_client.is_ready:
        raise HTTPException(
            status_code=status.HTTP_503_SERVICE_UNAVAILABLE,
            detail="Defense IA não conectado",
        )

    # Mapeia o nome amigável vindo do BioDoc (ex.: "Corb") para o `orgCode`
    # da sub-organização equivalente no Defense IA (ex.: "001015001"). Se o
    # nome não for encontrado, usamos o orgCode raiz configurado no .env
    # (DEFENSE_IA_ORG_CODE) — assim a pessoa entra ao menos no nó pai.
    resolved_org_code: str | None = None
    if log_data.required_name:
        try:
            resolved_org_code = await defense_client.resolve_org_code(log_data.required_name)
        except DefenseIAError as exc:
            logger.warning(
                "[WEBHOOK] ref=%s falha ao resolver orgCode para %r: %s — usando default",
                ref_id,
                log_data.required_name,
                exc,
            )
    if not resolved_org_code:
        resolved_org_code = defense_client.settings.org_code or "001"
        if log_data.required_name:
            logger.warning(
                "[WEBHOOK] ref=%s reguiredName=%r sem sub-org correspondente, usando %s",
                ref_id,
                log_data.required_name,
                resolved_org_code,
            )
    logger.debug(
        "[WEBHOOK] ref=%s id_Card=%s empresa=%r org_code=%s",
        ref_id,
        card,
        log_data.required_name,
        resolved_org_code,
    )

    try:
        await defense_client.sync_person(sync_request, resolved_org_code)
    except DefenseIAArgumentError as exc:
        logger.warning(
            "[WEBHOOK] ref=%s imagem inválida para id_Card=%s: %s",
            ref_id,
            card,
            exc,
        )
        raise HTTPException(
            status_code=status.HTTP_422_UNPROCESSABLE_CONTENT,
            detail="Biometria inválida: imagem facial não reconhecida pelo Defense IA",
        ) from exc
    except DefenseIANotReadyError as exc:
        raise HTTPException(
            status_code=status.HTTP_503_SERVICE_UNAVAILABLE,
            detail=str(exc),
        ) from exc
    except (DefenseIAUnavailableError, DefenseIAError) as exc:
        logger.exception(
            "[WEBHOOK] ref=%s falha ao sincronizar id_Card=%s: %s",
            ref_id,
            card,
            exc,
        )
        raise HTTPException(
            status_code=status.HTTP_502_BAD_GATEWAY,
            detail=defense_error_detail_public(exc),
        ) from exc

    logger.info(
        "[WEBHOOK] ref=%s id_Card=%s name=%s cadastrado no Defense IA (ACS person)",
        ref_id,
        card,
        name,
    )
    return {
        "status": "success",
        "external_id": card,
        "defense_sync": "ok",
    }
