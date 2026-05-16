import pytest
from pydantic import ValidationError

from src.api.schemas import SyncRequest


VALID_PAYLOAD = {
    "source": "biodoc",
    "operation": "upsert",
    "external_id": "123",
    "person": {"full_name": "Maria Silva", "document": "12345678900"},
    "biometrics": {"face_image_base64": "base64-image"},
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
