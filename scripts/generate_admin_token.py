"""
Gerador de ADMIN_API_TOKEN seguro para o middleware BIODOC-Intelbras.

Uso — local (recomendado):
    python scripts/generate_admin_token.py

Uso — somente gerar token dentro do container (sem atualizar .env):
    docker exec middleware-biodoc python -c "import secrets; print(secrets.token_urlsafe(32))"

O script gera um token criptograficamente seguro (32 bytes / 43 chars)
e atualiza automaticamente o ADMIN_API_TOKEN no arquivo .env.

Após atualizar o .env, reinicie o container para aplicar:
    docker compose up -d --force-recreate
"""

import os
import re
import secrets
from pathlib import Path

ENV_FILE = Path(__file__).resolve().parent.parent / ".env"
TOKEN_KEY = "ADMIN_API_TOKEN"


def generate_token() -> str:
    return secrets.token_urlsafe(32)


def update_env(token: str) -> bool:
    if not ENV_FILE.exists():
        print(f"[ERRO] Arquivo .env não encontrado em: {ENV_FILE}")
        return False

    content = ENV_FILE.read_text(encoding="utf-8")
    pattern = re.compile(rf"^{TOKEN_KEY}=.*$", re.MULTILINE)

    if pattern.search(content):
        updated = pattern.sub(f"{TOKEN_KEY}={token}", content)
        ENV_FILE.write_text(updated, encoding="utf-8")
        return True

    # Chave não existe no .env — adiciona ao final
    ENV_FILE.write_text(content.rstrip() + f"\n{TOKEN_KEY}={token}\n", encoding="utf-8")
    return True


def main() -> None:
    token = generate_token()

    print(f"\nToken gerado: {token}")
    print(f"Comprimento : {len(token)} caracteres")
    print(f"Entropia    : 32 bytes (~256 bits)\n")

    resposta = input(f"Atualizar {TOKEN_KEY} no arquivo .env? [s/N] ").strip().lower()
    if resposta == "s":
        if update_env(token):
            print(f"[OK] {TOKEN_KEY} atualizado em {ENV_FILE}")
            print("[!] Reinicie o container para aplicar: docker compose up -d --force-recreate")
        else:
            print("[ERRO] Não foi possível atualizar o .env.")
    else:
        print("Nenhuma alteração feita.")
        print(f"\nPara aplicar manualmente, edite o .env e defina:")
        print(f"  {TOKEN_KEY}={token}")


if __name__ == "__main__":
    main()
