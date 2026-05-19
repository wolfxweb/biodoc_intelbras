import pytest
from sqlalchemy.orm import Session

from src.core.bootstrap import ensure_integration_source, resolve_integration_key_from_env
from src.core.security import hash_token, verify_token
from src.models.integration_source import IntegrationSource


def test_bootstrap_creates_source_when_key_set(db_session: Session, monkeypatch: pytest.MonkeyPatch):
    monkeypatch.setenv("BIODOC_INTEGRATION_KEY", "chave-teste-bootstrap")
    monkeypatch.setenv("DEFAULT_INTEGRATION_SOURCE_NAME", "biodoc")

    ensure_integration_source(db_session)

    source = (
        db_session.query(IntegrationSource)
        .filter(IntegrationSource.name == "biodoc")
        .first()
    )
    assert source is not None
    assert source.is_active is True
    assert verify_token("chave-teste-bootstrap", source.token_hash)


def test_bootstrap_noop_without_key(db_session: Session, monkeypatch: pytest.MonkeyPatch):
    monkeypatch.delenv("BIODOC_INTEGRATION_KEY", raising=False)
    monkeypatch.delenv("ADMIN_API_TOKEN", raising=False)

    ensure_integration_source(db_session)

    assert db_session.query(IntegrationSource).count() == 0


def test_bootstrap_uses_admin_token_when_biodoc_key_missing(
    db_session: Session, monkeypatch: pytest.MonkeyPatch
):
    monkeypatch.delenv("BIODOC_INTEGRATION_KEY", raising=False)
    monkeypatch.setenv("ADMIN_API_TOKEN", "admin-unico")
    monkeypatch.setenv("DEFAULT_INTEGRATION_SOURCE_NAME", "biodoc")

    ensure_integration_source(db_session)

    source = db_session.query(IntegrationSource).filter(IntegrationSource.name == "biodoc").first()
    assert source is not None
    assert verify_token("admin-unico", source.token_hash)


def test_resolve_integration_key_prefers_biodoc(monkeypatch: pytest.MonkeyPatch):
    monkeypatch.setenv("BIODOC_INTEGRATION_KEY", "biodoc-key")
    monkeypatch.setenv("ADMIN_API_TOKEN", "admin-key")
    assert resolve_integration_key_from_env() == "biodoc-key"


def test_bootstrap_updates_hash_when_key_changes(db_session: Session, monkeypatch: pytest.MonkeyPatch):
    db_session.add(
        IntegrationSource(
            name="biodoc",
            token_hash=hash_token("antiga"),
            is_active=True,
        )
    )
    db_session.commit()

    monkeypatch.setenv("BIODOC_INTEGRATION_KEY", "nova-chave")
    ensure_integration_source(db_session)

    source = db_session.query(IntegrationSource).filter(IntegrationSource.name == "biodoc").first()
    assert source is not None
    assert verify_token("nova-chave", source.token_hash)
    assert not verify_token("antiga", source.token_hash)
