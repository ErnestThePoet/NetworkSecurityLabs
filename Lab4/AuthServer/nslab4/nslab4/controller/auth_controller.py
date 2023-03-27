import json
from ..utils.crypto_helper import *
from django.http import HttpResponse
from django.views.decorators.csrf import csrf_exempt


@csrf_exempt
def create_account(request):
    request_params = json.loads(request.body.decode("utf-8"))

    return HttpResponse(helper.encrypt_response_data({"success": True,
                                                      "data": {"data": [{"authority": "\u7483\u6708", "ID": "12644", "username": "\u9e7f\u91ce\u82d1", "chargename": "\u51dd\u5149", "chargephone": "15505589252", "expiretime": "2024-11-01"}]}}))
