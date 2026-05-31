from unittest.mock import AsyncMock

import httpx
import pytest
from sqlalchemy.orm import Session

from src.core.security import hash_token
from src.models.integration_source import IntegrationSource
from src.services.defense_ia_client import DefenseIAUnavailableError


VALID_SYNC_PAYLOAD = {
    "source": "biodoc",
    "operation": "upsert",
    "external_id": "123",
    "person": {"full_name": "Maria Silva", "document": "12345678900"},
}

import base64
_FAKE_JPEG_B64 = base64.b64encode(b"\xff\xd8\xff" + b"\x00" * 2048).decode()

VALID_SYNC_PAYLOAD_WITH_FACE = {
    **VALID_SYNC_PAYLOAD,
    "biometrics": {"face_image_base64": _FAKE_JPEG_B64},
}


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
        "message": "Usuário enviado ao Intelbras Defense IA com sucesso",
    }
    defense_client_mock.sync_person.assert_awaited_once()


@pytest.mark.asyncio
async def test_sync_person_rejects_unknown_source(api_client: httpx.AsyncClient):
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
    defense_client_mock.sync_person.side_effect = DefenseIAUnavailableError(
        "API do Defense IA indisponível"
    )

    response = await api_client.post(
        "/v1/person/sync",
        json=VALID_SYNC_PAYLOAD,
        headers=integration_headers(),
    )

    assert response.status_code == 502
    assert response.json() == {"detail": "API do Defense IA indisponível"}
