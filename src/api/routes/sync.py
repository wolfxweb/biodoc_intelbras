from typing import Annotated

from fastapi import APIRouter, Depends

from src.api.dependencies import get_defense_client, require_admin_token
from src.api.schemas import ManualSyncRequest, SyncResponse
from src.core.logging import logger
from src.services.defense_ia_client import DefenseIAClient
from src.services.defense_sync import sync_to_defense

router = APIRouter(
    prefix="/v1/person",
    tags=["Intelbras Defense — cadastro direto"],
    dependencies=[Depends(require_admin_token)],
)


@router.post(
    "/sync",
    response_model=SyncResponse,
    summary="Cadastro direto no Intelbras Defense",
    description=(
        "**Cadastro direto no Intelbras Defense** — envie nome, documento, foto e o bloco "
        "`defense` (modo, sub-org e portas).\n\n"
        "**Bloco `defense` (obrigatório):**\n"
        "- `sync_target`: `visitor` (nova visita) ou `person` (upsert ACS)\n"
        "- `org_code`: sub-organização no Defense (ex.: `001021`)\n"
        "- `acs_channel_ids` (opcional, só com `visitor`): portas no Defense "
        "(ex.: `1000049$7$0$0`)\n"
        "  - **Não informar** → middleware busca portas pelo `org_code`; "
        "se não achar, usa permissão padrão\n"
        "  - **`[]`** → permissão padrão do visitante (Configurações → Visitante), "
        "sem busca automática\n"
        "  - **Lista preenchida** → usa exatamente essas portas\n\n"
        "**Autenticação:** `Authorization: Bearer <ADMIN_API_TOKEN>`\n\n"
        "**Quando usar:** integração direta (ERP, script, homologação), reenvio ou "
        "correção manual.\n\n"
        "Com `biometrics.face_image_base64`, a foto facial é enviada ao Defense."
    ),
)
async def sync_person(
    payload: ManualSyncRequest,
    defense_client: Annotated[DefenseIAClient, Depends(get_defense_client)],
) -> SyncResponse:
    face_info = "none"
    if payload.biometrics and payload.biometrics.face_image_base64:
        face_info = f"<IMG:{len(payload.biometrics.face_image_base64)}_chars>"
    logger.debug(
        "[API IN] source=%s operation=%s external_id=%s sync_target=%s org_code=%s "
        "person=%s biometrics.face=%s",
        payload.source,
        payload.operation,
        payload.external_id,
        payload.defense.sync_target,
        payload.defense.org_code,
        payload.person.model_dump(),
        face_info,
    )

    sync_result = await sync_to_defense(
        payload,
        defense_client,
        sync_target=payload.defense.sync_target,
        org_code=payload.defense.org_code,
        acs_channel_ids=payload.defense.acs_channel_ids,
        log_context=f"source={payload.source} external_id={payload.external_id}",
    )

    logger.info(
        "Defense IA sync succeeded source=%s external_id=%s visitor_id=%s person_id=%s",
        payload.source,
        payload.external_id,
        sync_result.get("visitor_id"),
        sync_result.get("person_id"),
    )
    if payload.defense.sync_target == "visitor":
        message = "Visitante registrado no Intelbras Defense com sucesso"
    else:
        message = "Usuário enviado ao Intelbras Defense com sucesso"
    return SyncResponse(
        status="success",
        message=message,
        visitor_id=sync_result.get("visitor_id"),
        person_id=sync_result.get("person_id"),
    )
