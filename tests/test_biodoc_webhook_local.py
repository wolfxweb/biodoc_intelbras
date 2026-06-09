import pytest

from src.services.biodoc_client import ExternalAuditEntry, IntegrationLogData
from src.services.biodoc_webhook_service import (
    _pick_best_audit_entry,
    _resolve_local_name,
)


def test_pick_best_audit_entry_prefers_recent_same_day_over_stale():
    event_date = "04/06/2026 10:49:15"
    stale = ExternalAuditEntry(
        id="old-log",
        id_card="00271368992672000",
        name="Beneficiário",
        status="7",
        date="2026-06-01T19:37:03Z",
        required=None,
        required_name="Legado",
        description=None,
    )
    current = ExternalAuditEntry(
        id="new-log",
        id_card="00271368992672000",
        name="Beneficiário",
        status="7",
        date="2026-06-04T10:48:00Z",
        required=None,
        required_name="Colaboradores",
        description=None,
    )

    picked = _pick_best_audit_entry([stale, current], event_date)

    assert picked is not None
    assert picked.id == "new-log"


def test_pick_best_audit_entry_rejects_stale_log_without_event_match():
    event_date = "09/06/2026 14:53:02"
    stale = ExternalAuditEntry(
        id="old-log",
        id_card="00271368992672000",
        name="Beneficiário",
        status="7",
        date="2026-06-01T19:37:03Z",
        required=None,
        required_name=None,
        description=None,
    )

    picked = _pick_best_audit_entry([stale], event_date)

    assert picked is None


def test_resolve_local_name_details_operador_has_priority():
    audit_log = IntegrationLogData(
        id=1,
        id_card="123",
        name="Nome",
        status=2,
        main_image=None,
        path=None,
        required_name="Refeitorio",
        operador="OUTRO",
        local_name="Refeitorio",
        local_source="reguiredName",
    )

    value, source = _resolve_local_name(
        details_operador_hint="VIVER",
        audit_log=audit_log,
    )

    assert value == "VIVER"
    assert source == "url.org_code"


def test_resolve_local_name_uses_log_operador_when_no_details_hint():
    audit_log = IntegrationLogData(
        id=1,
        id_card="123",
        name="Nome",
        status=2,
        main_image=None,
        path=None,
        required_name="Refeitorio",
        operador="VIVER",
        local_name="Refeitorio",
        local_source="reguiredName",
    )

    value, source = _resolve_local_name(
        details_operador_hint=None,
        audit_log=audit_log,
    )

    assert value == "VIVER"
    assert source == "integrations/log.detail.operador"


def test_resolve_local_name_ignores_reguired_name_without_operador():
    value, source = _resolve_local_name(
        details_operador_hint=None,
        audit_log=IntegrationLogData(
            id=1,
            id_card="123",
            name="Nome",
            status=2,
            main_image=None,
            path=None,
            required_name="Refeitorio",
            operador=None,
            local_name="Refeitorio",
            local_source="reguiredName",
        ),
    )

    assert value is None
    assert source is None
