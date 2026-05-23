"""Utilitário para download de imagens e conversão para base64 JPEG/PNG."""

from __future__ import annotations

import base64

import httpx

from src.core.logging import logger

_JPEG_MAGIC = b"\xff\xd8\xff"
_PNG_MAGIC = b"\x89PNG"
_MIN_IMAGE_BYTES = 1024
_DOWNLOAD_TIMEOUT = 15.0


class ImageDownloadError(Exception):
    """Falha ao baixar ou validar a imagem."""


async def download_image_as_base64(url: str) -> str:
    """
    Baixa a imagem na URL fornecida e retorna como string base64.

    Levanta ImageDownloadError se:
    - A URL não puder ser acessada / timeout
    - O conteúdo não for uma imagem JPEG ou PNG válida
    - O conteúdo for menor que o mínimo esperado
    """
    logger.debug("[IMG DOWNLOAD] url=%s", url)
    try:
        async with httpx.AsyncClient(timeout=_DOWNLOAD_TIMEOUT) as client:
            response = await client.get(url, follow_redirects=True)
    except httpx.TimeoutException as exc:
        raise ImageDownloadError(f"Timeout ao baixar imagem: {url}") from exc
    except httpx.RequestError as exc:
        raise ImageDownloadError(f"Erro de rede ao baixar imagem ({url}): {exc}") from exc

    if not response.is_success:
        raise ImageDownloadError(
            f"Falha ao baixar imagem: HTTP {response.status_code} — {url}"
        )

    raw = response.content
    if len(raw) < _MIN_IMAGE_BYTES:
        raise ImageDownloadError(
            f"Imagem muito pequena ({len(raw)} bytes); mínimo esperado: {_MIN_IMAGE_BYTES} bytes"
        )

    if not (raw.startswith(_JPEG_MAGIC) or raw.startswith(_PNG_MAGIC)):
        raise ImageDownloadError(
            "Conteúdo baixado não é uma imagem JPEG ou PNG válida"
        )

    b64 = base64.b64encode(raw).decode("ascii")
    logger.debug("[IMG DOWNLOAD] ok bytes=%d b64_len=%d", len(raw), len(b64))
    return b64
