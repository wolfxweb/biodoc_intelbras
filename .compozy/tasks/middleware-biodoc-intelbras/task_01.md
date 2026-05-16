---
status: completed
title: Configuração Base (FastAPI, Logs e Banco de Dados SQLite)
type: backend
complexity: medium
dependencies: []
---

# Configuração Base (FastAPI, Logs e Banco de Dados SQLite)

## Visão Geral

Inicializar a aplicação FastAPI, configurar o sistema de logs diários em texto simples e preparar o banco de dados SQLite com o modelo `IntegrationSource`. Isso estabelece a fundação do middleware.

<critical>
- Leia os arquivos `_prd.md` e `_techspec.md` para o contexto completo.
- Consulte as seções "Arquitetura do Sistema" e "Modelos de Dados" do TechSpec.
- Foco em O QUE precisa ser feito, não em COMO codificar. Mantenha os trechos de código mínimos.
- NÃO implemente a lógica de autenticação aqui, apenas o esquema do banco de dados e a configuração da aplicação.
- Testes são obrigatórios para a configuração de logs e esquema de banco de dados.
</critical>

<requirements>
1. A aplicação DEVE ser inicializada usando o framework FastAPI.
2. O sistema de logs DEVE utilizar o formato de texto simples e rotacionar arquivos diariamente, salvando-os no diretório `/log`.
3. Um banco de dados SQLite DEVE ser configurado usando um ORM (ex: SQLAlchemy) para armazenar as integrações.
4. O modelo `IntegrationSource` DEVE incluir os campos `id` (Integer, PK), `name` (String, Único), `token_hash` (String) e `is_active` (Boolean).
</requirements>

## Subtarefas

- [x] Inicializar a estrutura do aplicativo FastAPI.
- [x] Configurar o mecanismo de logs para gerar arquivos de texto simples diariamente na pasta `/log`.
- [x] Configurar a conexão com o banco de dados SQLite e o gerenciamento de sessões.
- [x] Definir o modelo ORM `IntegrationSource`.
- [x] Criar migrações iniciais do banco de dados ou scripts de criação de tabelas.

## Detalhes de Implementação

### Arquivos Relevantes

- `src/main.py`: Ponto de entrada para o aplicativo FastAPI.
- `src/core/logging.py`: Configuração para rotação diária dos logs de texto.
- `src/core/database.py`: Configuração da conexão com o SQLite.
- `src/models/integration_source.py`: Modelo ORM para a tabela de sistemas parceiros.

### Arquivos Dependentes

- `requirements.txt` ou `pyproject.toml`: Para adicionar dependências como `fastapi`, `uvicorn`, `sqlalchemy`, etc.

### ADRs Relacionados

- [ADR-003: SQLite para Armazenamento de Origens (Sources) Autorizadas](adrs/adr-003.md)
- [ADR-004: Estratégia de Arquivos de Log (Plain Text Diário)](adrs/adr-004.md)

## Entregáveis

- Aplicação FastAPI básica rodando localmente.
- Arquivos de log com rotação diária gerados na pasta `/log`.
- Arquivo do banco de dados SQLite criado contendo a tabela `IntegrationSource`.
- Suíte de testes com pelo menos 80% de cobertura para os novos componentes.

## Testes

### Testes Unitários

- [x] Verificar se o modelo `IntegrationSource` define corretamente os campos do esquema.
- [x] Verificar se a configuração de logs define corretamente a rotação diária e escreve no caminho correto.

### Testes de Integração

- [x] Verificar se a aplicação FastAPI inicia sem erros.
- [x] Verificar se um registro pode ser inserido e lido no banco de dados SQLite.

## Critérios de Sucesso

- O servidor FastAPI inicia com sucesso.
- Logs são gravados em `/log` em texto simples com rotação diária.
- A tabela `IntegrationSource` existe no banco de dados SQLite.
- Todos os testes passando.
- Cobertura de testes >=80%.
