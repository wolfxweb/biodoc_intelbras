"""Schemas Pydantic para eventos de webhook enviados pelo BioDoc."""

from __future__ import annotations

from pydantic import BaseModel, Field


class BiodocWebhookPayload(BaseModel):
    """
    Payload enviado pelo BioDoc no evento de cadastro/liveness (formato 150r).

    Exemplo:
        {
          "id_Log": 1000,
          "percentage": "100%",
          "success": true,
          "status": 2,
          "message": "Sucesso ao realizar autenticação, nível de similaridade 100% e qualidade 100%.",
          "url": "https://api.sandbox.com.br/api/file/305",
          "reference_Id": "0c19bfff-9aba-4517-afd7-56e77ea1faeb"
        }

    Após receber, o middleware consulta `GET /integrations/log/{reference_Id}` na
    API BioDoc para obter `id_Card`, `name`, `mainImage` e `reguiredName`, que são
    usados para sincronizar o beneficiário no Defense IA.
    """

    id_Log: int | None = Field(default=None, description="ID numérico do log no BioDoc")
    percentage: str | None = Field(
        default=None, description="Similaridade/qualidade reportada pelo BioDoc (ex.: '100%')"
    )
    success: bool = Field(default=False)
    status: int | None = Field(
        default=None, description="Status BioDoc da interação (1/2 = ativo)"
    )
    message: str | None = None
    url: str | None = Field(
        default=None, description="URL temporária da imagem capturada (fallback)"
    )
    reference_Id: str | None = Field(
        default=None,
        description=(
            "UUID da interação BioDoc — usado para consultar "
            "GET /integrations/log/{reference_Id}"
        ),
    )


class BiodocWebhookResponse(BaseModel):
    status: str
    external_id: str
    defense_sync: str
