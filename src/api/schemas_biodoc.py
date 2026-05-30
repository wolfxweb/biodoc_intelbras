"""Schemas Pydantic para eventos de webhook enviados pelo BioDoc."""

from __future__ import annotations

from pydantic import BaseModel, Field


class BiodocWebhookPayload(BaseModel):
    """
    Payload enviado pelo BioDoc no evento de cadastro/liveness.

    Suporta dois formatos:

    Formato legado (campo `reference_Id` presente):
        {
          "id_Log": 1000,
          "percentage": "100%",
          "success": true,
          "status": 2,
          "message": "Sucesso ao realizar autenticação...",
          "url": "https://api.sandbox.com.br/api/file/305",
          "reference_Id": "0c19bfff-9aba-4517-afd7-56e77ea1faeb"
        }

    Formato novo (servidor BioDoc via Go-http-client):
        {
          "confidence": "100",
          "date": "2026-05-28T20:52:35Z",
          "response": 200,
          "message": "Face Reconhecida",
          "card": "00271368992672000",
          "image": "https://bioid-sandbox.s3.amazonaws.com/...",
          "success": true,
          "code": "OK",
          "logId": "303fc796-1d5f-474e-8e2d-74eaa81f9e4d"
        }
    """

    # --- campos presentes nos dois formatos ---
    success: bool = Field(default=False)
    message: str | None = None

    # --- formato legado ---
    id_Log: int | None = Field(default=None, description="ID numérico do log no BioDoc")
    percentage: str | None = Field(
        default=None, description="Similaridade/qualidade reportada pelo BioDoc (ex.: '100%')"
    )
    status: int | None = Field(
        default=None, description="Status BioDoc da interação (1/2 = ativo)"
    )
    url: str | None = Field(
        default=None, description="URL temporária da imagem capturada (fallback)"
    )
    reference_Id: str | None = Field(
        default=None,
        description="UUID da interação — usado em GET /integrations/log/{reference_Id}",
    )

    # --- formato novo (servidor BioDoc) ---
    card: str | None = Field(
        default=None, description="Número do cartão do beneficiário"
    )
    logId: str | None = Field(
        default=None,
        description="UUID do log — equivalente a reference_Id no novo formato",
    )
    image: str | None = Field(
        default=None, description="URL S3 da imagem capturada na verificação"
    )
    confidence: str | None = Field(
        default=None, description="Nível de similaridade (ex.: '100')"
    )
    response: int | None = Field(
        default=None, description="Código HTTP-like do resultado BioDoc (200=ok, 403=falha)"
    )
    code: str | None = Field(
        default=None, description="Código de erro/sucesso do BioDoc (ex.: ERR_CRD_FACE_MISMATCH)"
    )
    date: str | None = Field(
        default=None, description="Timestamp do evento ISO 8601"
    )
    name: str | None = Field(
        default=None, description="Nome do beneficiário (quando enviado no evento)"
    )
    details: dict[str, object] | str | None = Field(
        default=None,
        description=(
            "Detalhes opcionais da integração (ex.: {'operador': 'VIVER'}). "
            "Pode chegar como JSON string em algumas integrações."
        ),
    )
    operador: str | None = Field(
        default=None,
        description="Operador/grupo de negócio para mapear orgCode no Defense IA.",
    )


class BiodocWebhookResponse(BaseModel):
    status: str
    external_id: str
    defense_sync: str
