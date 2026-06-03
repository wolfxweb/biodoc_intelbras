"""Testes do helper sync_to_defense (person vs visitor)."""

from unittest.mock import AsyncMock

import pytest

from src.api.schemas import SyncRequest
from src.services.defense_ia_client import DefenseIAClient, DefenseIASettings
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
