class ParamChecker:
    def _make_check_result(self, msg: str = None):
        return {
            "ok": msg is None,
            "msg": "" if msg is None else msg
        }

    def check_account(self, account: str) -> dict:
        if account is None:
            return self._make_check_result("账号为null")

        if len(account) < 3 or len(account) > 15:
            return self._make_check_result("账号长度在3至15之间")

        return self._make_check_result()

    def check_hash1_base64(self, hash1_base64: str):
        if hash1_base64 is None:
            return self._make_check_result("散列值1为null")

        if hash1_base64 == "":
            return self._make_check_result("散列值1为空")

        return self._make_check_result()

    def check_hash2_base64(self, hash2_base64: str):
        if hash2_base64 is None:
            return self._make_check_result("散列值2为null")

        if hash2_base64 == "":
            return self._make_check_result("散列值2为空")

        return self._make_check_result()
