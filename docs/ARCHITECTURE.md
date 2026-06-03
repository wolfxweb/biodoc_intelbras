# BIODOC-Intelbras Middleware — Documentação de Funcionamento

## Visão Geral

O middleware atua como uma **ponte entre o sistema BIODOC e a API do Intelbras Defense IA**. Suporta dois fluxos de entrada:

- **Fluxo manual / API direta:** `POST /v1/person/sync` — qualquer sistema envia nome, documento e imagem; o middleware autentica via `ADMIN_API_TOKEN` e repassa ao Defense IA.
- **Fluxo nativo BioDoc:** `GET /webhook/biodoc` — redirect do navegador após verify; o middleware consulta a API BioDoc e registra visitante no Defense IA.

---

## Estrutura de Arquivos

```
src/
├── main.py                              # FastAPI, CORS, registro de routers
├── core/
│   ├── lifespan.py                      # Startup/shutdown: Defense IA + BioDoc clients
│   ├── logging.py                       # Logs com rotação diária
│   ├── security.py                      # Verificação ADMIN_API_TOKEN
│   ├── database.py                      # SQLAlchemy: engine, Base, get_db
│   └── bootstrap.py                     # ensure_integration_source no startup
├── models/
│   └── integration_source.py            # IntegrationSource (SQLite)
├── api/
│   ├── dependencies.py                  # Auth: admin, biodoc webhook, get_clients
│   ├── schemas.py                       # SyncRequest/Response (Pydantic)
│   ├── schemas_biodoc.py                # BiodocWebhookPayload/Response
│   └── routes/
│       ├── sync.py                      # POST /v1/person/sync
│       └── webhook.py                   # GET/POST /webhook/biodoc
└── services/
    ├── defense_ia_client.py             # Cliente HTTP Intelbras (BRMS/legacy)
    ├── defense_ia_crypto.py             # RSA para login BRMS
    ├── defense_sync.py                  # Helper compartilhado: sync + mapeamento de erros
    ├── biodoc_client.py                 # Cliente async API BioDoc (get_card_mainimage)
    ├── biodoc_image.py                  # Download de imagem URL → base64
    ├── biodoc_webhook_service.py        # Orquestrador fluxo webhook BioDoc
    └── defense_visitor.py              # Utilitários visitante (scripts)
```

---

## Fluxo Webhook BioDoc

```
BioDoc Verify ──GET /webhook/biodoc?card=...──▶ Middleware

Middleware:
  1. Valida response=success (query params)
  2. GET BioDoc /card/integration/mainimage?idCard=...
  3. GET BioDoc /logs/external-audits → local_token
  4. GET BioDoc /integrations/log/{audit_id} → foto da verify
  5. Download imagem → base64
  6. `local_token` / `reguiredName` → **host visitante** (texto, ex. `"recepção central"`)
  7. Middleware copia `acsChannelIds` via API 6.2.10 (visitantes existentes com mesmo host)
  8. Defense IA visitante via `DefenseIAClient.sync_visitor()`
     `visitedName` = host, `external_id` = id_Card (remark), face = base64
  9. Página HTML de sucesso Unimed
```

Ver [`VISITOR_CHANNEL_SETUP.md`](VISITOR_CHANNEL_SETUP.md) para resolução de portas.

---

## 1. Inicialização do Sistema (`src/core/lifespan.py`)

Quando o container sobe, o FastAPI executa o `lifespan` em `src/core/lifespan.py`:

1. Lê as variáveis de ambiente do `.env`.
2. Cria `DefenseIAClient` via `build_defense_client_from_env()` e chama `start()`:
   - Cria o `httpx.AsyncClient` (conexão HTTP persistente)
   - Faz o **login inicial** no Defense IA
   - Se o login falhar, loga um warning e **continua** (não derruba o servidor)
   - Inicia a **task de keep-alive em background**
3. Cria `BiodocClient` via `build_biodoc_client_from_env()` e chama `start()`:
   - Cria um `httpx.AsyncClient` com base URL e token da API BioDoc
   - Não faz nenhuma chamada no startup (stateless, sem token persistente)

Ao desligar (SIGTERM ou restart), o `finally` do lifespan fecha o cliente HTTP e cancela o keep-alive.

---

## 2. Token do Defense IA — Login e Keep-Alive (`src/services/defense_ia_client.py`)

### Login

O método `login()` é protegido por um `asyncio.Lock` para evitar logins simultâneos. Dependendo do `DEFENSE_IA_API_MODE`:

**Modo BRMS** (`api_mode=brms`, padrão):
- POST em `/brms/api/v1.0/accounts/authorize`
- Payload inclui: username, assinatura MD5 da senha, chave pública RSA, client_type
- Retorna: `token` e `$$signature`

**Modo Legacy**:
- POST em `/admin/API/accounts/authorize`
- Mesmo mecanismo, endpoint diferente

O token é armazenado internamente em `self._token`. A propriedade `is_ready` retorna `True` enquanto `_token` não for `None`.

### Keep-Alive (background task)

A cada `DEFENSE_IA_KEEP_ALIVE_SECONDS` segundos (padrão: 20s), a task `_keep_alive_loop` executa `keep_alive_once()`:

- **BRMS:** PUT em `/brms/api/v1.0/accounts/keepalive` com `{"token": "<token_atual>"}`
- **Legacy:** POST em `/admin/API/accounts/updateToken` com assinatura MD5

Se a resposta for `401` (token expirado), faz **re-login automático**.
Se o re-login falhar, loga um warning e **aguarda o próximo ciclo** — a task nunca morre.

```
startup
  └─ login() ──────────────────────────→ _token = "abc123"
       │
       └─ _keep_alive_loop() (background)
              ├─ sleep 20s
              ├─ keep_alive_once()
              │     ├─ PUT /keepalive → ok → continua
              │     └─ 401 → login() → _token = "novo_token"
              └─ (repete para sempre)
```

---

## 3. Autenticação do Middleware (`src/api/dependencies.py` + `src/core/security.py`)

O middleware usa **seu próprio token** para proteger a rota, separado do token do Defense IA.

### Fluxo de autenticação

1. O cliente envia `Authorization: Bearer <ADMIN_API_TOKEN>` no header
2. O `HTTPBearer` (`integration_bearer`) extrai o token — esta configuração ativa o **cadeado no Swagger UI**
3. `require_admin_token()` chama `verify_admin_token(token)` em `src/core/security.py`
4. A verificação usa `secrets.compare_digest()` (resistente a timing attacks) comparando com `ADMIN_API_TOKEN` do `.env`
5. Se inválido → `401 Unauthorized`

O `ADMIN_API_TOKEN` é definido no `.env`:
```
ADMIN_API_TOKEN=biodoc-admin-dev-token
```

---

## 4. Rota de Sincronização (`src/api/routes/sync.py`)

### `POST /v1/person/sync`

**Autenticação:** `Depends(require_admin_token)` aplicado no router inteiro.

**Fluxo completo de uma requisição:**

```
Cliente
  │ POST /v1/person/sync
  │ Authorization: Bearer <ADMIN_API_TOKEN>
  │ Body: { source, operation, external_id, person, biometrics, defense }
  ▼
[1] HTTPBearer extrai o token (dependencies.py)
  ▼
[2] require_admin_token valida contra ADMIN_API_TOKEN (security.py)
  │  → 401 se inválido
  ▼
[3] Pydantic valida o body (schemas.py):
  │  • source ∈ ALLOWED_SOURCES          → 422 se não permitido
  │  • operation = "upsert"               → 422 se diferente
  │  • external_id: 1-30 chars [A-Za-z0-9] → 422 se inválido
  │  • defense.org_code obrigatório       → 422 se ausente (nome da **regra de acesso** no painel)
  │  • face_image_base64 (se enviado):
  │      - base64 decodificável           → 422 se não
  │      - mínimo 1 KB após decode        → 422 se menor
  │      - magic bytes JPEG ou PNG        → 422 se outro formato
  ▼
[4] Verifica se defense_client.is_ready   → 503 se não conectado
  ▼
[5] logger.debug [API IN] loga o request recebido (sem base64)
  ▼
[6] sync_to_defense(payload, defense=…)
  │   ├─ access_rule := defense.org_code (nome da regra de acesso no painel)
  │   ├─ resolve portas via API ACS a partir da regra
  │   └─ sync_visitor() no Defense IA
  ▼
[7] Resposta:
  │  → 200 { status, message, visitor_id?, person_id? }  sucesso
  │  → 422 "Biometria inválida: ..."                    Defense IA recusou imagem (8044)
  │  → 503 "Defense IA não conectado"                   token ausente
  │  → 502 "API do Defense IA indisponível"             erro de servidor
```

---

## 5. Validação de Dados (`src/api/schemas.py`)

| Campo | Regra | Erro |
|---|---|---|
| `source` | Deve estar em `ALLOWED_SOURCES` (ex: `"biodoc"`) | 422 |
| `operation` | Deve ser exatamente `"upsert"` | 422 |
| `external_id` | 1–30 caracteres, somente `[A-Za-z0-9]` | 422 |
| `person.full_name` | Mínimo 1 caractere | 422 |
| `person.document` | Mínimo 1 caractere | 422 |
| `defense.org_code` | Obrigatório em POST `/sync` — **nome da regra de acesso** no painel Defense (ex.: `"CHU - CENTRAL"`) | 422 |
| `biometrics` | Obrigatório em POST `/sync` | 422 |
| `biometrics.face_image_base64` | Base64 válido + mínimo 1 KB + JPEG ou PNG | 422 |

Para adicionar um novo sistema de origem, basta incluir no vetor em `src/api/schemas.py`:

```python
ALLOWED_SOURCES: list[str] = [
    "biodoc",
    "novo_sistema",   # adicionar aqui
]
```

---

## 6. Logs (`src/core/logging.py`)

Arquivo: `log/app.log` (rotação diária, retenção de 30 dias)

| Nível | Quando aparece |
|---|---|
| `INFO` | Startup, shutdown, sync com sucesso, avisos de keep-alive |
| `DEBUG` | Payload enviado ao Defense IA, resposta recebida, request recebido na API |
| `WARNING` | Falha de login/keep-alive (recuperável) |
| `ERROR` | Falha de sync com traceback completo |

Controle via `.env`:
```
LOG_LEVEL=INFO    # produção (padrão) — somente INFO e acima
LOG_LEVEL=DEBUG   # diagnóstico — mostra payloads completos (sem base64 de imagens)
```

Exemplo de saída em modo DEBUG para uma requisição:
```
DEBUG [API IN]         source=biodoc operation=upsert external_id=Carlos01 org_code=CHU - CENTRAL person={'full_name': 'Carlos', 'document': '999'} biometrics.face=<IMG:...>
DEBUG [DEFENSE_IA OUT] method=POST url=... payload={...}
DEBUG [DEFENSE_IA IN]  method=POST url=... status=200 body={"code":1000,"desc":"Success"}
INFO  Defense IA sync succeeded source=biodoc external_id=Carlos01
```

---

## 7. Status do middleware (`src/main.py`)

```
GET /status
```

Resposta:
```json
{
  "middleware": "ok",
  "defense_ia": {
    "enabled": true,
    "connected": true,
    "api_mode": "brms"
  },
  "biodoc": {
    "api_url": "https://api.sandbox.biodoc.com.br/api",
    "configured": true,
    "ambiente": "sandbox"
  }
}
```

`defense_ia.connected: true` indica login no Defense e keep-alive ativos. `biodoc.configured` indica `BIODOC_TOKEN_API` definido no ambiente.

---

## 8. Variáveis de Ambiente (`.env`)

| Variável | Descrição | Exemplo |
|---|---|---|
| `DEFENSE_IA_SERVER_URL` | URL base do servidor Intelbras | `http://200.180.74.90` |
| `DEFENSE_IA_USERNAME` | Usuário da API Defense IA | `system` |
| `DEFENSE_IA_PASSWORD` | Senha da API Defense IA | `Easy@0901` |
| `DEFENSE_IA_API_MODE` | Modo da API: `brms` ou `legacy` | `brms` |
| `DEFENSE_IA_CLIENT_TYPE` | Tipo de cliente | `WINPC_V2` |
| `DEFENSE_IA_PUBLIC_KEY` | Chave pública RSA (gerada por `scripts/generate_defense_rsa_keys.py`) | `MIIBIjAN...` |
| `DEFENSE_IA_PRIVATE_KEY` | Chave privada RSA | `MIIEvAIB...` |
| `DEFENSE_IA_KEEP_ALIVE_SECONDS` | Intervalo do keep-alive em segundos | `20` |
| `DEFENSE_IA_TIMEOUT_SECONDS` | Timeout das requisições HTTP | `10` |
| `ADMIN_API_TOKEN` | Token de autenticação do middleware | `biodoc-admin-dev-token` |
| `LOG_LEVEL` | Nível de log: `INFO` ou `DEBUG` | `INFO` |
