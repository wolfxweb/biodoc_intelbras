# Cadastro de visitante — regra de acesso

O middleware registra **sempre visitante** no Intelbras Defense.

**Não envie** códigos de catraca (`1000045$7$0$0`) no JSON — o middleware monta `rightInfo.acsChannelIds`.

---

## POST `/v1/person/sync`

Informe em **`defense.org_code`** o **nome da regra de acesso** cadastrada no painel Defense (Autorização → Regra de acesso):

```json
"defense": {
  "org_code": "CHU - CENTRAL"
}
```

Exemplos de nomes válidos: `CHU - CENTRAL`, `Refeitorio`, `BLOCO A` — **igual ao painel**.

---

## Webhook BioDoc (`GET /webhook/biodoc`)

Fluxo automático via `local_name` / `reguiredName` / `details.nmLocal` do BioDoc. Ver [`ARCHITECTURE.md`](ARCHITECTURE.md).

---

## Resolução de portas (`acsChannelIds`)

Automática a partir da regra de acesso (API ACS 6.2.8). Detalhes técnicos nos logs do middleware.

---

## Testes

```bash
python scripts/list_visitor_config.py
python scripts/test_defense_sync_visitor.py --org-code "CHU - CENTRAL"
python scripts/test_sync_via_api.py
```
