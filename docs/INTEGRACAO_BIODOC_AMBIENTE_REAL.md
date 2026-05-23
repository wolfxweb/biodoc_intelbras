# Integração com ambiente real do BioDoc

Guia para configurar o middleware **BIODOC ↔ Intelbras Defense IA** com a API e o webhook oficiais do BioDoc (sandbox ou produção).

---

## Visão geral

Existem **três sistemas** e **duas direções** de comunicação:

```
┌─────────────┐     webhook POST      ┌──────────────────┐     API BRMS      ┌──────────────┐
│   BioDoc    │ ────────────────────► │    Middleware    │ ────────────────► │ Defense IA   │
│  (nuvem)    │   /webhook/biodoc     │  (sua API)       │   upsert pessoa   │  (on-prem)   │
└─────────────┘                       └────────┬─────────┘                   └──────────────┘
       ▲                                       │
       │         GET mainimage (TOKEN_API)     │
       └───────────────────────────────────────┘
```

| Etapa | Quem inicia | O que acontece |
|-------|-------------|----------------|
| 1 | Usuário | Faz liveness/cadastro no app ou fluxo BioDoc |
| 2 | BioDoc | Envia `POST` para a URL do webhook do middleware |
| 3 | Middleware | Valida token do webhook e `success=true` |
| 4 | Middleware | Consulta `GET /card/integration/mainimage?idCard=<card>` na API BioDoc |
| 5 | Middleware | Baixa a URL da foto, converte para base64 |
| 6 | Middleware | Faz upsert da pessoa no Defense (`personId` = número do cartão) |

O middleware **não** substitui o painel BioDoc: ele só recebe o evento e sincroniza no Defense.

---

## Pré-requisitos

### No BioDoc (painel administrativo)

- Conta com acesso à área de **integração / API**
- **TOKEN_API** — Bearer para o middleware consultar beneficiários
- Permissão para cadastrar **URL do WebHook** e **token do WebHook**
- Beneficiários de teste com cartão (`idCard`) e foto cadastrados (sandbox antes de produção)

### No Intelbras Defense IA

- Servidor acessível pela rede onde o middleware roda (IP público, VPN ou rede interna)
- Usuário de **API** (ex.: `system`, em minúsculas) e senha definidos no painel
- Chave RSA pareada (`DEFENSE_IA_PUBLIC_KEY`) — ver `README.md` e `scripts/generate_defense_rsa_keys.py`

### Infraestrutura do middleware

- URL **HTTPS pública** (recomendado) para o BioDoc chamar o webhook — ex.: `https://homologa.wolfx.com.br`
- O middleware precisa conseguir:
  - **Saída HTTPS** para `BIODOC_API_URL` (consulta de beneficiário)
  - **Saída HTTP(S)** para `DEFENSE_IA_SERVER_URL`
  - **Download** da URL da imagem retornada pelo BioDoc (geralmente HTTPS na nuvem BioDoc)

---

## Ambientes BioDoc: sandbox vs produção

| Item | Sandbox (homologação) | Produção |
|------|------------------------|----------|
| `BIODOC_API_URL` | `https://api.sandbox.biodoc.com.br/api` | `https://api.biodoc.com.br/api` |
| `BIODOC_AMBIENTE` | `sandbox` | `production` |
| Painel / credenciais | Conta de testes BioDoc | Conta produção |
| Webhook URL | URL de homolog do middleware | URL de produção do middleware |

**Importante:** `BIODOC_TOKEN_API` e `BIODOC_WEBHOOK_TOKEN` do sandbox **não** servem em produção (e vice-versa). Gere/configure tokens separados por ambiente.

---

## Variáveis de ambiente (`.env`)

Copie o modelo e preencha:

```bash
cp .env.example .env
```

### BioDoc (obrigatórias para o fluxo webhook)

| Variável | Obrigatória | Descrição |
|----------|:-----------:|-----------|
| `BIODOC_API_URL` | Sim | Base da API (tabela acima) |
| `BIODOC_TOKEN_API` | Sim | Bearer **TOKEN_API** do painel BioDoc |
| `BIODOC_WEBHOOK_TOKEN` | Sim | Token que **você define** e repete no painel BioDoc (header `Authorization: Bearer ...`) |
| `BIODOC_AMBIENTE` | Não | `sandbox` ou `production` — só para log/rastreio |

### Defense IA (obrigatórias para gravar pessoa)

| Variável | Descrição |
|----------|-----------|
| `DEFENSE_IA_SERVER_URL` | Ex.: `http://200.180.74.90` |
| `DEFENSE_IA_USERNAME` | Usuário API (minúsculas, ex.: `system`) |
| `DEFENSE_IA_PASSWORD` | Senha do usuário API |
| `DEFENSE_IA_PUBLIC_KEY` | RSA Base64 (gerar com `scripts/generate_defense_rsa_keys.py`) |
| `DEFENSE_IA_API_MODE` | `brms` (Defense IA 3.x) |

Demais variáveis Defense: ver `.env.example` e `README.md`.

### Middleware / deploy

| Variável | Uso |
|----------|-----|
| `MIDDLEWARE_URL` | Base usada pelos scripts de teste (ex.: `https://homologa.wolfx.com.br`) |
| `ADMIN_API_TOKEN` | Rotas admin e bootstrap da source `biodoc` |

**Não commitar** o arquivo `.env` (contém senhas e tokens).

---

## Passo a passo — configuração

### 1. Configurar o `.env` no servidor

No host onde o container roda (ex.: `/root/biodoc_intelbras/.env`):

```env
# BioDoc — ambiente real (exemplo sandbox)
BIODOC_API_URL=https://api.sandbox.biodoc.com.br/api
BIODOC_TOKEN_API=<cole_o_TOKEN_API_do_painel>
BIODOC_WEBHOOK_TOKEN=<gere_um_token_longo_aleatorio>
BIODOC_AMBIENTE=sandbox

# Defense (já existente no seu deploy)
DEFENSE_IA_SERVER_URL=http://200.180.74.90
DEFENSE_IA_USERNAME=system
DEFENSE_IA_PASSWORD=***
DEFENSE_IA_PUBLIC_KEY=***
# ...

MIDDLEWARE_URL=https://homologa.wolfx.com.br
```

Gere um `BIODOC_WEBHOOK_TOKEN` forte (ex.: 32+ caracteres aleatórios). Esse valor deve ser **idêntico** no `.env` e no painel BioDoc.

### 2. Subir o middleware

**Docker Compose (local / VM):**

```bash
cd biodoc_intelbras
docker compose up -d --build
curl -s http://localhost:8000/status | jq
```

**Docker Swarm (homologação Wolfx):**

- Stack: `docker-compose.swarm.yml`
- Host Traefik: `homologa.wolfx.com.br` → porta 8000 do serviço
- `env_file`: `/root/biodoc_intelbras/.env`

Confirme no `/status`:

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

Se `connected: false`, o webhook pode retornar **503** até o login no Defense concluir.

### 3. Configurar o WebHook no painel BioDoc

No painel BioDoc (área de integração / webhook):

| Campo no BioDoc | Valor |
|-----------------|--------|
| **URL do WebHook** | `https://<seu-dominio>/webhook/biodoc` |
| **Token / Authorization** | Mesmo valor de `BIODOC_WEBHOOK_TOKEN` |

Exemplos de URL:

| Ambiente | URL típica |
|----------|------------|
| Homolog Wolfx | `https://homologa.wolfx.com.br/webhook/biodoc` |
| Local (só teste interno) | `http://IP:8000/webhook/biodoc` — **o BioDoc na nuvem normalmente não alcança localhost** |

O BioDoc envia:

```http
POST /webhook/biodoc HTTP/1.1
Host: homologa.wolfx.com.br
Authorization: Bearer <BIODOC_WEBHOOK_TOKEN>
Content-Type: application/json
```

Corpo (exemplo):

```json
{
  "confidence": "98",
  "date": "2025-02-04T12:34:56Z",
  "response": 201,
  "message": "Cadastro realizado com sucesso!",
  "card": "1234567890",
  "image": "https://...",
  "success": true,
  "LogID": "abc-123"
}
```

### 4. Validar TOKEN_API (consulta de beneficiário)

O middleware chama:

```http
GET {BIODOC_API_URL}/card/integration/mainimage?idCard=<card>
Authorization: Bearer <BIODOC_TOKEN_API>
```

Resposta esperada (estrutura simplificada):

```json
{
  "data": {
    "name": "Nome do Beneficiário",
    "card": "1234567890",
    "status": true,
    "image": "https://url-da-foto.jpg"
  }
}
```

Se `status` for `false`, o middleware responde **422** e **não** envia ao Defense.

### 5. Mapeamento no Defense IA

| Campo middleware | Campo Defense |
|------------------|---------------|
| `card` (webhook) | `personId` / `external_id` |
| `name` (API BioDoc) | Nome da pessoa |
| `card` | Documento (mesmo valor do cartão) |
| Imagem (download) | `facePictures` (base64) |

---

## Checklist antes de ir para produção

- [ ] `.env` com `BIODOC_API_URL` e tokens do ambiente correto (sandbox testado antes)
- [ ] `GET /status` com Defense `connected: true` e BioDoc `configured: true`
- [ ] Webhook BioDoc apontando para URL **HTTPS** acessível da internet
- [ ] `BIODOC_WEBHOOK_TOKEN` igual no `.env` e no painel BioDoc
- [ ] Teste com um `card` real existente no BioDoc (sandbox)
- [ ] Pessoa aparece no Defense com `personId` = número do cartão
- [ ] Logs em `log/app.log` com linha `sincronizado com sucesso no Defense IA`
- [ ] Firewall: middleware → BioDoc API e → Defense liberados

---

## Como testar

### A) Teste com mocks (não grava no Defense)

Valida só a orquestração do código:

```bash
python3 scripts/test_webhook_flow_mocked.py --name "teste webhook"
```

### B) Teste webhook → Defense real (BioDoc mockado)

Útil quando ainda não tem TOKEN_API, mas Defense já está configurado:

```bash
PYTHONPATH=. python3 scripts/test_webhook_send_defense.py \
  --card testewebhook001 --name "teste webhook"
```

### C) Teste contra middleware publicado (simula o BioDoc)

Com `BIODOC_WEBHOOK_TOKEN` no `.env`:

```bash
python3 scripts/test_biodoc_webhook.py \
  --url https://homologa.wolfx.com.br \
  --card <cartao_real_no_sandbox>
```

**Nota:** esse script envia o webhook, mas a API BioDoc ainda será chamada de verdade no servidor (precisa de `BIODOC_TOKEN_API` válido no `.env` do container).

### D) Teste real completo (recomendado em sandbox)

1. Configure webhook no painel BioDoc apontando para o middleware
2. Realize liveness/cadastro no app BioDoc com um beneficiário de teste
3. Acompanhe `log/app.log` no servidor
4. Confirme a pessoa no Defense pelo `personId` = `card`

### E) Validar login Defense isolado

```bash
PYTHONPATH=. python3 scripts/test_defense_login.py
```

---

## Códigos de resposta do webhook

| HTTP | Significado | Ação |
|------|-------------|------|
| 200 | Sincronizado com sucesso | Conferir pessoa no Defense |
| 401 | Token webhook inválido ou ausente | Conferir `BIODOC_WEBHOOK_TOKEN` e header no BioDoc |
| 422 | `success=false`, sem `card`, inativo no BioDoc, sem imagem, face rejeitada | Ver payload e cadastro no BioDoc |
| 502 | API BioDoc inacessível, TOKEN_API inválido, ou erro do Defense | Ver logs e credenciais |
| 503 | Defense não conectado no startup | Ver `/status`, rede e login Defense |

Corpo de sucesso:

```json
{
  "status": "success",
  "external_id": "1234567890",
  "defense_sync": "ok"
}
```

---

## Rede e segurança

1. **HTTPS** no webhook em produção (certificado válido no Traefik/nginx).
2. **Token webhook** exclusivo, rotacionável; não reutilizar `ADMIN_API_TOKEN`.
3. **TOKEN_API** só no servidor; nunca expor no frontend.
4. Restringir origem se possível (IP BioDoc — consultar documentação/suporte BioDoc).
5. Logs em `log/app.log` — não habilitar log de imagem base64 em produção (`LOG_LEVEL=INFO`).

---

## Troubleshooting

| Sintoma | Causa provável | Solução |
|---------|----------------|---------|
| BioDoc não chama o webhook | URL errada, HTTP sem TLS, firewall | URL pública HTTPS; testar com `test_biodoc_webhook.py` |
| 401 no webhook | Token diferente entre painel e `.env` | Alinhar `BIODOC_WEBHOOK_TOKEN` |
| 502 "Credencial da API BioDoc" | `BIODOC_TOKEN_API` inválido ou ambiente errado | Token do mesmo ambiente que `BIODOC_API_URL` |
| 422 "inativo no BioDoc" | `status: false` no mainimage | Ativar beneficiário no painel |
| 422 sem imagem | Sem URL em API nem no payload | Cadastrar foto no BioDoc |
| 503 Defense | Login falhou no startup | `test_defense_login.py`, chaves RSA, usuário `system` |
| Pessoa não aparece no Defense | `personId` diferente do esperado | Buscar pelo **card**, não só pelo nome |

---

## Referências no repositório

| Arquivo | Conteúdo |
|---------|----------|
| `README.md` | Visão geral, Defense, Docker |
| `.env.example` | Modelo de variáveis |
| `src/services/biodoc_webhook_service.py` | Orquestração do fluxo |
| `src/services/biodoc_client.py` | Cliente API BioDoc |
| `docker-compose.swarm.yml` | Deploy homolog Wolfx |
| `scripts/test_biodoc_webhook.py` | Simula POST do BioDoc |
| `scripts/test_webhook_send_defense.py` | Webhook real + BioDoc mock |

---

## Resumo rápido (produção)

1. Obter **TOKEN_API** e cadastrar **webhook** no painel BioDoc produção.
2. Definir `BIODOC_API_URL=https://api.biodoc.com.br/api` e tokens no `.env`.
3. Publicar middleware em URL HTTPS de produção.
4. Garantir Defense acessível e `connected: true` no `/status`.
5. Testar um cadastro real no sandbox; depois repetir em produção com um beneficiário piloto.
