import json
from django.http import JsonResponse
from django.views.decorators.csrf import csrf_exempt
from ..service.auth_service import AuthService


class AuthController:
    def __init__(self):
        self.auth_service = AuthService()

    @csrf_exempt
    def create_user(self, request):
        request_obj = json.loads(request.body.decode("utf-8"))
        print("create_user", request_obj)
        return JsonResponse(self.auth_service.create_user(
            request_obj["account"], request_obj["hash1Base64"]))

    @csrf_exempt
    def auth_user(self, request):
        request_obj = json.loads(request.body.decode("utf-8"))
        print("auth_user", request_obj)
        return JsonResponse(self.auth_service.auth_user(
            request_obj["account"],
            request_obj["hash2Base64"],
            request_obj["clientAuthCodeBase64"]
        ))

    @csrf_exempt
    def change_password(self, request):
        request_obj = json.loads(request.body.decode("utf-8"))
        print("change_password", request_obj)
        return JsonResponse(self.auth_service.change_password(
            request_obj["account"],
            request_obj["hash2Base64"],
            request_obj["clientAuthCodeBase64"],
            request_obj["newHash1Base64"]
        ))
