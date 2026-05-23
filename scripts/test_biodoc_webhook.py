"""
Simula o envio de um webhook BioDoc para o middleware local ou de homologação.

Uso:
  python scripts/test_biodoc_webhook.py
  python scripts/test_biodoc_webhook.py --url https://homologa.wolfx.com.br
  python scripts/test_biodoc_webhook.py --success false
  python scripts/test_biodoc_webhook.py --card 9999999999

Requer no .env (ou variáveis de ambiente):
  BIODOC_WEBHOOK_TOKEN   — mesmo token configurado no painel BioDoc
  MIDDLEWARE_URL         — base URL do middleware (ex: http://localhost:8000)
"""

import argparse
import json
import os
import sys
from pathlib import Path

# Permite rodar da raiz do projeto sem instalar o pacote
sys.path.insert(0, str(Path(__file__).resolve().parent.parent))

if os.getenv("SKIP_DOTENV") != "1":
    try:
        from dotenv import load_dotenv
        load_dotenv(Path(__file__).resolve().parent.parent / ".env")
    except ImportError:
        pass

import httpx


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Simula webhook BioDoc → middleware")
    parser.add_argument(
        "--url",
        default=os.getenv("MIDDLEWARE_URL", "http://localhost:8000"),
        help="Base URL do middleware (padrão: MIDDLEWARE_URL do .env)",
    )
    parser.add_argument(
        "--card",
        default="1234567890",
        help="Número do cartão/beneficiário (padrão: 1234567890)",
    )
    parser.add_argument(
        "--success",
        default="true",
        choices=["true", "false"],
        help="Simular sucesso ou falha do liveness (padrão: true)",
    )
    parser.add_argument(
        "--image",
        default="https://upload.wikimedia.org/wikipedia/commons/thumb/a/a7/Camponotus_flavomarginatus_ant.jpg/320px-Camponotus_flavomarginatus_ant.jpg",
        help="URL de imagem para o payload",
    )
    parser.add_argument(
        "--log-id",
        default="test-script-001",
        help="LogID a incluir no payload (padrão: test-script-001)",
    )
    return parser.parse_args()


def main() -> None:
    args = parse_args()

    token = os.getenv("BIODOC_WEBHOOK_TOKEN", "").strip()
    if not token:
        print("[ERRO] BIODOC_WEBHOOK_TOKEN não definido no .env ou variáveis de ambiente")
        print("  Adicione: BIODOC_WEBHOOK_TOKEN=<mesmo token configurado no painel BioDoc>")
        sys.exit(1)

    endpoint = f"{args.url.rstrip('/')}/webhook/biodoc"
    payload = {
        "confidence": "98",
        "date": "2025-02-04T12:34:56Z",
        "response": 201,
        "message": "Cadastro realizado com sucesso!" if args.success == "true" else "Falha no liveness",
        "card": args.card,
        "image": args.image,
        "success": args.success == "true",
        "LogID": args.log_id,
    }
    headers = {
        "Authorization": f"Bearer {token}",
        "Content-Type": "application/json",
    }

    print(f"POST {endpoint}")
    print(f"Payload: {json.dumps(payload, indent=2, ensure_ascii=False)}")
    print()

    try:
        response = httpx.post(endpoint, json=payload, headers=headers, timeout=30)
    except httpx.RequestError as exc:
        print(f"[ERRO] Falha na requisição: {exc}")
        sys.exit(1)

    print(f"Status: {response.status_code}")
    try:
        print(f"Resposta: {json.dumps(response.json(), indent=2, ensure_ascii=False)}")
    except ValueError:
        print(f"Resposta (raw): {response.text}")

    if response.is_success:
        print("\n[OK] Webhook processado com sucesso!")
        print("Verifique log/app.log e o Intelbras Defense IA para confirmar o cadastro.")
    else:
        print(f"\n[FALHA] Servidor retornou {response.status_code}")
        sys.exit(1)


if __name__ == "__main__":
    main()
