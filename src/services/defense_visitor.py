"""API de visitantes Defense IA 3.x (somente leitura + utilitários)."""

from __future__ import annotations

import time
from typing import Any

from src.services.defense_ia_client import DefenseIAClient

VISITOR_PAGE = "/obms/api/v1.0/visitors/visitor/page"
VISITOR_DETAIL = "/obms/api/v1.0/visitors/visitor/{visitor_id}"
PERSON_DELETE_BATCH = "/obms/api/v1.1/acs/person/delete/batch"


def visitor_page_query(
    *,
    key: str = "",
    page: int = 1,
    page_size: int = 100,
    lookback_days: int = 730,
) -> dict[str, str]:
    now = int(time.time())
    return {
        "direction": "1",
        "orderType": "0",
        "endTime": str(now + 86400),
        "timeType": "-1",
        "page": str(page),
        "key": key,
        "status": "-1",
        "startTime": str(now - 86400 * lookback_days),
        "pagesize": str(page_size),
    }


async def list_visitors(
    client: DefenseIAClient, page: int = 1, page_size: int = 20
) -> dict[str, Any]:
    params = visitor_page_query()
    params["page"] = str(page)
    params["pagesize"] = str(page_size)
    response = await client._request(
        "GET",
        VISITOR_PAGE,
        params=params,
        headers=client._auth_headers(),
    )
    client._raise_for_response(response)
    return response.json() if response.content else {}


async def get_visitor(client: DefenseIAClient, visitor_id: str) -> dict[str, Any]:
    response = await client._request(
        "GET",
        VISITOR_DETAIL.format(visitor_id=visitor_id),
        headers=client._auth_headers(),
    )
    client._raise_for_response(response)
    return response.json() if response.content else {}


def extract_visitor_face_pictures(visitor_body: dict[str, Any]) -> list[str]:
    auth = visitor_body.get("authInfo") or visitor_body.get("data", {}).get("authInfo")
    if not isinstance(auth, dict):
        return []
    faces = auth.get("facePictures") or auth.get("facePicture") or []
    if isinstance(faces, str):
        return [faces] if faces else []
    if isinstance(faces, list):
        return [str(f) for f in faces if f]
    return []


async def delete_persons(client: DefenseIAClient, person_ids: list[str]) -> dict[str, Any]:
    response = await client._request(
        "POST",
        PERSON_DELETE_BATCH,
        json={"personIds": person_ids},
        headers=client._auth_headers(),
    )
    client._raise_for_response(response)
    return response.json() if response.content else {}
