import pytest
import httpx

pytestmark = pytest.mark.skip(
    reason="Rotas /v1/integration-sources não implementadas ainda (fora de escopo da fase atual)"
)


def admin_headers() -> dict[str, str]:
    return {"Authorization": "Bearer admin-token"}


@pytest.mark.asyncio
async def test_create_update_list_and_delete_integration_source(
    api_client: httpx.AsyncClient,
):
    create_response = await api_client.post(
        "/v1/integration-sources",
        json={"name": "biodoc"},
        headers=admin_headers(),
    )

    assert create_response.status_code == 201
    created = create_response.json()
    assert created["name"] == "biodoc"
    assert created["is_active"] is True
    assert created["integration_key"]

    list_response = await api_client.get(
        "/v1/integration-sources",
        headers=admin_headers(),
    )
    assert list_response.status_code == 200
    assert list_response.json()[0]["name"] == "biodoc"

    update_response = await api_client.patch(
        f"/v1/integration-sources/{created['id']}",
        json={"name": "biodoc-renamed", "rotate_token": True},
        headers=admin_headers(),
    )
    assert update_response.status_code == 200
    updated = update_response.json()
    assert updated["name"] == "biodoc-renamed"
    assert updated["integration_key"]
    assert updated["integration_key"] != created["integration_key"]

    delete_response = await api_client.delete(
        f"/v1/integration-sources/{created['id']}",
        headers=admin_headers(),
    )
    assert delete_response.status_code == 204

    list_after_delete = await api_client.get(
        "/v1/integration-sources",
        headers=admin_headers(),
    )
    assert list_after_delete.json()[0]["is_active"] is False


@pytest.mark.asyncio
async def test_integration_source_routes_require_admin_token(
    api_client: httpx.AsyncClient,
):
    response = await api_client.get("/v1/integration-sources")

    assert response.status_code == 401
