import threading
import os
from django.apps import AppConfig
from .mqtt_client import start_mqtt_client

class MqttAppConfig(AppConfig):
    default_auto_field = 'django.db.models.BigAutoField'
    name = 'mqtt_app'

    def ready(self):
        # Ne pas exécuter le thread dans le processus du reloader
        if os.environ.get('RUN_MAIN') != 'true':
            return

        # éviter le double-thread ici
        mqtt_thread = threading.Thread(target=start_mqtt_client)
        mqtt_thread.daemon = True
        mqtt_thread.start()
