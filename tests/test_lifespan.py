from types import SimpleNamespace

import pytest
from fastapi import FastAPI

from src.core import lifespan as lifespan_module


class FakeDefenseClient:
    def __init__(self) -> None:
        self.settings = SimpleNamespace(enabled=True)
        self.is_ready = True
        self.started = False
        self.closed = False

    async def start(self) -> None:
        self.started = True

    async def close(self) -> None:
        self.closed = True

    async def warmup_org_cache(self) -> None:
        return None


class FakeBiodocClient:
    def __init__(self) -> None:
        self.started = False
        self.closed = False

    async def start(self) -> None:
        self.started = True

    async def close(self) -> None:
        self.closed = True


@pytest.mark.asyncio
async def test_lifespan_starts_and_closes_defense_client(monkeypatch):
    fake_defense = FakeDefenseClient()
    fake_biodoc = FakeBiodocClient()

    monkeypatch.setattr(lifespan_module, "build_defense_client_from_env", lambda: fake_defense)
    monkeypatch.setattr(lifespan_module, "build_biodoc_client_from_env", lambda: fake_biodoc)

    app = FastAPI()

    async with lifespan_module.lifespan(app):
        assert app.state.defense_client is fake_defense
        assert app.state.biodoc_client is fake_biodoc
        assert fake_defense.started is True
        assert fake_biodoc.started is True

    assert fake_defense.closed is True
    assert fake_biodoc.closed is True
