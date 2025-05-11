/**
 * @file loraWAN.cpp
 * @brief Fonctions en rapport avec LoRaWAN.
 * @author Prenom
 * @date 04 2025
 * @version 1.0
 * 
 * Ce fichier contient les fonctions qui permettent la communication via LoRa.
 */

#include "secret.h"
#include "config.h"
#include "LoRaWanMinimal_APP.h"
#include "gps.h"
#include "main.h"

extern uint16_t userChannelsMask[6] = {0x00FF, 0, 0, 0, 0, 0};

/**
 * @brief Initialiser la connexion LoRaWAN via OTAA pour TTN.
 * 
 * Permet de réaliser la première communication entre la carte et le réseau TheThingsNetwork,
 *  permet le paramètrage de la carte et l'authentification de la carte auprès de TTN.
 * 
 * @note Les paramètres sont réglés pour l'europe en SF9 - 125KHz. 
 */
#ifndef NOLORAWAN
void configureJoinTTN()
{

#ifdef DEBUG
    Serial.println("DEBUG: Attempting to Join TTN...");
#endif

    LoRaWAN.begin(CLASS_A, LORAMAC_REGION_EU868);
    LoRaWAN.setAdaptiveDR(false);
    LoRaWAN.setFixedDR(DR_3);

    if(!LoRaWAN.isJoined()) {
        while(!LoRaWAN.joinOTAA(appEui, appKey, devEui)) {
#ifdef DEBUG
            Serial.println("DEBUG: Failed to join TTN.");
            Serial.printf("\nDEBUG: Retrying to join in %d seconds...", TTN_JOIN_FAIL_WAIT / 1000);
#endif
            delay(TTN_JOIN_FAIL_WAIT);
        }
    } else {
        return;
    }


}
#endif

/**
 * @brief Envoyer les données via le module LoRa de la carte.
 * 
 * Sollicite le module LoRa pour envoyer les données GPS acquises.
 * La fonction reçoit un tableau de données et sa taille, puis tente de les envoyés pendant retryCount tentative(s).
 * 
 * @note La fonction est non bloquante, en cas de réussite ou d'échec le système passera au cycle suivant. 
 */
void sendData(uint8_t *fullArray, size_t length)
{

#ifdef DEBUG
    Serial.println("DEBUG: Sending GPS data...");
#endif

    // Exemple : envoyer les données via Serial ou un autre protocole
    Serial.printf("Latitude: %.6f, Longitude: %.6f\n", GPS.location.lat(), GPS.location.lng());
#ifndef NOLORAWAN
    bool confirmed = false;
    int retryCount = RETRY_COUNT;

    while (retryCount > 0) {
        if (LoRaWAN.send(PAYLOAD_SIZE, fullArray, length, confirmed)) {
#ifdef DEBUG
            Serial.println("Send OK");
#endif
            break;
        } else {
#ifdef DEBUG
            Serial.println("Send FAILED, retrying...");
#endif
            retryCount--;
        }
    }
#ifdef DEBUG
    if (retryCount == 0) {
        Serial.println("Send FAILED after retries");
    }
#endif
#endif
}