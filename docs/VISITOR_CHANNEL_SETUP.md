# Cadastro de visitante — regra de acesso

O middleware registra **sempre visitante** no Intelbras Defense.

**Não envie** códigos de catraca (`1000045$7$0$0`) no JSON — o middleware monta `rightInfo.acsChannelIds`.

---

## POST `/v1/person/sync`

Informe em **`defense.org_code`** o **nível da árvore de dispositivos** (ou regra de acesso) no Defense. Qualquer nível válido funciona:

```json
"defense": {
  "org_code": "INT5"
}
```

Exemplos: `INT5`, `Int8`, `CDI`, `CHU Central`, `Refeitorio` — **igual ao painel** (ex.: `CHU Central`, não `CHU - CENTRAL`).

### Semântica do nível

O nome enviado define o **destino**. O middleware resolve o nó no `deviceOrg` e libera:

1. **Pastas no caminho** — cada ancestral até o destino (por nesting JSON **e** prefixo de `code`), incluindo **dispositivos soltos** nessas pastas; **sem** a raiz Local Atual / Current Site (`001`)
2. **O próprio nível** e **descendentes** (folhas do ramo / códigos mais longos com o mesmo prefixo)

Qualquer pasta no caminho com dispositivo solto libera esse acesso — a regra é dinâmica (não depende do nome INT5, Recepção, etc.).

A raiz do site (passarela e acessos genéricos de Local Atual) **não** entra quando o destino é um fluxo como CHU/Int5.

```text
001 Current Site     ← NÃO entra ao enviar Int5 (outro caminho / local base)
001002 CHU Central   ← entra ao enviar Int5 / Int8
Recepção             ← se INT5 estiver sob ela, dispositivos soltos da pasta entram
001002003 Int5       ← envia Int5 → pastas no caminho + Int5 + folhas
001002006 Int8       ← irmão; NÃO entra se enviou Int5
001003 Refeitorio    ← outro ramo; NÃO entra
```

Irmãos (mesmo pai, outro sufixo) **não** entram em `acsChannelIds`.

---

## Webhook BioDoc (`GET /biodoc`)

Fluxo automático via `local_name` / `reguiredName` / `details.nmLocal` do BioDoc. Ver [`ARCHITECTURE.md`](ARCHITECTURE.md). O mesmo critério (caminho + nível + folhas) vale quando o nome resolve via `deviceOrg`.

---

## Resolução de portas (`acsChannelIds`)

Ordem: regra de acesso (API ACS 6.2.8) → árvore `deviceOrg` (pastas no caminho via nesting + prefixo de `code` + descendentes) → cópia por host visitante. Detalhes nos logs do middleware (`acsChannelIds via deviceOrg`).

---

## Testes

```bash
python scripts/list_visitor_config.py
python scripts/test_defense_sync_visitor.py --org-code "INT5"
python scripts/test_sync_via_api.py
```
