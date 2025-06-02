/**
 * @file dataSender.h
 * @brief Définir les variables et fonctions à mi-chemin entre la lecture et l'envoi
 * 
 * @author Fabien
 * @date 04 2025
 * @version 1.0
 */

#ifndef DATA_SENDER_H
#define DATA_SENDER_H

#include "main.h"

/**
 * Définition du tableau d'octet pour le payload
 */
extern uint8_t fullArray[PAYLOAD_SIZE];

/**
 * @brief Réaliser la transformation des données et l'envoi des données via LoRa.
 * 
 * Dans un premier temps réalise la lecture et la transformation des valeurs via les méthodes concernés fournies par le GPS.
 * Réalise un envoie LoRa en utilisant le tableau d'octets fullArray comme buffer pour stockés les valeurs.
 */
void readSendData();

#endif