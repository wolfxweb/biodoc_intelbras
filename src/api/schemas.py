import base64
from typing import Literal

from pydantic import BaseModel, Field, field_validator, model_validator

ALLOWED_SOURCES: list[str] = [
    "biodoc",
]


class PersonData(BaseModel):
    full_name: str = Field(..., min_length=1, description="Nome completo da pessoa no Defense IA")
    document: str = Field(..., min_length=1, description="Documento (CPF ou identificador)")


_JPEG_MAGIC = b"\xff\xd8\xff"
_PNG_MAGIC = b"\x89PNG"
_MIN_IMAGE_BYTES = 1024


class BiometricData(BaseModel):
    face_image_base64: str | None = Field(
        default=None,
        description="Foto facial em base64 (JPEG/PNG). Opcional; envia ou atualiza a face no Defense",
    )

    @field_validator("face_image_base64")
    @classmethod
    def validate_face_image(cls, v: str | None) -> str | None:
        if v is None:
            return None
        v = v.strip().replace("\n", "").replace("\r", "").replace(" ", "")
        if not v:
            return None
        try:
            raw = base64.b64decode(v, validate=False)
        except Exception:
            raise ValueError("face_image_base64 não é um base64 válido")
        if len(raw) < _MIN_IMAGE_BYTES:
            raise ValueError(
                f"face_image_base64 muito pequeno ({len(raw)} bytes); "
                f"mínimo esperado: {_MIN_IMAGE_BYTES} bytes"
            )
        if not (raw.startswith(_JPEG_MAGIC) or raw.startswith(_PNG_MAGIC)):
            raise ValueError(
                "face_image_base64 não é uma imagem JPEG ou PNG válida"
            )
        return v


class DefenseSyncOptions(BaseModel):
    """Opções de cadastro no Intelbras Defense (obrigatório em POST /v1/person/sync)."""

    sync_target: Literal["visitor", "person"] = Field(
        ...,
        description=(
            "`visitor` — nova visita via POST /obms/api/v1.0/visitors/visitor; "
            "`person` — upsert ACS (personId = external_id)."
        ),
    )
    org_code: str = Field(
        ...,
        min_length=1,
        description="Código da sub-organização no Defense (ex.: `001021`).",
    )
    acs_channel_ids: list[str] | None = Field(
        default=None,
        description=(
            "Portas de acesso (formato `1000049$7$0$0`). Só com `sync_target=visitor`. "
            "Omitir: busca automática por org_code (fallback permissão padrão). "
            "`[]`: permissão padrão do visitante, sem busca. "
            "Lista preenchida: portas fixas enviadas ao Defense."
        ),
    )

    @model_validator(mode="after")
    def acs_channels_only_for_visitor(self) -> "DefenseSyncOptions":
        if self.acs_channel_ids is not None and self.sync_target != "visitor":
            raise ValueError("acs_channel_ids só se aplica quando sync_target=visitor")
        return self


class SyncRequest(BaseModel):
    source: str = Field(
        ...,
        min_length=1,
        description="Sistema de origem (ex.: `biodoc`). Deve estar na lista permitida.",
    )
    operation: str = Field(
        ...,
        pattern="^(upsert)$",
        description="Sempre `upsert`: cria ou atualiza a pessoa no Defense IA.",
    )

    @field_validator("source")
    @classmethod
    def source_must_be_allowed(cls, v: str) -> str:
        if v not in ALLOWED_SOURCES:
            raise ValueError(
                f"source '{v}' nao permitido. Permitidos: {ALLOWED_SOURCES}"
            )
        return v
    external_id: str = Field(
        ...,
        min_length=1,
        max_length=30,
        pattern=r"^[0-9A-Za-z]+$",
        description=(
            "Identificador único da pessoa no Defense IA (`personId`). "
            "Somente letras e números, máx. 30 caracteres."
        ),
    )
    person: PersonData
    biometrics: BiometricData | None = Field(
        default=None,
        description="Foto facial em base64. Opcional; envia ou atualiza a face no Defense.",
    )


class ManualSyncRequest(SyncRequest):
    """POST /v1/person/sync — exige bloco `defense` no body (não usa .env para modo/org/portas)."""

    defense: DefenseSyncOptions


class SyncResponse(BaseModel):
    status: str
    message: str
    visitor_id: str | None = Field(
        default=None,
        description="ID da visita gerado pelo Defense IA (modo visitante)",
    )
    person_id: str | None = Field(
        default=None,
        description="personId retornado pelo Defense IA",
    )


