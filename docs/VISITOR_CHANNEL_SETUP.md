# Cadastro de visitante — automático via orgCode

O middleware registra **sempre visitante** no Intelbras Defense.

Resolução de `orgCode` documentada em [`BIODOC_ORGCODE_SETUP.md`](BIODOC_ORGCODE_SETUP.md).

**Não é necessário** configurar `DEFENSE_IA_VISITOR_CHANNEL_MAP` nem
`DEFENSE_IA_VISITED_PERSON_ID` no `.env` para operação normal.

---

## Como funciona

### Webhook BioDoc (`GET /webhook/biodoc`)

1. BioDoc envia `local_token` (ex.: `CHU - ESPAÇO VIVER BEM`)
2. Middleware resolve → `orgCode` (ex.: `001021`) via `person-group/list`
3. `sync_visitor(org_code)` monta o payload e resolve portas automaticamente

### POST `/v1/person/sync` (cadastro direto)

Informe no JSON:

```json
"defense": {
  "org_code": "001021"
}
```

Portas de acesso são resolvidas automaticamente (mesma lógica do webhook).

---

## Resolução de portas (`acsChannelIds`)

Ordem automática:

1. Override opcional no `.env` (`DEFENSE_IA_VISITOR_CHANNEL_MAP`)
2. Árvore `GET /brms/api/v1.0/tree/deviceOrg?channelTypes=7` (por `orgCode` ou nome)
3. Config global `GET /brms/api/v1.1/config/visitor`
4. Fallback: permissão padrão do visitante (`enableDefaultRight`)

---

## Variáveis opcionais (.env)

| Variável | Quando usar |
|----------|-------------|
| `DEFENSE_IA_VISITOR_CHANNEL_MAP` | Override manual por orgCode (exceção) |
| `DEFENSE_IA_VISITOR_CHANNEL_DEFAULT` | Override CSV global (exceção) |
| `DEFENSE_IA_VISITED_PERSON_ID` | Só se o painel exigir anfitrião explícito |
| `DEFENSE_IA_VISITOR_STATUS` | `1` = em visita (padrão) |

---

## Testes

```bash
python scripts/list_person_orgs.py
python scripts/list_visitor_config.py
python scripts/test_defense_sync_visitor.py --org-code 001021
python scripts/test_sync_via_api.py
```
