/**
 * @file debug.h
 * @brief Définition de la fonction debug
 * 
 * @author Fabien
 * @date 04 2025
 * @version 1.0
 */
#ifndef LORAWAN_H
#define LORAWAN_H

#include "Arduino.h"

/**
 * Variable extern userChannelsMask définit ici mais initialisé dans le .cpp associé, permet de définir les canaux utilisés pour la communication LoRa.
 */
extern uint16_t userChannelsMask[6];

/**
 * @brief Initialiser la connexion LoRaWAN via OTAA pour TTN.
 * 
 * Permet de réaliser la première communication entre la carte et le réseau TheThingsNetwork,
 * permet le paramètrage de la carte et l'authentification de la carte auprès de TTN.
 */
void configureJoinTTN();

/**
 * @brief Envoyer les données via le module LoRa de la carte.
 * 
 * Sollicite le module LoRa pour envoyer les données GPS acquises.
 */
void sendData(uint8_t *fullArray, size_t length);

#endif