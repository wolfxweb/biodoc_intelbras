from src.core.security import hash_token, verify_token


def test_hash_token_verification():
    token_hash = hash_token("secret-token")

    assert token_hash != "secret-token"
    assert verify_token("secret-token", token_hash) is True
    assert verify_token("wrong-token", token_hash) is False
