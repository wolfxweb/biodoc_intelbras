import asyncio
import json
from types import SimpleNamespace
from unittest.mock import AsyncMock, Mock

import httpx
import pytest

from src.api.dependencies import get_visit_leave_settings
from src.main import app
from src.services.defense_ia_client import DefenseIAClient, DefenseIASettings
from src.services import visitor_leave as visitor_leave_module
from src.services.visitor_leave import (
    VisitLeaveSettings,
    build_leave_payload,
    is_finalized_visitor,
    poll_visitor_leave_history,
    process_visitor_leave_body,
    visitor_leave_poll_loop,
)


@pytest.fixture(autouse=True)
def _noop_sqlite(monkeypatch: pytest.MonkeyPatch) -> None:
    monkeypatch.setattr(visitor_leave_module, "persist_visitor_leave", lambda *a, **k: True)
    monkeypatch.setattr(visitor_leave_module, "get_webhook_url", lambda url="": url)
    monkeypatch.setattr(visitor_leave_module, "get_webhook_token", lambda token="": token)


def _visitor(
    visitor_id: str = "1842",
    leave_time: str = "1000000",
    *,
    status: str = "2",
) -> dict:
    return {
        "visitorId": visitor_id,
        "personId": "p-1842",
        "status": status,
        "visitorName": "Maria Silva",
        "idNum": "12345678900",
        "arrivalTime": "999000",
        "leaveTime": leave_time,
        "authInfo": {"cardNo": "0C987123", "facePictures": ["AAAA"]},
    }


def _defense(*visitors: dict) -> AsyncMock:
    defense = AsyncMock()
    defense.settings = SimpleNamespace(enabled=True)
    defense.iter_finalized_visitors_between = AsyncMock(return_value=list(visitors))
    return defense


def test_build_leave_payload_uses_only_history_fields() -> None:
    payload = build_leave_payload(_visitor())

    assert payload["event"] == "visitor_leave"
    assert payload["visitorId"] == "1842"
    assert payload["leaveTime"] == "1000000"
    assert payload["trigger"] == "poll"
    assert payload["channelId"] is None
    assert "facePictures" not in payload


@pytest.mark.parametrize(
    ("status", "leave_time", "expected"),
    [
        ("2", "1000000", True),
        ("2", "0", False),
        ("2", "", False),
        ("4", "1000000", False),
        ("1", "1000000", False),
    ],
)
def test_finalized_visitor_requires_status_2_and_leave_time(
    status: str,
    leave_time: str,
    expected: bool,
) -> None:
    assert is_finalized_visitor(_visitor(status=status, leave_time=leave_time)) is expected


@pytest.mark.asyncio
async def test_generic_alarm_is_always_ignored() -> None:
    defense = AsyncMock()
    result = await process_visitor_leave_body(
        {
            "sourceName": "ACESSO SERVIÇO SAIDA",
            "alarmType": "13104",
            "remark": '{"userId":"12345678901"}',
        },
        defense_client=defense,
        settings=VisitLeaveSettings(webhook_url="https://example.test/hook"),
    )

    assert result == {
        "status": "ignored",
        "processed": 0,
        "skipped": 1,
        "forwarded": 0,
    }
    defense.assert_not_awaited()


@pytest.mark.asyncio
async def test_poll_logs_and_posts_only_new_record(tmp_path, monkeypatch) -> None:
    state_path = tmp_path / "visitor_leave_state.json"
    defense = _defense(_visitor())
    logged: list[dict] = []
    forward = AsyncMock(return_value=True)
    monkeypatch.setattr(visitor_leave_module.time, "time", lambda: 1_000_100)
    monkeypatch.setattr(visitor_leave_module, "log_visitor_leave", logged.append)
    monkeypatch.setattr(visitor_leave_module, "forward_visitor_leave", forward)
    settings = VisitLeaveSettings(
        webhook_url="https://example.test/hook",
        state_path=str(state_path),
    )

    first = await poll_visitor_leave_history(
        defense_client=defense,
        settings=settings,
    )
    second = await poll_visitor_leave_history(
        defense_client=defense,
        settings=settings,
    )

    assert first["processed"] == 1
    assert first["forwarded"] == 1
    assert second["processed"] == 0
    assert len(logged) == 1
    forward.assert_awaited_once()
    state = json.loads(state_path.read_text(encoding="utf-8"))
    assert state["version"] == 2
    assert state["last_poll_ts"] == 1_000_100
    assert state["emitted"] == {"1842|1000000": 1000000}
    assert not (tmp_path / ".visitor_leave_state.json.tmp").exists()


@pytest.mark.asyncio
async def test_persistent_state_deduplicates_after_restart(tmp_path, monkeypatch) -> None:
    state_path = tmp_path / "visitor_leave_state.json"
    monkeypatch.setattr(visitor_leave_module.time, "time", lambda: 1_000_100)
    log = Mock()
    forward = AsyncMock(return_value=True)
    monkeypatch.setattr(visitor_leave_module, "log_visitor_leave", log)
    monkeypatch.setattr(visitor_leave_module, "forward_visitor_leave", forward)
    settings = VisitLeaveSettings(
        webhook_url="https://example.test/hook",
        state_path=str(state_path),
    )

    await poll_visitor_leave_history(
        defense_client=_defense(_visitor()),
        settings=settings,
    )
    restarted = await poll_visitor_leave_history(
        defense_client=_defense(_visitor()),
        settings=settings,
    )

    assert restarted["processed"] == 0
    log.assert_called_once()
    forward.assert_awaited_once()


@pytest.mark.asyncio
async def test_empty_webhook_url_logs_without_post(tmp_path, monkeypatch) -> None:
    monkeypatch.setattr(visitor_leave_module.time, "time", lambda: 1_000_100)
    logged: list[dict] = []
    monkeypatch.setattr(visitor_leave_module, "log_visitor_leave", logged.append)
    settings = VisitLeaveSettings(state_path=str(tmp_path / "state.json"))

    result = await poll_visitor_leave_history(
        defense_client=_defense(_visitor()),
        settings=settings,
    )

    assert result["processed"] == 1
    assert result["forwarded"] == 0
    assert len(logged) == 1


@pytest.mark.asyncio
async def test_poll_failure_does_not_advance_watermark(tmp_path, monkeypatch) -> None:
    state_path = tmp_path / "state.json"
    original = {"version": 2, "last_poll_ts": 900000, "emitted": {}}
    state_path.write_text(json.dumps(original), encoding="utf-8")
    monkeypatch.setattr(visitor_leave_module.time, "time", lambda: 1_000_100)
    defense = _defense()
    defense.iter_finalized_visitors_between.side_effect = RuntimeError("offline")

    result = await poll_visitor_leave_history(
        defense_client=defense,
        settings=VisitLeaveSettings(state_path=str(state_path)),
    )

    assert result["status"] == "error"
    assert json.loads(state_path.read_text(encoding="utf-8")) == original


@pytest.mark.asyncio
async def test_poll_queries_arrival_lookback_not_last_poll(tmp_path, monkeypatch) -> None:
    state_path = tmp_path / "state.json"
    state_path.write_text(
        json.dumps({"version": 2, "last_poll_ts": 1_000_050, "emitted": {}}),
        encoding="utf-8",
    )
    monkeypatch.setattr(visitor_leave_module.time, "time", lambda: 1_000_100)
    defense = _defense()

    await poll_visitor_leave_history(
        defense_client=defense,
        settings=VisitLeaveSettings(state_path=str(state_path)),
    )

    args = defense.iter_finalized_visitors_between.await_args.args
    assert args[0] == 1_000_100 - visitor_leave_module.HISTORY_LOOKBACK_SECONDS


@pytest.mark.asyncio
async def test_poll_emits_leave_when_visitor_arrived_hours_earlier(
    tmp_path, monkeypatch
) -> None:
    monkeypatch.setattr(visitor_leave_module.time, "time", lambda: 1_000_100)
    logged: list[dict] = []
    monkeypatch.setattr(visitor_leave_module, "log_visitor_leave", logged.append)
    visitor = _visitor(leave_time="1000090")
    visitor["arrivalTime"] = "990000"

    result = await poll_visitor_leave_history(
        defense_client=_defense(visitor),
        settings=VisitLeaveSettings(state_path=str(tmp_path / "state.json")),
    )

    assert result["processed"] == 1
    assert logged[0]["visitorId"] == "1842"


@pytest.mark.asyncio
async def test_history_pagination_uses_pages_of_100() -> None:
    client = DefenseIAClient(
        DefenseIASettings(
            server_url="http://defense.test",
            username="u",
            password="p",
        )
    )
    page_one = [_visitor(str(index)) for index in range(100)]
    page_two = [_visitor("last")]
    client._fetch_visitor_history = AsyncMock(side_effect=[page_one, page_two])

    result = await client.iter_finalized_visitors_between(900000, 1100000)

    assert len(result) == 101
    assert client._fetch_visitor_history.await_count == 2
    assert client._fetch_visitor_history.await_args_list[0].kwargs["page"] == 1
    assert client._fetch_visitor_history.await_args_list[1].kwargs["page"] == 2
    assert client._fetch_visitor_history.await_args_list[0].kwargs["page_size"] == 100
    assert client._fetch_visitor_history.await_args_list[0].kwargs["strict"] is True
    await client.close()


@pytest.mark.asyncio
async def test_poll_loop_runs_immediately_before_sleep(monkeypatch) -> None:
    poll = AsyncMock()
    monkeypatch.setattr(visitor_leave_module, "poll_visitor_leave_history", poll)

    async def stop_after_first_poll(_: float) -> None:
        raise asyncio.CancelledError

    monkeypatch.setattr(visitor_leave_module.asyncio, "sleep", stop_after_first_poll)
    with pytest.raises(asyncio.CancelledError):
        await visitor_leave_poll_loop(
            _defense(),
            VisitLeaveSettings(poll_interval_seconds=300),
        )

    poll.assert_awaited_once()


@pytest.mark.asyncio
async def test_defense_events_route_returns_200_without_creating_leave(
    api_client: httpx.AsyncClient,
) -> None:
    app.dependency_overrides[get_visit_leave_settings] = lambda: VisitLeaveSettings()
    try:
        response = await api_client.post(
            "/defense/events",
            json={
                "sourceName": "ACESSO SERVIÇO SAIDA",
                "remark": '{"userId":"12345678901"}',
            },
        )
    finally:
        app.dependency_overrides.pop(get_visit_leave_settings, None)

    assert response.status_code == 200
    assert response.json()["status"] == "ignored"
    assert response.json()["processed"] == 0


@pytest.mark.asyncio
async def test_poll_persists_sqlite_and_ignores_duplicate(tmp_path, monkeypatch) -> None:
    from sqlalchemy import create_engine
    from sqlalchemy.orm import sessionmaker

    from src.core.database import Base
    from src.models.visitor_leave import VisitorLeaveEvent
    from src.services import visitor_leave_store as store

    engine = create_engine(
        f"sqlite:///{tmp_path / 'middleware.db'}",
        connect_args={"check_same_thread": False},
    )
    Session = sessionmaker(autocommit=False, autoflush=False, bind=engine)
    Base.metadata.create_all(bind=engine)
    monkeypatch.setattr(store, "SessionLocal", Session)
    monkeypatch.setattr(visitor_leave_module, "persist_visitor_leave", store.persist_visitor_leave)
    monkeypatch.setattr(visitor_leave_module.time, "time", lambda: 1_000_100)
    monkeypatch.setattr(visitor_leave_module, "log_visitor_leave", lambda payload: None)

    settings = VisitLeaveSettings(state_path=str(tmp_path / "state.json"))
    first = await poll_visitor_leave_history(
        defense_client=_defense(_visitor()),
        settings=settings,
    )
    second = await poll_visitor_leave_history(
        defense_client=_defense(_visitor()),
        settings=settings,
    )

    assert first["processed"] == 1
    assert second["processed"] == 0
    with Session() as session:
        rows = session.query(VisitorLeaveEvent).all()
        assert len(rows) == 1
        assert rows[0].visitor_id == "1842"
        assert rows[0].leave_time == 1_000_000
