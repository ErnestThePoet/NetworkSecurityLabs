import json
from django.http import JsonResponse
from django.views.decorators.csrf import csrf_exempt
from ..service.auth_service import AuthService


class AuthController:
    def __init__(self):
        self.auth_service = AuthService()

    @csrf_exempt
    def create_user(self, request):
        return JsonResponse(self.auth_service.create_user(
                request.POST["account"], request.POST["hash1Base64"]))

    @csrf_exempt
    def auth_user(self, request):
        return JsonResponse(self.auth_service.auth_user(
            request.PUT["account"],
            request.PUT["hash2Base64"],
            request.PUT["clientAuthCodeBase64"]
        ))

    @csrf_exempt
    def change_password(self, request):
        return JsonResponse(self.auth_service.change_password(
            request.PUT["account"],
            request.PUT["hash2Base64"],
            request.PUT["clientAuthCodeBase64"],
            request.PUT["newHash1Base64"]
        ))
