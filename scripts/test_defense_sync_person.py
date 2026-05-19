"""
Teste manual de cadastro/atualização (upsert) no Defense IA 3.x.

1. GET person/{id} — se não existir, POST (criar)
2. Mesmo external_id de novo — PUT (atualizar)

Uso:
  docker compose run --rm --no-deps -v "${PWD}:/app" middleware-biodoc-intelbras python scripts/test_defense_sync_person.py
  docker compose run --rm --no-deps -v "${PWD}:/app" middleware-biodoc-intelbras python scripts/test_defense_sync_person.py biodocupsert001 --no-face
"""
import argparse
import asyncio
import sys
from pathlib import Path

from dotenv import load_dotenv

from scripts.load_test_face_base64 import load_test_face_base64
from src.core.lifespan import build_defense_client_from_env
from src.services.defense_ia_client import DefenseIAError, brms_person_path

load_dotenv(Path(__file__).resolve().parent.parent / ".env")

def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Teste upsert pessoa no Defense")
    parser.add_argument(
        "external_id",
        nargs="?",
        default="biodocupserttest001",
        help="personId alfanumerico (sem hifen)",
    )
    parser.add_argument(
        "--no-face",
        action="store_true",
        help="Cadastro sem foto (facePictures vazio)",
    )
    return parser.parse_args()


def sync_payload(external_id: str, full_name: str, face_b64: str | None) -> dict:
    body: dict = {
        "source": "biodoc",
        "operation": "upsert",
        "external_id": external_id,
        "person": {"full_name": full_name, "document": "12345678900"},
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

    print(f"Servidor: {client.settings.server_url}")
    print(f"personId (external_id): {external_id}")
    print(f"Com face: {'nao' if args.no_face else 'sim'}\n")

    try:
        await client.login()
        print("Login OK\n")

        path = brms_person_path(external_id)
        exists_before = await client._brms_person_exists(external_id)
        print(f"Antes do 1º sync — GET {path}: {'existe' if exists_before else 'nao existe'}")

        payload_create = SyncRequest.model_validate(
            sync_payload(external_id, "Pessoa Teste BIODOC", face_b64)
        )
        result1 = await client.sync_person(payload_create)
        print(f"1º sync (esperado POST se novo): {result1}\n")

        exists_after = await client._brms_person_exists(external_id)
        print(f"Depois do 1º sync — GET {path}: {'existe' if exists_after else 'nao existe'}")

        payload_update = SyncRequest.model_validate(
            sync_payload(external_id, "Pessoa Teste BIODOC Atualizada", face_b64)
        )
        result2 = await client.sync_person(payload_update)
        print(f"\n2º sync (esperado PUT): {result2}")
        print("\nOK — upsert concluido (criar + atualizar).")
    except DefenseIAError as exc:
        print(f"\nERRO Defense IA: {exc}")
        sys.exit(1)
    finally:
        await client.close()


if __name__ == "__main__":
    asyncio.run(main())
