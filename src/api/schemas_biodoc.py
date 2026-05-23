"""Schemas Pydantic para eventos de webhook enviados pelo BioDoc."""

from __future__ import annotations

from pydantic import BaseModel, Field, field_validator


class BiodocWebhookPayload(BaseModel):
    """
    Payload enviado pelo BioDoc no evento de cadastro/liveness.

    Exemplo:
        {
          "confidence": "98",
          "date": "2025-02-04T12:34:56Z",
          "response": 201,
          "message": "Cadastro realizado com sucesso!",
          "card": "1234567890",
          "image": "https://...",
          "success": true,
          "LogID": "abc-123"
        }
    """

    confidence: str | None = None
    date: str | None = None
    response: int | None = None
    message: str | None = None
    card: str | None = Field(default=None, description="Identificador do cartão/beneficiário")
    image: str | None = Field(default=None, description="URL da imagem de liveness")
    success: bool = Field(default=False)
    LogID: str | None = None

    @field_validator("card")
    @classmethod
    def card_max_length(cls, v: str | None) -> str | None:
        if v is not None and len(v) > 30:
            raise ValueError(
                f"card '{v[:30]}...' excede 30 caracteres — não pode ser usado como external_id no Defense IA"
            )
        return v


class BiodocWebhookResponse(BaseModel):
    status: str
    external_id: str
    defense_sync: str
