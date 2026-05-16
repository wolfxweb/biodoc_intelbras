---
status: completed
title: Lógica de Sincronização e Tratamento de Erros
type: backend
complexity: medium
dependencies: [task_03]
---

# Lógica de Sincronização e Tratamento de Erros

## Visão Geral

Conectar a rota POST `/v1/person/sync` ao `DefenseIAClient`. Executar o encaminhamento real dos dados de usuários e biometria facial para o Intelbras Defense IA, tratando erros de API adequadamente para permitir que o BIODOC realize tentativas futuras.

<critical>
- Leia os arquivos `_prd.md` e `_techspec.md` para o contexto completo.
- Consulte as seções "Pontos de Integração" e "Endpoints da API" do TechSpec.
- O foco é amarrar os componentes construídos nas tarefas anteriores.
- Testes são obrigatórios para o fluxo de sincronização ponta a ponta e para o tratamento de erros.
</critical>

<requirements>
1. A rota POST `/v1/person/sync` DEVE instanciar e utilizar o `DefenseIAClient` para repassar o payload previamente validado para a API do Defense IA.
2. A rota DEVE chamar o equivalente às funções `add_person`/`update_person` extraídas do `defenselib.py`.
3. Se a API do Defense IA estiver inacessível ou retornar erro crítico, o endpoint DEVE devolver o status `502 Bad Gateway` ao BIODOC.
4. Se a requisição falhar devido a um token expirado (erro HTTP 401 da Intelbras), o client DEVE realizar um re-login de forma síncrona e refazer a tentativa antes de decidir se falhou.
5. Sincronizações bem-sucedidas DEVEM retornar `200 OK` com uma mensagem de sucesso.
6. Todas as tentativas (sucesso ou falhas) DEVEM gerar logs textuais usando o sistema construído na Task 01.
</requirements>

## Subtarefas

- [x] Atualizar o `DefenseIAClient` adicionando métodos para criar/atualizar usuários e suas biometrias, adaptados do `defenselib.py`.
- [x] Injetar o `DefenseIAClient` na rota POST `/v1/person/sync`.
- [x] Implementar a lógica que pega os dados da requisição (`SyncRequest`) e os envia para a Intelbras via os métodos do Client.
- [x] Implementar tratamento de exceção para interceptar quedas da API do Defense IA, gerar logs precisos de falha e retornar a resposta HTTP `502 Bad Gateway`.
- [x] Adicionar lógica de re-login caso a API recuse o token em uso ativo.

## Detalhes de Implementação

### Arquivos Relevantes

- `src/api/routes/sync.py`: Atualização do endpoint de forma a enviar realmente os dados em vez de retornar falso sucesso.
- `src/services/defense_ia_client.py`: Inclusão dos métodos reais de negócio além da parte de auth.
- `docs/defenselib.py`: Referência de payload exigido pelo Defense IA para foto e dados.

### Arquivos Dependentes

- `src/core/logging.py`: Invocação do Logger para sucesso ou falhas na transação.

### ADRs Relacionados

- [ADR-001: Sincronização em Tempo Real (Event-Driven)](adrs/adr-001.md)

## Entregáveis

- Endpoint `/v1/person/sync` 100% funcional disparando informações validadas para o Intelbras Defense IA.
- Tratamento de erro robusto informando claramente aos sistemas parceiros (como o BIODOC) caso a Intelbras caia.
- Mecanismo de recuperação e re-login sob erros de autenticação tardios.
- Cobertura densa de testes E2E/Integração.

## Testes

### Testes Unitários

- [x] Verificar se os novos métodos do `DefenseIAClient` constroem corretamente os payloads esperados pela Intelbras.
- [x] Verificar se a lógica de re-login é imediatamente engatilhada quando um mock retornar `401 Unauthorized`.

### Testes de Integração

- [x] POST `/v1/person/sync` com informações corretas chama as funções adequadas no Client mockado e devolve `200 OK`.
- [x] POST `/v1/person/sync` quando o mock da API da Intelbras dispara exceção `500` gera o retorno limpo `502 Bad Gateway` para o cliente.
- [x] Verificar a correta gravação do status da transação nos logs físicos.

## Critérios de Sucesso

- Requisições disparadas são corretamente traduzidas para a API da Intelbras.
- BIODOC sabe exatamente quando deve tentar novamente, graças à devolução confiável de erros de HTTP como `502 Bad Gateway`.
- Todos os testes passando.
- Cobertura de testes >=80%.
