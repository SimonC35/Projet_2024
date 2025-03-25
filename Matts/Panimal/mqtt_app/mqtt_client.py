import json
import paho.mqtt.client as mqtt
from django.db import connection
from django.utils import timezone


# Configuration MQTT
MQTT_BROKER = "eu1.cloud.thethings.network"
MQTT_PORT = 8883
MQTT_USERNAME = "projet-animal-connecte@ttn"
MQTT_PASSWORD = "NNSXS.VLVYHG6EIS3UTSMRHVOAGWP2FZ7MWHHNZMLN54I.SODX63XINITMXUYZRRV2ZYIJOYJ5VQSVJBZMBR37UM6YRC5C53SQ"
MQTT_TOPIC = "v3/projet-animal-connecte@ttn/devices/animal-device-prototype-05/up"

# Callback quand la connexion est établie
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connecté au broker MQTT avec succès")
        client.subscribe(MQTT_TOPIC)  # S'abonner au sujet
        print(f"Abonné au sujet : {MQTT_TOPIC}")
    else:
        print(f"Erreur de connexion au broker MQTT : {rc}")

def on_message(client, userdata, msg):
    try:
        # Décoder le payload JSON
        payload = json.loads(msg.payload.decode('utf-8'))
        
        # Extraire les coordonnées de localisation à partir de decoded_payload
        decoded_payload = payload.get("uplink_message", {}).get("decoded_payload", {})
        latitude = decoded_payload.get("latitude")
        longitude = decoded_payload.get("longitude")
        altitude = decoded_payload.get("altitude", 0)  # Si pas d'altitude, mettre 0
        cardID = int(decoded_payload.get("cardID"))  # Si pas d'altitude, mettre 0
        
        if latitude is not None and longitude is not None:
            print(f"Location - Latitude: {latitude}, Longitude: {longitude}, Altitude: {altitude}, cardID: {cardID}")

            # Créer le format SQL d'insertion avec ST_GeomFromText pour les coordonnées
            timestamp = timezone.now().strftime('%Y-%m-%d %H:%M:%S')  # Format timestamp
            location_wkt = f"POINT({longitude} {latitude})"  # Formater la géométrie {altitude}
            sql_query = f"""
                INSERT INTO "Donnee2" (date, location, hdop, speed, course, sats, id_objet_id)
                VALUES ('{timestamp}', ST_GeomFromText('{location_wkt}', 4326), NULL, NULL, NULL, NULL, {int(cardID)}) 
                ON CONFLICT (id_data) DO NOTHING;
            """
            
            # Exécuter la requête SQL dans la base de données PostgreSQL
            with connection.cursor() as cursor:
                cursor.execute(sql_query)
            print("Données GPS sauvegardées dans la base de données.")
        else:
            print("Erreur : Coordonnées GPS non trouvées dans la trame JSON")

    except json.JSONDecodeError:
        print("Erreur : Le payload reçu n'est pas un JSON valide")
    except Exception as e:
        print(f"Erreur inattendue : {e}")


# Initialisation du client MQTT
def start_mqtt_client():
    client = mqtt.Client()
    client.username_pw_set(MQTT_USERNAME, MQTT_PASSWORD)  # Authentification
    client.tls_set()  # Utilisation de TLS/SSL

    client.on_connect = on_connect
    client.on_message = on_message

    try:
        print("Connexion au broker MQTT...")
        client.connect(MQTT_BROKER, MQTT_PORT, 60)
        client.loop_forever()  # Maintenir la connexion
    except Exception as e:
        print(f"Erreur lors de la connexion au broker MQTT : {e}")
