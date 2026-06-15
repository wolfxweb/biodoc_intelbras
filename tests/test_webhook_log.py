from src.core.webhook_log import (
    format_biodoc_call,
    format_fields_block,
    format_inbound_request,
    format_json_pretty,
    format_payload_summary,
    sanitize_biodoc_payload_for_log,
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
        path="/biodoc",
        client="52.5.95.105",
        query="",
        headers={"content-type": "application/json", "user-agent": "Go-http-client/2.0"},
        body_preview='{"success":true,"card":"123"}',
    )
    assert "POST /biodoc ← 52.5.95.105" in text
    assert "query: (empty)" in text
    assert "headers:" in text
    assert "body:" in text


def test_format_fields_block_aligns_keys():
    text = format_fields_block("[TEST]", {"a": 1, "long_key": "value"})
    assert "long_key" in text
    assert "value" in text


def test_sanitize_biodoc_payload_expands_detail_json():
    sanitized = sanitize_biodoc_payload_for_log(
        {"detail": '{"operador":"VIVER","nmLocal":"Hospital"}', "mainImage": "x" * 200}
    )
    assert isinstance(sanitized, dict)
    assert sanitized["detail"] == {"operador": "VIVER", "nmLocal": "Hospital"}
    assert len(str(sanitized["mainImage"])) <= 123


def test_format_biodoc_call_includes_response_body():
    from src.core.webhook_log import format_biodoc_call

    text = format_biodoc_call(
        direction="IN",
        method="GET",
        path="/integrations/log/ref-1",
        status=200,
        fields={"detail.operador": "VIVER"},
        response_body={"detail": '{"operador":"VIVER"}', "id_Card": "123"},
    )
    assert "detail.operador" in text
    assert "VIVER" in text
    assert "response:" in text
    assert "id_Card" in text
