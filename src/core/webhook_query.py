"""Parsing de query string no redirect BioDoc (GET /webhook/biodoc)."""

from __future__ import annotations

from urllib.parse import parse_qsl


def parse_biodoc_redirect_query(query: str) -> dict[str, str]:
    """Converte query do redirect BioDoc em dict.

    O BioDoc às vezes concatena parâmetros com ``?`` em vez de ``&`` quando o
    ``url=`` da verify já contém ``?org_code=...``, gerando por exemplo
    ``org_code=VIVER?card=123`` — nesse caso o Starlette não enxerga ``card``.
    """
    if not query:
        return {}
    normalized = query.replace("?", "&")
    return dict(parse_qsl(normalized, keep_blank_values=True))
