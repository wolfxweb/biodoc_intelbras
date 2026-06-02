"""
Teste do fluxo BIODOC -> middleware (localhost) -> Defense IA (IP do .env).

1. Cria source (se necessario) — ADMIN_API_TOKEN
2. POST http://localhost:8000/v1/person/sync — integration_key
3. Repete mesmo external_id (deve atualizar no Defense)
Uso (middleware rodando: docker compose up -d):
  python scripts/test_sync_via_api.py
  python scripts/test_sync_via_api.py biodocapi001
"""
import argparse
import os
from pathlib import Path

import httpx
from dotenv import load_dotenv

from scripts.load_test_face_base64 import load_test_face_base64

load_dotenv(Path(__file__).resolve().parent.parent / ".env")

BASE_URL = os.getenv("MIDDLEWARE_URL", "http://localhost:8000").rstrip("/")
ADMIN_TOKEN = os.getenv("ADMIN_API_TOKEN", "")
SOURCE_NAME = os.getenv("TEST_SOURCE_NAME", "biodoc")


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Teste sync via middleware")
    parser.add_argument("external_id", nargs="?", default="biodocapi001")
    return parser.parse_args()


def main() -> None:
    import sys

    args = parse_args()
    if not ADMIN_TOKEN:
        print("Defina ADMIN_API_TOKEN no .env")
        sys.exit(1)

    face_b64 = load_test_face_base64()
    print(f"Middleware: {BASE_URL}")
    print(f"Defense IA: {os.getenv('DEFENSE_IA_SERVER_URL')} (via .env do container)")
    print(f"external_id: {args.external_id}\n")

    with httpx.Client(base_url=BASE_URL, timeout=60.0) as client:
        integration_key = _ensure_source(client)
        print(f"integration_key: {integration_key[:12]}...\n")

        for i, name in enumerate(
            ("Maria Silva Teste", "Maria Silva Teste Atualizada"), start=1
        ):
            payload = {
                "source": SOURCE_NAME,
                "operation": "upsert",
                "external_id": args.external_id,
                "person": {"full_name": name, "document": "12345678900"},
                "defense": {
                    "org_code": os.getenv("DEFENSE_IA_ORG_CODE", "001"),
                },
                "biometrics": {"face_image_base64": face_b64},
            }
            response = client.post(
                "/v1/person/sync",
                headers={"Authorization": f"Bearer {integration_key}"},
                json=payload,
            )
            print(f"Sync {i} ({name}): HTTP {response.status_code}")
            print(response.text[:500])
            if not response.is_success:
                sys.exit(1)
            print()

    print("OK — dois syncs via API do middleware (criar + atualizar).")


def _ensure_source(client: httpx.Client) -> str:
    env_key = os.getenv("BIODOC_INTEGRATION_KEY", "").strip() or os.getenv(
        "ADMIN_API_TOKEN", ""
    ).strip()
    if env_key:
        print("Usando token do .env (ADMIN_API_TOKEN ou BIODOC_INTEGRATION_KEY)")
        return env_key

    list_resp = client.get(
        "/v1/integration-sources",
        headers={"Authorization": f"Bearer {ADMIN_TOKEN}"},
    )
    list_resp.raise_for_status()
    for item in list_resp.json():
        if item.get("name") == SOURCE_NAME and item.get("is_active"):
            key = _rotate_or_create(client, item["id"])
            return key

    create_resp = client.post(
        "/v1/integration-sources",
        headers={"Authorization": f"Bearer {ADMIN_TOKEN}"},
        json={"name": SOURCE_NAME, "is_active": True},
    )
    create_resp.raise_for_status()
    data = create_resp.json()
    print(f"Source criada: id={data['id']}")
    return data["integration_key"]


def _rotate_or_create(client: httpx.Client, source_id: int) -> str:
    patch_resp = client.patch(
        f"/v1/integration-sources/{source_id}",
        headers={"Authorization": f"Bearer {ADMIN_TOKEN}"},
        json={"rotate_token": True},
    )
    if patch_resp.status_code == 200 and patch_resp.json().get("integration_key"):
        print(f"Source existente id={source_id}, token rotacionado")
        return patch_resp.json()["integration_key"]

    create_resp = client.post(
        "/v1/integration-sources",
        headers={"Authorization": f"Bearer {ADMIN_TOKEN}"},
        json={"name": SOURCE_NAME, "is_active": True},
    )
    create_resp.raise_for_status()
    return create_resp.json()["integration_key"]


if __name__ == "__main__":
    main()
