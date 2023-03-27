from .success_msg_response import make_success_msg_response


def make_auth_response(server_auth_code_encrypted_base64: str) -> dict:
    response = make_success_msg_response()
    response["serverAuthCodeEncryptedBase64"] = server_auth_code_encrypted_base64
    return response
