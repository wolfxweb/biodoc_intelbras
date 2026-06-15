# Vínculo de portas: BioDoc → Defense IA via `orgCode`

> **POST `/v1/person/sync`:** em `defense.org_code`, envie o **nome da regra de acesso**
> no painel Defense (ex.: `CHU - CENTRAL`). Ver [`VISITOR_CHANNEL_SETUP.md`](./VISITOR_CHANNEL_SETUP.md).
>
> Este documento trata do fluxo **webhook BioDoc → cadastro de pessoa** com roteamento por
> **`reguiredName` → `orgCode`** de sub-organização (legado Unimed).

Guia operacional para liberar pessoas em portas específicas do Intelbras
Defense IA usando o campo **`reguiredName`** do BioDoc como chave de
roteamento.

> **Pré-requisito**: a integração base BioDoc ↔ middleware ↔ Defense IA
> já configurada conforme [`INTEGRACAO_BIODOC_AMBIENTE_REAL.md`](./INTEGRACAO_BIODOC_AMBIENTE_REAL.md).
> Este documento trata só do **roteamento por sub-organização**.

---

## TL;DR (30 segundos)

1. No painel desktop do **Defense IA**, descubra os códigos das sub-organizações
   (ex.: `Corb` = `001015001`, `Colaboradores` = `001008`).
2. No painel do **BioDoc**, ao cadastrar o beneficiário, preencha o campo
   **`reguiredName`** com **o código** da sub-org (ex.: `001015001`).
3. Pronto. O middleware envia a pessoa para a sub-org correta no Defense IA,
   que já tem as portas vinculadas pelo painel desktop.

Não há nada novo no `.env`. Não há mapeamento manual em arquivo.

---

## Por que `orgCode` (sub-organização) e não Access Permission Group?

A documentação oficial Intelbras BRMS 3.x descreve dois caminhos para liberar
portas para uma pessoa:

```
Caminho A (documentado):  Person → Access Permission Group → Door Group(s) → Channel(s)
Caminho B (em uso):       Person → Sub-Organização (orgCode) → Portas vinculadas no painel
```

Foi feita varredura no servidor de produção da Unimed (`200.180.74.90`) e
apenas o **Caminho B** funciona neste deploy:

- Os endpoints `/obms/api/v1.1/acs/access-group/list`,
  `/obms/api/v1.0/accessControl/door-group/page` e correlatos retornam
  **HTTP 404** (não estão habilitados no servidor).
- POST/PUT de pessoa com `accessGroupIds` ou `passageRules` retorna `200 OK`,
  mas o GET subsequente mostra esses campos **ignorados** (sempre `null`).
- POST/PUT de pessoa com `baseInfo.orgCode = "001015001"` (Corb) retorna
  `200 OK` e o GET mostra `orgInfos[0].hasAuthority = "1"` — a pessoa
  **está autorizada** nas portas que o painel desktop vinculou à sub-org.

Por isso o middleware passou a usar o `orgCode` da sub-org como mecanismo de
autorização. As portas continuam sendo vinculadas no painel desktop por
sub-org (não no `.env`, não via API).

---

## 1. Descobrir o `orgCode` de cada sub-organização

### 1.1 Pelo painel desktop do Defense IA

`Pessoas → Organizações` mostra a árvore de sub-orgs com seus códigos
hierárquicos. Exemplos no servidor da Unimed:

| Sub-org | `orgCode` |
|---|---|
| Unimed (raiz) | `001` |
| Médicos Não Cooperados | `001001` |
| Colaboradores | `001008` |
| Glow | `001014003` |
| Fornecedor – Prestador de SRV. | `001014021` |
| H&H Consultoria | `001014022` |
| **Corb** | **`001015001`** |
| Quimioterapia | `001015004` |
| Medicina Nuclear | `001015005` |
| Imuno | `001015006` |
| Estagiários | `001016` |
| Residente Médico | `001016005` |
| Personal – EVB | `001019` |
| Seguridade | `001020` |

### 1.2 Pelo middleware (recomendado)

Lista grupos via `GET /obms/api/v1.1/acs/person-group/list` (~200 ms):

```bash
docker compose run --rm --no-deps -v "${PWD}:/app" \
  middleware-biodoc-intelbras python scripts/list_person_orgs.py

# Forçar refresh do cache (após criar nova sub-org no painel)
python scripts/list_person_orgs.py --refresh
```

Saída:

```
Servidor: http://200.180.74.90
Login OK
Fonte dos dados: person-group/list

Total: 51 sub-organização(ões)

orgCode        orgName
--------------------------------------------------
001            Unimed
001008         Colaboradores
001014003      Glow
...
001015001      Corb
...
```

> O middleware aquece esse cache no boot. Subsequentes usam memória (TTL 30 min).

---

## 2. Cadastrar o usuário no BioDoc

No painel administrativo do BioDoc, ao cadastrar um beneficiário, há o campo
**`reguiredName`** (atenção: o BioDoc tem essa grafia com erro de digitação).
Esse campo é o que o middleware usa para decidir em qual sub-org do Defense IA
a pessoa entra.

### 2.1 Formato recomendado: código direto

| Campo BioDoc | Valor | Resultado no Defense IA |
|---|---|---|
| `reguiredName` | `001015001` | Pessoa criada com `orgCode=001015001` (Corb). **0 ms** de overhead. |
| `reguiredName` | `001008` | Pessoa criada com `orgCode=001008` (Colaboradores). |
| `reguiredName` | `001020` | Pessoa criada com `orgCode=001020` (Seguridade). |

**Vantagem**: instantâneo, não consulta a API do Defense IA antes do cadastro.

### 2.2 Formato alternativo: nome amigável

Se preferir digitar o nome em vez do código, também funciona:

| Campo BioDoc | Valor | Resultado |
|---|---|---|
| `reguiredName` ou `detail.nmLocal` | `Corb` | Middleware busca o nome no cache de `person-group/list`, encontra `001015001`. |
| `reguiredName` ou `detail.nmLocal` | `corb` / `CORB` / `  Corb  ` | Mesma coisa (case-insensitive, trim). |
| `reguiredName` ou `detail.nmLocal` | `Colaboradores` | `orgCode=001008`. |
| `reguiredName` | `CHU - ESPAÇO VIVER BEM` | `orgCode=001021`. |

O **local_name** (host visitante) vem de `GET /integrations/log/{reference_Id}`:
campo **`reguiredName`** (API BioDoc) ou **`detail.nmLocal` / `detail.RequiredName`**
(doc [Detail](https://docs.biodoc.com.br/detail/#estrutura-do-json-detail)).
O parâmetro `details` da URL verify (`nmLocal`, etc.) tem prioridade quando repassado
no redirect. O `detail.operador` **não** entra na resolução de local.

Quando o redirect não traz `reference_Id`/`logId`/`id_Log`, o middleware consulta
`GET /logs/external-audits?idCard=...` e em seguida `GET /integrations/log/{id}`
(janela ±15 min em torno de `date`).

> Lookup por nome consulta o cache de grupos (aquecido no boot via
> `person-group/list`). Após criar nova sub-org no painel, rode
> `scripts/list_person_orgs.py --refresh` ou reinicie o container.

### 2.3 Quando o `reguiredName` não casa

Se a string enviada não for um código válido nem casar com nenhuma sub-org
conhecida, a pessoa cai no `DEFENSE_IA_ORG_CODE` raiz do `.env` (default:
`001` = Unimed). O webhook **continua retornando 200**, mas é gravado um
`logger.warning`:

```
[WEBHOOK] ref=xxx reguiredName='Inexistente' sem sub-org correspondente, usando 001
```

Para mapear uma sub-org criada após o middleware ter bootado, rode
`scripts/list_person_orgs.py --refresh` (renova o cache) ou reinicie o
container.

---

## 3. Fluxo do dado: onde o `orgCode` é preenchido

```
┌───────────────────┐
│ 1) Painel BioDoc  │  Cadastra beneficiário com reguiredName="001015001"
└─────────┬─────────┘
          │ liveness / cadastro
          ▼
┌───────────────────┐
│ 2) Webhook BioDoc │  POST /biodoc {reference_Id, success, status, url, ...}
└─────────┬─────────┘
          │
          ▼
┌────────────────────────────────────────┐
│ 3) Middleware (este projeto)           │
│                                        │
│  a) GET /integrations/log/{reference_Id}│ ── busca dados completos no BioDoc
│     -> retorna id_Card, reguiredName,   │
│        detail.nmLocal (local_name)      │
│     (sem reference_Id: external-audits  │
│      -> integrations/log/{id})          │
│                                  │
│  b) defense_client.resolve_org_code(reguiredName)
│     -> "001015001" (atalho dígito) │
│        ou lookup no cache          │
│                                  │
│  c) build_person_payload(payload, org_code="001015001")
│     -> baseInfo.orgCode = "001015001"
│                                  │
│  d) POST/PUT /obms/api/v1.1/acs/person
└─────────┬────────────────────────┘
          │
          ▼
┌───────────────────────────────────────┐
│ 4) Defense IA grava                   │
│   baseInfo.orgCode = "001015001"      │
│   orgInfos = [{orgCode:..., orgName:"Corb",
│                hasAuthority:"1"}]     │
│   → Pessoa autorizada nas portas     │
│     que o painel desktop vinculou    │
│     à sub-org Corb.                   │
└───────────────────────────────────────┘
```

---

## 4. Onde isso está implementado no código

### 4.1 Recepção do `reguiredName`

`src/services/biodoc_client.py`

```python
@dataclass
class IntegrationLogData:
    ...
    required_name: str | None  # ← reguiredName do BioDoc

async def get_integration_log(self, reference_id: str) -> IntegrationLogData:
    ...

async def get_external_audits(
    self, id_card: str, *, initial_date=None, end_date=None
) -> list[ExternalAuditEntry]:
    """GET /logs/external-audits — usado como fallback para descobrir log id."""
    return IntegrationLogData(
        ...
        required_name=body.get("reguiredName") or None,
    )
```

### 4.2 Resolução `reguiredName` → `orgCode`

`src/services/biodoc_webhook_service.py` (no fluxo do webhook):

```python
resolved_org_code: str | None = None
if log_data.required_name:
    try:
        resolved_org_code = await defense_client.resolve_org_code(
            log_data.required_name
        )
    except DefenseIAError as exc:
        logger.warning(...)
if not resolved_org_code:
    resolved_org_code = defense_client.settings.org_code or "001"

await defense_client.sync_person(sync_request, resolved_org_code)
```

### 4.3 Lookup no cache (com atalho de código)

`src/services/defense_ia_client.py`:

```python
async def resolve_org_code(self, name: str | None) -> str | None:
    if not name:
        return None
    key = name.strip()
    if not key:
        return None
    # Atalho: código direto não consulta a API.
    if key.isdigit():
        return key
    orgs, _source = await self.list_available_orgs()  # person-group/list, cache TTL 30 min
    return orgs.get(_normalize_org_lookup_key(key))
```

`list_available_orgs()` chama `GET /obms/api/v1.1/acs/person-group/list` (resposta
em `data.results`), cacheado em `self._person_groups_cache` por
`PERSON_GROUPS_CACHE_TTL_SECONDS = 1800`. O cache é aquecido no boot do middleware.

### 4.4 Gravação no payload da pessoa

`src/services/defense_ia_client.py`, dentro de `build_person_payload`:

```python
candidate = (org_code or "").strip()
resolved_org_code = candidate or self.settings.org_code or "001"

return {
    "baseInfo": {
        "personId": payload.external_id,
        "lastName": "",
        "firstName": payload.person.full_name,
        "gender": "0",
        "orgCode": resolved_org_code,   # ← aqui é gravado
        "source": "0",
        "facePictures": face_pictures,
    },
    ...
    "accessInfo": {
        "accessType": "0",
        "guestUseTimes": "0",
        "allowLoginDevice": "0",
        # (sem accessGroupIds / enableAccessGroup — ignorados pelo servidor)
    },
    ...
}
```

---

## 5. Validação rápida no servidor

Após cadastrar uma pessoa via webhook, confirme via API que o `orgCode` e o
`hasAuthority` estão corretos:

```bash
PYTHONPATH=. python3 -c "
import asyncio
from src.core.lifespan import build_defense_client_from_env
from dotenv import load_dotenv; load_dotenv('.env')

async def main():
    c = build_defense_client_from_env()
    await c.login()
    r = await c._request('GET', '/obms/api/v1.1/acs/person/<personId>',
                         headers=c._auth_headers())
    bi = r.json()['data']['baseInfo']
    print('orgCode    :', bi.get('orgCode'))
    print('orgName    :', bi.get('orgName'))
    print('orgInfos   :', bi.get('orgInfos'))
    print('faces      :', len(bi.get('facePictures') or []))
    await c.close()

asyncio.run(main())
"
```

Saída esperada (caso de sucesso):

```
orgCode    : 001015001
orgName    : Corb
orgInfos   : [{'orgCode': '001015001', 'orgName': 'Corb', 'hasAuthority': '1'}]
faces      : 1
```

`hasAuthority: "1"` confirma que a pessoa está autorizada nas portas que o
painel desktop vinculou à sub-org `Corb`.

---

## 6. Troubleshooting

| Sintoma | Causa provável | Solução |
|---|---|---|
| Pessoa entra em `001` (Unimed) em vez da sub-org esperada | `local_name`/`reguiredName` em branco ou nome errado | Conferir log BioDoc (`reguiredName`, `detail.nmLocal`); usar `scripts/list_person_orgs.py` para checar nomes válidos. |
| Nome de grupo não resolve após boot | Cache frio ou grupo criado após boot | Aguardar warmup ou `python scripts/list_person_orgs.py --refresh`. |
| Sub-org criada agora não é reconhecida | Cache desatualizado (TTL 30 min) | `python scripts/list_person_orgs.py --refresh` ou reinicie o container. |
| `hasAuthority: "0"` no GET | Sub-org não tem porta vinculada no painel desktop | Acesse o painel e vincule as portas à sub-org. |
| Foto recebida do BioDoc rejeitada pelo Defense | Imagem fora do padrão Defense IA (resolução, ângulo) | O BioDoc real tem liveness; em testes use `scripts/download_test_face.py` com resolução >= 720x720. |

---

## 7. Referências no código

| Arquivo | Função |
|---|---|
| `src/services/biodoc_client.py` | Lê `reguiredName` da resposta `/integrations/log/{reference_Id}` |
| `src/services/biodoc_webhook_service.py` | Orquestra resolução `reguiredName → orgCode` |
| `src/services/defense_ia_client.py` | `list_person_groups()`, `list_available_orgs()`, `resolve_org_code()`, `build_person_payload()` |
| `scripts/list_person_orgs.py` | CLI para listar sub-orgs e refrescar cache |
| `tests/test_defense_ia_client.py` | Testes do cache, atalho de código e payload |
| `tests/test_webhook_biodoc.py` | Testes do fluxo end-to-end (mockado) |
