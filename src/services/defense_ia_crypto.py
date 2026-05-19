"""Criptografia de login Defense IA 3.x (HTTP API 3.1)."""

from __future__ import annotations

import base64
from typing import Any

from cryptography.hazmat.primitives import serialization
from cryptography.hazmat.primitives.asymmetric import rsa


def normalize_public_key_base64(key_material: str) -> str:
    """Remove PEM headers/whitespace; retorna Base64 DER (formato esperado pela API)."""
    text = key_material.strip().replace("\r", "").replace("\n", "")
    if "BEGIN" in text:
        lines = [
            line.strip()
            for line in key_material.splitlines()
            if line.strip() and not line.strip().startswith("-----")
        ]
        text = "".join(lines)
    return text


def generate_rsa_key_pair_base64(key_size: int = 2048) -> tuple[str, str]:
    """Gera par RSA; retorna (public_key_base64, private_key_base64) em PKCS8/SPKI."""
    private_key = rsa.generate_private_key(public_exponent=65537, key_size=key_size)
    public_der = private_key.public_key().public_bytes(
        encoding=serialization.Encoding.DER,
        format=serialization.PublicFormat.SubjectPublicKeyInfo,
    )
    private_der = private_key.private_bytes(
        encoding=serialization.Encoding.DER,
        format=serialization.PrivateFormat.PKCS8,
        encryption_algorithm=serialization.NoEncryption(),
    )
    return (
        base64.b64encode(public_der).decode("ascii"),
        base64.b64encode(private_der).decode("ascii"),
    )


def resolve_login_public_key(
    settings_public_key: str,
    use_server_public_key: bool,
    challenge: dict[str, Any],
) -> str:
    """
    Escolhe a publicKey da 2ª authorize.

    Ordem: DEFENSE_IA_PUBLIC_KEY > (se permitido) publickey da 1ª resposta.
    """
    if settings_public_key.strip():
        return normalize_public_key_base64(settings_public_key)

    server_key = challenge.get("publickey") or challenge.get("publicKey")
    if use_server_public_key and server_key:
        return normalize_public_key_base64(str(server_key))

    raise ValueError(
        "Defina DEFENSE_IA_PUBLIC_KEY (gerar com scripts/generate_defense_rsa_keys.py) "
        "ou DEFENSE_IA_USE_SERVER_PUBLICKEY=true para teste com a chave da 1ª resposta."
    )
