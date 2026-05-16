"""
Testes unitários para o módulo defenselib.

Estes testes verificam as funções do SDK Intelbras Defense IA
de forma isolada, usando mocks para evitar chamadas de rede.
"""
import hashlib
import json
import pytest
from unittest.mock import Mock, patch, MagicMock, call

import sys
sys.path.insert(0, '..')

import defenselib


class TestSignatureGeneration:
    """Testes para a geração de assinatura MD5."""

    def test_get_signature(self, mock_signature):
        """Verifica se a assinatura é gerada corretamente."""
        from defenselib import getSignature

        token = "test_token"
        tempSig = mock_signature["tempSig"]

        result = getSignature(token, tempSig)

        assert isinstance(result, type(mock_signature["signature"]))
        assert result.hexdigest() == mock_signature["hexdigest"]

    def test_signature_format(self):
        """Verifica se a assinatura tem o formato MD5 (32 caracteres hex)."""
        from defenselib import getSignature

        token = "any_token"
        # Cria um tempSig mock
        tempSig = hashlib.md5(b"test")

        signature = getSignature(token, tempSig)
        hexdigest = signature.hexdigest()

        assert len(hexdigest) == 32
        assert all(c in '0123456789abcdef' for c in hexdigest)


class TestLogin:
    """Testes para a função de login."""

    def test_login_first_request(self, mock_requests_post, mock_server_ip,
                                  mock_credentials, mock_token_response):
        """Verifica se a primeira requisição de login é feita corretamente."""
        # Mock da primeira resposta (solicitação de realm/randomKey)
        mock_response_1 = Mock()
        mock_response_1.json.return_value = mock_token_response
        mock_response_1.text = json.dumps(mock_token_response)

        # Mock da segunda resposta (token final)
        mock_response_2 = Mock()
        mock_response_2.json.return_value = {"token": "final_token", "code": 0}
        mock_response_2.text = json.dumps({"token": "final_token", "code": 0})

        mock_requests_post.side_effect = [mock_response_1, mock_response_2]

        with patch('defenselib.threading.Thread'):
            with patch('defenselib.updateToken'):
                result = defenselib.Login(
                    mock_server_ip,
                    mock_credentials["username"],
                    mock_credentials["password"]
                )

        assert mock_requests_post.call_count == 2

        # Verifica primeira chamada
        first_call = mock_requests_post.call_args_list[0]
        assert 'admin/API/accounts/authorize' in first_call[0][0]

    def test_login_returns_token(self, mock_requests_post, mock_server_ip,
                                  mock_credentials):
        """Verifica se o login retorna o token corretamente."""
        mock_response_1 = Mock()
        mock_response_1.json.return_value = {
            "realm": "realm",
            "randomKey": "key123"
        }

        mock_response_2 = Mock()
        mock_response_2.json.return_value = {
            "token": "my_secret_token",
            "code": 0
        }

        mock_requests_post.side_effect = [mock_response_1, mock_response_2]

        with patch('defenselib.threading.Thread'):
            with patch('defenselib.updateToken'):
                result = defenselib.Login(
                    mock_server_ip,
                    mock_credentials["username"],
                    mock_credentials["password"]
                )

        assert result[0] == "my_secret_token"
        assert len(result) == 3  # [token, tempSig, signature]


class TestPersonManagement:
    """Testes para gerenciamento de pessoas."""

    def test_add_person_request(self, mock_requests_post, mock_server_ip,
                                  mock_token_success, sample_person_data):
        """Verifica se add_person faz requisição correta."""
        mock_response = Mock()
        mock_response.text = json.dumps({"code": 0, "msg": "success"})
        mock_requests_post.return_value = mock_response

        with patch('defenselib.requests.post', return_value=mock_response):
            result = defenselib.add_person(mock_server_ip, mock_token_success["token"])

        mock_requests_post.assert_called_once()
        call_args = mock_requests_post.call_args

        # Verifica URL
        assert 'OBMS/accessControl/person' in call_args[0][0]

        # Verifica payload
        payload = json.loads(call_args[1]['data'])
        assert payload['baseInfo']['firstName'] == 'test'

    def test_get_person_request(self, mock_requests_get, mock_server_ip,
                                 mock_token_success):
        """Verifica se get_person faz requisição correta."""
        mock_response = Mock()
        mock_response.text = json.dumps({"code": 0, "data": {}})
        mock_requests_get.return_value = mock_response

        result = defenselib.get_person(mock_server_ip, mock_token_success["token"], "2048")

        mock_requests_get.assert_called_once()
        call_args = mock_requests_get.call_args

        assert 'OBMS/accessControl/person/2048' in call_args[0][0]

    def test_update_person_request(self, mock_requests_put, mock_server_ip,
                                     mock_token_success):
        """Verifica se update_person faz requisição PUT correta."""
        mock_response = Mock()
        mock_response.text = json.dumps({"code": 0, "msg": "success"})
        mock_requests_put.return_value = mock_response

        result = defenselib.update_person(mock_server_ip, mock_token_success["token"], "2048")

        mock_requests_put.assert_called_once()
        call_args = mock_requests_put.call_args

        assert 'OBMS/accessControl/person/2048' in call_args[0][0]

    def test_delete_person_request(self, mock_requests_delete, mock_server_ip,
                                    mock_token_success):
        """Verifica se delete_person faz requisição DELETE correta."""
        mock_response = Mock()
        mock_response.text = json.dumps({"code": 0, "msg": "success"})
        mock_requests_delete.return_value = mock_response

        result = defenselib.delete_person(mock_server_ip, mock_token_success["token"], "2048")

        mock_requests_delete.assert_called_once()
        call_args = mock_requests_delete.call_args

        assert 'OBMS/accessControl/person' in call_args[0][0]
        assert 'personIds=2048' in call_args[0][0]


class TestAccessControl:
    """Testes para controle de acesso (portas)."""

    def test_open_door_request(self, mock_requests_post, mock_server_ip,
                                mock_token_success):
        """Verifica se open_door envia comando correto."""
        mock_response = Mock()
        mock_response.text = json.dumps({"code": 0, "msg": "success"})
        mock_requests_post.return_value = mock_response

        defenselib.open_door(mock_server_ip, mock_token_success["token"], "1000000$7$0$0")

        mock_requests_post.assert_called_once()
        call_args = mock_requests_post.call_args

        assert 'OBMS/accessControl/door/control' in call_args[0][0]
        payload = json.loads(call_args[1]['data'])
        assert payload['status'] == "1"
        assert payload['channelId'] == "1000000$7$0$0"

    def test_close_door_request(self, mock_requests_post, mock_server_ip,
                                 mock_token_success):
        """Verifica se close_door envia comando correto."""
        mock_response = Mock()
        mock_response.text = json.dumps({"code": 0, "msg": "success"})
        mock_requests_post.return_value = mock_response

        defenselib.close_door(mock_server_ip, mock_token_success["token"], "1000000$7$0$0")

        mock_requests_post.assert_called_once()
        call_args = mock_requests_post.call_args

        payload = json.loads(call_args[1]['data'])
        assert payload['status'] == "0"

    def test_get_door_status_request(self, mock_requests_get, mock_server_ip,
                                      mock_token_success):
        """Verifica se get_door_globalcontrol_status faz requisição correta."""
        mock_response = Mock()
        mock_response.text = json.dumps({"code": 0, "data": {}})
        mock_requests_get.return_value = mock_response

        defenselib.get_door_globalcontrol_status(mock_server_ip, mock_token_success["token"])

        mock_requests_get.assert_called_once()
        call_args = mock_requests_get.call_args

        assert 'OBMS/accessControl/door/globalControl/config' in call_args[0][0]


class TestDepartmentAndDevices:
    """Testes para listagem de departamentos e dispositivos."""

    def test_list_department_request(self, mock_requests_get, mock_server_ip,
                                      mock_token_success, mock_signature):
        """Verifica se list_department faz requisição correta."""
        mock_response = Mock()
        mock_response.text = json.dumps({"code": 0, "data": []})
        mock_requests_get.return_value = mock_response

        result = defenselib.list_department(
            mock_server_ip,
            mock_token_success["token"],
            mock_signature["tempSig"]
        )

        mock_requests_get.assert_called_once()

    def test_get_device_tree_request(self, mock_requests_post, mock_server_ip,
                                      mock_token_success, mock_signature):
        """Verifica se get_device_tree faz requisição correta."""
        mock_response = Mock()
        mock_response.text = json.dumps({"code": 0, "data": []})
        mock_requests_post.return_value = mock_response

        result = defenselib.get_device_tree(
            mock_server_ip,
            mock_token_success["token"],
            mock_signature["signature"]
        )

        mock_requests_post.assert_called_once()
        call_args = mock_requests_post.call_args

        assert 'admin/API/tree/devices' in call_args[0][0]


class TestMQAddress:
    """Testes para obter configuração MQTT."""

    def test_get_mq_address_request(self, mock_requests_post, mock_server_ip,
                                     mock_token_success, mock_signature):
        """Verifica se get_MQ_address faz requisição correta."""
        mock_response = Mock()
        mock_response.json.return_value = {
            "data": {
                "password": "mqtt_pass",
                "mqtt": "mqtt://broker:1883",
                "addr": "tcp://broker:1883",
                "userName": "mqtt_user"
            }
        }
        mock_requests_post.return_value = mock_response

        result = defenselib.get_MQ_address(
            mock_server_ip,
            mock_token_success["token"],
            mock_signature["signature"]
        )

        mock_requests_post.assert_called_once()
        call_args = mock_requests_post.call_args

        assert 'admin/API/BRM/Config/GetMqConfig' in call_args[0][0]
        assert result[1] == "mqtt_pass"
        assert result[4] == "mqtt_user"
