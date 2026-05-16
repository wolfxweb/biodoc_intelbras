# Product Requirements Document: Integração BIODOC ↔ Intelbras Defense IA

## Overview

Este projeto entrega uma API middleware para integrar o sistema BIODOC ao sistema Intelbras Defense IA. Ele resolve o problema da sincronização automática de dados de pessoas e biometria facial para controle de acesso físico. A solução foi projetada para ser reutilizável, permitindo que outros sistemas externos se integrem utilizando a mesma estrutura sem a necessidade de novos desenvolvimentos estruturais.

## Goals

- Permitir a sincronização em tempo real de dados de pessoas e biometria do BIODOC para o Defense IA.
- Estabelecer uma arquitetura padronizada de ingestão de dados que permita a rápida integração de novos sistemas parceiros.
- Eliminar o trabalho manual de duplicar cadastros de pessoas e fotos em ambos os sistemas.

## User Stories

- Como administrador do sistema BIODOC, quero que um usuário cadastrado ou atualizado seja enviado automaticamente ao Defense IA, para que seu acesso físico seja liberado imediatamente.
- Como desenvolvedor de um novo sistema parceiro, quero utilizar um endpoint padrão para enviar usuários ao Defense IA, para não precisar construir a integração do zero.
- Como operador de segurança corporativa, quero logs de auditoria claros das integrações, para que eu possa identificar e solucionar falhas de sincronização.

## Core Features

- **Sincronização de Usuários e Biometria (Upsert):** Receber um payload contendo dados básicos da pessoa (Nome, Documento) e uma foto facial em base64. O comportamento cria o registro se não existir ou o atualiza se já existir, garantindo idempotência e evitando duplicações.
- **Autenticação e Controle de Origem:** Validação de acesso baseada em credenciais específicas do sistema (Tokens). O sistema mantém uma lista (vetor) de integrações autorizadas. Se o payload contiver um `source` inválido ou desconhecido, a API deve rejeitar a requisição imediatamente retornando a mensagem "source não cadastrado". Além disso, o `source` válido deve corresponder ao token autenticado da requisição.
- **Rastreamento de Logs e Falhas:** Registro sistemático de todas as requisições, erros de integração e falhas de transmissão para permitir fácil auditoria e retentativas manuais/automáticas.
- **Mecanismo de Origem Extensível:** A infraestrutura permite registrar novas origens e credenciais facilmente sem alterações no código, utilizando a mesma rota de sincronização.

## User Experience

O fluxo de integração é 100% invisível (backend-to-backend) para o usuário final, mas impacta diretamente sua experiência física:
1. O beneficiário é registrado no BIODOC (ou outro sistema de origem) e tira sua foto.
2. Em milissegundos, seus dados chegam ao middleware.
3. O middleware valida e despacha as informações para o Defense IA.
4. O beneficiário caminha até a catraca/porta, e o reconhecimento facial da Intelbras autoriza seu acesso imediatamente.

## High-Level Technical Constraints

- A sincronização deve ocorrer em tempo real via eventos push (webhooks) a partir do sistema de origem.
- O sistema de origem deve fornecer um identificador `source` compatível com sua credencial.
- A comunicação deve ser estritamente via HTTPS.
- Toda requisição deve apresentar um Bearer Token válido.
- O payload de biometria facial deve suportar o formato exigido pela API da Intelbras (Base64).
- A API deve ser entregue com uma suíte abrangente de testes automatizados (testes unitários e de integração de API) para garantir a confiabilidade.

## Non-Goals (Out of Scope)

- Sincronização reversa (do Defense IA de volta para o BIODOC).
- Processamento pesado de imagens ou compressão pelo middleware (a imagem deve chegar pronta, ou a API da Intelbras lida com isso).
- Interface visual ou dashboard para monitoramento (logs locais são suficientes para esta fase inicial).

## Phased Rollout Plan

### MVP (Phase 1)
- Endpoint principal operando e recebendo conexões síncronas do BIODOC.
- Autenticação por token e controle de origem totalmente funcionais.
- Roteamento bem-sucedido de dados e biometria para o Defense IA.
- Geração de arquivos de log locais para controle de erros.
- Suíte de testes automatizados da API.

### Phase 2
- Integração de um segundo sistema parceiro utilizando a mesma arquitetura para validar a premissa de escalabilidade.

## Success Metrics

- Tempo médio de propagação de um cadastro do BIODOC para o Defense IA.
- Taxa de sucesso (% de sincronizações bem-sucedidas vs. erros).
- Zero incidentes de segurança (acessos não autorizados à API).
- Cobertura de testes para os caminhos críticos da API.

## Risks and Mitigations

- **Indisponibilidade do Defense IA:** Pode causar falhas na liberação de acesso. *Mitigação:* O sistema de origem (BIODOC) deve ter capacidades de retentativa (retry) ao receber um erro da nossa API.
- **Sobrecarga de envios em massa inesperados:** *Mitigação:* A infraestrutura deve suportar escalabilidade horizontal, apesar de ser projetada para processamento em tempo real.

## Architecture Decision Records

- [ADR-001: Sincronização em Tempo Real (Event-Driven)](adrs/adr-001.md) — Optou-se por um modelo síncrono e em tempo real para priorizar a experiência de liberação imediata de acesso.

## Open Questions

- Quais são as dimensões de imagem específicas e os formatos suportados ou exigidos pelo Defense IA para Base64 (ex: JPEG, PNG, limites de tamanho)?
- Como os tokens de autenticação serão gerenciados e rotacionados a longo prazo?
