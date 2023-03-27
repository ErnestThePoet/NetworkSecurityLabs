from Crypto.Cipher import AES
from Crypto.Util.Padding import pad, unpad
import M2Crypto as m2c


def aes_encrypt(key: bytes, iv: bytes, plaintext: bytes) -> bytes:
    return AES.new(key, AES.MODE_CBC, iv).encrypt(pad(plaintext, AES.block_size))


def aes_decrypt(key: bytes, iv: bytes, cipher: bytes) -> bytes:
    return unpad(AES.new(key, AES.MODE_CBC, iv).decrypt(cipher), AES.block_size)


def rsa_decrypt(sk: bytes, cipher: bytes):
    key = m2c.RSA.load_key_string(sk)
    return key.private_decrypt(cipher, m2c.RSA.pkcs1_padding)
