import json
import paho.mqtt.client as mqtt
from django.db import connection
from django.utils import timezone

# Configuration MQTT
MQTT_BROKER = "eu1.cloud.thethings.network"
MQTT_PORT = 8883
MQTT_USERNAME = "projet-animal-connecte@ttn"
MQTT_PASSWORD = "NNSXS.EZ2QA7NXXEDUJ7FAVROO7HGHST4CRHB5V6O2ZIA.7VYYKUJIKTALUXYNKJDRI7UDQNIRQX72JEUY2Z3IDEFP7HJF6GGQ"
MQTT_TOPIC = "v3/projet-animal-connecte@ttn/devices/animal-device-1/up"

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
        
        if latitude is not None and longitude is not None:
            print(f"Location - Latitude: {latitude}, Longitude: {longitude}, Altitude: {altitude}")

            # Créer le format SQL d'insertion avec ST_GeomFromText pour les coordonnées
            timestamp = timezone.now().strftime('%Y-%m-%d %H:%M:%S')  # Format timestamp
            location_wkt = f"POINT({longitude} {latitude} {altitude})"  # Formater la géométrie
            sql_query = f"""
                INSERT INTO gps_data (date, location, hdop, speed, course, sats, age)
                VALUES ('{timestamp}', ST_GeomFromText('{location_wkt}', 4326), NULL, NULL, NULL, NULL, NULL)
                ON CONFLICT (date) DO NOTHING;
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
