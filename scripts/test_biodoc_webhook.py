"""
Simula o envio de um webhook BioDoc para o middleware local ou de homologação.

Uso:
  python scripts/test_biodoc_webhook.py
  python scripts/test_biodoc_webhook.py --middleware-url https://homologa.wolfx.com.br
  python scripts/test_biodoc_webhook.py --success false
  python scripts/test_biodoc_webhook.py --reference-id <uuid-da-interacao>

Requer no .env (ou variáveis de ambiente):
  BIODOC_WEBHOOK_TOKEN   — mesmo token configurado no painel BioDoc
  MIDDLEWARE_URL         — base URL do middleware (ex: http://localhost:8000)
"""

import argparse
import json
import os
import sys
from pathlib import Path

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
        "--middleware-url",
        dest="middleware_url",
        default=os.getenv("MIDDLEWARE_URL", "http://localhost:8000"),
        help="Base URL do middleware (padrão: MIDDLEWARE_URL do .env)",
    )
    parser.add_argument(
        "--success",
        default="true",
        choices=["true", "false"],
        help="Simular sucesso ou falha do liveness (padrão: true)",
    )
    parser.add_argument(
        "--status",
        type=int,
        default=2,
        help="Status BioDoc da interação (1/2 = ativo, padrão: 2)",
    )
    parser.add_argument(
        "--image-url",
        dest="image_url",
        default="https://upload.wikimedia.org/wikipedia/commons/thumb/a/a7/Camponotus_flavomarginatus_ant.jpg/320px-Camponotus_flavomarginatus_ant.jpg",
        help="URL temporária da imagem capturada (campo 'url' do payload)",
    )
    parser.add_argument(
        "--reference-id",
        dest="reference_id",
        default="0c19bfff-9aba-4517-afd7-56e77ea1faeb",
        help="reference_Id (UUID) a incluir no payload (padrão: UUID de teste)",
    )
    parser.add_argument(
        "--id-log",
        dest="id_log",
        type=int,
        default=1000,
        help="id_Log numérico do BioDoc (padrão: 1000)",
    )
    parser.add_argument(
        "--percentage",
        default="100%",
        help="Similaridade reportada pelo BioDoc (padrão: 100%%)",
    )
    return parser.parse_args()


def main() -> None:
    args = parse_args()

    token = os.getenv("BIODOC_WEBHOOK_TOKEN", "").strip()
    if not token:
        print("[ERRO] BIODOC_WEBHOOK_TOKEN não definido no .env ou variáveis de ambiente")
        print("  Adicione: BIODOC_WEBHOOK_TOKEN=<mesmo token configurado no painel BioDoc>")
        sys.exit(1)

    endpoint = f"{args.middleware_url.rstrip('/')}/webhook/biodoc"
    success = args.success == "true"
    payload = {
        "id_Log": args.id_log,
        "percentage": args.percentage,
        "success": success,
        "status": args.status,
        "message": (
            "Sucesso ao realizar autenticação, nível de similaridade 100% e qualidade 100%."
            if success
            else "Falha no liveness"
        ),
        "url": args.image_url,
        "reference_Id": args.reference_id,
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
