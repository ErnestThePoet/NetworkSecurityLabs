class Account:
    def __init__(self, account: str, hash1_base64: str, _id: int = None):
        self.id = _id
        self.account = account
        self.hash1_base64 = hash1_base64
