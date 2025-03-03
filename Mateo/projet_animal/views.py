from django.shortcuts import render
from django.http import JsonResponse
import requests
from django.views.decorators.http import require_http_methods
from django.views.decorators.csrf import csrf_exempt
import json
from datetime import datetime, timezone
from animals.models import Position

def home(request):
    return render(request, 'home.html')

@require_http_methods(["GET"])
def get_animals(request):
    # TODO: Implement get animals logic
    return JsonResponse({"animals": []})

@require_http_methods(["GET"])
def get_animal_positions(request, animal_id):
    # TODO: Implement get animal positions logic
    return JsonResponse({"positions": []})

@csrf_exempt
@require_http_methods(["POST"])
def add_position(request):
    # TODO: Implement add position logic
    return JsonResponse({"status": "success"})

@require_http_methods(["GET"])
def get_gps_data(request):
    try:
        # Récupérer le paramètre initial
        is_initial = request.GET.get('initial', 'false') == 'true'
        
        # Récupérer les dernières positions de la base de données
        if is_initial:
            positions = Position.objects.order_by('-timestamp')[:30]
        else:
            positions = Position.objects.order_by('-timestamp')[:1]
        
        # Formater les données
        data = []
        for pos in positions:
            data.append({
                "date": pos.timestamp.astimezone(timezone.utc).isoformat(),
                "location": {
                    "type": "Point",
                    "coordinates": [
                        pos.longitude,
                        pos.latitude
                    ]
                },
                "speed": 0,  # Ces champs ne sont pas dans notre modèle actuel
                "hdop": 1,   # Valeurs par défaut
                "sats": 8,
                "course": 0
            })
        
        print(f"Données récupérées: {len(data)} points")
        return JsonResponse(data, safe=False)
        
    except Exception as e:
        print(f"Erreur inattendue: {str(e)}")
        return JsonResponse({'error': str(e)}, status=500)

@require_http_methods(["GET"])
def get_external_gps_data(request):
    # TODO: Implement external GPS data fetch logic
    return JsonResponse({"gps_data": []})
