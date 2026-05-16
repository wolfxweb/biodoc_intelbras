# Agent Context & Operating Procedures

Bem-vindo ao repositório do Middleware BIODOC ↔ Intelbras Defense IA. Este documento estabelece as regras de ouro para qualquer agente de IA que atue neste projeto.

## 1. Arquitetura e Stack Tecnológico
- **Linguagem Principal:** Python 3.13+
- **Framework Web:** FastAPI (com suporte nativo a `async`/`await` e gerenciador de contexto `lifespan`).
- **Banco de Dados:** SQLite via ORM SQLAlchemy.
- **Validação de Dados:** Pydantic v2.
- **Testes:** Pytest (alvo de cobertura: >= 80%).
- **Integração Externa:** Intelbras Defense IA (utilizando a base de conhecimento de `docs/defenselib.py`).

## 2. Padrões de Projeto
- **Idempotência:** A sincronização (Upsert) deve ser capaz de receber chamadas repetidas sem causar duplicação no banco de dados da Intelbras.
- **Stateful Token (Keep-Alive):** A latência é crítica. Tokens da Intelbras NÃO podem ser obtidos a cada requisição. O login ocorre no startup do FastAPI, e uma rotina em background o mantém vivo.
- **Logs:** Os logs do sistema devem ser armazenados de forma persistente e rotacionados diariamente em texto simples na pasta `/log`.
- **Validação de Origem (Source):** Toda requisição deve vir autenticada com um Token e `source` válidos presentes na tabela `IntegrationSource` do banco SQLite.

## 3. Padrões de Comunicação e Artefatos
- **Idioma Obrigatório (Tarefas e Docs):** Todas as tarefas do Compozy, bem como os arquivos de documentação (PRD, TechSpec, ADRs), devem ser escritos e atualizados estritamente em **Português (PT-BR)**.
- **Idioma do Código:** O código-fonte, nomes de variáveis, métodos, classes e mensagens de erro da API (em JSON) devem ser legíveis, mas seguem o padrão em inglês para integração da sintaxe, exceto mensagens de cliente explícitas (ex: "source não cadastrado").
- **Verificação (Final Verify):** Nunca presuma que um código está funcionando sem testá-lo de fato. Toda execução de tarefa deve terminar com a comprovação via testes automatizados (pytest).

## 4. Gestão de Tarefas (Compozy Workflow)
Este projeto utiliza o framework interno **Compozy** para o fluxo de PRDs, TechSpecs e Tasks. 
- Nunca avance de etapa sem verificar os arquivos `.compozy/tasks/*/_prd.md` e `_techspec.md`.
- As tarefas são executadas estritamente a partir dos arquivos `task_XX.md`.
- Status tracking (`_tasks.md` e arquivos da tarefa) devem ser sempre atualizados (de `pending` para `completed`) com checkboxes marcados apenas *após* evidências concretas de que a implementação e os testes passaram.
