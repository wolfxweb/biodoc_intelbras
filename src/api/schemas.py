import base64

from pydantic import BaseModel, Field, field_validator

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


class SyncResponse(BaseModel):
    status: str
    message: str


