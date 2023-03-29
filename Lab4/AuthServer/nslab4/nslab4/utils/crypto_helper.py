import hashlib
from Cryptodome.Cipher import AES
from Cryptodome.Util.Padding import pad, unpad


def aes256_encrypt(key: bytes, iv: bytes, plaintext: bytes) -> bytes:
    return AES.new(key, AES.MODE_CBC, iv).encrypt(pad(plaintext, AES.block_size))


def aes256_decrypt(key: bytes, iv: bytes, cipher: bytes) -> bytes:
    return unpad(AES.new(key, AES.MODE_CBC, iv).decrypt(cipher), AES.block_size)


def sha256(message: bytes) -> bytes:
    return hashlib.sha256(message).digest()
