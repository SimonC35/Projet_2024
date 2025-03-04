from django.utils.dateparse import parse_datetime
from rest_framework.response import Response
from rest_framework.decorators import api_view
from .models import GPSData
from .serializers import GPSDataSerializer



@api_view(['GET'])
def get_gps_data(request):
    """
    API pour récupérer les données GPS filtrées par une plage de dates.
    """
    start_time = request.GET.get('start_time')  
    end_time = request.GET.get('end_time')      

    if start_time and end_time:
        start_time = parse_datetime(start_time)
        end_time = parse_datetime(end_time)

        if not start_time or not end_time:
            return Response({"error": "Format de date invalide"}, status=400)

        gps_data = GPSData.objects.filter(date__range=[start_time, end_time])
    else:
        gps_data = GPSData.objects.all()

    serializer = GPSDataSerializer(gps_data, many=True)
    return Response(serializer.data)

@api_view(['GET'])
def get_last_n_gps_data(request, n):
    try:
        n = int(n)  # Convertir le paramètre en entier
        gps_data = GPSData.objects.order_by('-date')[:n]  # Récupérer les N derniers
        serializer = GPSDataSerializer(gps_data, many=True)
        return Response(serializer.data)
    except ValueError:
        return Response({"error": "Le paramètre doit être un entier."}, status=400)

