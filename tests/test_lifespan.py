from types import SimpleNamespace

import pytest
from fastapi import FastAPI

from src.core import lifespan as lifespan_module


class FakeDefenseClient:
    def __init__(self) -> None:
        self.settings = SimpleNamespace(enabled=True)
        self.started = False
        self.closed = False

    async def start(self) -> None:
        self.started = True

    async def close(self) -> None:
        self.closed = True


@pytest.mark.asyncio
async def test_lifespan_starts_and_closes_defense_client(monkeypatch):
    fake_client = FakeDefenseClient()
    monkeypatch.setattr(
        lifespan_module,
        "build_defense_client_from_env",
        lambda: fake_client,
    )
    app = FastAPI()

    async with lifespan_module.lifespan(app):
        assert app.state.defense_client is fake_client
        assert fake_client.started is True

    assert fake_client.closed is True
