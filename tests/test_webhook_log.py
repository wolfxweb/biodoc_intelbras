from src.core.webhook_log import (
    format_fields_block,
    format_inbound_request,
    format_json_pretty,
    format_payload_summary,
)


def test_format_json_pretty_indents_object():
    text = format_json_pretty('{"card":"123","success":true}')
    assert '"card": "123"' in text
    assert "\n" in text


def test_format_payload_summary_multiline():
    text = format_payload_summary(
        reference_id=None,
        id_log=None,
        log_id=None,
        card="00271368992672000",
        success=True,
        status=None,
        percentage="90%",
        operador=None,
        date="2026-05-30T14:28:37Z",
        response_code=200,
    )
    assert "[WEBHOOK] payload resumido" in text
    assert "card" in text
    assert "00271368992672000" in text
    assert "(null)" in text


def test_format_inbound_request_structure():
    text = format_inbound_request(
        method="POST",
        path="/webhook/biodoc",
        client="52.5.95.105",
        query="",
        headers={"content-type": "application/json", "user-agent": "Go-http-client/2.0"},
        body_preview='{"success":true,"card":"123"}',
    )
    assert "POST /webhook/biodoc ← 52.5.95.105" in text
    assert "query: (empty)" in text
    assert "headers:" in text
    assert "body:" in text


def test_format_fields_block_aligns_keys():
    text = format_fields_block("[TEST]", {"a": 1, "long_key": "value"})
    assert "long_key" in text
    assert "value" in text
