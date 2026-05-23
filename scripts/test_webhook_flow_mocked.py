"""
Teste do fluxo webhook BioDoc → Defense IA com dados mockados.

Não chama a API BioDoc nem o servidor Defense de verdade. Injeta mocks
via dependency overrides do FastAPI e mostra cada etapa + o payload que
seria enviado ao Defense (sync_person).

Uso:
  python scripts/test_webhook_flow_mocked.py
  python scripts/test_webhook_flow_mocked.py --card 9876543210 --name "teste webhook"
"""

from __future__ import annotations

import argparse
import asyncio
import base64
import json
import os
import sys
from pathlib import Path
from unittest.mock import AsyncMock, patch

sys.path.insert(0, str(Path(__file__).resolve().parent.parent))

os.environ.setdefault("SKIP_DOTENV", "1")
os.environ.setdefault("BIODOC_WEBHOOK_TOKEN", "mock-webhook-token-for-local-test")

import httpx

from src.api.dependencies import get_biodoc_client, get_defense_client
from src.api.schemas import SyncRequest
from src.main import app
from src.services.biodoc_client import BiodocClient, CardMainImageData
from src.services.defense_ia_client import DefenseIASettings

_DUMMY_JPEG = b"\xff\xd8\xff" + b"\x00" * 2048
_FACE_B64 = base64.b64encode(_DUMMY_JPEG).decode()
DEFAULT_NAME = "teste webhook"


def _parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Fluxo webhook mockado: BioDoc API + download imagem + Defense sync_person",
    )
    parser.add_argument("--card", default="1234567890", help="Número do cartão no payload")
    parser.add_argument(
        "--name",
        default=DEFAULT_NAME,
        help=f"Nome retornado pela API BioDoc mockada (padrão: {DEFAULT_NAME})",
    )
    parser.add_argument(
        "--image-url",
        default="https://example.com/face-mock.jpg",
        help="URL de imagem (download também é mockado)",
    )
    return parser.parse_args()


def _make_defense_mock() -> AsyncMock:
    client = AsyncMock()
    client.sync_person.return_value = {"mock": True}
    client.settings = DefenseIASettings(
        server_url="http://defense.mock",
        username="mock",
        password="mock",
        api_mode="brms",
    )
    client.is_ready = True
    return client


def _make_biodoc_mock(card: str, name: str, image_url: str) -> AsyncMock:
    client = AsyncMock(spec=BiodocClient)
    client.get_card_mainimage.return_value = CardMainImageData(
        name=name,
        card=card,
        status=True,
        image=image_url,
    )
    return client


def _step(n: int, title: str, detail: str = "") -> None:
    print(f"\n[{n}] {title}")
    if detail:
        print(f"    {detail}")


async def run_flow(args: argparse.Namespace) -> int:
    card = args.card
    webhook_token = os.environ["BIODOC_WEBHOOK_TOKEN"]

    defense_mock = _make_defense_mock()
    biodoc_mock = _make_biodoc_mock(card, args.name, args.image_url)

    app.dependency_overrides[get_defense_client] = lambda: defense_mock
    app.dependency_overrides[get_biodoc_client] = lambda: biodoc_mock

    payload = {
        "confidence": "98",
        "date": "2025-02-04T12:34:56Z",
        "response": 201,
        "message": "Cadastro realizado com sucesso!",
        "card": card,
        "image": args.image_url,
        "success": True,
        "LogID": "mock-flow-001",
    }
    headers = {"Authorization": f"Bearer {webhook_token}"}

    print("=" * 60)
    print("Teste mockado: POST /webhook/biodoc → Defense sync_person")
    print("=" * 60)
    print(f"Card: {card} | Nome mock BioDoc: {args.name}")
    print("(BioDoc API e Defense IA reais NÃO são chamados)")

    _step(1, "POST /webhook/biodoc", json.dumps(payload, ensure_ascii=False)[:120] + "...")

    transport = httpx.ASGITransport(app=app)
    try:
        with patch(
            "src.services.biodoc_webhook_service.download_image_as_base64",
            new=AsyncMock(return_value=_FACE_B64),
        ):
            async with httpx.AsyncClient(
                transport=transport,
                base_url="http://testserver",
            ) as client:
                response = await client.post("/webhook/biodoc", json=payload, headers=headers)
    finally:
        app.dependency_overrides.clear()

    _step(2, "Resposta HTTP", f"status={response.status_code}")
    try:
        body = response.json()
        print(f"    body: {json.dumps(body, indent=2, ensure_ascii=False)}")
    except ValueError:
        print(f"    raw: {response.text}")
        return 1

    if response.status_code != 200:
        print("\n[FALHA] Webhook não retornou 200 — fluxo interrompido antes do Defense.")
        return 1

    biodoc_mock.get_card_mainimage.assert_awaited_once_with(card)
    _step(3, "BioDoc mock: get_card_mainimage", f"idCard={card} → name={args.name}, status=True")

    _step(4, "Download imagem mockado", f"base64 len={len(_FACE_B64)} chars")

    defense_mock.sync_person.assert_awaited_once()
    sync_req: SyncRequest = defense_mock.sync_person.await_args[0][0]

    defense_payload = {
        "source": sync_req.source,
        "operation": sync_req.operation,
        "external_id": sync_req.external_id,
        "person": {
            "full_name": sync_req.person.full_name,
            "document": sync_req.person.document,
        },
        "biometrics": {
            "face_image_base64": f"<{len(sync_req.biometrics.face_image_base64 or '')} chars>",
        },
    }
    _step(5, "Defense mock: sync_person (upsert)", json.dumps(defense_payload, indent=2, ensure_ascii=False))

    assert sync_req.source == "biodoc"
    assert sync_req.operation == "upsert"
    assert sync_req.external_id == card
    assert sync_req.person.full_name == args.name
    assert sync_req.person.document == card
    assert sync_req.biometrics.face_image_base64 == _FACE_B64

    print("\n" + "=" * 60)
    print("[OK] Fluxo completo validado com mocks:")
    print("  webhook → consulta BioDoc (mock) → imagem base64 (mock) → Defense sync_person (mock)")
    print("=" * 60)
    print("\nPara gravar de verdade no Defense, use credenciais reais em .env e:")
    print("  python scripts/test_biodoc_webhook.py --url http://localhost:8000 --card <card_real>")
    return 0


def main() -> None:
    args = _parse_args()
    raise SystemExit(asyncio.run(run_flow(args)))


if __name__ == "__main__":
    main()
