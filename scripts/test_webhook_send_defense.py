"""
Envia POST /webhook/biodoc e grava no Defense IA REAL.

- BioDoc API: mock (nome/foto/status fictícios)
- Download imagem: mock (data/test_face.jpg em base64)
- Defense IA: REAL (credenciais do .env)

Uso:
  PYTHONPATH=. python3 scripts/test_webhook_send_defense.py
  PYTHONPATH=. python3 scripts/test_webhook_send_defense.py --card testewebhook001
"""

from __future__ import annotations

import argparse
import asyncio
import os
import sys
from pathlib import Path
from unittest.mock import AsyncMock, patch

sys.path.insert(0, str(Path(__file__).resolve().parent.parent))

from dotenv import load_dotenv

load_dotenv(Path(__file__).resolve().parent.parent / ".env")

os.environ.setdefault("BIODOC_WEBHOOK_TOKEN", os.getenv("BIODOC_WEBHOOK_TOKEN", "local-webhook-test-token"))

import httpx

from scripts.load_test_face_base64 import load_test_face_base64
from src.api.dependencies import get_biodoc_client
from src.core.lifespan import lifespan
from src.main import app
from src.services.biodoc_client import BiodocClient, IntegrationLogData

DEFAULT_NAME = "teste webhook"
DEFAULT_CARD = "testewebhook001"
DEFAULT_REFERENCE_ID = "0c19bfff-9aba-4517-afd7-56e77ea1faeb"
DEFAULT_ID_LOG = 1000
DEFAULT_REQUIRED_NAME = "Empresa Teste"


def _parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Webhook → Defense IA (envio real)")
    parser.add_argument("--card", default=DEFAULT_CARD, help="visitorId no Defense (external_id) — usado no mock do BioDoc")
    parser.add_argument("--name", default=DEFAULT_NAME, help="Nome no Defense")
    parser.add_argument(
        "--reference-id",
        dest="reference_id",
        default=DEFAULT_REFERENCE_ID,
        help="reference_Id (UUID) no payload do webhook",
    )
    parser.add_argument(
        "--id-log",
        dest="id_log",
        type=int,
        default=DEFAULT_ID_LOG,
        help="id_Log numérico no payload",
    )
    parser.add_argument(
        "--required-name",
        dest="required_name",
        default=DEFAULT_REQUIRED_NAME,
        help="reguiredName retornado pelo BioDoc — resolvido em orgCode no Defense IA (ex.: 'Corb')",
    )
    return parser.parse_args()


def _make_biodoc_mock(card: str, name: str, required_name: str) -> AsyncMock:
    client = AsyncMock(spec=BiodocClient)
    client.get_integration_log.return_value = IntegrationLogData(
        id=1,
        id_card=card,
        name=name,
        status=1,
        main_image="https://example.com/face-mock.jpg",
        path=None,
        required_name=required_name,
    )
    return client


async def main() -> int:
    args = _parse_args()
    token = os.environ["BIODOC_WEBHOOK_TOKEN"]
    face_b64 = load_test_face_base64()

    biodoc_mock = _make_biodoc_mock(args.card, args.name, args.required_name)
    app.dependency_overrides[get_biodoc_client] = lambda: biodoc_mock

    payload = {
        "id_Log": args.id_log,
        "percentage": "100%",
        "success": True,
        "status": 2,
        "message": "Sucesso ao realizar autenticação, nível de similaridade 100% e qualidade 100%.",
        "url": "https://example.com/face-mock.jpg",
        "reference_Id": args.reference_id,
    }
    headers = {"Authorization": f"Bearer {token}"}

    print("=" * 60)
    print("ENVIO REAL: POST /webhook/biodoc → Defense IA (ACS person)")
    print("=" * 60)
    print(f"card/personId: {args.card}")
    print(f"reference_Id: {args.reference_id}")
    print(f"id_Log: {args.id_log}")
    print(f"nome: {args.name}")
    print(f"reguiredName (BioDoc): {args.required_name}")
    print(f"Defense URL: {os.getenv('DEFENSE_IA_SERVER_URL', '?')}")
    print()

    transport = httpx.ASGITransport(app=app)
    exit_code = 1
    try:
        with patch(
            "src.services.biodoc_webhook_service.download_image_as_base64",
            new=AsyncMock(return_value=face_b64),
        ):
            async with lifespan(app):
                async with httpx.AsyncClient(
                    transport=transport,
                    base_url="http://testserver",
                ) as client:
                    response = await client.post(
                        "/webhook/biodoc", json=payload, headers=headers
                    )

                print(f"HTTP {response.status_code}")
                try:
                    print(response.json())
                except ValueError:
                    print(response.text)

                if response.status_code != 200:
                    print("\n[FALHA] Webhook não concluiu o envio ao Defense.")
                    return 1

                defense = getattr(app.state, "defense_client", None)
                if defense and defense.settings.enabled:
                    person = await defense._fetch_brms_person(args.card)
                    exists = person is not None
                    print(
                        f"\nVerificação Defense GET /acs/person/{args.card}: "
                        f"{'EXISTE' if exists else 'NAO ENCONTRADO'}"
                    )
                    if not exists:
                        return 1
                else:
                    print("\n[AVISO] Defense desabilitado — sem verificação GET.")

                print("\n[OK] Enviado pelo webhook e cadastrado como ACS person no Defense IA.")
                exit_code = 0
    finally:
        app.dependency_overrides.clear()

    return exit_code


if __name__ == "__main__":
    raise SystemExit(asyncio.run(main()))
