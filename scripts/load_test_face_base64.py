"""Carrega face JPEG de teste em Base64 (data/test_face.jpg ou .env)."""
from __future__ import annotations

import base64
import os
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
DEFAULT_JPEG = ROOT / "data" / "test_face.jpg"
DEFAULT_B64_FILE = ROOT / "data" / "test_face.base64.txt"


def load_test_face_base64() -> str:
    from_env = os.getenv("TEST_FACE_BASE64", "").strip()
    if from_env:
        return from_env

    path = os.getenv("TEST_FACE_IMAGE_PATH", str(DEFAULT_JPEG)).strip()
    image_path = Path(path)
    if not image_path.is_absolute():
        image_path = ROOT / image_path

    if image_path.is_file():
        return base64.b64encode(image_path.read_bytes()).decode("ascii")

    if DEFAULT_B64_FILE.is_file():
        return DEFAULT_B64_FILE.read_text(encoding="ascii").strip()

    raise FileNotFoundError(
        "Defina TEST_FACE_BASE64 no .env ou coloque data/test_face.jpg "
        "(baixe com: python scripts/download_test_face.py)"
    )
