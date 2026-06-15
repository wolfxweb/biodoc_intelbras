#!/bin/bash
set -e

IMAGE="localhost:5000/biodoc-intelbras:latest"

echo "🔨 Construindo imagem: $IMAGE"
docker build --no-cache -t "$IMAGE" .

echo "📤 Enviando imagem para o registry local..."
docker push "$IMAGE"

echo "🚀 Fazendo deploy do stack: homologa (un.wolfx.com.br)"
docker stack deploy -c docker-compose.swarm.yml homologa --with-registry-auth

echo "♻️ Forçando recriação do serviço..."
docker service update --force --image "$IMAGE" homologa_biodoc_api

echo "✅ Deploy concluído! Acesse: https://un.wolfx.com.br"
