import json
import logging
from unittest.mock import AsyncMock

import httpx
import pytest

from src.api.dependencies import get_visit_leave_settings
from src.main import app
from src.services.visitor_leave import (
    VisitLeaveSettings,
    build_leave_payload,
    extract_event_lookup_keys,
    is_exit_direction,
    is_visitor_leave_event,
    iter_event_records,
    process_visitor_leave_body,
    reset_leave_dedup,
    unwrap_event_record,
)


@pytest.fixture(autouse=True)
def _reset_dedup() -> None:
    reset_leave_dedup()
    yield
    reset_leave_dedup()


def test_iter_and_unwrap_nested_alarm() -> None:
    body = {
        "data": {
            "personId": "99",
            "inAndOut": "2",
            "channelId": "1000054$7$0$0",
        }
    }
    records = iter_event_records(body)
    assert len(records) == 1
    flat = unwrap_event_record(records[0])
    lookup = extract_event_lookup_keys(flat)
    assert lookup["person_id"] == "99"
    assert is_exit_direction(flat) is True


def test_is_visitor_leave_requires_visitor_and_exit() -> None:
    visitor = {"visitorId": "1", "status": "1", "leaveTime": "0"}
    assert is_visitor_leave_event({"inAndOut": "2"}, visitor) is True
    assert is_visitor_leave_event({"inAndOut": "1"}, visitor) is False
    assert is_visitor_leave_event({"inAndOut": "2"}, None) is False
    left = {"visitorId": "1", "status": "2", "leaveTime": "123"}
    assert is_visitor_leave_event({}, left) is True


def test_build_leave_payload_omits_face() -> None:
    visitor = {
        "visitorId": "1842",
        "personId": "90011",
        "status": "2",
        "visitorName": "Maria Silva",
        "idNum": "12345678900",
        "remark": "00271368992672000",
        "visitedName": "EVB",
        "arrivalTime": "1692361501",
        "expectLeaveTime": "1723994674",
        "leaveTime": "1723994000",
        "authInfo": {"cardNo": "0C987123", "facePictures": ["AAAA"]},
    }
    payload = build_leave_payload(
        visitor=visitor,
        record={"channelId": "1000054$7$0$0"},
        lookup=extract_event_lookup_keys({"channelId": "1000054$7$0$0"}),
    )
    assert payload["event"] == "visitor_leave"
    assert payload["visitorId"] == "1842"
    assert payload["remark"] == "00271368992672000"
    assert "facePictures" not in payload
    assert payload["channelId"] == "1000054$7$0$0"


def test_visitor_leave_log_writes_json_line(tmp_path) -> None:
    from src.services.visitor_leave import log_visitor_leave

    log_path = tmp_path / "visitor_leave.log"
    handler = logging.FileHandler(log_path, encoding="utf-8")
    handler.setFormatter(logging.Formatter("%(message)s"))
    leave_logger = logging.getLogger("biodoc_intelbras.visitor_leave")
    previous = list(leave_logger.handlers)
    leave_logger.handlers = [handler]
    try:
        log_visitor_leave({"event": "visitor_leave", "visitorId": "1"})
        handler.flush()
        parsed = json.loads(log_path.read_text(encoding="utf-8").strip())
        assert parsed["visitorId"] == "1"
    finally:
        leave_logger.handlers = previous
        handler.close()


@pytest.mark.asyncio
async def test_process_forwards_and_skips_staff(monkeypatch: pytest.MonkeyPatch) -> None:
    defense = AsyncMock()
    defense.find_visitor_for_leave_event = AsyncMock(
        side_effect=[
            {
                "visitorId": "1",
                "personId": "9",
                "status": "2",
                "visitorName": "Maria",
                "idNum": "123",
                "leaveTime": "100",
                "remark": "card1",
            },
            None,
        ]
    )
    settings = VisitLeaveSettings(
        webhook_url="https://destino.test/baixa",
        webhook_token="tok",
        timeout_seconds=2.0,
    )
    captured: dict[str, object] = {}

    async def handler(request: httpx.Request) -> httpx.Response:
        captured["url"] = str(request.url)
        captured["body"] = json.loads(request.content.decode())
        captured["auth"] = request.headers.get("authorization")
        return httpx.Response(200, json={"ok": True})

    real_async_client = httpx.AsyncClient

    class _FakeAsyncClient:
        def __init__(self, *args: object, **kwargs: object) -> None:
            self._client = real_async_client(transport=httpx.MockTransport(handler))

        async def __aenter__(self) -> httpx.AsyncClient:
            return self._client

        async def __aexit__(self, *args: object) -> None:
            await self._client.aclose()

    monkeypatch.setattr("src.services.visitor_leave.httpx.AsyncClient", _FakeAsyncClient)

    first = await process_visitor_leave_body(
        {"personId": "9", "inAndOut": "2"},
        defense_client=defense,
        settings=settings,
    )
    second = await process_visitor_leave_body(
        {"personId": "staff", "inAndOut": "2"},
        defense_client=defense,
        settings=settings,
    )
    assert first["processed"] == 1
    assert first["forwarded"] == 1
    assert second["processed"] == 0
    assert captured["url"] == "https://destino.test/baixa"
    assert captured["auth"] == "Bearer tok"
    assert captured["body"]["visitorId"] == "1"


@pytest.mark.asyncio
async def test_process_deduplicates_same_leave() -> None:
    defense = AsyncMock()
    defense.find_visitor_for_leave_event = AsyncMock(
        return_value={
            "visitorId": "1",
            "personId": "9",
            "status": "2",
            "leaveTime": "100",
            "visitorName": "Maria",
        }
    )
    settings = VisitLeaveSettings()
    first = await process_visitor_leave_body(
        {"personId": "9", "inAndOut": "2"},
        defense_client=defense,
        settings=settings,
    )
    second = await process_visitor_leave_body(
        {"personId": "9", "inAndOut": "2"},
        defense_client=defense,
        settings=settings,
    )
    assert first["processed"] == 1
    assert second["processed"] == 0
    assert second["skipped"] == 1


@pytest.mark.asyncio
async def test_defense_events_route_returns_ok(
    api_client: httpx.AsyncClient,
    defense_client_mock: AsyncMock,
) -> None:
    defense_client_mock.find_visitor_for_leave_event = AsyncMock(
        return_value={
            "visitorId": "7",
            "personId": "8",
            "status": "2",
            "leaveTime": "55",
            "visitorName": "Joao",
            "idNum": "111",
        }
    )
    app.dependency_overrides[get_visit_leave_settings] = lambda: VisitLeaveSettings()
    try:
        response = await api_client.post(
            "/defense/events",
            json={"personId": "8", "inAndOut": "2", "channelId": "1000001$7$0$0"},
        )
    finally:
        app.dependency_overrides.pop(get_visit_leave_settings, None)
    assert response.status_code == 200
    body = response.json()
    assert body["status"] == "ok"
    assert body["processed"] == 1
