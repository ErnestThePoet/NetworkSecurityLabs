import random
import traceback
from ..utils.crypto_helper import *
from ..utils.encoding_helper import *
from ..entity.user import User
from ..dao.user_repository import UserRepository
from ..controller.response.success_msg_response import *
from ..controller.response.auth_response import *
from .utils.param_checker import ParamChecker


class AuthService:
    def __init__(self):
        self.user_repository = UserRepository()
        self.param_checker = ParamChecker()

    def create_user(self, account: str, hash1_base64: str):
        param_check_result = self.param_checker.check_account(account)
        if not param_check_result["ok"]:
            return make_success_msg_response(param_check_result["msg"])

        param_check_result = self.param_checker.check_hash1_base64(hash1_base64)
        if not param_check_result["ok"]:
            return make_success_msg_response(param_check_result["msg"])

        try:
            if self.user_repository.exists_by_account(account):
                return make_success_msg_response("账号已存在")
        except Exception as e:
            traceback.print_exception(e)
            return make_success_msg_response("查询数据库失败")

        try:
            self.user_repository.insert(User(None, account, hash1_base64))
        except Exception as e:
            traceback.print_exception(e)
            return make_success_msg_response("插入数据库失败")

        return make_success_msg_response()

    def auth_user(self, account: str, hash2_base64: str, client_auth_code_base64: str):
        param_check_result = self.param_checker.check_account(account)
        if not param_check_result["ok"]:
            return make_success_msg_response(param_check_result["msg"])

        param_check_result = self.param_checker.check_hash2_base64(hash2_base64)
        if not param_check_result["ok"]:
            return make_success_msg_response(param_check_result["msg"])

        user = None
        try:
            user = self.user_repository.get_by_account(account)
        except Exception as e:
            traceback.print_exception(e)
            return make_success_msg_response("查询数据库失败")

        if user is None:
            return make_success_msg_response("账号不存在")

        computed_hash2: bytes = sha256(
            base64_dec_str(user.hash1_base64) +
            base64_dec_str(client_auth_code_base64))

        if base64_dec_str(hash2_base64) != computed_hash2:
            return make_success_msg_response("认证失败")

        hash1: bytes = base64_dec_str(user.hash1_base64)
        server_auth_code: bytes = random.randbytes(32)
        server_auth_code_encrypted: bytes = aes256_encrypt(hash1, hash1[:16], server_auth_code)

        print(f"SERVER_AUTH_CODE: {base64_enc_str(server_auth_code)}")

        return make_auth_response(base64_enc_str(server_auth_code_encrypted))

    def change_password(self,
                        account: str,
                        hash2_base64: str,
                        client_auth_code_base64: str,
                        new_hash1_base64: str,
                        ):
        if not self.auth_user(account, hash2_base64, client_auth_code_base64)["success"]:
            return make_success_msg_response("认证失败")

        user = self.user_repository.get_by_account(account)
        user.hash1_base64 = new_hash1_base64

        try:
            self.user_repository.update(user)
        except Exception as e:
            traceback.print_exception(e)
            return make_success_msg_response("更新数据库失败")

        return make_success_msg_response()
