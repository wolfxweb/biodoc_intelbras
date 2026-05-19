import os

os.environ["SKIP_DOTENV"] = "1"

from collections.abc import Generator
from unittest.mock import AsyncMock

import httpx
import pytest
import pytest_asyncio
from sqlalchemy import create_engine
from sqlalchemy.orm import Session, sessionmaker
from sqlalchemy.pool import StaticPool

from src.api.dependencies import get_defense_client
from src.core.database import Base, get_db
from src.main import app


SQLALCHEMY_DATABASE_URL = "sqlite://"


@pytest.fixture()
def db_session() -> Generator[Session, None, None]:
    engine = create_engine(
        SQLALCHEMY_DATABASE_URL,
        connect_args={"check_same_thread": False},
        poolclass=StaticPool,
    )
    TestingSessionLocal = sessionmaker(autocommit=False, autoflush=False, bind=engine)
    Base.metadata.create_all(bind=engine)
    db = TestingSessionLocal()
    try:
        yield db
    finally:
        db.close()
        Base.metadata.drop_all(bind=engine)


@pytest.fixture()
def defense_client_mock() -> AsyncMock:
    from src.services.defense_ia_client import DefenseIASettings

    client = AsyncMock()
    client.sync_person.return_value = {"ok": True}
    client.settings = DefenseIASettings(
        server_url="http://defense.test",
        username="u",
        password="p",
        api_mode="brms",
    )
    client.token = "mock-token"
    client.is_ready = True
    return client


@pytest_asyncio.fixture()
async def api_client(
    db_session: Session,
    defense_client_mock: AsyncMock,
    monkeypatch: pytest.MonkeyPatch,
):
    monkeypatch.setenv("ADMIN_API_TOKEN", "admin-token")

    async def override_get_db() -> Generator[Session, None, None]:
        yield db_session

    app.dependency_overrides[get_db] = override_get_db

    async def override_get_defense_client() -> AsyncMock:
        return defense_client_mock

    app.dependency_overrides[get_defense_client] = override_get_defense_client
    transport = httpx.ASGITransport(app=app)
    async with httpx.AsyncClient(
        transport=transport,
        base_url="http://testserver",
    ) as client:
        yield client
    app.dependency_overrides.clear()
    os.environ.pop("ADMIN_API_TOKEN", None)
