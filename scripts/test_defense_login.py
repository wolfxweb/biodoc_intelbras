"""
Login Defense IA 3.x (BRMS) — credenciais do .env.

Referencia: docs/Defense IA - API/API Defense IA 3.0/HTTP API For Defense IA 3.1_EN for Intelbras.pdf

Uso:
  python scripts/generate_defense_rsa_keys.py   # gera DEFENSE_IA_PUBLIC_KEY
  python scripts/test_defense_login.py
  python scripts/test_defense_login.py http://200.180.74.90 legacy
"""
import os
import sys
from pathlib import Path

import httpx
from dotenv import load_dotenv

from src.services.defense_ia_client import (
    BRMS_AUTHORIZE,
    LEGACY_AUTHORIZE,
    build_auth_signatures,
    extract_token,
)
from src.services.defense_ia_crypto import resolve_login_public_key

load_dotenv(Path(__file__).resolve().parent.parent / ".env")

SERVER = (
    sys.argv[1]
    if len(sys.argv) > 1
    else os.getenv("DEFENSE_IA_SERVER_URL", "http://200.180.74.90")
).rstrip("/")
API_MODE = sys.argv[2] if len(sys.argv) > 2 else os.getenv("DEFENSE_IA_API_MODE", "brms")
USERNAME = os.getenv("DEFENSE_IA_USERNAME", "System")
PASSWORD = os.getenv("DEFENSE_IA_PASSWORD", "")
CLIENT_TYPE = os.getenv("DEFENSE_IA_CLIENT_TYPE", "WINPC_V2")
USER_TYPE = os.getenv("DEFENSE_IA_USER_TYPE", "0")
PUBLIC_KEY = os.getenv("DEFENSE_IA_PUBLIC_KEY", "").strip()
USE_SERVER_KEY = os.getenv("DEFENSE_IA_USE_SERVER_PUBLICKEY", "false").lower() in (
    "1",
    "true",
    "yes",
)

HEADERS = {"content-type": "application/json;charset=UTF-8"}


def parse_challenge(response: httpx.Response) -> dict | None:
    print(f"--- resposta 1 (HTTP {response.status_code}) ---")
    print(response.text[:2000])
    if response.status_code not in (200, 401):
        return None
    try:
        body = response.json()
    except ValueError:
        return None
    if not body.get("randomKey"):
        print(f"\nSem randomKey (code={body.get('code')}): {body.get('desc', '')}")
        return None
    return body


def try_brms_login() -> None:
    url = f"{SERVER}{BRMS_AUTHORIZE}"
    print(f"\n{'=' * 60}")
    print(f"Modo: brms (Defense IA 3.x) -> POST {url}")
    print(f"userName={USERNAME} clientType={CLIENT_TYPE}\n")

    r1 = httpx.post(
        url,
        json={"userName": USERNAME, "ipAddress": "", "clientType": CLIENT_TYPE},
        headers=HEADERS,
        timeout=15.0,
    )
    body1 = parse_challenge(r1)
    if not body1:
        return

    signature, _ = build_auth_signatures(
        USERNAME, PASSWORD, body1.get("realm", ""), body1["randomKey"]
    )
    try:
        login_public_key = resolve_login_public_key(PUBLIC_KEY, USE_SERVER_KEY, body1)
    except ValueError as exc:
        print(f"\nERRO: {exc}")
        print("Execute: python scripts/generate_defense_rsa_keys.py")
        return

    payload2 = {
        "userName": USERNAME,
        "randomKey": body1["randomKey"],
        "mac": "",
        "signature": signature,
        "encryptType": body1.get("encryptType", "MD5"),
        "ipAddress": "",
        "clientType": CLIENT_TYPE,
        "userType": USER_TYPE,
        "publicKey": login_public_key,
    }

    r2 = httpx.post(url, json=payload2, headers=HEADERS, timeout=15.0)
    print(f"\n--- resposta 2 (HTTP {r2.status_code}) ---")
    print(r2.text[:2000])
    try:
        body2 = r2.json()
    except ValueError:
        return
    code = body2.get("code")
    if code not in (None, 0, "0", 1000, "1000"):
        print(f"\nFalha authorize: code={code} — {body2.get('desc', '')}")
        if code in (2001, "2001"):
            print("Dica: use DEFENSE_IA_USERNAME=system (minusculas) e confira a senha da API.")
        return
    try:
        token = extract_token(body2)
        print(f"\nOK — token obtido: {token[:20]}...")
    except Exception as exc:
        print(f"\nResposta sem token: {exc}")


def try_legacy_login() -> None:
    url = f"{SERVER}{LEGACY_AUTHORIZE}"
    print(f"\n{'=' * 60}")
    print("Modo: legacy (V7) — use apenas se o servidor for antigo.\n")
    r1 = httpx.post(
        url,
        json={"userName": USERNAME, "ipAddress": "", "clientType": CLIENT_TYPE},
        headers=HEADERS,
        timeout=15.0,
    )
    body1 = parse_challenge(r1)
    if not body1 or not body1.get("realm"):
        print("\nLegacy exige realm na 1a resposta.")
        return
    signature, _ = build_auth_signatures(
        USERNAME, PASSWORD, body1["realm"], body1["randomKey"]
    )
    r2 = httpx.post(
        url,
        json={
            "userName": USERNAME,
            "randomKey": body1["randomKey"],
            "mac": "",
            "encryptType": "MD5",
            "ipAddress": "",
            "signature": signature,
            "clientType": CLIENT_TYPE,
        },
        headers=HEADERS,
        timeout=15.0,
    )
    print(f"\n--- resposta 2 (HTTP {r2.status_code}) ---")
    print(r2.text[:2000])


def main() -> None:
    print(f"Servidor: {SERVER}  modo: {API_MODE}")
    if API_MODE == "legacy":
        try_legacy_login()
    elif API_MODE == "brms":
        try_brms_login()
    else:
        print("Modo invalido. Use brms ou legacy.")
        sys.exit(1)


if __name__ == "__main__":
    main()
