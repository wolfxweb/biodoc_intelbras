---
status: completed
title: Autenticação e Endpoint de Sincronização (Auth Manager)
type: backend
complexity: medium
dependencies: [task_01]
---

# Autenticação e Endpoint de Sincronização (Auth Manager)

## Visão Geral

Criar o endpoint POST `/v1/person/sync` com validação Pydantic e implementar a injeção de dependência para validar o `source` e o `Bearer token` contra o banco de dados SQLite. Isso garante que apenas requisições autorizadas sejam processadas.

<critical>
- Leia os arquivos `_prd.md` e `_techspec.md` para o contexto completo.
- Consulte as seções "Interfaces Principais" e "Endpoints da API" do TechSpec.
- Foco em O QUE precisa ser feito, não em COMO codificar.
- Testes são obrigatórios para os esquemas Pydantic e lógica de autorização.
</critical>

<requirements>
1. Um endpoint POST em `/v1/person/sync` DEVE ser criado.
2. O endpoint DEVE aceitar um payload JSON correspondente ao esquema `SyncRequest` definido no TechSpec.
3. O endpoint DEVE exigir um header `Authorization: Bearer <token>`.
4. A aplicação DEVE validar o campo `source` e o token fornecido consultando a tabela `IntegrationSource` no banco de dados.
5. Tokens inválidos ou origens não registradas DEVEM retornar `401 Unauthorized` ou `400 Bad Request` com a mensagem "source não cadastrado", respectivamente.
6. Uma requisição válida DEVE retornar temporariamente um `200 OK` (a lógica de integração real será adicionada em uma tarefa futura).
</requirements>

## Subtarefas

- [x] Definir esquemas Pydantic (`PersonData`, `BiometricData`, `SyncRequest`).
- [x] Implementar uma dependência no FastAPI para validação do token e source no banco de dados.
- [x] Criar o endpoint POST `/v1/person/sync` utilizando os esquemas de validação e as dependências de autenticação.
- [x] Implementar o tratamento de erros para requisições não autorizadas e origens (sources) não registradas.

## Detalhes de Implementação

### Arquivos Relevantes

- `src/api/schemas.py`: Modelos Pydantic para o payload de sincronização.
- `src/core/security.py`: Lógica para validar os Bearer tokens (hashing/verificação).
- `src/api/dependencies.py`: Injeção de dependência do FastAPI para validação de auth e source.
- `src/api/routes/sync.py`: A definição final da rota no FastAPI.

### Arquivos Dependentes

- `src/main.py`: Para registrar o novo roteador de rotas.

### ADRs Relacionados

- [ADR-003: SQLite para Armazenamento de Origens (Sources) Autorizadas](adrs/adr-003.md)

## Entregáveis

- Esquemas Pydantic completos para o payload de sincronização.
- Dependência de autenticação que valida requisições contra o SQLite.
- O endpoint POST `/v1/person/sync` retornando sucesso ou erros apropriados.
- Suíte de testes com pelo menos 80% de cobertura para esquemas e lógica de autenticação.

## Testes

### Testes Unitários

- [x] Verificar se os modelos Pydantic validam payloads válidos corretamente.
- [x] Verificar se os modelos Pydantic rejeitam payloads inválidos (ex: campos ausentes, tipo de operação errada).
- [x] Verificar se a função de validação de auth checa corretamente o hash do token com o banco de dados.

### Testes de Integração

- [x] POST `/v1/person/sync` com um token válido e source registrado retorna `200 OK`.
- [x] POST `/v1/person/sync` com um token inválido retorna `401 Unauthorized`.
- [x] POST `/v1/person/sync` com um source não registrado retorna `400 Bad Request`.

## Critérios de Sucesso

- O endpoint valida os payloads perfeitamente de acordo com o esquema.
- Requisições com credenciais ou origens inválidas são rejeitadas com os códigos HTTP e mensagens corretas.
- Todos os testes passando.
- Cobertura de testes >=80%.
