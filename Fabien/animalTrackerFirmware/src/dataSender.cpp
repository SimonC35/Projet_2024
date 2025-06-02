/**
 * @file dataSender.cpp
 * @brief Fonction regroupant la logique de lecture du GPS et envoi
 * 
 * @author Fabien
 * @date 04 2025
 * @version 1.0
 */

#include "dataSender.h"

/**
 * Variable fullArray pour le stockage des valeurs fournies par le GPS
 * La taille PAYLOAD_SIZE est calculé à la compilation dans config.h selon les flags présent la taille est modifié
 */
uint8_t fullArray[PAYLOAD_SIZE] = {0};


/**
 * @brief Réaliser la transformation des données et l'envoi des données via LoRa.
 * 
 * Dans un premier temps réalise la lecture et la transformation des valeurs via les méthodes concernés fournies par le GPS.
 * Réalise un envoie LoRa en utilisant le tableau d'octets fullArray comme buffer pour stockés les valeurs.
 */
void readSendData()
{
#ifdef DEBUG
    Serial.println("DEBUG: Distance threshold exceeded, sending data...");
#endif

    readGPSStoreAsBytes(fullArray); // Lecture & transformation des valeurs
    sendData(fullArray, PAYLOAD_SIZE); // Envoi des valeurs 

    previousLatitude = GPS.location.lat(); // Stockage des coordonnées actuelle pour future comparaison
    previousLongitude = GPS.location.lng(); 

    mcuStatus = STATE_SLEEP; // Passage dans l'état STATE_SLEEP une fois l'envoi réalisé qu'il soit un échec ou une réussite
}