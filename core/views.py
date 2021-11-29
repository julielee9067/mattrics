import json

from django.http import HttpResponse


def index(request):
    dummy = {
        "Adam johnson": [
            {
                "seizure occurrences": 2,
                "breathing pattern": "Low",
                "GMFCS Level": "IV",
                "price": 1643.00,
            },
        ]
    }
    return HttpResponse(json.dumps(dummy))
