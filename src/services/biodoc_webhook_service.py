"""
Orquestrador do fluxo de webhook BioDoc → Defense IA.

Recebe o payload do evento BioDoc, enriquece os dados via API BioDoc,
constrói o SyncRequest e chama o cliente Intelbras (cadastro como ACS person).
"""

from __future__ import annotations

import json
from datetime import datetime, timedelta, timezone

from fastapi import HTTPException, status

from src.api.schemas import BiometricData, PersonData, SyncRequest
from src.api.schemas_biodoc import BiodocWebhookPayload
from src.core.logging import logger
from src.services.biodoc_client import (
    BiodocAPIUnavailableError,
    BiodocAPIUnauthorizedError,
    BiodocClient,
    CardMainImageData,
    ExternalAuditEntry,
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

AUDIT_LOOKUP_WINDOW_MINUTES = 15


def _parse_event_datetime(event_date: str | None) -> datetime | None:
    if not event_date or not event_date.strip():
        return None
    text = event_date.strip()
    for fmt in (
        "%d/%m/%Y %H:%M:%S",
        "%d/%m/%Y %H:%M",
    ):
        try:
            return datetime.strptime(text, fmt).replace(tzinfo=timezone.utc)
        except ValueError:
            continue
    if text.endswith("Z"):
        text = f"{text[:-1]}+00:00"
    try:
        parsed = datetime.fromisoformat(text)
    except ValueError:
        return None
    if parsed.tzinfo is None:
        return parsed.replace(tzinfo=timezone.utc)
    return parsed.astimezone(timezone.utc)


def _audit_date_range(event_date: str | None) -> tuple[str, str]:
    """Retorna (initialDate, endDate) em YYYY-MM-DD para external-audits."""
    anchor = _parse_event_datetime(event_date) or datetime.now(timezone.utc)
    window = timedelta(minutes=AUDIT_LOOKUP_WINDOW_MINUTES)
    start = anchor - window
    end = anchor + window
    return start.date().isoformat(), end.date().isoformat()


def _audit_entry_timestamp(entry: ExternalAuditEntry) -> datetime | None:
    return _parse_event_datetime(entry.date)


def _pick_best_audit_entry(
    entries: list[ExternalAuditEntry],
    event_date: str | None,
) -> ExternalAuditEntry | None:
    if not entries:
        return None

    event_dt = _parse_event_datetime(event_date)
    if event_dt is None:
        sorted_entries = sorted(
            entries,
            key=lambda e: _audit_entry_timestamp(e) or datetime.min.replace(tzinfo=timezone.utc),
            reverse=True,
        )
        return sorted_entries[0] if sorted_entries else None

    def distance(entry: ExternalAuditEntry) -> float:
        entry_dt = _audit_entry_timestamp(entry)
        if entry_dt is None:
            return float("inf")
        return abs((entry_dt - event_dt).total_seconds())

    return min(entries, key=distance)


def _log_has_group_hints(log_data: IntegrationLogData) -> bool:
    return bool(log_data.operador or log_data.local_token or log_data.required_name)


async def _resolve_log_via_external_audits(
    card: str,
    event_date: str | None,
    biodoc_client: BiodocClient,
) -> IntegrationLogData | None:
    """
    Descobre o log da verify via GET /logs/external-audits + GET /integrations/log/{id}.

    Falhas são silenciosas (retorna None) para não quebrar o webhook.
    """
    initial_date, end_date = _audit_date_range(event_date)
    try:
        entries = await biodoc_client.get_external_audits(
            card,
            initial_date=initial_date,
            end_date=end_date,
        )
    except (BiodocAPIUnauthorizedError, BiodocAPIUnavailableError) as exc:
        logger.warning(
            "[WEBHOOK] card=%s external-audits indisponível (%s) — operador via fallback",
            card,
            exc,
        )
        return None

    if not isinstance(entries, list):
        entries = []

    best = _pick_best_audit_entry(entries, event_date)
    if best is None or best.id is None:
        logger.info(
            "[WEBHOOK] card=%s external-audits sem entradas (janela %s..%s)",
            card,
            initial_date,
            end_date,
        )
        return None

    ref_id = str(best.id)
    logger.info(
        "[WEBHOOK] card=%s external-audits escolheu id=%s date=%r",
        card,
        ref_id,
        best.date,
    )
    try:
        log_data = await biodoc_client.get_integration_log(ref_id)
    except (BiodocAPIUnauthorizedError, BiodocAPIUnavailableError) as exc:
        logger.warning(
            "[WEBHOOK] card=%s GET /integrations/log/%s falhou (%s) — operador via fallback",
            card,
            ref_id,
            exc,
        )
        return None

    if not _log_has_group_hints(log_data):
        logger.warning(
            "[WEBHOOK] card=%s log id=%s sem operador/local_token/requiredName",
            card,
            ref_id,
        )
        return None

    logger.info(
        "[WEBHOOK] card=%s operador via external-audits+log: operador=%r localToken=%r requiredName=%r",
        card,
        log_data.operador,
        log_data.local_token,
        log_data.required_name,
    )
    return log_data


def _effective_reference_id(payload: BiodocWebhookPayload) -> str | None:
    """Identificador para GET /integrations/log/{reference_Id}."""
    if payload.reference_Id and payload.reference_Id.strip():
        return payload.reference_Id.strip()
    if payload.logId and payload.logId.strip():
        return payload.logId.strip()
    if payload.id_Log is not None:
        return str(payload.id_Log)
    return None


def _extract_operador_from_details(payload: BiodocWebhookPayload) -> str | None:
    """Fallback: operador em `details` do POST (quando BioDoc repassa o parâmetro da verify)."""
    details = payload.details
    if details is None:
        return None

    if isinstance(details, dict):
        raw = details.get("operador") or details.get("operator") or details.get("grupo")
        if raw is None:
            return None
        value = str(raw).strip()
        return value or None

    if isinstance(details, str):
        text = details.strip()
        if not text:
            return None
        try:
            parsed = json.loads(text)
        except ValueError:
            return None
        if not isinstance(parsed, dict):
            return None
        raw = parsed.get("operador") or parsed.get("operator") or parsed.get("grupo")
        if raw is None:
            return None
        value = str(raw).strip()
        return value or None

    return None


def _unique_group_candidates(*values: str | None) -> list[str]:
    seen: set[str] = set()
    candidates: list[str] = []
    for raw in values:
        if raw is None:
            continue
        value = raw.strip()
        if not value:
            continue
        key = value.casefold()
        if key in seen:
            continue
        seen.add(key)
        candidates.append(value)
    return candidates


async def _resolve_group_org_code(
    *,
    ref_label: str,
    defense_client: DefenseIAClient,
    operador: str | None = None,
    local_token: str | None = None,
    required_name: str | None = None,
) -> tuple[str, list[str], str | None]:
    """Resolve orgCode tentando operador → local_token → requiredName → default."""
    candidates = _unique_group_candidates(operador, local_token, required_name)
    resolved_org_code: str | None = None
    matched_source: str | None = None

    for candidate in candidates:
        try:
            code = await defense_client.resolve_org_code(candidate)
        except DefenseIAError as exc:
            logger.warning(
                "[WEBHOOK] ref=%s falha ao resolver orgCode para %r: %s",
                ref_label,
                candidate,
                exc,
            )
            continue
        if code:
            resolved_org_code = code
            matched_source = candidate
            break

    if resolved_org_code and matched_source:
        logger.info(
            "[WEBHOOK] ref=%s grupo candidatos=%s orgCode=%s fonte=%r",
            ref_label,
            candidates,
            resolved_org_code,
            matched_source,
        )
    elif candidates:
        resolved_org_code = defense_client.settings.org_code or "001"
        logger.warning(
            "[WEBHOOK] ref=%s grupo candidatos=%s orgCode=%s fonte=fallback (grupo não encontrado no Defense)",
            ref_label,
            candidates,
            resolved_org_code,
        )
    else:
        resolved_org_code = defense_client.settings.org_code or "001"
        logger.warning(
            "[WEBHOOK] ref=%s grupo candidatos=[] orgCode=%s fonte=fallback (sem operador/local_token/requiredName)",
            ref_label,
            resolved_org_code,
        )

    return resolved_org_code, candidates, matched_source


async def process_biodoc_webhook(
    payload: BiodocWebhookPayload,
    biodoc_client: BiodocClient,
    defense_client: DefenseIAClient,
) -> dict:
    """
    Fluxo completo — suporta formato legado (reference_Id) e novo (card + logId):

    1. Rejeita success=false ou response != 2xx
    2. Tenta obter reference_id efetivo: reference_Id → logId → id_Log
    3a. Com reference_id → GET /integrations/log/{id} (dados completos)
    3b. Sem reference_id mas com card → GET /card/mainimage/{card}
    4. Baixa imagem → base64
    5. Resolve reguiredName → orgCode no Defense IA
    6. Upsert no Defense IA e retorna
    """
    # --- 1. Verificar sucesso ---
    # BioDoc exige sempre 200 OK; retornar 4xx/5xx faz o BioDoc parar de enviar.
    # Para eventos sem sucesso apenas logamos e retornamos status "ignored".
    response_ok = payload.response is None or (200 <= payload.response < 300)
    if not payload.success or not response_ok:
        label = payload.reference_Id or payload.logId or payload.card or "?"
        logger.warning(
            "[WEBHOOK] ref=%s verificação BioDoc sem sucesso (success=%s response=%s code=%s) — ignorando sem erro",
            label,
            payload.success,
            payload.response,
            payload.code,
        )
        return {
            "status": "ignored",
            "external_id": payload.card or label,
            "defense_sync": "skipped",
        }

    # --- 2. Detectar formato e rotear ---
    effective_ref_id = _effective_reference_id(payload)
    if effective_ref_id:
        return await _process_by_reference_id(
            effective_ref_id=effective_ref_id,
            payload=payload,
            biodoc_client=biodoc_client,
            defense_client=defense_client,
        )

    # card + image: foto do evento; nome/status enriquecidos via API BioDoc
    if payload.card and payload.image:
        logger.info(
            "[WEBHOOK] card=%s — image do payload + nome/status via API BioDoc (mainimage)",
            payload.card,
        )
        return await _process_card_with_payload_image(
            payload,
            biodoc_client,
            defense_client,
        )

    if payload.card:
        logger.info(
            "[WEBHOOK] sem reference_Id/logId/image — usando card=%s via API BioDoc",
            payload.card,
        )
        return await process_biodoc_webhook_by_card(
            payload.card,
            biodoc_client,
            defense_client,
            event_date=payload.date,
        )

    logger.warning("[WEBHOOK] payload sem reference_Id, logId ou card — descartando")
    raise HTTPException(
        status_code=status.HTTP_422_UNPROCESSABLE_CONTENT,
        detail="Payload sem identificador: informe 'reference_Id', 'logId' ou 'card'",
    )


async def _process_by_reference_id(
    *,
    effective_ref_id: str,
    payload: BiodocWebhookPayload,
    biodoc_client: BiodocClient,
    defense_client: DefenseIAClient,
) -> dict:
    """Fluxo oficial: consulta GET /integrations/log/{effective_ref_id}."""
    try:
        log_data: IntegrationLogData = await biodoc_client.get_integration_log(
            effective_ref_id
        )
    except BiodocAPIUnauthorizedError as exc:
        logger.error("[WEBHOOK] ref=%s BIODOC_TOKEN_API inválido: %s", effective_ref_id, exc)
        raise HTTPException(
            status_code=status.HTTP_502_BAD_GATEWAY,
            detail="Credencial da API BioDoc inválida no servidor",
        ) from exc
    except BiodocAPIUnavailableError as exc:
        logger.error("[WEBHOOK] ref=%s API BioDoc indisponível: %s", effective_ref_id, exc)
        raise HTTPException(
            status_code=status.HTTP_502_BAD_GATEWAY,
            detail="API BioDoc indisponível — tente novamente",
        ) from exc

    effective_operador = log_data.operador or _extract_operador_from_details(payload)
    if effective_operador and log_data.operador:
        logger.info(
            "[WEBHOOK] ref=%s operador identificado no log BioDoc: %s",
            effective_ref_id,
            effective_operador,
        )
    elif effective_operador:
        logger.info(
            "[WEBHOOK] ref=%s operador via details do POST (log sem detail): %s",
            effective_ref_id,
            effective_operador,
        )
    logger.info(
        "[WEBHOOK] ref=%s log resumo id_Card=%s name=%s operador=%s localToken=%r requiredName=%r status=%d",
        effective_ref_id,
        log_data.id_card,
        log_data.name,
        effective_operador,
        log_data.local_token,
        log_data.required_name,
        log_data.status,
    )

    if log_data.status not in (1, 2):
        logger.warning(
            "[WEBHOOK] ref=%s beneficiário id_Card=%s inativo no BioDoc (status=%d), ignorando",
            effective_ref_id,
            log_data.id_card,
            log_data.status,
        )
        raise HTTPException(
            status_code=status.HTTP_422_UNPROCESSABLE_CONTENT,
            detail=f"Beneficiário inativo no BioDoc (status={log_data.status})",
        )

    # Prioriza mainImage; fallback para path, depois image do payload (novo formato), depois url
    image_url = log_data.main_image or log_data.path or payload.image or payload.url
    if not image_url:
        logger.warning("[WEBHOOK] ref=%s sem URL de imagem disponível", effective_ref_id)
        raise HTTPException(
            status_code=status.HTTP_422_UNPROCESSABLE_CONTENT,
            detail="Sem imagem disponível para o beneficiário — não é possível sincronizar sem foto",
        )

    try:
        face_b64 = await download_image_as_base64(image_url)
    except ImageDownloadError as exc:
        logger.error(
            "[WEBHOOK] ref=%s falha ao baixar imagem %s: %s",
            effective_ref_id,
            image_url,
            exc,
        )
        raise HTTPException(
            status_code=status.HTTP_422_UNPROCESSABLE_CONTENT,
            detail=f"Falha ao baixar imagem do beneficiário: {exc}",
        ) from exc

    card = log_data.id_card
    name = log_data.name or card

    return await _sync_to_defense(
        ref_label=effective_ref_id,
        card=card,
        name=name,
        face_b64=face_b64,
        required_name=log_data.required_name,
        operador=effective_operador,
        local_token=log_data.local_token,
        defense_client=defense_client,
    )


async def _process_card_with_payload_image(
    payload: BiodocWebhookPayload,
    biodoc_client: BiodocClient,
    defense_client: DefenseIAClient,
) -> dict:
    """
    Formato BioDoc (card + image no payload): usa a foto capturada no evento
    e enriquece nome/status via GET /card/integration/mainimage.
    """
    card = payload.card  # type: ignore[assignment]
    image_url = payload.image  # type: ignore[assignment]
    ref_label = payload.logId or card

    try:
        card_data: CardMainImageData = await biodoc_client.get_card_mainimage(card)
    except BiodocAPIUnauthorizedError as exc:
        logger.error("[WEBHOOK] card:%s BIODOC_TOKEN_API inválido: %s", card, exc)
        raise HTTPException(
            status_code=status.HTTP_502_BAD_GATEWAY,
            detail="Credencial da API BioDoc inválida no servidor",
        ) from exc
    except BiodocAPIUnavailableError as exc:
        logger.error("[WEBHOOK] card:%s API BioDoc indisponível: %s", card, exc)
        raise HTTPException(
            status_code=status.HTTP_502_BAD_GATEWAY,
            detail="API BioDoc indisponível — tente novamente",
        ) from exc

    if not card_data.status:
        logger.warning(
            "[WEBHOOK] card:%s beneficiário inativo no BioDoc (mainimage), ignorando",
            card,
        )
        raise HTTPException(
            status_code=status.HTTP_422_UNPROCESSABLE_CONTENT,
            detail="Beneficiário inativo no BioDoc",
        )

    payload_name = payload.name.strip() if payload.name and payload.name.strip() else None
    name = payload_name or card_data.name or card

    operador: str | None = None
    local_token: str | None = None
    required_name: str | None = None
    if not _effective_reference_id(payload):
        audit_log = await _resolve_log_via_external_audits(
            card,
            payload.date,
            biodoc_client,
        )
        if audit_log:
            operador = audit_log.operador
            local_token = audit_log.local_token
            required_name = audit_log.required_name
        else:
            logger.warning(
                "[WEBHOOK] card=%s sem reference_Id/logId/id_Log e external-audits "
                "não resolveu operador — orgCode via fallback",
                card,
            )

    logger.info(
        "[WEBHOOK] card:%s resumo name=%s operador=%s (image do payload, nome via mainimage)",
        card,
        name,
        operador,
    )

    try:
        face_b64 = await download_image_as_base64(image_url)
    except ImageDownloadError as exc:
        logger.error(
            "[WEBHOOK] ref=%s falha ao baixar imagem do payload %s: %s",
            ref_label,
            image_url[:80],
            exc,
        )
        raise HTTPException(
            status_code=status.HTTP_422_UNPROCESSABLE_CONTENT,
            detail=f"Falha ao baixar imagem do beneficiário: {exc}",
        ) from exc

    return await _sync_to_defense(
        ref_label=ref_label,
        card=card,
        name=name,
        face_b64=face_b64,
        required_name=required_name,
        operador=operador,
        local_token=local_token,
        defense_client=defense_client,
    )


async def _sync_to_defense(
    *,
    ref_label: str,
    card: str,
    name: str,
    face_b64: str,
    required_name: str | None,
    operador: str | None,
    defense_client: DefenseIAClient,
    local_token: str | None = None,
) -> dict:
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

    resolved_org_code, _, _ = await _resolve_group_org_code(
        ref_label=ref_label,
        defense_client=defense_client,
        operador=operador,
        local_token=local_token,
        required_name=required_name,
    )

    try:
        await defense_client.sync_person(sync_request, resolved_org_code)
    except DefenseIAArgumentError as exc:
        logger.warning(
            "[WEBHOOK] ref=%s imagem inválida para id_Card=%s: %s",
            ref_label,
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
            ref_label,
            card,
            exc,
        )
        raise HTTPException(
            status_code=status.HTTP_502_BAD_GATEWAY,
            detail=defense_error_detail_public(exc),
        ) from exc

    logger.info(
        "[WEBHOOK] ref=%s id_Card=%s name=%s orgCode=%s cadastrado no Defense IA (ACS person)",
        ref_label,
        card,
        name,
        resolved_org_code,
    )
    return {
        "status": "success",
        "external_id": card,
        "defense_sync": "ok",
    }


async def _face_base64_from_image_field(image: str) -> str:
    value = image.strip()
    if value.lower().startswith(("http://", "https://")):
        return await download_image_as_base64(value)
    return value


async def process_biodoc_webhook_by_card(
    card: str,
    biodoc_client: BiodocClient,
    defense_client: DefenseIAClient,
    *,
    required_name: str | None = None,
    image_url_hint: str | None = None,
    event_date: str | None = None,
) -> dict:
    """
    Fluxo via cartão: consulta GET /card/mainimage/{card} para nome e imagem cadastrada.

    `image_url_hint` aceita a URL de imagem já presente no payload do webhook
    (campo `image` do novo formato), usada como fallback se mainimage não tiver foto.
    """
    ref_label = f"card:{card}"
    try:
        card_data: CardMainImageData = await biodoc_client.get_card_mainimage(card)
    except BiodocAPIUnauthorizedError as exc:
        logger.error("[WEBHOOK] %s BIODOC_TOKEN_API inválido: %s", ref_label, exc)
        raise HTTPException(
            status_code=status.HTTP_502_BAD_GATEWAY,
            detail="Credencial da API BioDoc inválida no servidor",
        ) from exc
    except BiodocAPIUnavailableError as exc:
        logger.error("[WEBHOOK] %s API BioDoc indisponível: %s", ref_label, exc)
        raise HTTPException(
            status_code=status.HTTP_502_BAD_GATEWAY,
            detail="API BioDoc indisponível — tente novamente",
        ) from exc

    if not card_data.status:
        logger.warning(
            "[WEBHOOK] %s beneficiário inativo no BioDoc (status=false), ignorando",
            ref_label,
        )
        raise HTTPException(
            status_code=status.HTTP_422_UNPROCESSABLE_CONTENT,
            detail="Beneficiário inativo no BioDoc",
        )

    # Prioriza imagem cadastrada (mainimage); fallback para a capturada no evento
    effective_image = card_data.image or image_url_hint
    if not effective_image:
        logger.warning("[WEBHOOK] %s sem imagem disponível (mainimage e hint ausentes)", ref_label)
        raise HTTPException(
            status_code=status.HTTP_422_UNPROCESSABLE_CONTENT,
            detail="Sem imagem disponível para o beneficiário — não é possível sincronizar sem foto",
        )

    try:
        face_b64 = await _face_base64_from_image_field(effective_image)
    except ImageDownloadError as exc:
        logger.error(
            "[WEBHOOK] %s falha ao obter imagem %s: %s",
            ref_label,
            effective_image[:80],
            exc,
        )
        raise HTTPException(
            status_code=status.HTTP_422_UNPROCESSABLE_CONTENT,
            detail=f"Falha ao baixar imagem do beneficiário: {exc}",
        ) from exc

    name = card_data.name or card

    operador: str | None = None
    local_token: str | None = None
    effective_required_name = required_name
    audit_log = await _resolve_log_via_external_audits(card, event_date, biodoc_client)
    if audit_log:
        operador = audit_log.operador
        local_token = audit_log.local_token
        if effective_required_name is None:
            effective_required_name = audit_log.required_name
    elif effective_required_name is None:
        logger.warning(
            "[WEBHOOK] %s sem reference_Id/logId/id_Log e external-audits "
            "não resolveu operador — orgCode via fallback",
            ref_label,
        )

    return await _sync_to_defense(
        ref_label=ref_label,
        card=card_data.card or card,
        name=name,
        face_b64=face_b64,
        required_name=effective_required_name,
        operador=operador,
        local_token=local_token,
        defense_client=defense_client,
    )


def parse_redirect_response_success(response: str | None) -> bool:
    if response is None or response.strip() == "":
        return True
    normalized = response.strip().lower()
    if normalized in ("200", "201", "204", "true", "1", "ok", "success"):
        return True
    if normalized in ("false", "0", "fail", "error"):
        return False
    try:
        return int(normalized) >= 200 and int(normalized) < 300
    except ValueError:
        return False
