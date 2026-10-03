import httpx
import pytest
from sqlalchemy.orm import Session

from src.services.visitor_leave import ForwardResult

from src.models.visitor_leave import (
    WEBHOOK_TOKEN_SETTING_KEY,
    WEBHOOK_URL_SETTING_KEY,
    VisitorLeaveEvent,
)
from src.services.visitor_leave_store import get_setting, set_webhook_url


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

    login_page = await api_client.get("/login")
    assert "Desenvolvido por" in login_page.text
    assert "Unimed" in login_page.text
    assert 'href="https://wolfx.com.br"' in login_page.text

    ok = await _login(api_client)
    assert ok.status_code == 303
    assert ok.headers["location"] == "/"
    assert "visit_leave_ui" in api_client.cookies

    listed = await api_client.get("/")
    assert listed.status_code == 200
    assert "Unimed" in listed.text
    assert "Maria Silva" in listed.text
    assert "wolfx.com.br" in listed.text
    assert 'href="https://wolfx.com.br"' in listed.text
    assert "EVB" in listed.text
    assert "Enviado para URL destino" in listed.text
    assert "URL de destino" in listed.text
    assert 'href="/docs"' in listed.text


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
async def test_list_filters_unsent(
    api_client: httpx.AsyncClient,
    db_session: Session,
) -> None:
    _insert_event(
        db_session,
        visitor_id="1",
        visitor_name="Pendente Silva",
        leave_time=1_000_100,
        forwarded=False,
    )
    _insert_event(
        db_session,
        visitor_id="2",
        visitor_name="Jafoi Lima",
        leave_time=1_000_200,
        forwarded=True,
    )
    await _login(api_client)

    page = await api_client.get("/", params={"sent": "no"})
    assert "Pendente Silva" in page.text
    assert "Jafoi Lima" not in page.text
    assert "Reenvios" in page.text
    assert "Não enviados" in page.text
    assert "Com reenvio" in page.text
    assert "<th>Último erro</th>" not in page.text
    assert 'class="badge badge-pending">Não enviado</span>' in page.text


@pytest.mark.asyncio
async def test_list_filters_rows_with_resend(
    api_client: httpx.AsyncClient,
    db_session: Session,
) -> None:
    retried = _insert_event(
        db_session,
        visitor_id="1",
        visitor_name="Retentou Silva",
        leave_time=1_000_100,
        forwarded=True,
    )
    retried.forward_attempts = 2
    _insert_event(
        db_session,
        visitor_id="2",
        visitor_name="Zerou Lima",
        leave_time=1_000_200,
        forwarded=False,
    )
    db_session.commit()
    await _login(api_client)

    page = await api_client.get("/", params={"sent": "retry"})
    assert "Retentou Silva" in page.text
    assert "Zerou Lima" not in page.text
    assert 'value="retry" selected' in page.text


@pytest.mark.asyncio
async def test_modal_hides_raw_timeout_chain(
    api_client: httpx.AsyncClient,
    db_session: Session,
) -> None:
    row = _insert_event(db_session, visitor_name="Timeout Silva", forwarded=False)
    row.forward_attempts = 1
    row.last_error = (
        "ConnectTimeout | TimeoutError | CancelledError: "
        "Cancelled via cancel scope 7fabef0a7e30; reason: deadline exceeded"
    )
    db_session.commit()
    await _login(api_client)

    page = await api_client.get("/")
    assert "Tempo esgotado ao conectar no destino" not in page.text
    assert "cancel scope" not in page.text
    assert "Último erro" not in page.text
    assert "Sem resposta" not in page.text
    assert 'class="badge badge-pending">Não enviado</span>' in page.text
    assert 'id="meta-attempts"' in page.text
    assert ">Reenvios</span>" in page.text
    assert "Payload" in page.text


@pytest.mark.asyncio
async def test_resend_selected_and_all_unsent(
    api_client: httpx.AsyncClient,
    db_session: Session,
    monkeypatch: pytest.MonkeyPatch,
) -> None:
    calls: list[str] = []

    async def fake_resend(**kwargs):
        calls.append(kwargs["payload_json"])
        return ForwardResult(sent=True, attempted=True, status_code=200)

    monkeypatch.setattr(
        "src.api.routes.visit_leave_ui.resend_stored_leave",
        fake_resend,
    )
    pending = _insert_event(
        db_session,
        visitor_id="1",
        visitor_name="Pendente",
        leave_time=1_000_100,
        forwarded=False,
    )
    other = _insert_event(
        db_session,
        visitor_id="3",
        visitor_name="Outro",
        leave_time=1_000_300,
        forwarded=False,
    )
    sent = _insert_event(
        db_session,
        visitor_id="2",
        visitor_name="Enviado",
        leave_time=1_000_200,
        forwarded=True,
    )
    pending.payload_json = '{"event":"visitor_leave","visitorId":"1"}'
    other.payload_json = '{"event":"visitor_leave","visitorId":"3"}'
    sent.payload_json = '{"event":"visitor_leave","visitorId":"2"}'
    db_session.commit()
    set_webhook_url("https://example.test/hook", db=db_session)

    denied = await api_client.post(
        "/visit-leave/resend",
        data={"scope": "selected", "ids": str(pending.id)},
        follow_redirects=False,
    )
    assert denied.status_code == 303
    assert denied.headers["location"] == "/login"

    await _login(api_client)
    one = await api_client.post(
        "/visit-leave/resend",
        data={"scope": "selected", "ids": str(pending.id)},
        follow_redirects=True,
    )
    assert one.status_code == 200
    assert calls == ['{"event":"visitor_leave","visitorId":"1"}']
    assert "1 enviado(s), 0 falha(s)" in one.text

    calls.clear()
    everyone = await api_client.post(
        "/visit-leave/resend",
        data={"scope": "unsent"},
        follow_redirects=True,
    )
    assert everyone.status_code == 200
    assert sorted(calls) == [
        '{"event":"visitor_leave","visitorId":"1"}',
        '{"event":"visitor_leave","visitorId":"3"}',
    ]


@pytest.mark.asyncio
async def test_save_webhook_url(
    api_client: httpx.AsyncClient,
    db_session: Session,
) -> None:
    await _login(api_client)
    response = await api_client.post(
        "/settings/webhook-url",
        data={
            "webhook_url": "https://example.test/hook",
            "webhook_token": "secret-token",
        },
        follow_redirects=False,
    )
    assert response.status_code == 303
    assert "saved=1" in response.headers["location"]
    assert get_setting(WEBHOOK_URL_SETTING_KEY, db=db_session) == "https://example.test/hook"
    assert get_setting(WEBHOOK_TOKEN_SETTING_KEY, db=db_session) == "secret-token"


@pytest.mark.asyncio
async def test_manuals_require_login_and_render_existing_docs(
    api_client: httpx.AsyncClient,
) -> None:
    denied = await api_client.get("/manuais", follow_redirects=False)
    assert denied.status_code == 303
    assert denied.headers["location"] == "/login"

    await _login(api_client)
    index = await api_client.get("/manuais")
    assert index.status_code == 200
    assert "Middleware BIODOC" in index.text
    assert "Webhook opcional de baixa de visita" in index.text
    assert "URL de destino da baixa de visita" in index.text

    detail = await api_client.get("/manuais/middleware")
    assert detail.status_code == 200
    assert "Intelbras Defense IA" in detail.text
    assert "<h1>" in detail.text


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
