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

/** Exemple de commit */

#include "main.h"

/**
 * @brief Initialise le système à la mise sous tension ou au redémarrage.
 * 
 * Configure les interfaces de communication (série, LoRa), initialise le GPS, 
 * le timer de réveil, et réalise une première tentative d’acquisition de position. 
 * Si un fix GPS est valide, la position est stockée pour une vérification future de la distance parcourue.
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
    configureJoinTTN(); // Configuration de la liaison TTN
#endif

    gpsAcquire(); // Première acquisition des données GPS

    configureTimer(); // Configuration du timer

    if (GPS.location.isValid()) { // Vérification de la validité de la position
        previousLatitude = GPS.location.lat(); // Sauvegarde de la position pour future comparaison
        previousLongitude = GPS.location.lng();
        mcuStatus = STATE_SEND_THRESHOLD_EXCEEDED; // Passe de la carte dans l'état STATE_SEND_THRESHOLD_EXCEEDED dans le cas ou les coordonnées sont valides

    } else {
        mcuStatus = STATE_GPS_ACQUIRE; // Dans le cas ou les coordonnées ne sont pas valides la carte est mise dans l'état STATE_GPS_ACQUIRE, l'état sommeil
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
    #ifdef DEBUG
    Serial.printf("------------------------\n\n/!\\ Start of the loop() function /!\\\n\n");
    #endif DEBUG

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

    default:    // Cas par défaut, la carte est mise en sommeil si l'état est inconnu ou corrompu
        mcuStatus = STATE_SLEEP;
        break;
    }
}
