# Cadastro de visitante — automático via orgCode

O fluxo visitante reutiliza **a mesma resolução de `orgCode`** do cadastro de pessoa ACS
(`local_token` / `reguiredName` BioDoc → `person-group/list`), documentada em
[`BIODOC_ORGCODE_SETUP.md`](BIODOC_ORGCODE_SETUP.md).

**Não é necessário** configurar `DEFENSE_IA_VISITOR_CHANNEL_MAP` nem
`DEFENSE_IA_VISITED_PERSON_ID` no `.env` para operação normal.

---

## Como funciona

1. BioDoc envia `local_token` (ex.: `CHU - ESPAÇO VIVER BEM`)
2. Middleware resolve → `orgCode` (ex.: `001021`) via `person-group/list`
3. `sync_visitor(org_code)` monta o payload:
   - `visitedName` / `visitedOrgName` / `visitorOrgName` ← nome da sub-org no Defense
   - `acsChannelIds` ← resolvidos automaticamente:
     1. Override opcional no `.env` (`DEFENSE_IA_VISITOR_CHANNEL_MAP`)
     2. Árvore `GET /brms/api/v1.0/tree/deviceOrg?channelTypes=7` (por `orgCode` ou nome)
     3. Config global `GET /brms/api/v1.1/config/visitor`
     4. Lista vazia `[]` → Defense aplica **direito padrão do visitante** (`enableDefaultRight`)

Mesma ideia do cadastro de pessoa: portas vinculadas no painel Defense; o middleware
só passa a chave (`orgCode` + nome da org).

---

## Variáveis (todas opcionais)

| Variável | Quando usar |
|----------|-------------|
| `DEFENSE_IA_SYNC_TARGET=visitor` | Padrão. Use `person` só para rollback ACS |
| `DEFENSE_IA_VISITOR_CHANNEL_MAP` | Override manual por orgCode (exceção) |
| `DEFENSE_IA_VISITOR_CHANNEL_DEFAULT` | Override CSV global (exceção) |
| `DEFENSE_IA_VISITED_PERSON_ID` | Só se o painel exigir anfitrião explícito |
| `DEFENSE_IA_VISITOR_STATUS` | `1` = em visita (padrão) |

---

## Testes

```bash
python scripts/list_person_orgs.py          # orgCodes BioDoc
python scripts/list_visitor_config.py      # config global visitante
python scripts/test_defense_sync_visitor.py --org-code 001021
```

---

## Rollback para pessoa ACS

```env
DEFENSE_IA_SYNC_TARGET=person
```

Reinicie o serviço (`bash deploy.sh`).
