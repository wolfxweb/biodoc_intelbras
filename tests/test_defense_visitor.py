from src.services.defense_visitor import extract_visitor_face_pictures


def test_extract_face_from_auth_info():
    body = {"authInfo": {"facePictures": ["abc123", "def456"]}}
    assert extract_visitor_face_pictures(body) == ["abc123", "def456"]


def test_extract_face_from_nested_data():
    body = {"data": {"authInfo": {"facePictures": ["x"]}}}
    assert extract_visitor_face_pictures(body) == ["x"]


def test_extract_face_empty():
    assert extract_visitor_face_pictures({}) == []
