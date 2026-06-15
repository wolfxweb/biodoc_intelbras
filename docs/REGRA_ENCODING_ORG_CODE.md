# Regra de encoding — `org_code` na URL Verify BioDoc

Guia rápido para montar a URL de integração e escolher a **porta / regra de acesso** no Defense.

URLs prontas: [`url_exemple.txt`](../url_exemple.txt)  
Guia completo: [`VERIFY_URL_ORG_CODE.md`](./VERIFY_URL_ORG_CODE.md)

---

## Padrão oficial

```
https://web.sandbox.biodoc.com.br/#/integration/verify
  ?card=CARTAO
  &token=TOKEN
  &details={"operador":"NOME_DA_PORTA"}
  &url=https://un.wolfx.com.br/biodoc?org_code=NOME_DA_PORTA
```

Na verify, o `url=` vai **encoded** (tabela abaixo).

---

## Regra de ouro

### 1. Sempre `%3D` antes do nome da porta

`%3D` = caractere **`=`** (igual).

```
.../biodoc%3Forg_code%3DNOME
                      ↑         ↑
                     %3F       %3D  ← SEMPRE %3D (nunca %3 sozinho)
                      ?         =
```

| Errado | Certo |
|--------|-------|
| `%3Org_code%3VIVER` | `%3Forg_code%3DVIVER` |
| `%3Forg_code%3 Oncologia` | `%3Forg_code%3DOncologia` |

### 2. Espaço no nome → sempre `%20`

Cada espaço vira **`%20`**. Não use espaço literal na URL.

| Nome no Defense | Depois de `%3D` |
|-----------------|-----------------|
| `VIVER` | `VIVER` |
| `Oncologia` | `Oncologia` |
| `CHU - CENTRAL` | `CHU%20-%20CENTRAL` |
| `BLOCO A` | `BLOCO%20A` |

| Errado | Certo |
|--------|-------|
| `%3DCHU - CENTRAL` | `%3DCHU%20-%20CENTRAL` |
| `%3DCHU+-+CENTRAL` | `%3DCHU%20-%20CENTRAL` |

### 3. Acentos → UTF-8 encoded

| Caractere | Encoding |
|-----------|----------|
| `Ç` | `%C3%87` |
| `ã` | `%C3%A3` |
| `ó` | `%C3%B3` |

Exemplo: `CHU - ESPAÇO VIVER BEM` →  
`%3DCHU%20-%20ESPA%C3%87O%20VIVER%20BEM`

---

## Tabela de encoding

| Caractere | Na URL verify | Significado |
|-----------|---------------|-------------|
| `?` | `%3F` | início da query do callback |
| `=` | `%3D` | **sempre antes do nome** |
| espaço | `%20` | **sempre no lugar de espaço** |
| `"` (no details) | `%22` | aspas do JSON |

---

## Onde trocar o nome (2 lugares)

Substitua **o mesmo nome** nos dois trechos:

```
① details  →  %22VIVER%22
              %22CHU%20-%20CENTRAL%22

② url=     →  %3DVIVER
              %3DCHU%20-%20CENTRAL
```

O nome deve ser **idêntico** ao cadastrado no Defense (Autorização → Regra de acesso).

---

## Exemplos prontos

### VIVER (nome simples)

```
&details=%7B%22operador%22%3A%22VIVER%22%7D
&url=https://un.wolfx.com.br/biodoc%3Forg_code%3DVIVER
```

### Oncologia

```
&details=%7B%22operador%22%3A%22Oncologia%22%7D
&url=https://un.wolfx.com.br/biodoc%3Forg_code%3DOncologia
```

### CHU - CENTRAL (nome composto)

```
&details=%7B%22operador%22%3A%22CHU%20-%20CENTRAL%22%7D
&url=https://un.wolfx.com.br/biodoc%3Forg_code%3DCHU%20-%20CENTRAL
```

---

## Checklist antes de testar

- [ ] `%3Forg_code%3D` presente no `url=`
- [ ] `%3D` completo (com **D** no final)
- [ ] Espaços trocados por `%20`
- [ ] Mesmo nome em `details` e em `url=`
- [ ] Nome igual ao painel Defense
- [ ] Webhook: `.../biodoc` (com **c**)

---

## Erros comuns

| O que aparece | Causa | Correção |
|---------------|-------|----------|
| Regra de acesso não encontrada | Nome errado ou inexistente no Defense | Conferir painel |
| `org_code%3Oncologia=` no redirect | `%3D` incompleto | Usar `%3D` |
| `card` ausente | `?org_code=` sem encode na verify | Usar `%3Forg_code%3D...` |
| URL quebrada | Espaço literal no nome | Trocar por `%20` |

---

## Resumo em uma linha

> **`%3Forg_code%3D` + nome da porta; se tiver espaço → `%20`; acento → UTF-8 (`%C3%...`).**
