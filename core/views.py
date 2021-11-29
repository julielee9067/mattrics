import json

from django.http import HttpResponse


def index(request):
    dummy = {
        "Adam johnson": [
            {
                "seizure occurrences": 2,
                "breathing pattern": "Low",
                "GMFCS Level": "IV",
                "DOB": "2011-01-01",
            },
        ]
    }
    return HttpResponse(json.dumps(dummy))
