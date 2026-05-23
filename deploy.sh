#!/bin/bash
set -e

IMAGE="localhost:5000/biodoc-intelbras:latest"
STACK="biodoc"

echo "🔨 Construindo imagem: $IMAGE"
docker build -t "$IMAGE" .

echo "📤 Enviando imagem para o registry local..."
docker push "$IMAGE"

echo "🚀 Fazendo deploy do stack: $STACK"
docker stack deploy -c docker-compose.swarm.yml "$STACK" --with-registry-auth

echo "✅ Deploy concluído! Acesse: https://homologa.wolfx.com.br"
