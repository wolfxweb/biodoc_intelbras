# URL de destino da baixa de visita

Este middleware detecta visita **baixada** no Defense IA (`status=2`). Grava o registro na tela e, se a **URL de destino** estiver preenchida, envia **um POST JSON** para o sistema de vocês.

URL vazia = só lista local, sem envio.

---

## 1. Como configurar a URL

1. Entre em https://un.celx.com.br/ (senha da tela).
2. No campo **URL de destino**, informe o endpoint HTTPS que vai receber as baixas.  
   Exemplo: `https://sistema.seudominio.com.br/webhooks/visitor-leave`
3. Clique em **Salvar URL**. A URL passa a ser usada nos envios seguintes. Não precisa editar `.env` nem reiniciar.

Para parar o envio, apague a URL e salve de novo.

Opcional no `.env` do middleware (não aparece nesta tela):

```env
VISIT_LEAVE_WEBHOOK_TOKEN=um-segredo-combinado
```

Se esse token estiver preenchido, o POST leva `Authorization: Bearer …`. O sistema de vocês deve validar o mesmo valor.

---

## 2. Como os dados são enviados

| Item | Valor |
|------|--------|
| Método | `POST` |
| URL | a URL salva na tela |
| Content-Type | `application/json;charset=UTF-8` |
| Body | **uma** baixa por request (não é lote) |
| Timeout | 10 segundos |
| Sucesso | qualquer HTTP `2xx` |
| Retry | só rede ou `429` / `500` / `502` / `503` / `504` (espera 1s e 3s) |

Não envia foto, funcionário nem alarme de catraca. O envio ocorre depois que a baixa é registrada neste middleware.

Chave única (não reenvia a mesma): `visitorId` + `|` + `leaveTime`. O receptor deve deduplicar mesmo assim.

Na lista, a coluna **Enviado para URL destino** fica **sim** quando o POST retornou `2xx`.

---

## 3. Payload (exemplo real)

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

| Campo | Tipo | Uso |
|-------|------|-----|
| `event` | string | Sempre `visitor_leave` |
| `loggedAt` | string ISO | Quando o middleware registrou (`America/Sao_Paulo`) |
| `visitorId` | string | ID da visita no Defense |
| `personId` | string ou null | Pessoa no Defense |
| `status` | string | Sempre `"2"` (baixada) |
| `visitorName` | string ou null | Nome do visitante |
| `idNum` | string ou null | Documento / prontuário (formato varia) |
| `cardNo` | string ou null | Cartão, se houver |
| `remark` | string ou null | Observação / código auxiliar |
| `visitedName` | string ou null | Setor/host (`CENTRAL`, `CDI`, `EVB`, `INT5`…) |
| `visitedOrgName` | string ou null | Organização visitada |
| `arrivalTime` | string ou null | Unix da chegada |
| `expectLeaveTime` | string ou null | Unix da saída prevista |
| `leaveTime` | string ou null | Unix da baixa (saída real) |
| `channelId` | null | Sempre `null` neste fluxo |
| `sourceName` | null | Sempre `null` neste fluxo |
| `trigger` | string | Sempre `poll` |
| `source` | string | Sempre `defense_ia` |

`arrivalTime`, `leaveTime` e `expectLeaveTime` vêm como **string de Unix**. Não vem foto. Campos podem ser `null`.

---

## 4. Código de exemplo — receber no sistema de vocês

Exemplo mínimo em FastAPI. Cole em uma VPS/app **de vocês** (não neste middleware). Responda `200` rápido; grave no banco depois.

`requirements.txt`:

```
fastapi==0.115.0
uvicorn==0.32.0
```

`app.py`:

```python
import os
from fastapi import FastAPI, Header, HTTPException
from pydantic import BaseModel

app = FastAPI(title="Receptor baixa de visita")
TOKEN = os.getenv("WEBHOOK_TOKEN", "").strip()
seen: set[str] = set()


class VisitorLeave(BaseModel):
    event: str
    visitorId: str | None = None
    personId: str | None = None
    status: str | None = None
    visitorName: str | None = None
    idNum: str | None = None
    cardNo: str | None = None
    remark: str | None = None
    visitedName: str | None = None
    visitedOrgName: str | None = None
    arrivalTime: str | None = None
    expectLeaveTime: str | None = None
    leaveTime: str | None = None
    loggedAt: str | None = None
    trigger: str | None = None
    source: str | None = None


@app.get("/health")
def health():
    return {"ok": True}


@app.post("/webhooks/visitor-leave")
def receive(body: VisitorLeave, authorization: str | None = Header(default=None)):
    if TOKEN and authorization != f"Bearer {TOKEN}":
        raise HTTPException(status_code=401, detail="unauthorized")
    if body.event != "visitor_leave" or body.status != "2":
        return {"ok": True, "ignored": True}

    key = f"{body.visitorId}|{body.leaveTime}"
    if key in seen:
        return {"ok": True, "duplicate": True}

    seen.add(key)
    # TODO: gravar no banco de vocês
    print(key, body.visitorName, body.visitedName, flush=True)
    return {"ok": True}
```

Subir:

```bash
export WEBHOOK_TOKEN=um-segredo-combinado
uvicorn app:app --host 0.0.0.0 --port 8000
```

Na tela do middleware, salve:

`https://SEU_DOMINIO/webhooks/visitor-leave`

O `seen` em memória some no restart. Em produção use banco e HTTPS.

---

## 5. Teste local no receptor

```bash
curl -sS -X POST http://127.0.0.1:8000/webhooks/visitor-leave \
  -H "Content-Type: application/json;charset=UTF-8" \
  -H "Authorization: Bearer um-segredo-combinado" \
  -d '{
    "event": "visitor_leave",
    "visitorId": "251437",
    "status": "2",
    "visitorName": "Teste",
    "visitedName": "EVB",
    "leaveTime": "1789583040",
    "arrivalTime": "1789580178",
    "trigger": "poll",
    "source": "defense_ia"
  }'
```

Primeira vez: `{"ok":true}`.  
Mesmo `visitorId` + `leaveTime`: `{"ok":true,"duplicate":true}`.

Checklist: HTTPS, token igual nos dois lados, firewall libera a VPS deste middleware, resposta `2xx` em menos de 10s, dedup `visitorId|leaveTime`, URL salva na tela.
