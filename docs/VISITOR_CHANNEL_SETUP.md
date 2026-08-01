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

Exemplos: `INT5`, `CDI`, `CHU - CENTRAL`, `Refeitorio` — **igual ao painel**.

### Semântica do nível

O nome enviado é o **ponto de início do fluxo**. O middleware libera **todos os dispositivos** daquele nó até as folhas do ramo na árvore `deviceOrg`:

```text
CHU - CENTRAL     ← envia isto → todos os dispositivos sob o topo
 └── INT5         ← envia isto → todos sob INT5 até as folhas
      ├── Catraca A
      └── Sala B
 └── OutroNivel   ← fora do ramo se enviou INT5
```

Irmãos fora do ramo escolhido **não** entram em `acsChannelIds`.

---

## Webhook BioDoc (`GET /biodoc`)

Fluxo automático via `local_name` / `reguiredName` / `details.nmLocal` do BioDoc. Ver [`ARCHITECTURE.md`](ARCHITECTURE.md). O mesmo critério de ramo vale quando o nome resolve via `deviceOrg`.

---

## Resolução de portas (`acsChannelIds`)

Ordem: regra de acesso (API ACS 6.2.8) → árvore `deviceOrg` (nó + descendentes) → cópia por host visitante. Detalhes nos logs do middleware (`acsChannelIds via deviceOrg`).

---

## Testes

```bash
python scripts/list_visitor_config.py
python scripts/test_defense_sync_visitor.py --org-code "INT5"
python scripts/test_sync_via_api.py
```
