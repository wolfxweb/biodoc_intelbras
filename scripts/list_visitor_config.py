"""
Lista a configuração global de visitantes no Defense IA 3.x.

Uso:
  docker compose run --rm --no-deps -v "${PWD}:/app" middleware-biodoc-intelbras python scripts/list_visitor_config.py
"""
import asyncio
import json
import sys
from pathlib import Path

from dotenv import load_dotenv

load_dotenv(Path(__file__).resolve().parent.parent / ".env")

from src.core.lifespan import build_defense_client_from_env


async def main() -> None:
    client = build_defense_client_from_env()
    if not client.settings.enabled:
        print("Defense IA desabilitado: configure DEFENSE_IA_* no .env")
        sys.exit(1)

    await client.login()
    config = await client._fetch_visitor_config()
    channels = config.get("acsChannelIds") or []
    print(f"Servidor: {client.settings.server_url}")
    print(f"sync_target: {client.settings.sync_target}")
    print(f"visited_person_id: {client.settings.visited_person_id or '(nao configurado)'}")
    print(f"\nConfig global (/brms/api/v1.1/config/visitor):")
    print(json.dumps(config, indent=2, ensure_ascii=False))
    print(f"\nacsChannelIds padrao ({len(channels)}):")
    for channel in channels:
        print(f"  - {channel}")
    await client.close()


if __name__ == "__main__":
    asyncio.run(main())
