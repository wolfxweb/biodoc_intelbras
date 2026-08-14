from unittest.mock import AsyncMock

import base64

import httpx
import pytest
from sqlalchemy.orm import Session

from src.core.security import hash_token
from src.models.integration_source import IntegrationSource
from src.services.defense_ia_client import DefenseIAUnavailableError

_FAKE_JPEG_B64 = base64.b64encode(b"\xff\xd8\xff" + b"\x00" * 2048).decode()

VALID_SYNC_PAYLOAD = {
    "source": "biodoc",
    "operation": "upsert",
    "external_id": "123",
    "person": {"full_name": "Maria Silva", "document": "12345678900"},
    "defense": {
        "org_code": "recepção central",
    },
    "biometrics": {"face_image_base64": _FAKE_JPEG_B64},
}

VALID_SYNC_PAYLOAD_WITH_FACE = VALID_SYNC_PAYLOAD


def create_source(db_session: Session, token: str = "integration-token") -> None:
    db_session.add(
        IntegrationSource(
            name="biodoc",
            token_hash=hash_token(token),
            is_active=True,
        )
    )
    db_session.commit()


def integration_headers(token: str = "admin-token") -> dict[str, str]:
    return {"Authorization": f"Bearer {token}"}


@pytest.mark.asyncio
async def test_sync_person_returns_401_when_authorization_missing(
    api_client: httpx.AsyncClient,
    db_session: Session,
):
    create_source(db_session)

    response = await api_client.post(
        "/v1/person/sync",
        json=VALID_SYNC_PAYLOAD,
    )

    assert response.status_code == 401
    assert "Authorization ausente" in response.json()["detail"]


@pytest.mark.asyncio
async def test_sync_person_success(
    api_client: httpx.AsyncClient,
    db_session: Session,
    defense_client_mock: AsyncMock,
):
    create_source(db_session)

    response = await api_client.post(
        "/v1/person/sync",
        json=VALID_SYNC_PAYLOAD,
        headers=integration_headers(),
    )

    assert response.status_code == 200
    assert response.json() == {
        "status": "success",
        "message": "Visitante registrado no Intelbras Defense com sucesso",
        "visitor_id": "1",
        "person_id": "999",
    }
    defense_client_mock.sync_visitor.assert_awaited_once()
    call_args = defense_client_mock.sync_visitor.await_args
    assert call_args.args[1] == "recepção central"
    assert call_args.kwargs.get("entrance_ids") is None


@pytest.mark.asyncio
async def test_sync_rejects_missing_biometrics(
    api_client: httpx.AsyncClient,
    db_session: Session,
):
    create_source(db_session)
    payload = {
        "source": "biodoc",
        "operation": "upsert",
        "external_id": "123",
        "person": {"full_name": "Maria Silva", "document": "12345678900"},
        "defense": {"org_code": "001021"},
    }
    response = await api_client.post(
        "/v1/person/sync",
        json=payload,
        headers=integration_headers(),
    )
    assert response.status_code == 422


@pytest.mark.asyncio
async def test_sync_rejects_missing_defense(
    api_client: httpx.AsyncClient,
    db_session: Session,
):
    create_source(db_session)
    payload = {
        "source": "biodoc",
        "operation": "upsert",
        "external_id": "123",
        "person": {"full_name": "Maria Silva", "document": "12345678900"},
    }
    response = await api_client.post(
        "/v1/person/sync",
        json=payload,
        headers=integration_headers(),
    )
    assert response.status_code == 422


@pytest.mark.asyncio
async def test_sync_rejects_unknown_source(api_client: httpx.AsyncClient):
    # source "desconhecido" é rejeitado pelo schema Pydantic com 422
    invalid_payload = {**VALID_SYNC_PAYLOAD, "source": "desconhecido"}
    response = await api_client.post(
        "/v1/person/sync",
        json=invalid_payload,
        headers=integration_headers(),
    )

    assert response.status_code == 422


@pytest.mark.asyncio
async def test_sync_person_rejects_invalid_token(
    api_client: httpx.AsyncClient,
    db_session: Session,
):
    create_source(db_session)

    response = await api_client.post(
        "/v1/person/sync",
        json=VALID_SYNC_PAYLOAD,
        headers=integration_headers("wrong-token"),
    )

    assert response.status_code == 401
    assert "inválido" in response.json()["detail"]


@pytest.mark.asyncio
async def test_sync_person_with_optional_face(api_client: httpx.AsyncClient, db_session: Session):
    create_source(db_session)

    response = await api_client.post(
        "/v1/person/sync",
        json=VALID_SYNC_PAYLOAD_WITH_FACE,
        headers=integration_headers(),
    )

    assert response.status_code == 200


@pytest.mark.asyncio
async def test_sync_person_accepts_admin_api_token(
    api_client: httpx.AsyncClient,
    db_session: Session,
):
    create_source(db_session, token="different-source-token")

    response = await api_client.post(
        "/v1/person/sync",
        json=VALID_SYNC_PAYLOAD,
        headers=integration_headers("admin-token"),
    )

    assert response.status_code == 200


@pytest.mark.asyncio
async def test_sync_person_returns_503_when_defense_not_ready(
    api_client: httpx.AsyncClient,
    db_session: Session,
    defense_client_mock: AsyncMock,
):
    create_source(db_session)
    defense_client_mock.is_ready = False

    response = await api_client.post(
        "/v1/person/sync",
        json=VALID_SYNC_PAYLOAD,
        headers=integration_headers(),
    )

    assert response.status_code == 503
    assert "Defense IA não conectado" in response.json()["detail"]


@pytest.mark.asyncio
async def test_sync_person_returns_502_when_defense_ia_fails(
    api_client: httpx.AsyncClient,
    db_session: Session,
    defense_client_mock: AsyncMock,
):
    create_source(db_session)
    defense_client_mock.sync_visitor.side_effect = DefenseIAUnavailableError(
        "API do Defense IA indisponível"
    )

    response = await api_client.post(
        "/v1/person/sync",
        json=VALID_SYNC_PAYLOAD,
        headers=integration_headers(),
    )

    assert response.status_code == 502
    assert response.json() == {"detail": "API do Defense IA indisponível"}


@pytest.mark.asyncio
async def test_sync_person_returns_503_on_session_conflict(
    api_client: httpx.AsyncClient,
    db_session: Session,
    defense_client_mock: AsyncMock,
):
    from src.services.defense_ia_client import (
        SESSION_CONFLICT_PUBLIC_DETAIL,
        DefenseIAError,
    )

    create_source(db_session)
    defense_client_mock.sync_visitor.side_effect = DefenseIAError(
        "Defense IA retornou código 2004: The user has logged in."
    )

    response = await api_client.post(
        "/v1/person/sync",
        json=VALID_SYNC_PAYLOAD,
        headers=integration_headers(),
    )

    assert response.status_code == 503
    assert response.json() == {"detail": SESSION_CONFLICT_PUBLIC_DETAIL}
