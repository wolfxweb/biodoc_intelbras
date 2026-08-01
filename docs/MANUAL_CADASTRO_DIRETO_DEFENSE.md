# Manual — Cadastro direto no Intelbras Defense

Este manual explica como cadastrar visitantes diretamente no Intelbras Defense IA usando a API do middleware BioDoc/Intelbras.

## Quando usar

Use este fluxo quando um sistema externo, operador ou script precisar cadastrar um visitante diretamente no Defense, sem passar pela tela de verify do BioDoc.

Endpoint de produção:

```text
POST https://un.wolfx.com.br/v1/person/sync
```

## Autenticação

Toda chamada precisa enviar o token administrativo do middleware:

```http
Authorization: Bearer <ADMIN_API_TOKEN>
Content-Type: application/json
```

O valor de `<ADMIN_API_TOKEN>` é fornecido pela equipe técnica responsável. Não coloque esse token em planilhas públicas, prints ou repositórios.

## Payload obrigatório

```json
{
  "source": "biodoc",
  "operation": "upsert",
  "external_id": "5326591235",
  "person": {
    "full_name": "Fernando Silva",
    "document": "12345678900"
  },
  "biometrics": {
    "face_image_base64": "/9j/4AAQSkZJRgABAQ..."
  },
  "defense": {
    "org_code": "INT5"
  }
}
```

## Campos

| Campo | Obrigatório | Descrição |
| --- | --- | --- |
| `source` | Sim | Use sempre `biodoc`. |
| `operation` | Sim | Use sempre `upsert`. Cada chamada cria uma nova visita no Defense. |
| `external_id` | Sim | Identificador de rastreio. Use cartão, matrícula ou código interno. Apenas letras e números, até 30 caracteres. |
| `person.full_name` | Sim | Nome completo do visitante. |
| `person.document` | Sim | CPF, RG ou outro documento. |
| `biometrics.face_image_base64` | Sim | Foto facial em base64, JPEG ou PNG, sem `data:image/...`. Mínimo de 1 KB após decodificar. |
| `defense.org_code` | Sim | Nível da árvore de dispositivos (ou regra de acesso) no Defense. Ex.: `INT5`, `VIVER`, `CHU - CENTRAL`. Libera **todos os dispositivos** daquele nó até as folhas do ramo. |

## Como preparar a imagem

A imagem deve ser enviada em base64 puro.

Exemplo Linux/macOS:

```bash
base64 -w 0 foto.jpg
```

Se o comando não aceitar `-w 0`, use:

```bash
base64 foto.jpg | tr -d '\n'
```

Importante:

- A foto precisa ser JPEG ou PNG.
- Não envie o prefixo `data:image/jpeg;base64,`.
- Não envie imagem muito pequena; o middleware exige pelo menos 1 KB.
- Use uma foto frontal e nítida para melhorar o reconhecimento facial nas catracas.

## Exemplo com curl

```bash
curl -X POST "https://un.wolfx.com.br/v1/person/sync" \
  -H "Authorization: Bearer <ADMIN_API_TOKEN>" \
  -H "Content-Type: application/json" \
  -d '{
    "source": "biodoc",
    "operation": "upsert",
    "external_id": "5326591235",
    "person": {
      "full_name": "Fernando Silva",
      "document": "12345678900"
    },
    "biometrics": {
      "face_image_base64": "/9j/4AAQSkZJRgABAQ..."
    },
    "defense": {
      "org_code": "INT5"
    }
  }'
```

## Exemplo usando arquivo de imagem

```bash
IMG_BASE64="$(base64 -w 0 foto.jpg)"

curl -X POST "https://un.wolfx.com.br/v1/person/sync" \
  -H "Authorization: Bearer <ADMIN_API_TOKEN>" \
  -H "Content-Type: application/json" \
  -d "{
    \"source\": \"biodoc\",
    \"operation\": \"upsert\",
    \"external_id\": \"5326591235\",
    \"person\": {
      \"full_name\": \"Fernando Silva\",
      \"document\": \"12345678900\"
    },
    \"biometrics\": {
      \"face_image_base64\": \"$IMG_BASE64\"
    },
    \"defense\": {
      \"org_code\": \"INT5\"
    }
  }"
```

## Resposta de sucesso

```json
{
  "status": "success",
  "message": "Visitante registrado no Intelbras Defense com sucesso",
  "visitor_id": "163691",
  "person_id": "178043801032500317"
}
```

Guarde o `visitor_id` para conferência no módulo de visitantes do Defense.

## Erros comuns

| HTTP | Causa provável | Como corrigir |
| --- | --- | --- |
| `401` | Token administrativo ausente ou incorreto. | Conferir header `Authorization: Bearer <ADMIN_API_TOKEN>`. |
| `422` | Payload inválido. | Conferir campos obrigatórios, `external_id` alfanumérico, imagem base64 e `defense.org_code`. |
| `422` | Foto maior que 100 KB (Defense código `8079`). | Comprimir a imagem (JPEG) para no máximo **100 KB** e reenviar. |
| `503` | Middleware sem conexão com Defense IA. | Acionar equipe técnica para verificar `/status` e login no Defense. |
| `502` | Erro retornado pelo Defense IA. | Conferir regra de acesso, dados do visitante e logs do middleware. |

## Checklist antes de enviar

- `Authorization` está preenchido.
- `source` está como `biodoc`.
- `operation` está como `upsert`.
- `external_id` tem somente letras e números.
- `full_name` e `document` estão preenchidos.
- `face_image_base64` é JPEG/PNG em base64 puro **e a imagem decodificada tem no máximo 100 KB**.
- `defense.org_code` é o nível da árvore (ex.: `INT5`) e libera todos os dispositivos desse fluxo.

## Observação importante sobre visitas

Este endpoint cria uma nova visita no Intelbras Defense a cada chamada. Ele não reaproveita uma visita anterior pelo `external_id`.
