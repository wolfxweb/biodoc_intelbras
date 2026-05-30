"""Parse da query do redirect BioDoc (inclui correção de `?` duplicado na URL)."""

from __future__ import annotations

from dataclasses import dataclass
from urllib.parse import parse_qs


@dataclass(frozen=True)
class BiodocRedirectParams:
    token: str | None
    card: str | None
    reference_id: str | None
    response: str | None
    message: str | None
    date: str | None
    justify_id: str | None
    id_transaction: str | None
    name: str | None
    id_log: int | None
    percentage: str | None
    biodoc_status: int | None
    url: str | None


def _first(parsed: dict[str, list[str]], *keys: str) -> str | None:
    for key in keys:
        values = parsed.get(key)
        if not values:
            continue
        value = values[-1].strip()
        if value.lower() in ("null", "undefined", ""):
            return None
        return value
    return None


def normalize_biodoc_redirect_query_string(query: str) -> str:
    """BioDoc costuma acrescentar `?card=...` quando a callback já tem query (`&` esperado)."""
    normalized = (query or "").strip()
    if normalized.startswith("?"):
        normalized = normalized[1:]
    return normalized.replace("?", "&")


def parse_biodoc_redirect_params(query: str) -> BiodocRedirectParams:
    parsed = parse_qs(
        normalize_biodoc_redirect_query_string(query),
        keep_blank_values=True,
    )
    token = _first(parsed, "token")

    id_log_raw = _first(parsed, "id_Log", "id_log")
    id_log = int(id_log_raw) if id_log_raw and id_log_raw.isdigit() else None

    status_raw = _first(parsed, "status")
    biodoc_status = int(status_raw) if status_raw and status_raw.isdigit() else None

    return BiodocRedirectParams(
        token=token,
        card=_first(parsed, "card"),
        reference_id=_first(parsed, "reference_Id", "reference_id"),
        response=_first(parsed, "response"),
        message=_first(parsed, "message"),
        date=_first(parsed, "date"),
        justify_id=_first(parsed, "justifyId", "justify_id"),
        id_transaction=_first(parsed, "idTransaction", "id_transaction"),
        name=_first(parsed, "name"),
        id_log=id_log,
        percentage=_first(parsed, "percentage"),
        biodoc_status=biodoc_status,
        url=_first(parsed, "url"),
    )
