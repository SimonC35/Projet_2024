/**
 * @file main.cpp
 * @brief Logique globale du système.
 * @author Fabien 
 * @date 04 2025
 * @version 1.0
 * 
 * Ce fichier contient les fonctions setup et loop nécessaire au système.
 * 
 */

#include "main.h"

/**
 * @brief Initialise le système à la mise sous tension ou au redémarrage.
 * 
 * Configure les interfaces de communication (série, LoRa), initialise le GPS, 
 * le timer de réveil, et réalise une première tentative d’acquisition de position. 
 * Si un fix GPS est valide, la position est stockée pour référence future.
 * 
 * @note Cette fonction est appelée une seule fois au démarrage du microcontrôleur.
 * 
 */

void setup()
{

#ifdef DEBUG
    Serial.begin(115200);
    Serial.println("DEBUG: Starting setup...");
#endif

#ifndef NOLORAWAN
    configureJoinTTN();
#endif

    GPS.begin();
    gpsAcquire();

    configureTimer();

    if (GPS.location.isValid()) {
        previousLatitude = GPS.location.lat();
        previousLongitude = GPS.location.lng();
        mcuStatus = STATE_SEND_THRESHOLD_EXCEEDED;

    } else {
        mcuStatus = STATE_GPS_ACQUIRE;
    }


}

/**
 * @brief Boucle principale du programme.
 * 
 * Loop() gère les différentes étapes de l'application en fonction de l'état actuel du microcontrôleur.
 * 
 * @note Le système reste dans le contexte de cette fonction après l'initialisation du système. 
 * 
 */
void loop()
{
    switch (mcuStatus) {
    case STATE_GPS_ACQUIRE:
        gpsAcquire();
        break;

    case STATE_SEND_THRESHOLD_EXCEEDED:
        readSendData();
        break;

    case STATE_SLEEP:
        lowPowerSleep();
        break;

    default:
        mcuStatus = STATE_SLEEP;
        break;
    }
}
