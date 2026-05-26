"""
Lista as sub-organizações de pessoas cadastradas no Defense IA.

O servidor BRMS 3.x deste cliente não expõe endpoint listador de organizações
(/obms/.../org/list, /org/tree, /department/* respondem 404), então este
script usa o cliente para varrer todas as pessoas via /obms/api/v1.1/acs/person/page
e extrair os pares (orgCode, orgName).

Use a saída para ver quais nomes podem aparecer em `reguiredName` do BioDoc.
O middleware faz o mapeamento `reguiredName -> orgCode` automaticamente em
runtime; este script é só diagnóstico/visibilidade.

Uso:
  docker compose run --rm --no-deps -v "${PWD}:/app" \\
    middleware-biodoc-intelbras python scripts/list_person_orgs.py

  # Force refresh (ignora cache em memória, sempre repagina)
  python scripts/list_person_orgs.py --refresh
"""

from __future__ import annotations

import argparse
import asyncio
import sys
from pathlib import Path

from dotenv import load_dotenv

from src.core.lifespan import build_defense_client_from_env
from src.services.defense_ia_client import DefenseIAError

load_dotenv(Path(__file__).resolve().parent.parent / ".env")


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description=(
            "Lista as sub-organizações de pessoas cadastradas no Defense IA "
            "(usadas para vincular ACS person a portas no painel desktop)."
        ),
    )
    parser.add_argument(
        "--refresh",
        action="store_true",
        help="Ignora o cache em memória e força nova varredura",
    )
    return parser.parse_args()


def _print_table(orgs: dict[str, str]) -> None:
    if not orgs:
        print("Nenhuma sub-organização encontrada.")
        print("Verifique no painel se já existem pessoas cadastradas.")
        return

    print(f"\nTotal: {len(orgs)} sub-organização(ões)\n")
    header = f"{'orgCode':<14} {'orgName':<40}"
    print(header)
    print("-" * len(header))
    # `orgs` é {nome_lower: orgCode}; reverter pra nome -> code preservando ordem
    inverted = sorted(((code, name) for name, code in orgs.items()), key=lambda x: x[0])
    for code, name in inverted:
        print(f"{code:<14} {name[:40]}")

    print(
        "\nO middleware usa esses nomes para resolver `reguiredName` (BioDoc) -> "
        "`orgCode` (Defense IA). Cadastre o `reguiredName` no painel BioDoc com "
        "exatamente o `orgName` desejado (case-insensitive)."
    )


async def main() -> None:
    args = parse_args()
    client = build_defense_client_from_env()
    if not client.settings.enabled:
        print(
            "Defense IA desabilitado: configure DEFENSE_IA_SERVER_URL/USERNAME/PASSWORD no .env"
        )
        sys.exit(1)

    print(f"Servidor: {client.settings.server_url}")

    try:
        await client.start()
        if not client.is_ready:
            print("Login falhou — verifique credenciais e logs do middleware.")
            sys.exit(1)
        print("Login OK\n")

        orgs = await client.list_person_orgs(force_refresh=args.refresh)
        _print_table(orgs)
    except DefenseIAError as exc:
        print(f"ERRO Defense IA: {exc}")
        sys.exit(1)
    finally:
        await client.close()


if __name__ == "__main__":
    asyncio.run(main())
