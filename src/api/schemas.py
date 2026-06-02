import base64

from pydantic import BaseModel, ConfigDict, Field, field_validator

ALLOWED_SOURCES: list[str] = [
    "biodoc",
]


class PersonData(BaseModel):
    """Dados cadastrais do visitante enviados ao Intelbras Defense."""

    full_name: str = Field(
        ...,
        min_length=1,
        description=(
            "Nome completo do visitante. Aparece no cadastro de visita no Defense "
            "(campo `visitorName`)."
        ),
        examples=["Maria Silva"],
    )
    document: str = Field(
        ...,
        min_length=1,
        description=(
            "Documento de identificação (CPF, RG ou outro identificador). "
            "Gravado no visitante para consulta no painel."
        ),
        examples=["12345678900"],
    )


_JPEG_MAGIC = b"\xff\xd8\xff"
_PNG_MAGIC = b"\x89PNG"
_MIN_IMAGE_BYTES = 1024


class BiometricData(BaseModel):
    """Biometria facial opcional. Omita o bloco inteiro para cadastrar sem foto."""

    face_image_base64: str | None = Field(
        default=None,
        description=(
            "Foto do rosto em base64 (JPEG ou PNG), sem prefixo `data:image/...`. "
            "Mínimo 1 KB após decodificação. Com foto, o visitante pode passar "
            "pelas catracas com reconhecimento facial."
        ),
        examples=["/9j/4AAQSkZJRg..."],
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
    """Destino no Intelbras Defense. Portas de acesso são resolvidas automaticamente."""

    org_code: str = Field(
        ...,
        min_length=1,
        description=(
            "Código da sub-organização no Defense (`orgCode`). Define onde a visita "
            "é registrada. Liste códigos disponíveis com "
            "`python scripts/list_person_orgs.py`. Ex.: `001021`."
        ),
        examples=["001021"],
    )


class SyncRequest(BaseModel):
    source: str = Field(
        ...,
        min_length=1,
        description=(
            "Sistema de origem do cadastro. Valor permitido: `biodoc` "
            "(demais valores retornam 422)."
        ),
        examples=["biodoc"],
    )
    operation: str = Field(
        ...,
        pattern="^(upsert)$",
        description=(
            "Operação de sincronização. Use sempre `upsert`. "
            "Cada chamada cria uma **nova visita** no Defense (novo `visitor_id`); "
            "não atualiza visita anterior pelo `external_id`."
        ),
        examples=["upsert"],
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
            "Identificador de rastreio no Defense (gravado em `remark`). "
            "Use o ID do cartão BioDoc, matrícula ou código interno. "
            "Somente letras e números (sem hífen ou espaço), máximo 30 caracteres."
        ),
        examples=["KPHnIL", "00271368992672000"],
    )
    person: PersonData = Field(
        ...,
        description="Nome e documento do visitante.",
    )
    biometrics: BiometricData | None = Field(
        default=None,
        description=(
            "Biometria facial. **Opcional** — omita para cadastrar visitante sem foto. "
            "Envie `face_image_base64` para liberar acesso por reconhecimento facial."
        ),
    )


class ManualSyncRequest(SyncRequest):
    """Body de `POST /v1/person/sync` — cadastro direto de visitante no Intelbras Defense."""

    defense: DefenseSyncOptions = Field(
        ...,
        description=(
            "Configuração de destino no Defense. Informe `org_code`; "
            "portas de acesso são resolvidas automaticamente pelo middleware."
        ),
    )

    model_config = ConfigDict(
        json_schema_extra={
            "examples": [
                {
                    "source": "biodoc",
                    "operation": "upsert",
                    "external_id": "KPHnIL",
                    "person": {
                        "full_name": "Maria Silva",
                        "document": "12345678900",
                    },
                    "biometrics": {
                        "face_image_base64": "<JPEG ou PNG em base64, minimo 1 KB>",
                    },
                    "defense": {
                        "org_code": "001021",
                    },
                },
                {
                    "source": "biodoc",
                    "operation": "upsert",
                    "external_id": "00271368992672000",
                    "person": {
                        "full_name": "Joao Souza",
                        "document": "98765432100",
                    },
                    "defense": {
                        "org_code": "001021",
                    },
                },
            ]
        }
    )


class SyncResponse(BaseModel):
    status: str = Field(
        ...,
        description="Resultado da operação. Sucesso: `success`.",
        examples=["success"],
    )
    message: str = Field(
        ...,
        description="Mensagem legível sobre o resultado.",
        examples=["Visitante registrado no Intelbras Defense com sucesso"],
    )
    visitor_id: str | None = Field(
        default=None,
        description=(
            "ID da visita gerado pelo Defense (`visitorId`). "
            "Consulte no módulo **Visitante** do painel Intelbras."
        ),
        examples=["163691"],
    )
    person_id: str | None = Field(
        default=None,
        description=(
            "ID interno retornado pelo Defense ao criar o visitante. "
            "Não é o cadastro permanente de pessoa ACS."
        ),
        examples=["178043801032500317"],
    )


