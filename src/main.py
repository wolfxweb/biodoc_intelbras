import os
from pathlib import Path

from dotenv import load_dotenv

if os.getenv("SKIP_DOTENV") != "1":
    load_dotenv(Path(__file__).resolve().parent.parent / ".env")

from src.api.webhook_audit_middleware import WebhookAuditMiddleware
from src.core.lifespan import lifespan
from src.api.routes.sync import router as sync_router
from src.api.routes.webhook import router as webhook_router
from fastapi import FastAPI, Request
from fastapi.middleware.cors import CORSMiddleware
from fastapi.openapi.docs import get_swagger_ui_html
from fastapi.responses import HTMLResponse

_SWAGGER_DOCS_BRANDING = """
<style>
  .swagger-ui .topbar { display: none !important; }
  .swagger-ui .info .url { display: none !important; }
  .swagger-ui .info .main > span.url { display: none !important; }

  #wolfx-brand {
    display: flex;
    align-items: center;
    justify-content: center;
    padding: 0.75rem 1.5rem;
    background: #ffffff;
    border-bottom: 1px solid #e4e4e7;
    font-family: ui-sans-serif, system-ui, -apple-system, "Segoe UI", sans-serif;
  }

  #wolfx-brand .wolfx-credit {
    margin: 0;
    color: #18181b;
    font-size: 0.875rem;
  }

  #wolfx-brand .wolfx-credit a {
    color: #18181b;
    font-weight: 600;
    text-decoration: none;
  }

  #wolfx-brand .wolfx-credit a:hover {
    color: #2563eb;
    text-decoration: underline;
  }
</style>
"""

_WOLFX_BRAND_HTML = """
<div id="wolfx-brand">
  <p class="wolfx-credit">
    Desenvolvido por <a href="https://wolfx.com.br" target="_blank" rel="noopener noreferrer">wolfx.com.br</a>
  </p>
</div>
"""

app = FastAPI(
    title="BIODOC-Intelbras Middleware",
    version="0.3.4",
    lifespan=lifespan,
    docs_url=None,
    redoc_url=None,
)
app.add_middleware(WebhookAuditMiddleware)
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_methods=["*"],
    allow_headers=["*"],
)
app.include_router(sync_router)
app.include_router(webhook_router)


@app.get("/docs", include_in_schema=False)
async def custom_swagger_ui_html() -> HTMLResponse:
    response = get_swagger_ui_html(
        openapi_url=app.openapi_url,
        title=f"{app.title} — Documentação",
        swagger_ui_parameters={
            "docExpansion": "list",
            "filter": True,
            "persistAuthorization": True,
        },
    )
    html = response.body.decode()
    html = html.replace("</head>", f"{_SWAGGER_DOCS_BRANDING}</head>")
    html = html.replace("<div id=\"swagger-ui\">", f"{_WOLFX_BRAND_HTML}<div id=\"swagger-ui\">")
    return HTMLResponse(html)


@app.get(
    "/status",
    tags=["status"],
    summary="Status do middleware e integrações",
    description=(
        "Retorna se a API está no ar e o estado das integrações "
        "(Intelbras Defense IA e cliente BioDoc configurado no startup)."
    ),
)
async def integration_status(request: Request) -> dict:
    defense = request.app.state.defense_client
    defense_enabled = defense.settings.enabled
    biodoc_configured = bool(os.getenv("BIODOC_TOKEN_API", "").strip())
    return {
        "middleware": "ok",
        "defense_ia": {
            "enabled": defense_enabled,
            "connected": defense.is_ready if defense_enabled else False,
            "api_mode": defense.settings.api_mode,
        },
        "biodoc": {
            "api_url": os.getenv("BIODOC_API_URL", ""),
            "configured": biodoc_configured,
            "ambiente": os.getenv("BIODOC_AMBIENTE", "sandbox"),
        },
    }
