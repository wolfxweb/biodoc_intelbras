"""Modelos SQLite das baixas de visita e configurações da tela."""

from sqlalchemy import Boolean, Integer, String, Text, UniqueConstraint
from sqlalchemy.orm import Mapped, mapped_column

from src.core.database import Base

WEBHOOK_URL_SETTING_KEY = "visit_leave_webhook_url"
WEBHOOK_TOKEN_SETTING_KEY = "visit_leave_webhook_token"


class VisitorLeaveEvent(Base):
    __tablename__ = "visitor_leave_events"
    __table_args__ = (UniqueConstraint("visitor_id", "leave_time", name="uq_visitor_leave"),)

    id: Mapped[int] = mapped_column(Integer, primary_key=True, autoincrement=True)
    visitor_id: Mapped[str] = mapped_column(String(64), nullable=False, index=True)
    visitor_name: Mapped[str | None] = mapped_column(String(255), nullable=True)
    id_num: Mapped[str | None] = mapped_column(String(64), nullable=True)
    visited_name: Mapped[str | None] = mapped_column(String(255), nullable=True, index=True)
    arrival_time: Mapped[int | None] = mapped_column(Integer, nullable=True)
    leave_time: Mapped[int] = mapped_column(Integer, nullable=False, index=True)
    logged_at: Mapped[str | None] = mapped_column(String(64), nullable=True)
    forwarded: Mapped[bool] = mapped_column(Boolean, default=False, nullable=False)
    payload_json: Mapped[str] = mapped_column(Text, nullable=False)


class AppSetting(Base):
    __tablename__ = "app_settings"

    key: Mapped[str] = mapped_column(String(64), primary_key=True)
    value: Mapped[str] = mapped_column(Text, nullable=False, default="")
