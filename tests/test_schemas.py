import base64

import pytest
from pydantic import ValidationError

from src.api.schemas import SyncRequest

# Mínimo base64 JPEG válido: magic bytes + padding para >= 1024 bytes
_FAKE_JPEG_B64 = base64.b64encode(b"\xff\xd8\xff" + b"\x00" * 2048).decode()

VALID_PAYLOAD = {
    "source": "biodoc",
    "operation": "upsert",
    "external_id": "123",
    "person": {"full_name": "Maria Silva", "document": "12345678900"},
    "biometrics": {"face_image_base64": _FAKE_JPEG_B64},
}


def test_sync_request_accepts_valid_payload():
    payload = SyncRequest.model_validate(VALID_PAYLOAD)

    assert payload.source == "biodoc"
    assert payload.person.full_name == "Maria Silva"


def test_sync_request_rejects_invalid_operation():
    invalid_payload = VALID_PAYLOAD | {"operation": "delete"}

    with pytest.raises(ValidationError):
        SyncRequest.model_validate(invalid_payload)


def test_sync_request_rejects_missing_person():
    invalid_payload = VALID_PAYLOAD.copy()
    invalid_payload.pop("person")

    with pytest.raises(ValidationError):
        SyncRequest.model_validate(invalid_payload)


def test_sync_request_rejects_external_id_with_special_chars():
    invalid_payload = VALID_PAYLOAD | {"external_id": "biodoc-test-001"}

    with pytest.raises(ValidationError):
        SyncRequest.model_validate(invalid_payload)


def test_sync_request_accepts_payload_without_biometrics():
    payload = VALID_PAYLOAD.copy()
    payload.pop("biometrics")

    model = SyncRequest.model_validate(payload)

    assert model.biometrics is None


def test_sync_request_accepts_empty_face():
    payload = VALID_PAYLOAD | {"biometrics": {"face_image_base64": None}}

    model = SyncRequest.model_validate(payload)

    assert model.biometrics is not None
    assert model.biometrics.face_image_base64 is None
