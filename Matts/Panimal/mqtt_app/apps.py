from django.apps import AppConfig
import threading  # Import du module threading
from .mqtt_client import start_mqtt_client

class MqttAppConfig(AppConfig):
    default_auto_field = 'django.db.models.BigAutoField'
    name = 'mqtt_app'

    def ready(self):
        # Lancer le client MQTT dans un thread séparé
        mqtt_thread = threading.Thread(target=start_mqtt_client)
        mqtt_thread.daemon = True  # Le thread s'arrêtera avec le processus principal
        mqtt_thread.start()
