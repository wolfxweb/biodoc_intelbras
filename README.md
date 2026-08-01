# Middleware BIODOC ↔ Intelbras Defense IA

API middleware em FastAPI para receber eventos de cadastro/atualização de pessoas do BIODOC, validar a origem da integração e sincronizar os dados com o Intelbras Defense IA.

## Stack

- Python 3.13+
- FastAPI
- SQLite com SQLAlchemy
- Pydantic v2
- Pytest
- Docker Compose

## Funcionalidades

- Cadastro e gestão de `integration_sources`.
- Geração automática de chave de integração.
- Armazenamento apenas do hash da chave no SQLite.
- Validação de `source` + `Authorization: Bearer <token>`.
- Endpoint de sincronização de pessoa e biometria facial.
- Client Defense IA com token em memória, login no startup, keep-alive e re-login em `401`.
- Logs em texto simples com rotação diária.

## Variáveis de Ambiente

Copie o modelo e ajuste os valores:

```bash
cp .env.example .env
```

O arquivo `.env` fica na raiz do projeto (ignorado pelo Git). Em desenvolvimento local, as variáveis são carregadas automaticamente via `python-dotenv`. No Docker Compose, o mesmo arquivo é injetado com `env_file`.

| Variável | Obrigatória | Descrição |
|---|---:|---|
| `ADMIN_API_TOKEN` | Sim | Bearer para `POST /v1/person/sync` e rotas administrativas. |
| `DEFENSE_IA_SERVER_URL` | Sim, para envio real | URL base do Defense IA. Ex: `http://192.168.0.10`. |
| `DEFENSE_IA_USERNAME` | Sim, para envio real | Usuário da **API** no servidor Defense (ex.: `system`, em **minúsculas**). `System` pode falhar com código 2001 neste servidor. Não é o login do app Windows. |
| `DEFENSE_IA_PASSWORD` | Sim, para envio real | Senha desse usuário de API no painel do Defense. |
| `DEFENSE_IA_API_MODE` | Não | `brms` (Defense IA 3.x, padrão) ou `legacy` (`/admin/API/...`, V7). |
| `DEFENSE_IA_CLIENT_TYPE` | Não | `WINPC_V2` para 3.x; `WINPC` só em servidores antigos. |
| `DEFENSE_IA_USER_TYPE` | Não | Tipo de usuário na 2ª authorize BRMS. Padrão: `0` (system). |
| `DEFENSE_IA_PUBLIC_KEY` | Sim em `brms` (produção) | RSA X.509 em Base64 (`python scripts/generate_defense_rsa_keys.py`). Campo JSON: `publicKey`. |
| `DEFENSE_IA_USE_SERVER_PUBLICKEY` | Não | `true` = usa `publickey` da 1ª authorize (só teste, doc 3.1). |
| `DEFENSE_IA_ORG_CODE` | Não | Fallback interno; no POST `/sync` use `defense.org_code` com o nome da regra de acesso no painel. |
| `DEFENSE_IA_VISITOR_CHANNEL_MAP` | Não | Override JSON `orgCode` → `acsChannelIds` (exceção). Ver [`docs/VISITOR_CHANNEL_SETUP.md`](docs/VISITOR_CHANNEL_SETUP.md). |
| `DEFENSE_IA_VISITOR_CHANNEL_DEFAULT` | Não | Fallback CSV de `acsChannelIds` se o orgCode não estiver no mapa. |
| `DEFENSE_IA_VISITOR_STATUS` | Não | `1` = em visita (padrão); `0` = agendado. |
| `DEFENSE_IA_VISITED_PERSON_ID` | Não | Só se o painel exigir anfitrião explícito. |
| `DEFENSE_IA_VISITED_NAME` | Não | Nome do anfitrião no payload de visitante (opcional). |
| `DEFENSE_IA_VISITED_ORG_NAME` | Não | Organização do anfitrião no payload (opcional). |
| `DEFENSE_IA_KEEP_ALIVE_SECONDS` | Não | Intervalo de keep-alive BRMS. Padrão: `20`. |
| `DEFENSE_IA_TIMEOUT_SECONDS` | Não | Timeout das chamadas HTTP. Padrão: `10`. |
| `BIODOC_INTEGRATION_KEY` | Opcional | Se definida, usada no sync/bootstrap em vez de `ADMIN_API_TOKEN`. |
| `DEFAULT_INTEGRATION_SOURCE_NAME` | Não | Nome do `source` no JSON (padrão: `biodoc`). |
| `DEFENSE_IA_EXPOSE_ERROR` | Não | `true` expõe detalhe bruto do Defense em `502` (só homologação). |
| `BIODOC_API_URL` | Sim (webhook) | URL base da API BioDoc. Sandbox: `https://api.sandbox.biodoc.com.br/api`. Produção: `https://api.biodoc.com.br/api`. |
| `BIODOC_TOKEN_API` | Sim (webhook) | Bearer para `GET /card/integration/mainimage` (TOKEN_API do painel BioDoc). |
| `BIODOC_WEBHOOK_TOKEN` | Sim (webhook) | Token que o BioDoc envia no header `Authorization` do webhook. Deve corresponder ao valor configurado no painel BioDoc. |
| `BIODOC_AMBIENTE` | Não | `sandbox` ou `production` — apenas para log/rastreabilidade. |

Sem as variáveis do Defense IA, a API sobe com o client desabilitado. Isso permite testar `/status`, documentação OpenAPI e rotas que usam mocks em teste.

### Produção BIODOC (bootstrap da chave)

1. Defina no `.env`: `ADMIN_API_TOKEN=<token>` e `DEFAULT_INTEGRATION_SOURCE_NAME=biodoc`.
2. Suba o middleware (`docker compose up -d`). O startup grava o hash no SQLite.
3. No `/docs` ou no BIODOC: `Authorization: Bearer <ADMIN_API_TOKEN>` no sync e no admin.

### Login Defense IA 3.x — produção (recomendado)

1. Gerar par RSA:
   ```bash
   python scripts/generate_defense_rsa_keys.py
   ```
   No Docker:
   ```bash
   docker compose run --rm --no-deps -v "${PWD}:/app" middleware-biodoc-intelbras python scripts/generate_defense_rsa_keys.py
   ```
2. Copiar `DEFENSE_IA_PUBLIC_KEY=...` (e opcionalmente `DEFENSE_IA_PRIVATE_KEY=...`) para o `.env`. Não commitar chaves.
3. Subir de novo o middleware:
   ```bash
   docker compose up -d
   ```

Validar login manualmente:

```bash
docker compose run --rm --no-deps -v "${PWD}:/app" --env-file .env middleware-biodoc-intelbras python scripts/test_defense_login.py
```

**Teste rápido (não produção):** `DEFENSE_IA_USE_SERVER_PUBLICKEY=true` — usa a `publickey` da 1ª resposta; a documentação 3.1 desaconselha desvincular depois.

## Executando com Docker Compose

```bash
docker compose up -d --build
```

A API ficará disponível em:

```text
http://localhost:8000
```

Status das integrações:

```bash
curl -i http://localhost:8000/status
```

Resposta esperada (com Defense configurado e login OK):

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

Se o login no startup falhar, `connected` fica `false` e `POST /v1/person/sync` retorna `503`.

## Rotas Administrativas

As rotas administrativas exigem:

```http
Authorization: Bearer <ADMIN_API_TOKEN>
```

### Criar source

```bash
curl -X POST http://localhost:8000/v1/integration-sources \
  -H "Authorization: Bearer $ADMIN_API_TOKEN" \
  -H "Content-Type: application/json" \
  -d '{"name":"biodoc","is_active":true}'
```

A resposta retorna `integration_key`. Guarde essa chave, pois ela não é armazenada em texto puro.

### Listar sources

```bash
curl http://localhost:8000/v1/integration-sources \
  -H "Authorization: Bearer $ADMIN_API_TOKEN"
```

### Alterar source ou rotacionar chave

```bash
curl -X PATCH http://localhost:8000/v1/integration-sources/1 \
  -H "Authorization: Bearer $ADMIN_API_TOKEN" \
  -H "Content-Type: application/json" \
  -d '{"name":"biodoc","is_active":true,"rotate_token":true}'
```

Quando `rotate_token` for `true`, a resposta traz uma nova `integration_key`.

### Desativar source

```bash
curl -X DELETE http://localhost:8000/v1/integration-sources/1 \
  -H "Authorization: Bearer $ADMIN_API_TOKEN"
```

## Webhook BioDoc — fluxo nativo

O endpoint `/biodoc` recebe eventos automáticos do BioDoc após validação de liveness, consulta a API BioDoc para obter nome e foto, e sincroniza o beneficiário no Defense IA.

**Guia completo (sandbox + produção, painel BioDoc, rede, checklist):** [docs/INTEGRACAO_BIODOC_AMBIENTE_REAL.md](docs/INTEGRACAO_BIODOC_AMBIENTE_REAL.md)

### Configuração

1. Preencha no `.env`:
   ```
   BIODOC_API_URL=https://api.sandbox.biodoc.com.br/api
   BIODOC_TOKEN_API=<TOKEN_API do painel BioDoc>
   BIODOC_WEBHOOK_TOKEN=<token que você define e configura no painel BioDoc>
   ```
2. No painel BioDoc, configure o WebHook:
   - URL: `https://un.wolfx.com.br/biodoc`
   - Token: mesmo valor de `BIODOC_WEBHOOK_TOKEN`

### Fluxo técnico

```
Usuário realiza liveness no BioDoc
        ↓
BioDoc envia POST /biodoc
  Authorization: Bearer <BIODOC_WEBHOOK_TOKEN>
  { "reference_Id": "...", "success": true, "url": "...", "status": 2, ... }
        ↓
Middleware valida token + success + reference_Id
        ↓
GET BioDoc /integrations/log/{reference_Id}
  → { id_Card, name, status, mainImage, path, reguiredName }
        ↓
Download da imagem URL (mainImage > path > url) → base64
        ↓
Upsert no Intelbras Defense IA
  external_id = id_Card, document = id_Card, face = base64
        ↓
Resposta: { "status": "success", "external_id": "...", "defense_sync": "ok" }
```

### Payload enviado pelo BioDoc

```json
{
  "id_Log": 1000,
  "percentage": "100%",
  "success": true,
  "status": 2,
  "message": "Sucesso ao realizar autenticação, nível de similaridade 100% e qualidade 100%.",
  "url": "https://api.sandbox.com.br/api/file/305",
  "reference_Id": "0c19bfff-9aba-4517-afd7-56e77ea1faeb"
}
```

### Resposta em caso de sucesso (200)

```json
{
  "status": "success",
  "external_id": "1234567890",
  "defense_sync": "ok"
}
```

### Códigos de erro

| Código | Causa |
|--------|-------|
| 401 | `BIODOC_WEBHOOK_TOKEN` inválido ou ausente |
| 422 | `success: false` / `reference_Id` ausente / beneficiário inativo no BioDoc (status fora de 1/2) / sem imagem |
| 502 | API BioDoc inacessível ou Defense IA retornou erro |
| 503 | Defense IA não conectado no startup |

### Teste local

```bash
python scripts/test_biodoc_webhook.py
# Com opções:
python scripts/test_biodoc_webhook.py --url https://un.wolfx.com.br --card 9999999999
python scripts/test_biodoc_webhook.py --success false
```

## Duas URLs (não confundir)

| O quê | URL | Onde configura |
|--------|-----|----------------|
| **Middleware BIODOC** (sua API) | `http://localhost:8000` | `docker compose` expõe porta 8000 |
| **Intelbras Defense IA** (servidor) | `http://200.180.74.90` | `DEFENSE_IA_SERVER_URL` no `.env` |

Fluxo: o BIODOC chama **`POST http://localhost:8000/v1/person/sync`**; o middleware autentica a origem e repassa o cadastro para o Defense no IP do `.env`.

Foto de teste (rosto JPEG da internet):

```bash
python scripts/download_test_face.py
```

Isso grava `data/test_face.jpg`. Os scripts de teste leem `TEST_FACE_IMAGE_PATH` (padrão: `data/test_face.jpg`).

Teste completo **via middleware** (com `docker compose up -d`):

```bash
docker compose run --rm --no-deps -v "${PWD}:/app" middleware-biodoc-intelbras python scripts/test_sync_via_api.py
```

## Sincronização com a Intelbras

Todo cadastro no Defense é **visitante**. Cada chamada gera uma **nova visita** (não upsert por cartão).

Manual operacional para a equipe: [`docs/MANUAL_CADASTRO_DIRETO_DEFENSE.md`](docs/MANUAL_CADASTRO_DIRETO_DEFENSE.md).

| Fluxo | Destino no Defense | Portas |
|-------|-------------------|--------|
| **Webhook BioDoc** (`GET /biodoc`) | Automático (`local_name` / `reguiredName` / `details.nmLocal` → host visitante) | Automático |
| **POST /v1/person/sync** | `defense.org_code` = **nível** (ex.: `INT5`, `Int8`) | Ramo (ancestrais sem Local Atual) + nível + descendentes |

Exemplo de body para sync manual: [`data/test_post_sync.json`](data/test_post_sync.json) — resumo em [`data/test_post_sync.md`](data/test_post_sync.md). Qualquer nível válido em `org_code` (ex.: `INT5`, `Int8`) libera ancestrais e descendentes pelo prefixo do código no `deviceOrg`; ver [`docs/VISITOR_CHANNEL_SETUP.md`](docs/VISITOR_CHANNEL_SETUP.md).

Testes:

```bash
docker compose run --rm --no-deps -v "${PWD}:/app" middleware-biodoc-intelbras python scripts/list_visitor_config.py
docker compose run --rm --no-deps -v "${PWD}:/app" middleware-biodoc-intelbras python scripts/test_defense_sync_visitor.py
docker compose run --rm --no-deps -v "${PWD}:/app" middleware-biodoc-intelbras python scripts/test_sync_via_api.py
```

### Face de visitante → leitura (sem painel)

Reutiliza a foto (`authInfo.facePictures`) de um visitante já cadastrado no Defense (somente leitura):

```bash
# Listar visitantes
docker compose run --rm --no-deps -v "${PWD}:/app" middleware-biodoc-intelbras python scripts/test_visitor_face_lifecycle.py --list

# Fluxo completo (substitua 123 pelo visitorId)
docker compose run --rm --no-deps -v "${PWD}:/app" middleware-biodoc-intelbras python scripts/test_visitor_face_lifecycle.py 123

# Só validar leitura da face
docker compose run --rm --no-deps -v "${PWD}:/app" middleware-biodoc-intelbras python scripts/test_visitor_face_lifecycle.py 123 --dry-run
```

Endpoint:

```http
POST /v1/person/sync
```

Header:

```http
Authorization: Bearer <ADMIN_API_TOKEN>
```

Payload (sempre visitante):

`external_id` vai para `remark` no visitante (rastreio). Somente letras e números, máx. 30 caracteres.

Resposta inclui `visitor_id` e `person_id` (interno do Defense).

```json
{
  "source": "biodoc",
  "operation": "upsert",
  "external_id": "00271368992672000",
  "person": {
    "full_name": "Maria Silva",
    "document": "12345678900"
  },
  "biometrics": {
    "face_image_base64": "..."
  },
  "defense": {
    "org_code": "CHU - CENTRAL"
  }
}
```

`biometrics.face_image_base64` é **obrigatório** — JPEG ou PNG em base64 (mín. 1 KB decodificado).

Scripts: `python scripts/test_sync_via_api.py` (usa foto de teste do `.env`/fixtures).

Exemplo:

```bash
curl -X POST http://localhost:8000/v1/person/sync \
  -H "Authorization: Bearer $ADMIN_API_TOKEN" \
  -H "Content-Type: application/json" \
  -d '{
    "source": "biodoc",
    "operation": "upsert",
    "external_id": "00271368992672000",
    "person": {
      "full_name": "Maria Silva",
      "document": "12345678900"
    },
    "biometrics": {
      "face_image_base64": "base64-da-imagem"
    },
    "defense": {
      "org_code": "CHU - CENTRAL"
    }
  }'
```

Respostas principais:

- `200 OK`: dados sincronizados.
- `400 Bad Request`: `source não cadastrado`.
- `401 Unauthorized`: token inválido ou incompatível com o source.
- `503 Service Unavailable`: Defense habilitado mas sem sessão (login falhou ou ainda não conectou).
- `502 Bad Gateway`: API do Defense IA indisponível ou erro de negócio no sync.

## Testes

```bash
pytest
```


## Persistência e Logs

No Docker Compose:

- `./data` é montado em `/app/data` para persistir o SQLite.
- `./log` é montado em `/log` para persistir logs diários.

## Documentação Interativa

Com a API rodando:

- Swagger UI: `http://localhost:8000/docs`
