from src.core.webhook_query import parse_biodoc_redirect_query


def test_parse_biodoc_redirect_query_normal():
    parsed = parse_biodoc_redirect_query(
        "org_code=VIVER&card=00271368992672000&response=200"
    )
    assert parsed["org_code"] == "VIVER"
    assert parsed["card"] == "00271368992672000"


def test_parse_biodoc_redirect_query_fixes_biodoc_question_mark_glue():
    """BioDoc redirect: org_code=VIVER?card=123 — card ausente sem normalização."""
    parsed = parse_biodoc_redirect_query(
        "org_code=VIVER?card=00271368992672000&date=09%2F06%2F2026&response=200"
    )
    assert parsed["org_code"] == "VIVER"
    assert parsed["card"] == "00271368992672000"
    assert parsed["response"] == "200"
