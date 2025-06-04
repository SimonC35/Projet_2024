/**
 * @file debug.cpp
 * @brief Fonction de debug
 * 
 * @author Fabien
 * @date 04 2025
 * @version 1.0
 */

#include "debug.h"
#include "utils.h"
#include "Arduino.h"

/**
 * @brief Affichage des informations de debug
 * 
 * Utilisation d'une logique de décodage basé sur les identifiants pour afficher les différentes valeurs.
 */
void printDebugInfo(uint8_t datalen, uint8_t *data)
{
    if (data == nullptr || datalen == 0) {  // Si le pointeur fournie n'est pas valide ou la taille des données est égale à 0, il n'y a pas de données à afficher
        // Fin de la fonction
        Serial.println("Error: Invalid data pointer or empty data length.");
        return;
    }

    Serial.println("\n--- DEBUG PAYLOAD ---\n");

    Serial.printf("RAW  : ");   // Affichage des données bruts, tableau complet.
    for (int i = 0; i < datalen; ++i) { // Parcours du tableau de 0 à datalen
        Serial.printf("%02X ", data[i]);    // Utilisation des indexs pour l'affichage
    }
    Serial.println();

    uint8_t *ptr = data; // Pointeur octet non signé sur le début du tableau de donnée

    while (ptr < data + datalen) { // Tant que l'adresse ptr est inférieur à l'adresse début du tableau + taille alors on continue
        if (ptr >= data + datalen) { // Si la taille à dépassé, erreur
            Serial.println("Error: Pointer out of bounds.");
            break;
        }

        // Lecture de l'identifiant du champ de données actuel et post-incrémentation du pointeur.
        // Lit l'octet pointé par ptr (donc *ptr), le stocke dans l'octet identifier, puis avance ptr d'un octet.
        uint8_t identifier = *(ptr++);
        Serial.printf("\nIdentifier: 0x%02X\n", identifier); 

        switch (identifier) {
        case 0x01: { // coordonnées

            // Vérification de la taille restante du tableau :
            // Il faut 8 octets supplémentaires (2 x 4 octets pour latitude et longitude).
            // Si on dépasse les limites du tableau, on affiche une erreur et sortie de la fonction.
            if (ptr + 2 * sizeof(uint32_t) > data + datalen) {
                Serial.println("Error: Insufficient data for GPS coordinates.");
                return;
            }

            int32_t latitude, longitude;

            memcpy(&latitude, ptr, sizeof(int32_t));
            ptr += sizeof(int32_t);

            memcpy(&longitude, ptr, sizeof(int32_t));
            ptr += sizeof(int32_t);

            Serial.printf("Latitude: %.5f\n", (float)((float)latitude / (float)digitPrecision5)); // Opération inverse pour retrouvé la valeur réel
            Serial.printf("Longitude: %.5f\n", (float)((float)longitude / (float)digitPrecision5));
            break;
        }
        case 0x02: { // Altitude
            // Vérification de la taille restante du tableau :
            // Il faut 2 octets supplémentaires (2 octets pour l'altitude).
            // Si on dépasse les limites du tableau, on affiche une erreur et sortie de la fonction.
            if (ptr + sizeof(uint16_t) > data + datalen) {
                Serial.println("Error: Insufficient data for altitude.");
                return;
            }

            uint16_t altitude;
            memcpy(&altitude, ptr, sizeof(uint16_t));
            ptr += sizeof(uint16_t);

            Serial.printf("Altitude: %d meters\n", altitude);
            break;
        }
        case 0x03: { // HDOP
            if (ptr + sizeof(uint8_t) > data + datalen) {
                Serial.println("Error: Insufficient data for HDOP.");
                return;
            }

            uint8_t hdop = *(ptr++);
            Serial.printf("HDOP: %.2f\n", (float) ((float)hdop / 10.0));
            break;
        }
        case 0x04: {
            if (ptr + sizeof(uint8_t) > data + datalen) {
                Serial.println("Error: Insufficient data for speed.");
                return;
            }

            uint8_t speed = *(ptr++);
            Serial.printf("Speed: %.2f m/s\n", (float) ((float)speed / digitPrecision1));
            break;
        }
        case 0x05: { // Course
            if (ptr + sizeof(uint8_t) > data + datalen) {
                Serial.println("Error: Insufficient data for course.");
                return;
            }

            uint8_t course = *(ptr++);
            Serial.printf("Course: %.2f degrees\n", (course * 360.0) / 255.0 ); // Rééchelonnage [0,255] vers [0, 360], perte de précision. 1 degrés -> 1.4 degrés
            break;
        }
        case 0x06: { // Satellites
            if (ptr + sizeof(uint8_t) > data + datalen) {
                Serial.println("Error: Insufficient data for satellites.");
                return;
            }

            uint8_t satellites = *(ptr++);
            Serial.printf("Satellites: %d\n", satellites);
            break;
        }
        default: {
            Serial.printf("Unknown Identifier: 0x%02X\n", identifier);
            return;
        }
        }
    }

    Serial.println("\n--------------------------");
    Serial.println("End of DEBUG PAYLOAD");
    delay(1000);
}