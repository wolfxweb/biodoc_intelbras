# URL Verify BioDoc — como trocar a porta (`org_code`)

Guia para montar a URL de integração BioDoc Verify e escolher **qual regra de acesso (porta)** no Defense IA.

**Padrão oficial** (homologação):

```
https://web.sandbox.biodoc.com.br/#/integration/verify
  ?card=CARTAO
  &token=TOKEN
  &details={"operador":"NOME_DA_PORTA"}
  &url=https://un.wolfx.com.br/biodoc?org_code=NOME_DA_PORTA
```

Na verify, o trecho `url=` precisa ir **encoded** (veja abaixo). Exemplos prontos: [`url_exemple.txt`](../url_exemple.txt).

---

## 1. O que é `%3D` e onde trocar o nome

Na URL da verify, `%3D` é o caractere **`=`** (igual) codificado.

| Texto legível (callback) | Na verify (encoded) |
|--------------------------|---------------------|
| `?org_code=VIVER` | `%3Forg_code%3DVIVER` |
| `?org_code=Oncologia` | `%3Forg_code%3DOncologia` |

Tabela de encoding usado no padrão:

| Caractere | Encoding | Onde aparece |
|-----------|----------|--------------|
| `?` | `%3F` | início da query do callback dentro de `url=` |
| `=` | `%3D` | depois de `org_code` |
| espaço | `%20` | nomes compostos (ex.: `CHU%20-%20CENTRAL`) |
| `ç`, `ã`, etc. | UTF-8 (`%C3%87`…) | acentos no nome da regra |

### Onde trocar **VIVER** por outra porta

Substitua o nome em **dois lugares** da mesma URL:

```
...&details=%7B%22operador%22%3A%22VIVER%22%7D
                              ^^^^^ ① operador no JSON

...&url=https://un.wolfx.com.br/biodoc%3Forg_code%3DVIVER
                                                              ^^^^^ ② org_code no callback
```

**Regra:** os dois valores devem ser **iguais** e **idênticos** ao nome cadastrado no Defense (Autorização → Regra de acesso).

---

## 2. Passo a passo — trocar VIVER por outra porta simples

Nome **sem espaço** (ex.: `Oncologia`, `Colaboradores`):

1. Abra a URL de referência VIVER em [`url_exemple.txt`](../url_exemple.txt).
2. Troque `VIVER` → `Oncologia` no `details` (parte `%22VIVER%22`).
3. Troque `VIVER` → `Oncologia` no `url=` (parte `%3DVIVER`).
4. Cole no navegador e teste.

**Antes (VIVER):**

```
...&details=%7B%22operador%22%3A%22VIVER%22%7D&url=...%3Forg_code%3DVIVER
```

**Depois (Oncologia):**

```
...&details=%7B%22operador%22%3A%22Oncologia%22%7D&url=...%3Forg_code%3DOncologia
```

---

## 3. Nomes compostos (com espaço, hífen ou acento)

O nome no Defense pode ter espaços e caracteres especiais. Exemplos reais:

- `CHU - CENTRAL`
- `CHU - ESPAÇO VIVER BEM`
- `BLOCO A`

**Importante:** copie o nome **exatamente** como aparece no painel Defense (maiúsculas, espaços, hífens, acentos).

### 3.1 Não coloque espaços “ crus ” na verify

| Errado | Por quê |
|--------|---------|
| `%3DCHU - CENTRAL` | espaços quebram a URL |
| `%3DCHU+-+CENTRAL` | `+` nem sempre vira espaço no redirect |

| Correto | Valor decodificado |
|---------|-------------------|
| `%3DCHU%20-%20CENTRAL` | `CHU - CENTRAL` |

### 3.2 Exemplo completo — `CHU - CENTRAL`

**Callback decodificado:**

```
https://un.wolfx.com.br/biodoc?org_code=CHU - CENTRAL
```

**URL verify (sandbox):**

```
https://web.sandbox.biodoc.com.br/#/integration/verify?card=00271368992672000&token=UG5iZndNeWlVZWxRYmMxWExXaUNFL3cya0VwWEQ1emZqMjNaS05lendpVlkzTGRqaG45RDVLLzl4RTZZTFNJeA==&details=%7B%22operador%22%3A%22CHU%20-%20CENTRAL%22%7D&url=https://un.wolfx.com.br/biodoc%3Forg_code%3DCHU%20-%20CENTRAL
```

Detalhe do encoding:

| Parte | Encoded |
|-------|---------|
| `details` → `{"operador":"CHU - CENTRAL"}` | `%7B%22operador%22%3A%22CHU%20-%20CENTRAL%22%7D` |
| `url=` → `...biodoc?org_code=CHU - CENTRAL` | `...biodoc%3Forg_code%3DCHU%20-%20CENTRAL` |

### 3.3 Exemplo com acento — `CHU - ESPAÇO VIVER BEM`

```
...&details=%7B%22operador%22%3A%22CHU%20-%20ESPA%C3%87O%20VIVER%20BEM%22%7D
...&url=https://un.wolfx.com.br/biodoc%3Forg_code%3DCHU%20-%20ESPA%C3%87O%20VIVER%20BEM
```

(`%C3%87` = `Ç`)

---

## 4. Gerar encoding automaticamente (Python)

Se o nome for longo ou tiver acentos, use este script para não errar o `%20` / `%3D`:

```python
from urllib.parse import quote
import json

NOME = "CHU - CENTRAL"  # ← troque aqui (igual ao Defense)

details = quote(
    json.dumps({"operador": NOME}, ensure_ascii=False, separators=(",", ":")),
    safe="",
)
url_suffix = f"https://un.wolfx.com.br/biodoc%3Forg_code%3D{quote(NOME, safe='')}"

print("details=", details)
print("url=", url_suffix)
```

Cole `details` e `url` na URL da verify.

---

## 5. O que acontece após a verify (redirect)

O BioDoc redireciona o navegador para o webhook. É **normal** aparecer `?` duas vezes:

```
https://un.wolfx.com.br/biodoc?org_code=VIVER?card=00271368992672000&response=200&...
```

O middleware corrige isso e lê `org_code=VIVER` normalmente.

Com nome composto, o redirect pode vir assim:

```
...?org_code=CHU - CENTRAL?card=...&response=200
```

ou com `%20` nos espaços — ambos funcionam.

---

## 6. Checklist rápido

| Item | Verificar |
|------|-----------|
| Nome igual ao Defense | Autorização → Regra de acesso |
| `details.operador` = `url org_code` | Mesmo texto nos 2 lugares |
| `%3D` antes do nome | Nunca `%3` sozinho |
| Espaços no nome | `%20` na verify |
| Acentos | UTF-8 (`%C3%...`) |
| Webhook | `https://un.wolfx.com.br/biodoc` (com **c**) |

---

## 7. Erros comuns

| Sintoma | Causa provável | Correção |
|---------|----------------|----------|
| “Regra de acesso não encontrada” | Nome não existe no Defense ou typo | Conferir painel; corrigir `Oconlogia` → `Oncologia` |
| `org_code%3Oncologia=` no redirect | `%3D` incompleto (`%3` em vez de `%3D`) | Usar `%3Forg_code%3DNOME` |
| `card` ausente no log | `?org_code=` sem encode dentro da verify | Seguir padrão `%3Forg_code%3D...` |
| Porta errada no Defense | Só trocou em um dos 2 lugares | Alinhar `details` **e** `url=` |

---

## 8. Referência visual

```
VERIFY (BioDoc)
├── card=...
├── token=...
├── details={"operador":"NOME"}     ← ① porta para exibição / fallback
└── url= biodoc?org_code=NOME  ← ② porta enviada no redirect (encoded)

        ↓ face reconhecida

REDIRECT (navegador → middleware)
/biodoc?org_code=NOME?card=...&response=200

        ↓ middleware

Defense IA
visitedName = NOME  →  canais da regra de acesso
```

Documentação BioDoc (detail / log): https://docs.biodoc.com.br/detail/
