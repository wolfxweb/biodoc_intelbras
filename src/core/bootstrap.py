"""Bootstrap de startup: garante que a integration_source padrão exista no banco."""

import os

from sqlalchemy.orm import Session

from src.core.security import hash_token
from src.models.integration_source import IntegrationSource


def resolve_integration_key_from_env() -> str | None:
    """
    Retorna a chave de integração a ser usada no bootstrap.
    Prefere BIODOC_INTEGRATION_KEY; fallback para ADMIN_API_TOKEN.
    """
    biodoc_key = os.getenv("BIODOC_INTEGRATION_KEY", "").strip()
    if biodoc_key:
        return biodoc_key
    admin_token = os.getenv("ADMIN_API_TOKEN", "").strip()
    return admin_token or None


def ensure_integration_source(db: Session) -> None:
    """
    Cria ou atualiza a integration_source padrão (DEFAULT_INTEGRATION_SOURCE_NAME).

    Se nenhuma chave estiver configurada (BIODOC_INTEGRATION_KEY nem ADMIN_API_TOKEN),
    não faz nada.
    """
    key = resolve_integration_key_from_env()
    if not key:
        return

    name = os.getenv("DEFAULT_INTEGRATION_SOURCE_NAME", "biodoc")
    token_hash = hash_token(key)

    source = db.query(IntegrationSource).filter(IntegrationSource.name == name).first()
    if source is None:
        db.add(IntegrationSource(name=name, token_hash=token_hash, is_active=True))
    else:
        source.token_hash = token_hash
        source.is_active = True

    db.commit()
