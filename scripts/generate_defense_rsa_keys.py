"""
Gera par RSA para login Defense IA 3.x (HTTP API 3.1).

Uso:
  python scripts/generate_defense_rsa_keys.py

Coloque a linha DEFENSE_IA_PUBLIC_KEY no .env (nunca commite a PRIVATE_KEY).
"""
from src.services.defense_ia_crypto import generate_rsa_key_pair_base64


def main() -> None:
    public_b64, private_b64 = generate_rsa_key_pair_base64()
    print("# Cole no .env (chave publica na 2a authorize como publicKey):")
    print(f"DEFENSE_IA_PUBLIC_KEY={public_b64}")
    print()
    print("# Guarde em local seguro (nao commitar); para decrypt de secretKey/secretVector:")
    print(f"DEFENSE_IA_PRIVATE_KEY={private_b64}")
    print()
    print("# Teste rapido sem par proprio (doc 3.1 — nao desvincula depois):")
    print("DEFENSE_IA_USE_SERVER_PUBLICKEY=true")


if __name__ == "__main__":
    main()
