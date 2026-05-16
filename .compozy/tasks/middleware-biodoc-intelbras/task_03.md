---
status: completed
title: Integração Defense IA e Token Persistente (Keep-Alive)
type: backend
complexity: high
dependencies: [task_02]
---

# Integração Defense IA e Token Persistente (Keep-Alive)

## Visão Geral

Encapsular o script `defenselib.py` num client compatível com programação assíncrona. Implementar o processo de login na inicialização da API e a tarefa de keep-alive em background para manter um token do Intelbras Defense IA persistente em memória. Isso elimina a latência de autenticação nas requisições reais de sincronização.

<critical>
- Leia os arquivos `_prd.md` e `_techspec.md` para o contexto completo.
- Consulte a seção "Pontos de Integração" do TechSpec e a `ADR-002`.
- Foque no QUE precisa ser feito. Garanta que o `defenselib.py` seja adaptado para funcionar perfeitamente em um contexto assíncrono moderno do FastAPI.
- Testes são obrigatórios para o client e mecanismo de keep-alive.
</critical>

<requirements>
1. O arquivo existente `defenselib.py` DEVE ser adaptado ou encapsulado para funcionar sem bloquear a thread principal (event loop) do FastAPI.
2. O middleware DEVE realizar login na API do Defense IA durante o evento de inicialização (startup).
3. O token de autenticação obtido DEVE ser armazenado com segurança em memória.
4. Uma tarefa em background DEVE renovar o token periodicamente (keep-alive) antes que ele expire.
5. Se o keep-alive falhar ou o token expirar, o client DEVE suportar forçar a re-autenticação (re-login).
</requirements>

## Subtarefas

- [x] Analisar e refatorar o `defenselib.py` (especialmente as funções `Login` e `updateToken`) para uso não bloqueante ou envolvê-lo em um thread-pool executor.
- [x] Criar a classe `DefenseIAClient` para gerenciar o estado da autenticação e as chamadas de API.
- [x] Implementar a sequência de login inicial disparada durante o evento de `lifespan` do FastAPI.
- [x] Implementar uma tarefa em background (usando `asyncio` ou mecanismos do FastAPI) para rodar a lógica de renovação de token periodicamente.
- [x] Expor o token ativo pela interface do Client para ser usado pela rota de sincronização.

## Detalhes de Implementação

### Arquivos Relevantes

- `docs/defenselib.py`: A biblioteca existente que servirá de base.
- `src/services/defense_ia_client.py`: A nova classe wrapper que guarda o estado do token.
- `src/core/lifespan.py`: Configuração dos eventos de lifespan do FastAPI para ativar o login e o keep-alive no boot.

### Arquivos Dependentes

- `src/main.py`: Para integrar os eventos de lifespan.

### ADRs Relacionados

- [ADR-002: Gerenciamento de Token Persistente para API do Defense IA](adrs/adr-002.md)

## Entregáveis

- `DefenseIAClient` construído e gerenciando autenticação com estado em memória.
- Lógica de inicialização do FastAPI realizando o login com sucesso no Defense IA.
- Tarefa de background mantendo o token ativo via chamadas de atualização.
- Suíte de testes com pelo menos 80% de cobertura, utilizando abundância de mocks para a API real da Intelbras.

## Testes

### Testes Unitários

- [x] Verificar se o `DefenseIAClient` armazena e retorna o token corretamente.
- [x] Verificar se a lógica da tarefa de background tenta renovar o token nos intervalos apropriados.
- [x] Verificar se chamadas de rede externas da API dentro do client estão sofrendo mock perfeitamente.

### Testes de Integração

- [x] Verificar se o evento de inicialização do FastAPI aciona o processo de login (usando servidor de mock).
- [x] Verificar se o token é renovado sem erros durante o ciclo de vida simulado da aplicação.

## Critérios de Sucesso

- A aplicação consegue obter o token na inicialização.
- A tarefa de background roda sem travar ou interromper a aplicação.
- O token atualizado pode ser acessado de forma síncrona em memória a qualquer momento.
- Todos os testes passando.
- Cobertura de testes >=80%.
