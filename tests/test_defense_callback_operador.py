from src.services.defense_callback_service import (
    operador_from_callback_query,
    reference_id_from_callback_query,
)


def test_operador_from_callback_query_reads_details_json():
    assert operador_from_callback_query(details='{"operador":"VIVER"}') == "VIVER"


def test_operador_from_callback_query_reads_detail_json():
    assert operador_from_callback_query(detail='{"operador":"Colaboradores"}') == "Colaboradores"


def test_operador_from_callback_query_reads_direct_param():
    assert operador_from_callback_query(operador="VIVER") == "VIVER"


def test_operador_from_callback_query_reads_local_aliases():
    assert operador_from_callback_query(local="VIVER") == "VIVER"
    assert operador_from_callback_query(local_de_acesso="Colaboradores") == "Colaboradores"
    assert operador_from_callback_query(acesso="VIVER") == "VIVER"


def test_operador_from_callback_query_reads_org_code_param():
    assert operador_from_callback_query(org_code="VIVER") == "VIVER"


def test_operador_from_callback_query_org_code_has_priority_over_operador():
    assert (
        operador_from_callback_query(org_code="VIVER", operador="Colaboradores")
        == "VIVER"
    )


def test_operador_from_callback_query_details_has_priority_over_log_fallback():
    assert (
        operador_from_callback_query(
            details='{"operador":"NOVO"}',
            operador="VIVER",
        )
        == "NOVO"
    )


def test_reference_id_from_callback_query_accepts_reference_id_variants():
    assert reference_id_from_callback_query(reference_id="abc-123") == "abc-123"
    assert reference_id_from_callback_query(reference_id_alt="abc-123") == "abc-123"
    assert reference_id_from_callback_query(reference_id_camel="abc-123") == "abc-123"


def test_reference_id_from_callback_query_ignores_undefined():
    assert reference_id_from_callback_query(id_transaction="undefined") is None
    assert reference_id_from_callback_query(log_id="null") is None


def test_reference_id_from_callback_query_priority_order():
    assert (
        reference_id_from_callback_query(
            reference_id="first",
            log_id="second",
        )
        == "first"
    )
