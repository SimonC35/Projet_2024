from django.utils.dateparse import parse_datetime
from rest_framework.response import Response
from rest_framework.decorators import api_view
from .models import Donnee
from .serializers import DonneeSerializer
from django.contrib.auth.models import User
from django.contrib.auth import authenticate
from rest_framework.response import Response
from rest_framework.decorators import api_view, permission_classes
from rest_framework.permissions import AllowAny, IsAuthenticated
from rest_framework_simplejwt.tokens import RefreshToken
from rest_framework import status
import psycopg2
from django.http import JsonResponse
from rest_framework.decorators import api_view, permission_classes
from rest_framework.permissions import AllowAny
from django.contrib.auth.hashers import make_password
from django.conf import settings
from django.contrib.auth.hashers import check_password
from rest_framework_simplejwt.tokens import RefreshToken
from django.db import connection
from django.contrib.auth.hashers import make_password

@api_view(['GET'])
# @permission_classes([IsAuthenticated])  # Nécessite un token JWT
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

        gps_data = Donnee.objects.filter(date__range=[start_time, end_time])
    else:
        gps_data = Donnee.objects.all()

    serializer = DonneeSerializer(gps_data, many=True)
    return Response(serializer.data)

@api_view(['GET'])
# @permission_classes([IsAuthenticated])  # Nécessite un token JWT
def get_last_n_gps_data(request, n):
    try:
        n = int(n)  # Convertir le paramètre en entier
        gps_data = Donnee.objects.order_by('-date')[:n]  # Récupérer les N derniers
        serializer = DonneeSerializer(gps_data, many=True)
        return Response(serializer.data)
    except ValueError:
        return Response({"error": "Le paramètre doit être un entier."}, status=400)


@api_view(['POST'])
def register(request):
    print(request.data)  # Debugging

    # Vérification du type de données reçues
    if isinstance(request.data, list):
        return Response({"error": "Les données envoyées sont mal formées, une liste a été reçue au lieu d'un objet."}, status=status.HTTP_400_BAD_REQUEST)

    # Récupération des champs du formulaire
    nom = request.data.get("name")
    email = request.data.get("email")
    password = request.data.get("password")

    # Vérifier que tous les champs sont remplis
    if not nom or not email or not password:
        return Response({"error": "Tous les champs sont requis"}, status=status.HTTP_400_BAD_REQUEST)

    try:
        with connection.cursor() as cursor:
            # 🔹 Vérifier si l'email existe déjà
            cursor.execute('SELECT 1 FROM "Compte" WHERE mail = %s', [email])
            if cursor.fetchone():
                return Response({"error": "Cet email est déjà utilisé."}, status=status.HTTP_400_BAD_REQUEST)

            # 🔹 Hachage du mot de passe avant insertion
            hashed_password = make_password(password)

            # 🔹 Insertion du nouvel utilisateur
            cursor.execute(
                'INSERT INTO "Compte" (nom, mail, passwd) VALUES (%s, %s, %s)',
                [nom, email, hashed_password]
            )

        return Response({"message": "Utilisateur créé avec succès"}, status=status.HTTP_201_CREATED)

    except Exception as e:
        return Response({"error": f"Erreur lors de la création de l'utilisateur : {str(e)}"}, status=status.HTTP_500_INTERNAL_SERVER_ERROR)


@api_view(['POST'])
@permission_classes([AllowAny])
def login(request):
    print(request.data)  # Debugging

    email = request.data.get("email")
    password = request.data.get("password")

    if not email or not password:
        return Response({"error": "Email et mot de passe requis"}, status=400)

    try:
        query = f'SELECT id_user, passwd FROM "Compte" WHERE mail = \'{email}\''
        
        with connection.cursor() as cursor:
            cursor.execute(query)
            user = cursor.fetchone()

        if user is None:
            return Response({"error": "Utilisateur introuvable"}, status=401)

        user_id, hashed_password = user

        # 🔹 Vérification du mot de passe haché
        if check_password(password, hashed_password):
            refresh = RefreshToken()
            refresh["user_id"] = user_id  
            return Response({
                "message": "Connexion réussie",
                "access_token": str(refresh.access_token),
                "refresh_token": str(refresh),
            }, status=200)
        else:
            return Response({"error": "Mot de passe incorrect"}, status=401)

    except Exception as e:
        return Response({"error": f"Erreur lors de la connexion : {str(e)}"}, status=500)
