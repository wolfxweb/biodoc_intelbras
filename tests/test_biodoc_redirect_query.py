from src.api.biodoc_redirect_query import (
    normalize_biodoc_redirect_query_string,
    parse_biodoc_redirect_params,
)


def test_normalize_replaces_extra_question_marks():
    raw = "token=abc&card=111?card=222&response=200"
    assert normalize_biodoc_redirect_query_string(raw) == "token=abc&card=111&card=222&response=200"


def test_parse_prefers_last_card_when_duplicated():
    params = parse_biodoc_redirect_params(
        "token=abc&card=111?card=00271368992672000&response=200&message=ok"
    )
    assert params.card == "00271368992672000"
    assert params.response == "200"


def test_parse_ignores_null_placeholders():
    params = parse_biodoc_redirect_params(
        "token=abc&card=123&justifyId=null&idTransaction=undefined"
    )
    assert params.justify_id is None
    assert params.id_transaction is None
