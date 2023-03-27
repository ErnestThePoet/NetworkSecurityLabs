import base64


def base64_enc_str(message: bytes) -> str:
    return base64.b64encode(message).decode("utf-8")


def base64_dec_str(encoded: str) -> bytes:
    return base64.b64decode(encoded)
