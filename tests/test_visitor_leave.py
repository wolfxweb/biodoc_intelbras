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
    ForwardResult,
    VisitLeaveSettings,
    _error_text,
    build_leave_payload,
    is_finalized_visitor,
    poll_visitor_leave_history,
    process_visitor_leave_body,
    resend_stored_leave,
    retry_unsent_visitor_leaves,
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


def test_stored_timeout_error_is_short_on_screen() -> None:
    raw = (
        "ConnectTimeout | ConnectTimeout | TimeoutError | "
        "CancelledError: Cancelled via cancel scope 7fabef0a7e30; "
        "reason: deadline exceeded: destino não respondeu no timeout, sem corpo HTTP"
    )
    text = visitor_leave_module.format_stored_forward_error(raw)
    assert text == "Tempo esgotado ao conectar no destino. Ele não chegou a responder."
    assert "cancel scope" not in text


def test_error_text_keeps_exception_type_when_message_is_empty() -> None:
    class SilentError(Exception):
        def __str__(self) -> str:
            return ""

    assert _error_text(SilentError()) == "Falha de rede (SilentError)."


def test_vista_public_url_keeps_host_and_token_path(monkeypatch) -> None:
    monkeypatch.setattr(
        visitor_leave_module,
        "_traefik_ip",
        lambda: "10.0.1.2",
    )
    url, headers, extensions = visitor_leave_module._delivery_target(
        "https://vista.wolfx.com.br/api/v1/events"
    )
    assert url == "https://10.0.1.2/api/v1/events"
    assert headers["Host"] == "vista.wolfx.com.br"
    assert extensions["sni_hostname"] == "vista.wolfx.com.br"


@pytest.mark.asyncio
async def test_forward_saves_http_response_body() -> None:
    def handler(request: httpx.Request) -> httpx.Response:
        return httpx.Response(422, json={"message": "visitorId já recebido"})

    transport = httpx.MockTransport(handler)
    real_client = httpx.AsyncClient

    class _Client(real_client):
        def __init__(self, *args, **kwargs):
            kwargs["transport"] = transport
            super().__init__(*args, **kwargs)

    monkeypatch = pytest.MonkeyPatch()
    monkeypatch.setattr(visitor_leave_module.httpx, "AsyncClient", _Client)
    try:
        result = await visitor_leave_module.forward_visitor_leave(
            {"event": "visitor_leave", "visitorId": "9", "leaveTime": "5"},
            VisitLeaveSettings(webhook_url="https://example.test/hook"),
        )
    finally:
        monkeypatch.undo()

    assert result.sent is False
    assert result.attempted is True
    assert result.status_code == 422
    assert "visitorId já recebido" in (result.error or "")


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


def _bind_store(tmp_path, monkeypatch):
    from sqlalchemy import create_engine
    from sqlalchemy.orm import sessionmaker

    from src.core.database import Base
    from src.services import visitor_leave_store as store

    engine = create_engine(
        f"sqlite:///{tmp_path / 'middleware.db'}",
        connect_args={"check_same_thread": False},
    )
    Session = sessionmaker(autocommit=False, autoflush=False, bind=engine)
    Base.metadata.create_all(bind=engine)
    monkeypatch.setattr(store, "engine", engine)
    monkeypatch.setattr(store, "SessionLocal", Session)
    return store, Session


@pytest.mark.asyncio
async def test_resend_posts_stored_payload(tmp_path, monkeypatch) -> None:
    store, Session = _bind_store(tmp_path, monkeypatch)
    stored = {
        "event": "visitor_leave",
        "visitorId": "9",
        "leaveTime": "5",
        "loggedAt": "kept",
    }
    with Session() as session:
        session.add(
            __import__("src.models.visitor_leave", fromlist=["VisitorLeaveEvent"]).VisitorLeaveEvent(
                visitor_id="9",
                leave_time=5,
                logged_at="2026-10-03T11:00:00-03:00",
                forwarded=False,
                payload_json=json.dumps(stored),
            )
        )
        session.commit()

    captured: list[dict] = []

    async def fake_forward(payload, settings):
        captured.append(payload)
        return ForwardResult(sent=True, attempted=True, status_code=200)

    monkeypatch.setattr(visitor_leave_module, "forward_visitor_leave", fake_forward)
    result = await resend_stored_leave(
        visitor_id="9",
        leave_time=5,
        payload_json=json.dumps(stored),
        settings=VisitLeaveSettings(webhook_url="https://example.test/hook"),
    )

    assert result.sent is True
    assert captured == [stored]
    with Session() as session:
        from src.models.visitor_leave import VisitorLeaveEvent

        row = session.query(VisitorLeaveEvent).one()
        assert row.forwarded is True
        assert row.payload_json == json.dumps(stored)
        assert row.forward_attempts == 0


@pytest.mark.asyncio
async def test_resend_without_url_does_not_count_attempt(tmp_path, monkeypatch) -> None:
    store, Session = _bind_store(tmp_path, monkeypatch)
    payload = '{"event":"visitor_leave","visitorId":"9","leaveTime":"5"}'
    with Session() as session:
        from src.models.visitor_leave import VisitorLeaveEvent

        session.add(
            VisitorLeaveEvent(
                visitor_id="9",
                leave_time=5,
                forwarded=False,
                payload_json=payload,
            )
        )
        session.commit()

    called = False

    async def fake_forward(payload, settings):
        nonlocal called
        called = True
        return ForwardResult(sent=True, attempted=True)

    monkeypatch.setattr(visitor_leave_module, "forward_visitor_leave", fake_forward)
    result = await resend_stored_leave(
        visitor_id="9",
        leave_time=5,
        payload_json=payload,
        settings=VisitLeaveSettings(),
    )

    assert result.attempted is False
    assert called is False
    with Session() as session:
        from src.models.visitor_leave import VisitorLeaveEvent

        row = session.query(VisitorLeaveEvent).one()
        assert row.forward_attempts == 0
        assert row.payload_json == payload


@pytest.mark.asyncio
async def test_retry_skips_sent_and_rows_outside_window(tmp_path, monkeypatch) -> None:
    from datetime import datetime
    from zoneinfo import ZoneInfo

    store, Session = _bind_store(tmp_path, monkeypatch)
    now = datetime(2026, 10, 3, 12, 0, tzinfo=ZoneInfo("America/Sao_Paulo"))
    monkeypatch.setattr(store, "_clock", lambda: now)
    from src.models.visitor_leave import VisitorLeaveEvent

    def add(visitor_id: str, leave_time: int, logged_at: str, forwarded: bool, marker: str) -> None:
        with Session() as session:
            session.add(
                VisitorLeaveEvent(
                    visitor_id=visitor_id,
                    leave_time=leave_time,
                    logged_at=logged_at,
                    forwarded=forwarded,
                    payload_json=json.dumps(
                        {
                            "event": "visitor_leave",
                            "visitorId": visitor_id,
                            "leaveTime": str(leave_time),
                            "loggedAt": marker,
                        }
                    ),
                )
            )
            session.commit()

    add("recent", 30, "2026-10-03T11:00:00-03:00", False, "recent")
    add("old", 20, "2026-10-03T03:00:00-03:00", False, "old")
    add("sent", 10, "2026-10-03T11:30:00-03:00", True, "sent")

    captured: list[str] = []

    async def fake_forward(payload, settings):
        captured.append(payload["loggedAt"])
        return ForwardResult(sent=False, attempted=True, error="HTTP 503: down", status_code=503)

    monkeypatch.setattr(visitor_leave_module, "forward_visitor_leave", fake_forward)
    summary = await retry_unsent_visitor_leaves(
        VisitLeaveSettings(
            webhook_url="https://example.test/hook",
            retry_window_hours=6,
        )
    )

    assert summary["failed"] == 1
    assert captured == ["recent"]
    with Session() as session:
        recent = session.query(VisitorLeaveEvent).filter_by(visitor_id="recent").one()
        old = session.query(VisitorLeaveEvent).filter_by(visitor_id="old").one()
        assert recent.forward_attempts == 1
        assert recent.last_error == "HTTP 503: down"
        assert old.forward_attempts == 0
        assert '"loggedAt": "recent"' in recent.payload_json


@pytest.mark.asyncio
async def test_auto_retry_stops_at_max_attempts_and_manual_does_not(tmp_path, monkeypatch) -> None:
    from datetime import datetime
    from zoneinfo import ZoneInfo

    store, Session = _bind_store(tmp_path, monkeypatch)
    now = datetime(2026, 10, 3, 12, 0, tzinfo=ZoneInfo("America/Sao_Paulo"))
    monkeypatch.setattr(store, "_clock", lambda: now)
    from src.models.visitor_leave import VisitorLeaveEvent

    payload = json.dumps(
        {
            "event": "visitor_leave",
            "visitorId": "capped",
            "leaveTime": "40",
            "loggedAt": "capped",
        }
    )
    with Session() as session:
        session.add(
            VisitorLeaveEvent(
                visitor_id="capped",
                leave_time=40,
                logged_at="2026-10-03T11:00:00-03:00",
                forwarded=False,
                forward_attempts=6,
                payload_json=payload,
            )
        )
        session.commit()

    captured: list[str] = []

    async def fake_forward(body, settings):
        captured.append(body["visitorId"])
        return ForwardResult(sent=True, attempted=True, status_code=200)

    monkeypatch.setattr(visitor_leave_module, "forward_visitor_leave", fake_forward)
    settings = VisitLeaveSettings(
        webhook_url="https://example.test/hook",
        retry_window_hours=6,
        retry_max_attempts=6,
    )
    summary = await retry_unsent_visitor_leaves(settings)
    assert summary["sent"] == 0
    assert captured == []

    result = await resend_stored_leave(
        visitor_id="capped",
        leave_time=40,
        payload_json=payload,
        settings=settings,
    )
    assert result.sent is True
    assert captured == ["capped"]
