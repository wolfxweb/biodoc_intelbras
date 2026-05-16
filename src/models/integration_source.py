from sqlalchemy import Column, Integer, String, Boolean
from src.core.database import Base

class IntegrationSource(Base):
    __tablename__ = "integration_sources"

    id = Column(Integer, primary_key=True, index=True)
    name = Column(String, unique=True, index=True, nullable=False)
    token_hash = Column(String, nullable=False)
    is_active = Column(Boolean, default=True)
