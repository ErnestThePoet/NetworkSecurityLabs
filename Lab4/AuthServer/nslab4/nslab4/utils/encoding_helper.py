import base64


def base64_enc_str(message: bytes) -> str:
    return base64.b64encode(message).decode("utf-8")
