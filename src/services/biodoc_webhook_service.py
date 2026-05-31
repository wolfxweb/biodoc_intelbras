"""
Orquestrador do fluxo de webhook BioDoc → Defense IA.

Recebe o payload do evento BioDoc, enriquece os dados via API BioDoc,
constrói o SyncRequest e chama o cliente Intelbras (cadastro como ACS person).
"""

from __future__ import annotations

import asyncio
import json
from datetime import datetime, timedelta, timezone

from fastapi import HTTPException, status

from src.api.schemas import BiometricData, PersonData, SyncRequest
from src.api.schemas_biodoc import BiodocWebhookPayload
from src.core.logging import logger
from src.core.webhook_log import format_flow_step, truncate_text
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
EXTERNAL_AUDITS_RETRY_SECONDS = 2.0
EXTERNAL_AUDITS_RETRY_ATTEMPTS = 3
# BioDoc sandbox: initialDate=endDate=hoje costuma retornar vazio; incluir dia anterior.
AUDIT_FALLBACK_LOOKBACK_DAYS = 7


def _operador_from_payload(payload: BiodocWebhookPayload) -> str | None:
    """Extrai operador/grupo do POST (campo operador ou details da URL verify)."""
    if payload.operador and payload.operador.strip():
        return payload.operador.strip()
    details = payload.details
    if details is None:
        return None
    parsed: dict[str, object] | None = None
    if isinstance(details, dict):
        parsed = details
    elif isinstance(details, str) and details.strip():
        try:
            loaded = json.loads(details)
            if isinstance(loaded, dict):
                parsed = loaded
        except json.JSONDecodeError:
            return None
    if not parsed:
        return None
    for key in ("operador", "operator", "grupo"):
        raw = parsed.get(key)
        if raw is not None and str(raw).strip():
            return str(raw).strip()
    return None


def webhook_event_succeeded(payload: BiodocWebhookPayload) -> bool:
    """True somente quando BioDoc indica verify/cadastro bem-sucedido."""
    response_ok = payload.response is None or (200 <= payload.response < 300)
    return bool(payload.success and response_ok)


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


def _audit_anchor_date(event_date: str | None) -> datetime.date:
    anchor = _parse_event_datetime(event_date) or datetime.now(timezone.utc)
    return anchor.date()


def _audit_date_ranges_to_try(event_date: str | None) -> list[tuple[str, str]]:
    """
    Janelas para GET /logs/external-audits.

    A API BioDoc (sandbox) frequentemente retorna lista vazia quando
    initialDate == endDate == hoje, mas encontra o log ao incluir o dia anterior.
    """
    day = _audit_anchor_date(event_date)
    yesterday = day - timedelta(days=AUDIT_FALLBACK_LOOKBACK_DAYS)
    return [
        (day.isoformat(), day.isoformat()),
        (yesterday.isoformat(), day.isoformat()),
    ]


def _audit_entry_timestamp(entry: ExternalAuditEntry) -> datetime | None:
    return _parse_event_datetime(entry.date)


def _audit_status_rank(entry: ExternalAuditEntry) -> int:
    """Prioriza status 7 (autenticado), como no fluxo de homologação de 30/05."""
    raw = (entry.status or "").strip()
    if raw == "7":
        return 0
    if raw.lower() in ("autenticado", "authenticated", "success", "ok"):
        return 1
    try:
        code = int(raw)
        if 200 <= code < 300:
            return 1
    except ValueError:
        pass
    return 2


def _pick_best_audit_entry(
    entries: list[ExternalAuditEntry],
    event_date: str | None,
) -> ExternalAuditEntry | None:
    if not entries:
        return None

    ranked = sorted(entries, key=_audit_status_rank)
    success_like = [e for e in ranked if _audit_status_rank(e) <= 1]
    pool = success_like if success_like else entries

    event_dt = _parse_event_datetime(event_date)
    if event_dt is None:
        sorted_entries = sorted(
            pool,
            key=lambda e: _audit_entry_timestamp(e) or datetime.min.replace(tzinfo=timezone.utc),
            reverse=True,
        )
        return sorted_entries[0] if sorted_entries else None

    def distance(entry: ExternalAuditEntry) -> float:
        entry_dt = _audit_entry_timestamp(entry)
        if entry_dt is None:
            return float("inf")
        return abs((entry_dt - event_dt).total_seconds())

    return min(pool, key=distance)


def _log_has_group_hints(log_data: IntegrationLogData) -> bool:
    return bool(log_data.local_token or log_data.required_name)


async def _resolve_log_via_external_audits(
    card: str,
    event_date: str | None,
    biodoc_client: BiodocClient,
) -> IntegrationLogData | None:
    """
    Descobre o log da verify via GET /logs/external-audits + GET /integrations/log/{id}.

    Falhas são silenciosas (retorna None) para não quebrar o webhook.
    """
    date_ranges = _audit_date_ranges_to_try(event_date)
    try:
        entries: list[ExternalAuditEntry] = []
        for range_index, (initial_date, end_date) in enumerate(date_ranges):
            for attempt in range(EXTERNAL_AUDITS_RETRY_ATTEMPTS):
                entries = await biodoc_client.get_external_audits(
                    card,
                    initial_date=initial_date,
                    end_date=end_date,
                )
                if entries:
                    if range_index > 0 or attempt > 0:
                        logger.info(
                            "[WEBHOOK] external-audits encontrou entradas "
                            "(janela=%s..%s tentativa=%d)",
                            initial_date,
                            end_date,
                            attempt + 1,
                        )
                    break
                if attempt + 1 < EXTERNAL_AUDITS_RETRY_ATTEMPTS:
                    logger.info(
                        "[WEBHOOK] external-audits vazio (janela=%s..%s) — "
                        "nova tentativa em %.0fs",
                        initial_date,
                        end_date,
                        EXTERNAL_AUDITS_RETRY_SECONDS,
                    )
                    await asyncio.sleep(EXTERNAL_AUDITS_RETRY_SECONDS)
            if entries:
                break
            if range_index + 1 < len(date_ranges):
                logger.info(
                    "[WEBHOOK] external-audits sem entradas em %s..%s — "
                    "ampliando janela para %s..%s",
                    initial_date,
                    end_date,
                    date_ranges[range_index + 1][0],
                    date_ranges[range_index + 1][1],
                )
        initial_date, end_date = date_ranges[-1]
    except (BiodocAPIUnauthorizedError, BiodocAPIUnavailableError) as exc:
        logger.warning(
            "[WEBHOOK] card=%s external-audits indisponível (%s) — local_token via fallback",
            card,
            exc,
        )
        return None

    if not isinstance(entries, list):
        entries = []

    best = _pick_best_audit_entry(entries, event_date)
    if best is None or best.id is None:
        tried = ", ".join(f"{a}..{b}" for a, b in date_ranges)
        logger.info(
            format_flow_step(
                "external-audits sem entradas",
                card=card,
                janelas=tried,
                event_date=event_date,
            )
        )
        return None

    ref_id = str(best.id)
    logger.info(
        format_flow_step(
            "external-audits escolheu log",
            card=card,
            audit_id=ref_id,
            audit_date=best.date,
            audit_status=best.status,
        )
    )
    try:
        log_data = await biodoc_client.get_integration_log(ref_id)
    except (BiodocAPIUnauthorizedError, BiodocAPIUnavailableError) as exc:
        logger.warning(
            "[WEBHOOK] card=%s GET /integrations/log/%s falhou (%s) — local_token via fallback",
            card,
            ref_id,
            exc,
        )
        return None

    if not _log_has_group_hints(log_data):
        logger.warning(
            "[WEBHOOK] card=%s log id=%s sem local_token/requiredName",
            card,
            ref_id,
        )
        return None

    logger.info(
        format_flow_step(
            "local_token resolvido (external-audits → integrations/log)",
            card=card,
            audit_id=ref_id,
            local_token=log_data.local_token,
            required_name=log_data.required_name,
        )
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
    local_token: str | None = None,
    required_name: str | None = None,
) -> tuple[str, list[str], str | None]:
    """Resolve orgCode tentando local_token → requiredName → default."""
    candidates = _unique_group_candidates(local_token, required_name)
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
            format_flow_step(
                "orgCode resolvido",
                ref=ref_label,
                candidatos=candidates,
                orgCode=resolved_org_code,
                fonte=matched_source,
            )
        )
    elif candidates:
        resolved_org_code = defense_client.settings.org_code or "001"
        logger.warning(
            format_flow_step(
                "orgCode fallback (grupo não encontrado no Defense)",
                ref=ref_label,
                candidatos=candidates,
                orgCode=resolved_org_code,
            )
        )
    else:
        resolved_org_code = defense_client.settings.org_code or "001"
        logger.warning(
            format_flow_step(
                "orgCode fallback (sem local_token/requiredName)",
                ref=ref_label,
                orgCode=resolved_org_code,
            )
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
    # Falhas (success=false ou response 403/4xx) não vão ao Defense — só log + ignored.
    if not webhook_event_succeeded(payload):
        label = payload.reference_Id or payload.logId or payload.card or "?"
        logger.warning(
            format_flow_step(
                "verify falhou — Defense NÃO acionado",
                ref=label,
                card=payload.card,
                success=payload.success,
                response=payload.response,
                code=payload.code,
                logId=payload.logId,
                defense_sync="skipped",
            )
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

    logger.info(
        "[WEBHOOK] ref=%s log resumo id_Card=%s name=%s localToken=%r requiredName=%r status=%d",
        effective_ref_id,
        log_data.id_card,
        log_data.name,
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

    if log_data.local_token:
        group_required_name = log_data.required_name
    else:
        group_required_name = (
            log_data.required_name
            or log_data.operador
            or _operador_from_payload(payload)
        )

    return await _sync_to_defense(
        ref_label=effective_ref_id,
        card=card,
        name=name,
        face_b64=face_b64,
        required_name=group_required_name,
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

    local_token: str | None = None
    required_name: str | None = None
    if not _effective_reference_id(payload):
        audit_log = await _resolve_log_via_external_audits(
            card,
            payload.date,
            biodoc_client,
        )
        if audit_log:
            local_token = audit_log.local_token
            if local_token:
                required_name = audit_log.required_name
            else:
                required_name = audit_log.required_name or _operador_from_payload(payload)
        else:
            required_name = _operador_from_payload(payload)
            logger.warning(
                "[WEBHOOK] card=%s sem reference_Id/logId/id_Log e external-audits "
                "não resolveu local_token — orgCode via operador/requiredName ou fallback",
                card,
            )

    logger.info(
        "[WEBHOOK] card:%s resumo name=%s localToken=%r (image do payload, nome via mainimage)",
        card,
        name,
        local_token,
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
        format_flow_step(
            "sync concluído no Defense IA",
            ref=ref_label,
            id_Card=card,
            name=name,
            orgCode=resolved_org_code,
        )
    )
    payload: dict = {
        "status": "success",
        "external_id": card,
        "defense_sync": "ok",
        "orgCode": resolved_org_code,
        "name": name,
    }
    if local_token:
        payload["local_name"] = local_token
    elif required_name:
        payload["local_name"] = required_name
    return payload


async def _face_base64_from_image_field(image: str) -> str:
    value = image.strip()
    if value.lower().startswith(("http://", "https://")):
        return await download_image_as_base64(value)
    return value


def _integration_log_image_url(log_data: IntegrationLogData | None) -> str | None:
    if log_data is None:
        return None
    return log_data.main_image or log_data.path


def _pick_face_image_url(
    *,
    audit_log: IntegrationLogData | None,
    card_image: str | None,
    image_url_hint: str | None,
) -> tuple[str | None, str | None]:
    """Prioriza captura da verify (integrations/log) → payload → mainimage cadastrada."""
    audit_url = _integration_log_image_url(audit_log)
    if audit_url:
        return audit_url, "integrations/log"
    if image_url_hint:
        return image_url_hint, "payload.image"
    if card_image:
        return card_image, "mainimage"
    return None, None


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
    Fluxo via cartão: external-audits → integrations/log (imagem da verify) + mainimage.

    `image_url_hint` aceita URL do payload do webhook quando integrations/log não tiver foto.
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

    local_token: str | None = None
    effective_required_name = required_name
    audit_log = await _resolve_log_via_external_audits(card, event_date, biodoc_client)
    if audit_log:
        local_token = audit_log.local_token
        if effective_required_name is None:
            effective_required_name = audit_log.required_name
    elif effective_required_name is None:
        logger.warning(
            "[WEBHOOK] %s sem reference_Id/logId/id_Log e external-audits "
            "não resolveu local_token — orgCode via operador/requiredName ou fallback",
            ref_label,
        )

    effective_image, image_source = _pick_face_image_url(
        audit_log=audit_log,
        card_image=card_data.image,
        image_url_hint=image_url_hint,
    )
    if not effective_image:
        logger.warning(
            "[WEBHOOK] %s sem imagem (integrations/log, mainimage e hint ausentes)",
            ref_label,
        )
        raise HTTPException(
            status_code=status.HTTP_422_UNPROCESSABLE_CONTENT,
            detail="Sem imagem disponível para o beneficiário — não é possível sincronizar sem foto",
        )

    logger.info(
        format_flow_step(
            "imagem resolvida",
            ref=ref_label,
            fonte=image_source,
            url=truncate_text(effective_image, max_len=120),
        )
    )

    try:
        face_b64 = await _face_base64_from_image_field(effective_image)
    except ImageDownloadError as exc:
        logger.error(
            "[WEBHOOK] %s falha ao obter imagem (%s) %s: %s",
            ref_label,
            image_source,
            effective_image[:80],
            exc,
        )
        raise HTTPException(
            status_code=status.HTTP_422_UNPROCESSABLE_CONTENT,
            detail=f"Falha ao baixar imagem do beneficiário: {exc}",
        ) from exc

    name = (audit_log.name if audit_log and audit_log.name else None) or card_data.name or card

    return await _sync_to_defense(
        ref_label=ref_label,
        card=card_data.card or card,
        name=name,
        face_b64=face_b64,
        required_name=effective_required_name,
        local_token=local_token,
        defense_client=defense_client,
    )

