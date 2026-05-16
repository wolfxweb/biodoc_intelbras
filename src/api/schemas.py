from pydantic import BaseModel, Field


class PersonData(BaseModel):
    full_name: str = Field(..., min_length=1)
    document: str = Field(..., min_length=1)


class BiometricData(BaseModel):
    face_image_base64: str = Field(..., min_length=1)


class SyncRequest(BaseModel):
    source: str = Field(..., min_length=1)
    operation: str = Field(..., pattern="^(upsert)$")
    external_id: str = Field(..., min_length=1)
    person: PersonData
    biometrics: BiometricData


class SyncResponse(BaseModel):
    status: str
    message: str


class IntegrationSourceCreate(BaseModel):
    name: str = Field(..., min_length=1)
    is_active: bool = True


class IntegrationSourceUpdate(BaseModel):
    name: str | None = Field(default=None, min_length=1)
    is_active: bool | None = None
    rotate_token: bool = False


class IntegrationSourceRead(BaseModel):
    id: int
    name: str
    is_active: bool

    model_config = {"from_attributes": True}


class IntegrationSourceCreated(BaseModel):
    id: int
    name: str
    is_active: bool
    integration_key: str


class IntegrationSourceUpdated(BaseModel):
    id: int
    name: str
    is_active: bool
    integration_key: str | None = None
