import pytest
from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker
from src.core.database import Base
from src.models.integration_source import IntegrationSource

SQLALCHEMY_DATABASE_URL = "sqlite:///:memory:"

engine = create_engine(
    SQLALCHEMY_DATABASE_URL, connect_args={"check_same_thread": False}
)
TestingSessionLocal = sessionmaker(autocommit=False, autoflush=False, bind=engine)

Base.metadata.create_all(bind=engine)

@pytest.fixture()
def db():
    db = TestingSessionLocal()
    try:
        yield db
    finally:
        db.close()

def test_integration_source_model(db):
    new_source = IntegrationSource(name="biodoc_test", token_hash="dummy_hash")
    db.add(new_source)
    db.commit()
    db.refresh(new_source)

    assert new_source.id is not None
    assert new_source.name == "biodoc_test"
    assert new_source.token_hash == "dummy_hash"
    assert new_source.is_active == True

    db_source = db.query(IntegrationSource).filter(IntegrationSource.name == "biodoc_test").first()
    assert db_source is not None
    assert db_source.token_hash == "dummy_hash"
