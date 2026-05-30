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

**Não** acrescente parâmetros na URL do webhook (`?operador=`, `?details=`, etc.).
O middleware resolve orgCode via `GET /integrations/log/{reference_Id}` usando
**local_token** (`json.Local Token`) e, em seguida, **reguiredName**.

**Fallback quando o POST urlWebhook não traz `reference_Id`/`logId`/`id_Log`**
(o BioDoc envia só `card` + `image`):

1. `GET /logs/external-audits?idCard={card}&initialDate=&endDate=` — janela de ±15 min
   em torno de `date` do payload (ou últimos 15 min se `date` ausente)
2. Escolhe o log mais recente / mais próximo do timestamp do evento
3. `GET /integrations/log/{id}` — extrai `local_token`, `reguiredName`

Se external-audits ou integrations/log falhar, o sync continua com `orgCode` fallback (`001`).

Alternativa preferencial (quando o BioDoc inclui o ID no callback): use callback **`url`** (redirect)
em vez de `urlWebhook`, apontando para `/webhook/biodoc/redirect` — o BioDoc
inclui `reference_Id` na query do redirect e o fluxo `integrations/log` funciona direto.

Exemplo de URL verify (sandbox, Colaboradores, **sem** parâmetros no webhook):

```
https://web.sandbox.biodoc.com.br/#/integration/verify?card=00271368992672000&token=UG5iZndNeWlVZWxRYmMxWExXaUNFL3cya0VwWEQ1emZqMjNaS05lendpVlkzTGRqaG45RDVLLzl4RTZZTFNJeA==&details=%7B%22operador%22%3A%22colaboradores%22%7D&url=https://homologa.wolfx.com.br/webhook/biodoc/redirect
```

Ou, se precisar manter urlWebhook, peça ao suporte BioDoc para incluir `logId` no
body do POST (junto com `card` e `image`).

Exemplos de URL do webhook (sem query):

```http
POST /webhook/biodoc HTTP/1.1
Host: homologa.wolfx.com.br
Authorization: Bearer <BIODOC_WEBHOOK_TOKEN>
Content-Type: application/json
```

Corpo (exemplo):

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

### 4. Validar TOKEN_API (consulta da interação)

Com o `reference_Id` recebido no webhook, o middleware chama:

```http
GET {BIODOC_API_URL}/integrations/log/{reference_Id}
Authorization: Bearer <BIODOC_TOKEN_API>
```

Resposta esperada (estrutura simplificada):

```json
{
  "id": 1,
  "id_Card": "1234567890",
  "name": "Nome do Beneficiário",
  "status": 2,
  "mainImage": "https://url-temporaria-da-foto.jpg",
  "path": "https://url-temporaria-da-foto.jpg",
  "reguiredName": "Nome da empresa que realizou a interação",
  "detail": "{\"operador\":\"VIVER\"}"
}
```

O middleware extrai **local_token** (`json.Local Token`) e **reguiredName** do
log para mapear `orgCode` no Defense IA (ordem: local_token → reguiredName →
fallback `001`). O campo `detail.operador` da verify **não** é usado.
Identificadores aceitos no POST do webhook:
`reference_Id` → `logId` → `id_Log` (convertido para string na chamada à API).

**Sem identificador no POST:** fallback automático via
`GET /logs/external-audits` + `GET /integrations/log/{id}` (ver seção 3 acima).
Limitação: correlação por janela de tempo — várias verifies do mesmo cartão no
intervalo podem associar log incorreto; preferível redirect com `reference_Id`
ou BioDoc incluir `logId` no body do urlWebhook.

Se `status` não for **1** ou **2** (ativo), o middleware responde **422** e **não** envia ao Defense.

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
