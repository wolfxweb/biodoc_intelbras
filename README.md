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
| `ADMIN_API_TOKEN` | Sim, para rotas administrativas | Token usado para criar, alterar, listar e desativar sources. |
| `DEFENSE_IA_SERVER_URL` | Sim, para envio real | URL base do Defense IA. Ex: `http://192.168.0.10`. |
| `DEFENSE_IA_USERNAME` | Sim, para envio real | Usuário da **API** no servidor Defense (ex.: `system`, em **minúsculas**). `System` pode falhar com código 2001 neste servidor. Não é o login do app Windows. |
| `DEFENSE_IA_PASSWORD` | Sim, para envio real | Senha desse usuário de API no painel do Defense. |
| `DEFENSE_IA_API_MODE` | Não | `brms` (Defense IA 3.x, padrão) ou `legacy` (`/admin/API/...`, V7). |
| `DEFENSE_IA_CLIENT_TYPE` | Não | `WINPC_V2` para 3.x; `WINPC` só em servidores antigos. |
| `DEFENSE_IA_USER_TYPE` | Não | Tipo de usuário na 2ª authorize BRMS. Padrão: `0` (system). |
| `DEFENSE_IA_PUBLIC_KEY` | Sim em `brms` (produção) | RSA X.509 em Base64 (`python scripts/generate_defense_rsa_keys.py`). Campo JSON: `publicKey`. |
| `DEFENSE_IA_USE_SERVER_PUBLICKEY` | Não | `true` = usa `publickey` da 1ª authorize (só teste, doc 3.1). |
| `DEFENSE_IA_ORG_CODE` | Não | `orgCode` no cadastro de pessoa BRMS. Padrão: `001`. |
| `DEFENSE_IA_KEEP_ALIVE_SECONDS` | Não | Intervalo de keep-alive BRMS. Padrão: `20`. |
| `DEFENSE_IA_TIMEOUT_SECONDS` | Não | Timeout das chamadas HTTP. Padrão: `10`. |
| `ADMIN_API_TOKEN` | Sim | Bearer do admin **e** do `POST /v1/person/sync` (startup grava hash na source `biodoc`). |
| `BIODOC_INTEGRATION_KEY` | Opcional | Se definida, usada no sync/bootstrap em vez de `ADMIN_API_TOKEN`. |
| `DEFAULT_INTEGRATION_SOURCE_NAME` | Não | Nome do `source` no JSON (padrão: `biodoc`). |
| `DEFENSE_IA_EXPOSE_ERROR` | Não | `true` expõe detalhe bruto do Defense em `502` (só homologação). |

Sem as variáveis do Defense IA, a API sobe com o client desabilitado. Isso permite testar `/health`, documentação OpenAPI e rotas que usam mocks em teste.

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

Healthcheck:

```bash
curl -i http://localhost:8000/health
```

Resposta esperada (com Defense configurado e login OK):

```json
{
  "status": "ok",
  "defense_ia": {
    "enabled": true,
    "connected": true,
    "api_mode": "brms"
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

Em modo `brms` (Defense IA 3.x), o upsert funciona assim:

1. `GET /obms/api/v1.1/acs/person/{external_id}` — verifica se a pessoa já existe
2. Se não existir → `POST /obms/api/v1.1/acs/person` (cadastro)
3. Se existir → `PUT /obms/api/v1.1/acs/person/{external_id}` (atualização)

Teste manual de cadastro + atualização:

```bash
docker compose run --rm --no-deps -v "${PWD}:/app" middleware-biodoc-intelbras python scripts/test_defense_sync_person.py
```

### Face de visitante → pessoa de teste (sem painel)

Reutiliza a foto (`authInfo.facePictures`) de um visitante já cadastrado no Defense, cria uma **pessoa ACS** com outro nome, tenta inativar (`baseInfo.status=0`) e remove com `POST .../person/delete/batch`. O visitante original **não é alterado**.

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
Authorization: Bearer <integration_key>
```

Payload:

`external_id` vira `personId` no Defense IA e deve ser **somente letras e números** (`^[0-9A-Za-z]+$`).

```json
{
  "source": "biodoc",
  "operation": "upsert",
  "external_id": "12345",
  "person": {
    "full_name": "Maria Silva",
    "document": "12345678900"
  },
  "biometrics": {
    "face_image_base64": "..."
  }
}
```

`biometrics` é opcional:

- **Criação** sem foto: cadastro sem `facePictures`.
- **Atualização** sem foto: mantém as fotos já existentes no Defense (não envia lista vazia no PUT).
- **Com** `biometrics.face_image_base64`: envia ou substitui a foto.

Scripts: `python scripts/test_sync_via_api.py --no-face` (usa `BIODOC_INTEGRATION_KEY` do `.env`).

Exemplo:

```bash
curl -X POST http://localhost:8000/v1/person/sync \
  -H "Authorization: Bearer $INTEGRATION_KEY" \
  -H "Content-Type: application/json" \
  -d '{
    "source": "biodoc",
    "operation": "upsert",
    "external_id": "123",
    "person": {
      "full_name": "Maria Silva",
      "document": "12345678900"
    },
    "biometrics": {
      "face_image_base64": "base64-da-imagem"
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
- OpenAPI JSON: `http://localhost:8000/openapi.json`
