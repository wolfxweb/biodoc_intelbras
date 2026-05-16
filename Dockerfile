# Usa a imagem oficial do Python 3.13 baseada em Debian Slim para ser mais leve
FROM python:3.13-slim

# Define o diretório de trabalho dentro do container
WORKDIR /app

# Variáveis de ambiente para o Python e FastAPI
ENV PYTHONDONTWRITEBYTECODE=1
ENV PYTHONUNBUFFERED=1
ENV PYTHONPATH=/app

# Copia o arquivo de dependências primeiro para aproveitar o cache do Docker
COPY requirements.txt /app/

# Instala as dependências
RUN pip install --no-cache-dir --upgrade -r requirements.txt

# Copia todo o código-fonte da aplicação
COPY src /app/src

# Cria o diretório de logs esperado pela aplicação (definido no core/logging.py como /log)
RUN mkdir -p /log

# Expõe a porta que o FastAPI vai rodar
EXPOSE 8000

# Comando para iniciar o servidor Uvicorn
CMD ["uvicorn", "src.main:app", "--host", "0.0.0.0", "--port", "8000"]
