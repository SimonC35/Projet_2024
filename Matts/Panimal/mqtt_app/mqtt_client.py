import json
import paho.mqtt.client as mqtt
from django.utils import timezone
from django.db import connection

# Singleton
mqtt_client_instance = None

def get_mqtt_client():
    global mqtt_client_instance
    if mqtt_client_instance is None:
        mqtt_client_instance = mqtt.Client()
    return mqtt_client_instance


def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connecté au broker MQTT avec succès")
        client.subscribe(MQTT_TOPIC)
        print(f"Abonné au sujet : {MQTT_TOPIC}")
    else:
        print(f"Erreur de connexion au broker MQTT : {rc}")

def on_message(client, userdata, msg):
    try:
        payload = json.loads(msg.payload.decode('utf-8'))
        decoded_payload = payload.get("uplink_message", {}).get("decoded_payload", {})

        latitude = decoded_payload.get("latitude")
        longitude = decoded_payload.get("longitude")
        altitude = decoded_payload.get("altitude", 0)
        cardID = int(decoded_payload.get("cardID"))

        if latitude is not None and longitude is not None:
            print(f"Location - Latitude: {latitude}, Longitude: {longitude}, Altitude: {altitude}, cardID: {cardID}")

            with connection.cursor() as cursor:
                cursor.execute('SELECT 1 FROM "Objet" WHERE id_objet = %s', [cardID])
                exists = cursor.fetchone()

            if not exists:
                with connection.cursor() as cursor:
                    cursor.execute(
                        'INSERT INTO "Objet" (id_objet, nom_objet) VALUES (%s, %s)',
                        [cardID, f"Objet_{cardID}"]
                    )
                print(f"Nouvel objet ajouté à la base : {cardID}")

            timestamp = timezone.now().strftime('%Y-%m-%d %H:%M:%S')
            location_wkt = f"POINT({longitude} {latitude})"
            sql_query = f"""
                INSERT INTO "Donnee2" (date, location, hdop, speed, course, sats, id_objet_id)
                VALUES (%s, ST_GeomFromText(%s, 4326), NULL, NULL, NULL, NULL, %s)
            """
            with connection.cursor() as cursor:
                cursor.execute(sql_query, [timestamp, location_wkt, cardID])

            print("Données GPS sauvegardées dans la base de données.")
        else:
            print("Erreur : Coordonnées GPS non trouvées dans la trame JSON")

    except json.JSONDecodeError:
        print("Erreur : Le payload reçu n'est pas un JSON valide")
    except Exception as e:
        print(f"Erreur inattendue : {e}")

# Configuration MQTT
MQTT_BROKER = "eu1.cloud.thethings.network"
MQTT_PORT = 8883
MQTT_USERNAME = "projet-animal-connecte@ttn"
MQTT_PASSWORD = "NNSXS.VLVYHG6EIS3UTSMRHVOAGWP2FZ7MWHHNZMLN54I.SODX63XINITMXUYZRRV2ZYIJOYJ5VQSVJBZMBR37UM6YRC5C53SQ"
MQTT_TOPIC = "v3/projet-animal-connecte@ttn/devices/animal-device-prototype-05/up"

def start_mqtt_client():
    client = get_mqtt_client()
    client.username_pw_set(MQTT_USERNAME, MQTT_PASSWORD)
    client.tls_set()

    client.on_connect = on_connect
    client.on_message = on_message

    try:
        print("Connexion au broker MQTT (singleton)...")
        client.connect(MQTT_BROKER, MQTT_PORT, 60)
        client.loop_start()  # Utilise loop_start() ici pour éviter blocage
    except Exception as e:
        print(f"Erreur lors de la connexion au broker MQTT : {e}")



