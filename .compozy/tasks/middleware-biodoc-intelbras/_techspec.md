# Especificação Técnica: Integração BIODOC ↔ Intelbras Defense IA

## Resumo Executivo

Esta especificação técnica detalha a implementação de uma API middleware construída com Python e FastAPI para integrar o BIODOC com o Intelbras Defense IA. A arquitetura foi projetada como um receptor síncrono de webhooks orientado a eventos, que valida os payloads recebidos contra um banco de dados SQLite embarcado e encaminha os dados para a API do Defense IA utilizando um token persistente em memória. O principal trade-off técnico é a natureza stateful (com estado) do middleware (mantendo o token do Defense IA em memória com uma rotina de keep-alive em background) para atingir a exigência de latência quase zero para o controle de acesso físico.

## Arquitetura do Sistema

### Visão Geral dos Componentes

- **FastAPI Web Server**: O servidor HTTP principal que expõe os endpoints de webhook e lida com a validação das requisições via Pydantic.
- **Auth Manager**: Valida a origem (`source`) e o Bearer token da requisição utilizando o banco de dados SQLite local.
- **Defense IA Client**: Um wrapper (encapsulador) em torno do script existente `defenselib.py` que mantém o token de sessão ativo e gerencia as chamadas `add_person`/`update_person`.
- **Banco de Dados SQLite**: Armazena os sistemas parceiros autorizados (sources) e o hash de suas credenciais.
- **Logger**: Um componente de log baseado em arquivo que gera logs em texto simples com rotação diária.

## Design de Implementação

### Interfaces Principais

```python
from pydantic import BaseModel, Field

class PersonData(BaseModel):
    full_name: str
    document: str

class BiometricData(BaseModel):
    face_image_base64: str

class SyncRequest(BaseModel):
    source: str
    operation: str = Field(..., pattern="^(upsert)$")
    external_id: str
    person: PersonData
    biometrics: BiometricData
```

### Modelos de Dados

- **IntegrationSource (SQLite)**
  - `id`: Integer (PK)
  - `name`: String (Unique) - ex: 'biodoc'
  - `token_hash`: String - Bearer token salvo com hash seguro
  - `is_active`: Boolean

### Endpoints da API

- **Método**: `POST`
- **Caminho**: `/v1/person/sync`
- **Descrição**: Recebe dados de usuários e biometria facial para sincronização com o Defense IA.
- **Formato da Requisição**: JSON (esquema `SyncRequest`). Requer `Authorization: Bearer <token>`.
- **Formato da Resposta**: 
  - `200 OK`: `{"status": "success", "message": "Dados sincronizados com sucesso"}`
  - `401 Unauthorized`: `{"detail": "Token ou source inválido"}`
  - `400 Bad Request`: `{"detail": "source não cadastrado"}`
  - `502 Bad Gateway`: `{"detail": "API do Defense IA indisponível"}`

## Pontos de Integração

- **Intelbras Defense IA**
  - **Propósito**: Destino final dos dados de controle de acesso físico.
  - **Autenticação**: Token com estado obtido via processo de hashing MD5 em múltiplas etapas (`defenselib.py`).
  - **Tratamento de Erros**: O middleware irá capturar erros de conexão e retornar um código 502 para o BIODOC, permitindo que o BIODOC tente novamente mais tarde.

## Análise de Impacto

| Componente | Tipo de Impacto | Descrição e Risco | Ação Necessária |
|-----------|-------------|---------------------|-----------------|
| `defenselib.py` | modificado | O script existente precisa de pequenas refatorações para suportar chamadas dentro do contexto assíncrono do FastAPI sem bloquear a thread principal. | Atualizar o script para disparar exceções de erro em vez de usar apenas comandos `print`. |

## Estratégia de Testes

### Testes Unitários

- Estratégia: Testar a lógica do Auth Manager, garantindo que as queries no SQLite validam corretamente os tokens e rejeitam origens (sources) inválidas.
- Mocking: Fazer mock das chamadas ao `defenselib.py` para validar o roteamento do FastAPI e as validações do Pydantic sem precisar atingir a API real da Intelbras.

### Testes de Integração

- Iniciar o servidor FastAPI com um banco de dados SQLite de testes contendo uma origem (source) falsa.
- Enviar uma requisição POST válida com uma imagem em mock e verificar a resposta `200 OK`.
- Enviar uma requisição POST inválida (token errado ou source não registrado) e verificar a resposta `401/400` ("source não cadastrado").

## Sequência de Desenvolvimento

### Ordem de Construção

1. **Infraestrutura e Autenticação**: Configurar FastAPI, logging (arquivo de texto diário) e o esquema do banco de dados SQLite para `IntegrationSource`.
2. **Camada de API**: Implementar a rota POST `/v1/person/sync` com esquemas Pydantic e injeção de dependência para validação de token/source (Depende do Passo 1).
3. **Defense IA Client**: Integrar o `defenselib.py`, implementando o login na inicialização e a tarefa de keep-alive em background (Depende do Passo 2).
4. **Lógica de Integração**: Conectar a rota POST ao Defense IA Client para realizar o envio real dos dados (Depende do Passo 3).

### Dependências Técnicas

- Um ambiente de testes do Intelbras Defense IA válido e acessível para validar as operações de `add_person` e o fluxo de autenticação.

## Monitoramento e Observabilidade

- **Métricas**: Acompanhar a taxa de sucesso/falha do endpoint `/v1/person/sync`.
- **Logs**: Logs em texto simples gerados diariamente na pasta `/log`, contendo IDs de requisição, sources de origem e mensagens explícitas de erro retornadas pela Intelbras.

## Considerações Técnicas

### Principais Decisões

- Decisão: Token em memória para a API da Intelbras.
- Justificativa: Necessário para obter latência quase zero no acesso físico.
- Trade-offs: Torna o middleware uma aplicação stateful (que guarda estado).

### Riscos Conhecidos

- Risco: A tarefa de keep-alive em background pode falhar silenciosamente, deixando o token da Intelbras expirar.
- Mitigação: O client deve capturar erros `401 Unauthorized` da Intelbras, realizar um re-login síncrono e tentar novamente a requisição antes de falhar o webhook para o BIODOC.

## Registros de Decisões de Arquitetura (ADRs)

- [ADR-001: Sincronização em Tempo Real (Event-Driven)](adrs/adr-001.md)
- [ADR-002: Gerenciamento de Token Persistente para API do Defense IA](adrs/adr-002.md)
- [ADR-003: SQLite para Armazenamento de Origens (Sources) Autorizadas](adrs/adr-003.md)
- [ADR-004: Estratégia de Arquivos de Log (Plain Text Diário)](adrs/adr-004.md)
