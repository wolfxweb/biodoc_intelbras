"""
Baixa JPEG de rosto da internet para testes (data/test_face.jpg).

Uso: python scripts/download_test_face.py
"""
from pathlib import Path

import httpx

ROOT = Path(__file__).resolve().parent.parent
OUT = ROOT / "data" / "test_face.jpg"
URL = (
    "https://images.unsplash.com/photo-1507003211169-0a1dd7228f2d"
    "?w=200&h=200&fit=crop&crop=face"
)


def main() -> None:
    OUT.parent.mkdir(parents=True, exist_ok=True)
    response = httpx.get(URL, timeout=30.0, follow_redirects=True)
    response.raise_for_status()
    OUT.write_bytes(response.content)
    print(f"Salvo: {OUT} ({len(response.content)} bytes)")
    print("Use em testes: TEST_FACE_IMAGE_PATH=data/test_face.jpg no .env")


if __name__ == "__main__":
    main()
