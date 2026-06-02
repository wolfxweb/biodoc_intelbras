"""
Teste manual de cadastro de visitante no Defense IA 3.x.

Cada execução cria uma nova visita (POST /obms/api/v1.0/visitors/visitor).

Uso:
  docker compose run --rm --no-deps -v "${PWD}:/app" middleware-biodoc-intelbras python scripts/test_defense_sync_visitor.py
  docker compose run --rm --no-deps -v "${PWD}:/app" middleware-biodoc-intelbras python scripts/test_defense_sync_visitor.py cardtest001 --org-code 001021
"""
import argparse
import asyncio
import sys
from pathlib import Path

from dotenv import load_dotenv

from scripts.load_test_face_base64 import load_test_face_base64
from src.core.lifespan import build_defense_client_from_env
from src.services.defense_ia_client import DefenseIAError

load_dotenv(Path(__file__).resolve().parent.parent / ".env")


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Teste cadastro visitante no Defense")
    parser.add_argument(
        "external_id",
        nargs="?",
        default="biodocvisitortest001",
        help="Valor em remark / rastreio (nao vira visitorId)",
    )
    parser.add_argument(
        "--no-face",
        action="store_true",
        help="Cadastro sem foto (facePictures vazio)",
    )
    parser.add_argument(
        "--name",
        default="Visitante Teste BIODOC",
        help="Nome do visitante no Defense",
    )
    parser.add_argument(
        "--org-code",
        default="",
        help="orgCode para resolver acsChannelIds (default: DEFENSE_IA_ORG_CODE)",
    )
    return parser.parse_args()


def sync_payload(external_id: str, full_name: str, face_b64: str | None) -> dict:
    body: dict = {
        "source": "biodoc",
        "operation": "upsert",
        "external_id": external_id,
        "person": {"full_name": full_name, "document": external_id},
    }
    if face_b64:
        body["biometrics"] = {"face_image_base64": face_b64}
    return body


async def main() -> None:
    from src.api.schemas import SyncRequest

    args = parse_args()
    external_id = args.external_id
    face_b64: str | None = None if args.no_face else load_test_face_base64()
    client = build_defense_client_from_env()
    if not client.settings.enabled:
        print("Defense IA desabilitado: configure DEFENSE_IA_* no .env")
        sys.exit(1)
    if not client.settings.is_visitor_sync:
        print("DEFENSE_IA_SYNC_TARGET nao e 'visitor' — ajuste o .env")
        sys.exit(1)

    org_code = args.org_code or client.settings.org_code or "001"
    print(f"Servidor: {client.settings.server_url}")
    print(f"external_id (remark): {external_id}")
    print(f"orgCode (canais): {org_code}")
    print(f"Com face: {'nao' if args.no_face else 'sim'}\n")

    try:
        await client.login()
        print("Login OK\n")
        payload = SyncRequest.model_validate(
            sync_payload(external_id, args.name, face_b64)
        )
        result = await client.sync_visitor(payload, org_code)
        print(f"Resposta Defense: {result}\n")
        data = result.get("data", result)
        if isinstance(data, dict):
            print(f"visitorId: {data.get('visitorId')}")
            print(f"personId:  {data.get('personId')}")
        print("\nOK — visitante criado.")
    except DefenseIAError as exc:
        print(f"\nERRO Defense IA: {exc}")
        sys.exit(1)
    finally:
        await client.close()


if __name__ == "__main__":
    asyncio.run(main())
