"""Parsing de query string no redirect BioDoc (GET /biodoc)."""

from __future__ import annotations

import re
from urllib.parse import parse_qsl, unquote

_MALFORMED_ORG_CODE_KEY = re.compile(
    r"^org_code%3[dD]?(?P<value>.+)$",
    re.IGNORECASE,
)


def _recover_org_code_from_malformed_params(parsed: dict[str, str]) -> None:
    """Recupera org_code quando o redirect traz chave corrompida (ex.: ``org_code%3Oncologia``).

    Ocorre se ``url=`` na verify usa encoding incompleto (``%3`` em vez de ``%3D``).
    """
    existing = (parsed.get("org_code") or "").strip()
    if existing:
        return
    for key, value in parsed.items():
        match = _MALFORMED_ORG_CODE_KEY.match(key)
        if not match:
            continue
        candidate = (value or match.group("value") or "").strip()
        if candidate:
            parsed["org_code"] = unquote(candidate)
            return


def parse_biodoc_redirect_query(query: str) -> dict[str, str]:
    """Converte query do redirect BioDoc em dict.

    O BioDoc às vezes concatena parâmetros com ``?`` em vez de ``&`` quando o
    ``url=`` da verify já contém ``?org_code=...``, gerando por exemplo
    ``org_code=VIVER?card=123`` — nesse caso o Starlette não enxerga ``card``.
    """
    if not query:
        return {}
    normalized = query.replace("?", "&")
    parsed = dict(parse_qsl(normalized, keep_blank_values=True))
    _recover_org_code_from_malformed_params(parsed)
    return parsed
