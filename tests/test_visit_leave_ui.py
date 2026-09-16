import httpx
import pytest
from sqlalchemy.orm import Session

from src.models.visitor_leave import WEBHOOK_URL_SETTING_KEY, VisitorLeaveEvent
from src.services.visitor_leave_store import get_setting


async def _login(client: httpx.AsyncClient, password: str = "admin-token") -> httpx.Response:
    return await client.post(
        "/login",
        data={"password": password},
        follow_redirects=False,
    )


def _insert_event(db: Session, **overrides) -> VisitorLeaveEvent:
    payload = {
        "event": "visitor_leave",
        "visitorId": overrides.get("visitor_id", "1842"),
        "visitorName": overrides.get("visitor_name", "Maria Silva"),
        "visitedName": overrides.get("visited_name", "EVB"),
        "leaveTime": str(overrides.get("leave_time", 1_000_000)),
    }
    row = VisitorLeaveEvent(
        visitor_id=payload["visitorId"],
        visitor_name=payload["visitorName"],
        id_num=overrides.get("id_num", "123"),
        visited_name=payload["visitedName"],
        arrival_time=overrides.get("arrival_time", 999_000),
        leave_time=int(payload["leaveTime"]),
        logged_at="2026-09-16T15:22:02-03:00",
        forwarded=overrides.get("forwarded", False),
        payload_json='{"event":"visitor_leave"}',
    )
    db.add(row)
    db.commit()
    db.refresh(row)
    return row


@pytest.mark.asyncio
async def test_root_redirects_to_login_without_cookie(
    api_client: httpx.AsyncClient,
) -> None:
    response = await api_client.get("/", follow_redirects=False)
    assert response.status_code == 303
    assert response.headers["location"] == "/login"


@pytest.mark.asyncio
async def test_login_sets_cookie_and_lists_events(
    api_client: httpx.AsyncClient,
    db_session: Session,
) -> None:
    _insert_event(db_session)

    denied = await _login(api_client, "wrong-password")
    assert denied.status_code == 303
    assert denied.headers["location"] == "/login?error=1"

    ok = await _login(api_client)
    assert ok.status_code == 303
    assert ok.headers["location"] == "/"
    assert "visit_leave_ui" in api_client.cookies

    listed = await api_client.get("/")
    assert listed.status_code == 200
    assert "Maria Silva" in listed.text
    assert "EVB" in listed.text
    assert "URL de destino" in listed.text


@pytest.mark.asyncio
async def test_list_filters_by_name_and_setor(
    api_client: httpx.AsyncClient,
    db_session: Session,
) -> None:
    _insert_event(
        db_session,
        visitor_id="1",
        visitor_name="Ana Souza",
        visited_name="CENTRAL",
        leave_time=1_000_100,
    )
    _insert_event(
        db_session,
        visitor_id="2",
        visitor_name="Bruno Lima",
        visited_name="EVB",
        leave_time=1_000_200,
    )
    await _login(api_client)

    by_name = await api_client.get("/", params={"name": "Ana"})
    assert "Ana Souza" in by_name.text
    assert "Bruno Lima" not in by_name.text

    by_setor = await api_client.get("/", params={"setor": "EVB"})
    assert "Bruno Lima" in by_setor.text
    assert "Ana Souza" not in by_setor.text


@pytest.mark.asyncio
async def test_save_webhook_url(
    api_client: httpx.AsyncClient,
    db_session: Session,
) -> None:
    await _login(api_client)
    response = await api_client.post(
        "/settings/webhook-url",
        data={"webhook_url": "https://example.test/hook"},
        follow_redirects=False,
    )
    assert response.status_code == 303
    assert "saved=1" in response.headers["location"]
    assert get_setting(WEBHOOK_URL_SETTING_KEY, db=db_session) == "https://example.test/hook"


@pytest.mark.asyncio
async def test_status_and_docs_skip_ui_login(api_client: httpx.AsyncClient) -> None:
    from src.main import app
    from unittest.mock import AsyncMock
    from src.services.defense_ia_client import DefenseIASettings

    mock_defense = AsyncMock()
    mock_defense.settings = DefenseIASettings(
        server_url="http://defense.test",
        username="u",
        password="p",
        api_mode="brms",
    )
    mock_defense.is_ready = True
    app.state.defense_client = mock_defense

    status = await api_client.get("/status")
    docs = await api_client.get("/docs")
    assert status.status_code == 200
    assert docs.status_code == 200
