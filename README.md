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

| Variável | Obrigatória | Descrição |
|---|---:|---|
| `ADMIN_API_TOKEN` | Sim, para rotas administrativas | Token usado para criar, alterar, listar e desativar sources. |
| `DEFENSE_IA_SERVER_URL` | Sim, para envio real | URL base do Defense IA. Ex: `http://192.168.0.10`. |
| `DEFENSE_IA_USERNAME` | Sim, para envio real | Usuário do Defense IA. |
| `DEFENSE_IA_PASSWORD` | Sim, para envio real | Senha do Defense IA. |
| `DEFENSE_IA_KEEP_ALIVE_SECONDS` | Não | Intervalo de renovação do token. Padrão: `20`. |
| `DEFENSE_IA_TIMEOUT_SECONDS` | Não | Timeout das chamadas HTTP. Padrão: `10`. |

Sem as variáveis do Defense IA, a API sobe com o client desabilitado. Isso permite testar `/health`, documentação OpenAPI e rotas que usam mocks em teste.

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

Resposta esperada:

```json
{"status":"ok"}
```

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

## Sincronização com a Intelbras

Endpoint:

```http
POST /v1/person/sync
```

Header:

```http
Authorization: Bearer <integration_key>
```

Payload:

```json
{
  "source": "biodoc",
  "operation": "upsert",
  "external_id": "123",
  "person": {
    "full_name": "Maria Silva",
    "document": "12345678900"
  },
  "biometrics": {
    "face_image_base64": "..."
  }
}
```

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
- `502 Bad Gateway`: API do Defense IA indisponível.

## Testes

```bash
pytest
```

Suíte atual:

```text
17 passed
```

## Persistência e Logs

No Docker Compose:

- `./data` é montado em `/app/data` para persistir o SQLite.
- `./log` é montado em `/log` para persistir logs diários.

## Documentação Interativa

Com a API rodando:

- Swagger UI: `http://localhost:8000/docs`
- OpenAPI JSON: `http://localhost:8000/openapi.json`
