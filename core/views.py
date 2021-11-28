import json

from django.http import HttpResponse


def index(request):
    dummy = {
        "stocks": [
            {"stockName": "Apple", "price": 393.43},
            {"stockName": "Walmart", "price": 131.47},
            {"stockName": "Tesla", "price": 1643.00},
        ]
    }
    return HttpResponse(json.dumps(dummy))
