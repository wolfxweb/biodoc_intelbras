"""
Teste: face de visitante existente -> cadastro pessoa (nome diferente) -> inativar -> excluir.

Somente via API (sem painel Defense). O visitante original NAO e alterado.

Uso:
  # Listar visitantes no servidor
  docker compose run --rm --no-deps -v "${PWD}:/app" middleware-biodoc-intelbras \\
    python scripts/test_visitor_face_lifecycle.py --list

  # Usar visitante id=123
  docker compose run --rm --no-deps -v "${PWD}:/app" middleware-biodoc-intelbras \\
    python scripts/test_visitor_face_lifecycle.py 123

  # Pular cadastro (so testar leitura da face)
  docker compose run --rm --no-deps -v "${PWD}:/app" middleware-biodoc-intelbras \\
    python scripts/test_visitor_face_lifecycle.py 123 --dry-run
"""
from __future__ import annotations

import argparse
import asyncio
import sys
import time
from pathlib import Path

from dotenv import load_dotenv

from src.core.lifespan import build_defense_client_from_env
from src.services.defense_ia_client import DefenseIAError, brms_person_path
from src.api.schemas import SyncRequest
from src.services.defense_visitor import (
    delete_persons,
    extract_visitor_face_pictures,
    get_visitor,
    list_visitors,
)

load_dotenv(Path(__file__).resolve().parent.parent / ".env")


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Teste visitante -> pessoa ACS")
    parser.add_argument("visitor_id", nargs="?", help="ID do visitante no Defense")
    parser.add_argument("--list", action="store_true", help="Lista visitantes e sai")
    parser.add_argument(
        "--dry-run",
        action="store_true",
        help="So le face do visitante, nao cadastra pessoa",
    )
    parser.add_argument(
        "--skip-delete",
        action="store_true",
        help="Nao exclui a pessoa de teste ao final",
    )
    return parser.parse_args()


async def main() -> None:
    args = parse_args()
    client = build_defense_client_from_env()
    if not client.settings.enabled:
        print("Defense IA desabilitado no .env")
        sys.exit(1)

    await client.login()
    print(f"Servidor: {client.settings.server_url}\n")

    if args.list or not args.visitor_id:
        data = await list_visitors(client)
        rows = data.get("data") or data.get("pageData") or data
        if isinstance(rows, dict):
            items = rows.get("list") or rows.get("records") or rows.get("data") or []
        else:
            items = rows if isinstance(rows, list) else []
        print(f"Visitantes (ate 20): {len(items)} registro(s)\n")
        for item in items[:20]:
            if not isinstance(item, dict):
                continue
            vid = item.get("visitorId") or item.get("id")
            name = item.get("visitorName") or item.get("name")
            status = item.get("status")
            faces = len((item.get("authInfo") or {}).get("facePictures") or [])
            print(f"  id={vid} status={status} faces={faces} nome={name}")
        if not args.visitor_id:
            print("\nInforme o visitor_id: python scripts/test_visitor_face_lifecycle.py <id>")
            return

    visitor_id = args.visitor_id
    print(f"Visitante id={visitor_id} (somente leitura)")
    visitor = await get_visitor(client, visitor_id)
    visitor_data = visitor.get("data", visitor)
    if isinstance(visitor_data, dict):
        visitor_body = visitor_data
    else:
        visitor_body = visitor

    vname = visitor_body.get("visitorName", "N/A")
    faces = extract_visitor_face_pictures(visitor_body)
    print(f"  Nome no Defense (visitante): {vname}")
    print(f"  Fotos no cadastro visitante: {len(faces)}")
    if not faces:
        print("  AVISO: visitante sem facePictures — use outro visitante ou data/test_face.jpg")
        sys.exit(1)

    person_id = f"testv{int(time.time())}"
    test_name = f"Teste API BIODOC {person_id}"
    document = visitor_body.get("idNum") or "00000000000"
    print(f"\nPessoa de teste personId={person_id}")
    print(f"  Nome ACS (diferente): {test_name}")

    if args.dry_run:
        print("\n--dry-run: encerrado sem cadastrar pessoa.")
        return

    sync = SyncRequest.model_validate(
        {
            "source": "biodoc",
            "operation": "upsert",
            "external_id": person_id,
            "person": {"full_name": test_name, "document": str(document)[:20]},
            "biometrics": {"face_image_base64": faces[0]},
        }
    )
    exists_before = await client._brms_person_exists(person_id)
    print(f"\nAntes sync: pessoa existe={exists_before}")
    result_create = await client.sync_person(sync)
    print(f"1) Cadastro/upsert: {result_create}")

    # 2) Inativar (PUT com status=0 se suportado)
    inactive_payload = client.build_person_payload(sync)
    inactive_payload.setdefault("baseInfo", {})["status"] = "0"
    inactive_payload.setdefault("extensionInfo", {})["remark"] = "INATIVO-TESTE-API-BIODOC"
    inactive_response = await client._request(
        "PUT",
        brms_person_path(person_id),
        json=inactive_payload,
        headers=client._auth_headers(),
    )
    try:
        client._raise_for_response(inactive_response)
        print(f"2) Inativacao (PUT status=0): HTTP {inactive_response.status_code} OK")
    except DefenseIAError as exc:
        print(f"2) Inativacao: {exc} (campo status pode variar por versao)")

    # 3) Excluir
    if args.skip_delete:
        print(f"\n3) Exclusao ignorada (--skip-delete). personId={person_id}")
    else:
        del_result = await delete_persons(client, [person_id])
        print(f"3) Exclusao batch: {del_result}")
        exists_after = await client._brms_person_exists(person_id)
        print(f"   Apos exclusao: pessoa existe={exists_after}")

    print("\nConcluido. Visitante original nao foi modificado.")


if __name__ == "__main__":
    try:
        asyncio.run(main())
    except DefenseIAError as exc:
        print(f"\nERRO: {exc}")
        sys.exit(1)
