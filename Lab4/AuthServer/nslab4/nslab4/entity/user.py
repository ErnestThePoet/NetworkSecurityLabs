class User:
    def __init__(self, _id: int | None, account: str, hash1_base64: str):
        self.id = _id
        self.account = account
        self.hash1_base64 = hash1_base64
