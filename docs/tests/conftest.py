"""
Fixtures e configurações para os testes.
"""
import hashlib
import pytest
from unittest.mock import Mock, patch, MagicMock


@pytest.fixture
def mock_server_ip():
    """IP do servidor de teste."""
    return "127.0.0.1"


@pytest.fixture
def mock_credentials():
    """Credenciais de teste."""
    return {
        "username": "system",
        "password": "admin123"
    }


@pytest.fixture
def mock_token_response():
    """Resposta mock do login inicial (primeira etapa)."""
    return {
        "realm": "test_realm",
        "randomKey": "9c2b603650f54bcb"
    }


@pytest.fixture
def mock_token_success():
    """Resposta mock do login bem-sucedido (segunda etapa)."""
    return {
        "token": "test_token_12345",
        "code": 0,
        "msg": "success"
    }


@pytest.fixture
def mock_signature():
    """Assinatura MD5 calculada para testes."""
    # Baseado no algoritmo de autenticação da Intelbras Defense IA
    password = "admin123"
    username = "system"
    realm = "test_realm"
    randomKey = "9c2b603650f54bcb"

    # Etapas da assinatura (mesmo código do defenselib.py)
    tempSignature = hashlib.md5(password.encode('utf-8'))
    tempStr = username + tempSignature.hexdigest()
    tempSig = hashlib.md5(tempStr.encode('utf-8'))
    tempSig = hashlib.md5(tempSig.hexdigest().encode('utf-8'))
    tempStr = username + ":" + realm + ":" + tempSig.hexdigest()
    tempSig = hashlib.md5(tempStr.encode('utf-8'))
    tempStr = tempSig.hexdigest() + ":" + randomKey
    signature = hashlib.md5(tempStr.encode('utf-8'))

    return {
        "tempSig": tempSig,
        "signature": signature,
        "hexdigest": signature.hexdigest()
    }


@pytest.fixture
def sample_person_data():
    """Dados de pessoa para testes."""
    return {
        "details": {
            "companyName": "Empresa Teste",
            "expireTime": "1877615999",
            "position": "Analista",
            "nickName": "usuario_teste",
            "useTimes": "100",
            "remark": "Teste automatizado",
            "tel": "11999999999",
            "email": "teste@exemplo.com",
            "idType": "0",
            "maritalStatus": "2",
            "birthday": "1990-01-15",
            "idNum": "12345678901",
            "address": "Cidade Teste",
            "degree": "3",
            "nationalityId": "brazil",
            "initialTime": "1609459200"
        },
        "baseInfo": {
            "authority": "0",
            "householder": "0",
            "unitId": "1",
            "id": "",
            "pictureData": "/9j/4AAQSkZJRgABAQEASABIAAD/2wBDAAgGBgcGBQgHBwcJCQgKDBQNDAsLDBkSEw8UHRofHh0aHBwgJC4nICIsIxwcKDcpLDAxNDQ0Hyc5PTgyPC4zNDL/2wBDAQkJCQwLDBgNDRgyIRwhMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjL/wAARCAABAAEDASIAAhEBAxEB/8QAFQABAQAAAAAAAAAAAAAAAAAAAAn/xAAUEAEAAAAAAAAAAAAAAAAAAAAA/8QAFQEBAQAAAAAAAAAAAAAAAAAAAAX/xAAUEQEAAAAAAAAAAAAAAAAAAAAA/9oADAMBAAIRAxEAPwCwAB//2Q==",
            "lastName": "Silva",
            "personId": "2048",
            "firstName": "Usuario",
            "departmentId": "001001",
            "cardType": "0",
            "gender": "1",
            "status": "0",
            "stageId": "",
            "roomId": "36",
            "buildingId": "12"
        },
        "accessRight": {
            "entranceDeviceCode": [],
            "channelId": ["1000006$7$0$0"],
            "doorGroupId": ["2"]
        },
        "authentication": {
            "password": "9bb829...",
            "cars": [],
            "infraredFaceCodes": [],
            "carportAmount": "1",
            "fingerPrints": [],
            "cards": [
                {
                    "cardState": "0",
                    "masterFlag": "1",
                    "duressFlag": "0",
                    "cardNo": "00666998",
                    "changeDate": "1609459200",
                    "issueDate": "1609459200"
                }
            ],
            "facePictures": []
        }
    }


@pytest.fixture
def mock_requests_post():
    """Mock para requests.post."""
    with patch('defenselib.requests.post') as mock:
        yield mock


@pytest.fixture
def mock_requests_get():
    """Mock para requests.get."""
    with patch('defenselib.requests.get') as mock:
        yield mock


@pytest.fixture
def mock_requests_put():
    """Mock para requests.put."""
    with patch('defenselib.requests.put') as mock:
        yield mock


@pytest.fixture
def mock_requests_delete():
    """Mock para requests.delete."""
    with patch('defenselib.requests.delete') as mock:
        yield mock
