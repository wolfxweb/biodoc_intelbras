"""Testes do helper sync_to_defense (person vs visitor)."""

from unittest.mock import AsyncMock

import pytest
from fastapi import HTTPException

from src.api.schemas import SyncRequest
from src.services.defense_ia_client import (
    FACE_SIZE_LIMIT_PUBLIC_DETAIL,
    DefenseIAArgumentError,
    DefenseIAClient,
    DefenseIAError,
    DefenseIASettings,
    defense_error_detail_public,
)
from src.services.defense_sync import sync_to_defense


def _sync_request() -> SyncRequest:
    return SyncRequest.model_validate(
        {
            "source": "biodoc",
            "operation": "upsert",
            "external_id": "card001",
            "person": {"full_name": "Test User", "document": "123"},
        }
    )


@pytest.mark.asyncio
async def test_sync_to_defense_visitor_mode():
    client = DefenseIAClient(
        settings=DefenseIASettings(
            server_url="http://defense.test",
            username="u",
            password="p",
            sync_target="visitor",
        )
    )
    client._token = "t"
    client.sync_visitor = AsyncMock(
        return_value={"code": 1000, "data": {"visitorId": "9", "personId": "8"}}
    )

    result = await sync_to_defense(
        _sync_request(), client, sync_target="visitor", org_code="001021"
    )

    assert result["visitor_id"] == "9"
    assert result["person_id"] == "8"
    client.sync_visitor.assert_awaited_once_with(
        _sync_request(),
        "001021",
        access_rule_name="001021",
        entrance_ids=None,
    )


@pytest.mark.asyncio
async def test_sync_to_defense_visitor_explicit_channels():
    client = DefenseIAClient(
        settings=DefenseIASettings(
            server_url="http://defense.test",
            username="u",
            password="p",
            sync_target="visitor",
        )
    )
    client._token = "t"
    client.sync_visitor = AsyncMock(return_value={"code": 1000, "data": {"visitorId": "1"}})

    channels = ["1000049$7$0$0"]
    await sync_to_defense(
        _sync_request(),
        client,
        sync_target="visitor",
        org_code="001021",
        acs_channel_ids=channels,
    )

    client.sync_visitor.assert_awaited_once_with(
        _sync_request(),
        "001021",
        access_rule_name="001021",
        entrance_ids=channels,
    )


def test_defense_error_detail_public_maps_face_size_limit():
    exc = DefenseIAError(
        "Defense IA retornou código 8079: person face size over limit：100k."
    )
    assert defense_error_detail_public(exc) == FACE_SIZE_LIMIT_PUBLIC_DETAIL


@pytest.mark.asyncio
async def test_sync_to_defense_face_size_limit_returns_422():
    client = DefenseIAClient(
        settings=DefenseIASettings(
            server_url="http://defense.test",
            username="u",
            password="p",
            sync_target="visitor",
        )
    )
    client._token = "t"
    client.sync_visitor = AsyncMock(
        side_effect=DefenseIAArgumentError(FACE_SIZE_LIMIT_PUBLIC_DETAIL)
    )

    with pytest.raises(HTTPException) as exc_info:
        await sync_to_defense(
            _sync_request(), client, sync_target="visitor", org_code="INT6"
        )

    assert exc_info.value.status_code == 422
    assert exc_info.value.detail == FACE_SIZE_LIMIT_PUBLIC_DETAIL
