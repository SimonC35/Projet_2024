/**
 * @file powerManagement.cpp
 * @brief Gestion de l'energie.
 * @author Fabien
 * @date 05 2025
 * @version 1.0
 * 
 * Ce fichier contient les fonctions permettant de gérer l'énergie et l'activation / désactivation de certains composants.
 * 
 */

#include "powerManagement.h"
#include "config.h"
#include "hw.h"
#include "low_power.h"
#include "Arduino.h"
#include "gps.h"

/**
 * Variable mcuStatus pour garder en mémoire l'état actuel de la carte
 */
device_state_t mcuStatus = STATE_SLEEP;

/**
 * Objet TimerEvent_t utilisé pour gérer le timer de mise en veille/réveil.
 */
TimerEvent_t sleepWakeTimer;


/**
 * @brief Initialiser la connexion LoRaWAN via OTAA pour TTN.
 * 
 * Permet de réaliser la première communication entre la carte et le réseau TheThingsNetwork,
 *  permet le paramètrage de la carte et l'authentification de la carte auprès de TTN.
 * 
 * @note Les paramètres sont réglés pour l'europe en SF9 - 125KHz. 
 */
void configureTimer()
{
    TimerInit(&sleepWakeTimer, onSleepWake); // Armement du timer sleepWakeTimer, avec pour fonction de callback onSleepWake() qui est appellé à la fin du timer
    TimerSetValue(&sleepWakeTimer, LPM_SLEEP_TIME); // Armement du timer sleepWakeTimer, avec comme temps la variable fournie par config.h LPM_SLEEP_TIME
}

void lowPowerSleep()
{
#ifdef DEBUG
    Serial.println("DEBUG: Entering low power mode...");
#endif

    #ifdef DEBUG
        Serial.printf("\nDEBUG: Starting to sleep for %d seconds...", LPM_SLEEP_TIME / 1000);
    #endif

    GPS.end(); // Eteindre complétement le GPS via la méthode end()

    TimerStart(&sleepWakeTimer); // Démarrage du Timer
    lowPowerHandler(); // Passage du du processeur en deepSleep
}

/**
 * @brief Réveil implicite du processeur réveiller par la fin du timer
 * 
 * Une fois le temps du timer écoulée, cette fonction qui est une fonction de callback aux yeux du timer est appellée
 * 
 * @note La fonction met la carte dans l'état STATE_GPS_ACQUIRE, car après un réveil il faut reccomencer le cycle en réalisant une acquisition de donnée
 */
void onSleepWake()
{
    #ifdef DEBUG
    Serial.println("DEBUG: Exiting low power mode, starting next cycle.");
    #endif

    mcuStatus = STATE_GPS_ACQUIRE;
}
