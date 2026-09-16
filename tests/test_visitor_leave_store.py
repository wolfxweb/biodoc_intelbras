import json
from pathlib import Path

from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker

from src.core.database import Base
from src.models.visitor_leave import VisitorLeaveEvent
from src.services import visitor_leave_store as store


def _payload(**overrides) -> dict:
    body = {
        "event": "visitor_leave",
        "loggedAt": "2026-09-16T15:22:02-03:00",
        "visitorId": "1842",
        "visitorName": "Maria Silva",
        "idNum": "12345678900",
        "visitedName": "EVB",
        "arrivalTime": "999000",
        "leaveTime": "1000000",
    }
    body.update(overrides)
    return body


def _bind_tmp_db(tmp_path: Path, monkeypatch) -> sessionmaker:
    engine = create_engine(
        f"sqlite:///{tmp_path / 'middleware.db'}",
        connect_args={"check_same_thread": False},
    )
    Session = sessionmaker(autocommit=False, autoflush=False, bind=engine)
    monkeypatch.setattr(store, "engine", engine)
    monkeypatch.setattr(store, "SessionLocal", Session)
    Base.metadata.create_all(bind=engine)
    return Session


def test_persist_inserts_and_ignores_duplicate(tmp_path, monkeypatch) -> None:
    Session = _bind_tmp_db(tmp_path, monkeypatch)
    payload = _payload()

    assert store.persist_visitor_leave(payload) is True
    assert store.persist_visitor_leave(payload) is False

    with Session() as session:
        rows = session.query(VisitorLeaveEvent).all()
        assert len(rows) == 1
        assert rows[0].visitor_id == "1842"
        assert rows[0].leave_time == 1000000
        assert rows[0].forwarded is False


def test_persist_marks_forwarded_on_existing_row(tmp_path, monkeypatch) -> None:
    Session = _bind_tmp_db(tmp_path, monkeypatch)
    payload = _payload()

    store.persist_visitor_leave(payload, forwarded=False)
    assert store.persist_visitor_leave(payload, forwarded=True) is False

    with Session() as session:
        row = session.query(VisitorLeaveEvent).one()
        assert row.forwarded is True


def test_webhook_url_prefers_database_over_env(tmp_path, monkeypatch) -> None:
    _bind_tmp_db(tmp_path, monkeypatch)

    assert store.get_webhook_url("https://env.example/hook") == "https://env.example/hook"
    store.set_webhook_url("https://db.example/hook")
    assert store.get_webhook_url("https://env.example/hook") == "https://db.example/hook"


def test_import_logs_once_when_table_empty(tmp_path, monkeypatch) -> None:
    Session = _bind_tmp_db(tmp_path, monkeypatch)
    log_dir = tmp_path / "log"
    log_dir.mkdir()
    (log_dir / "visitor_leave.log").write_text(
        json.dumps(_payload(), ensure_ascii=False) + "\n",
        encoding="utf-8",
    )
    (log_dir / "visitor_leave.log.1").write_text(
        json.dumps(_payload(visitorId="1843", leaveTime="1000001"), ensure_ascii=False)
        + "\nnot-json\n",
        encoding="utf-8",
    )

    imported = store.import_visitor_leave_logs_if_empty(log_dir=log_dir)
    again = store.import_visitor_leave_logs_if_empty(log_dir=log_dir)

    assert imported == 2
    assert again == 0
    with Session() as session:
        assert session.query(VisitorLeaveEvent).count() == 2
