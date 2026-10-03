# Webhook opcional de baixa de visita

O middleware BioDoc-Intelbras detecta visita finalizada (`status=2`). Grava `log/visitor_leave.log`, persiste na tela `GET /` e, **se** `VISIT_LEAVE_WEBHOOK_URL` estiver preenchida, faz **um POST por registro novo** para um destino de terceiros. Falhas ficam na tela para reenvio manual ou automático, sempre com o mesmo JSON.

Não há app receptor neste servidor. URL vazia = só log local + tela.

---

## Papel

1. Middleware lê o histórico oficial (`GET /obms/api/v1.1/visitor/history/record/page?status=2`).
2. Deduplica por `visitorId|leaveTime`.
3. Grava `log/visitor_leave.log` e o SQLite da tela `GET /`.
4. Se a URL estiver preenchida, envia o mesmo JSON ao destino.

Não envia funcionário nem alarme de catraca. O envio ocorre depois que a baixa é registrada neste middleware.

---

## Configuração no middleware

No `.env` do `middleware-biodoc`:

```env
VISIT_LEAVE_WEBHOOK_URL=
# VISIT_LEAVE_WEBHOOK_TOKEN=
VISIT_LEAVE_POLL_SECONDS=60
VISIT_LEAVE_RETRY_SECONDS=600
VISIT_LEAVE_RETRY_WINDOW_HOURS=6
VISIT_LEAVE_RETRY_MAX_ATTEMPTS=6
VISIT_LEAVE_UI_TOKEN=troque-por-um-token-da-tela
```

URL vazia = só log local e tela, sem POST.

Reinicie:

```bash
docker compose up -d --force-recreate middleware-biodoc-intelbras
```

---

## Contrato HTTP (destino opcional)

```
POST {VISIT_LEAVE_WEBHOOK_URL}
Content-Type: application/json;charset=UTF-8
Authorization: Bearer {VISIT_LEAVE_WEBHOOK_TOKEN}
```

| Item | Valor |
|------|--------|
| Método | `POST` |
| Body | JSON de **uma** baixa (não é lote) |
| Timeout do cliente | 10 segundos |
| Retry curto | rede ou HTTP `429`, `500`, `502`, `503`, `504` (espera 1s e 3s), conta como uma tentativa |
| Reenvio automático | a cada `VISIT_LEAVE_RETRY_SECONDS` (padrão 600), por `VISIT_LEAVE_RETRY_WINDOW_HOURS` (padrão 6) desde `loggedAt`, no máximo `VISIT_LEAVE_RETRY_MAX_ATTEMPTS` falhas (padrão 6; 0 sem teto) |
| Reenvio manual | na tela: um, vários selecionados ou todos os não enviados do filtro, sem prazo e sem teto de tentativas |
| Sucesso | qualquer HTTP `2xx` |
| Sem retry curto | `401`, `404`, `422`, etc. A tela ainda pode reenviar o mesmo JSON |

O destino deve responder **200 rápido**.

GET `/health` no destino (se existir) não é exigido pelo middleware.

---

## Payload

Exemplo real do log:

```json
{
  "event": "visitor_leave",
  "loggedAt": "2026-09-16T15:22:02-03:00",
  "visitorId": "251437",
  "personId": "178958048913503142",
  "status": "2",
  "visitorName": "SANDRA MARIA DE SOUZA TORTATO",
  "idNum": "00278770000403014",
  "cardNo": null,
  "remark": "00278770000403014",
  "visitedName": "EVB",
  "visitedOrgName": null,
  "arrivalTime": "1789580178",
  "expectLeaveTime": "1789752978",
  "leaveTime": "1789583040",
  "channelId": null,
  "sourceName": null,
  "trigger": "poll",
  "source": "defense_ia"
}
```

| Campo | Tipo | Descrição |
|-------|------|-----------|
| `event` | string | Sempre `visitor_leave` |
| `loggedAt` | string ISO | Quando o middleware gravou (`America/Sao_Paulo`) |
| `visitorId` | string | ID da visita no Defense |
| `personId` | string \| null | Pessoa no Defense |
| `status` | string | Sempre `"2"` (visita baixada) |
| `visitorName` | string \| null | Nome do visitante |
| `idNum` | string \| null | Documento / prontuário (formato varia) |
| `cardNo` | string \| null | Cartão, se houver |
| `remark` | string \| null | Observação / código auxiliar |
| `visitedName` | string \| null | Setor/host: `CENTRAL`, `CDI`, `EVB`, `INT5`… |
| `visitedOrgName` | string \| null | Organização visitada |
| `arrivalTime` | string \| null | Unix da chegada |
| `expectLeaveTime` | string \| null | Unix da saída prevista |
| `leaveTime` | string \| null | Unix da baixa (saída real) |
| `channelId` | null | Sempre `null` neste fluxo |
| `sourceName` | null | Sempre `null` neste fluxo |
| `trigger` | string | Sempre `poll` |
| `source` | string | Sempre `defense_ia` |

Não envia foto. Campos podem vir `null`.

**Idempotência:** `visitorId + "|" + leaveTime`. O middleware não reenvia a mesma chave.

---

## Tela neste servidor

Lista autenticada em `GET /` do middleware (https://un.celx.com.br/), login com `VISIT_LEAVE_UI_TOKEN`.
