import hashlib
import os
import secrets


def generate_integration_key() -> str:
    return secrets.token_urlsafe(32)


def hash_token(token: str) -> str:
    return hashlib.sha256(token.encode("utf-8")).hexdigest()


def verify_token(token: str, token_hash: str) -> bool:
    return secrets.compare_digest(hash_token(token), token_hash)


def get_admin_token() -> str | None:
    return os.getenv("ADMIN_API_TOKEN")


def verify_admin_token(token: str) -> bool:
    admin_token = get_admin_token()
    if not admin_token:
        return False
    return secrets.compare_digest(token, admin_token)
