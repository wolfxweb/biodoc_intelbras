"""Páginas HTML pós-validação BioDoc — padrão visual Unimed Joinville."""

from __future__ import annotations

import html


def _escape(value: str | None) -> str:
    return html.escape((value or "").strip(), quote=True)


_UNIMED_BASE_STYLES = """
        :root {
            --green: #00995A;
            --dark-green: #007f4a;
            --text-dark: #333;
            --text-light: #555;
            --bg: #f9f9f9;
        }
        * { box-sizing: border-box; }
        body {
            font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Helvetica, Arial, sans-serif;
            margin: 0; padding: 20px;
            display: flex; flex-direction: column;
            align-items: center; justify-content: center;
            min-height: 100vh;
            background: var(--bg);
            color: var(--text-dark);
            text-align: center;
        }
        .card {
            max-width: 520px; width: 100%;
            padding: 40px 32px;
            background: #fff;
            border-radius: 12px;
            box-shadow: 0 4px 24px rgba(0,0,0,0.08);
            border-top: 5px solid var(--green);
        }
        .logo { max-width: 160px; height: auto; margin-bottom: 28px; }
        .icon {
            font-size: 4rem;
            color: var(--green);
            margin-bottom: 12px;
            line-height: 1;
        }
        .icon-warn { color: #c77800; }
        h1 { font-size: 1.6rem; margin: 0 0 8px; color: var(--text-dark); }
        p  { font-size: 1rem; color: var(--text-light); line-height: 1.6; margin: 0; }
        .local-name {
            font-size: 1.1rem;
            font-weight: 600;
            color: var(--green);
            margin: 14px 0 4px;
        }
        .local-label, .name-label {
            font-size: 0.78rem;
            text-transform: uppercase;
            letter-spacing: 0.06em;
            color: #999;
            margin-bottom: 2px;
        }
        .name-label { margin-top: 14px; }
        .name {
            font-size: 1.05rem;
            font-weight: 600;
            color: var(--text-dark);
            margin-top: 2px;
        }
        .defense-msg {
            font-size: 0.9rem;
            color: var(--text-light);
            margin: 14px 0 4px;
        }
        .divider {
            border: none;
            border-top: 1px solid #eee;
            margin: 18px 0;
        }
"""


def render_unimed_success_page(
    *,
    card: str,
    name: str | None = None,
    local_name: str | None = None,
) -> str:
    beneficiary = _escape(name) if name else _escape(card)
    local_display = _escape(local_name) if local_name else "—"

    return f"""<!DOCTYPE html>
<html lang="pt-BR">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Validação Biométrica - Unimed Joinville</title>
    <style>{_UNIMED_BASE_STYLES}</style>
</head>
<body>
    <div class="card">
        <img src="https://www.unimed.coop.br/site/image/layout_set_logo?img_id=20960851&amp;t=1761031417217"
             alt="Unimed Joinville" class="logo">
        <div class="icon">&#10003;</div>
        <h1>Validação realizada com sucesso.</h1>

        <hr class="divider">

        <p class="name-label">Beneficiário</p>
        <p class="name">{beneficiary}</p>

        <hr class="divider">

        <p class="local-label">Local de acesso</p>
        <p class="local-name">{local_display}</p>

        <p class="defense-msg">Seu cadastro foi adicionado no Defense.</p>
    </div>
</body>
</html>
"""


def render_unimed_error_page(*, title: str, message: str) -> str:
    return f"""<!DOCTYPE html>
<html lang="pt-BR">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Validação Biométrica - Unimed Joinville</title>
    <style>{_UNIMED_BASE_STYLES}</style>
</head>
<body>
    <div class="card">
        <img src="https://www.unimed.coop.br/site/image/layout_set_logo?img_id=20960851&amp;t=1761031417217"
             alt="Unimed Joinville" class="logo">
        <div class="icon icon-warn">!</div>
        <h1>{_escape(title)}</h1>
        <p>{_escape(message)}</p>
    </div>
</body>
</html>
"""
