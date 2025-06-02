/**
 * @file powerManagement.h
 * @brief Gestion de l'energie.
 * @author Fabien
 * @date 05 2025
 * @version 1.0
 * 
 * Ce fichier contient les fonctions permettant de gérer l'énergie et l'activation / désactivation de certains composants.
 * 
 */
#ifndef POWER_MANAGEMENT_H
#define POWER_MANAGEMENT_H

#include "Arduino.h"

/**
 * Enum regroupant les différents états dans lequel la carte peut être
 */
typedef enum {
    STATE_GPS_ACQUIRE,
    STATE_SEND_THRESHOLD_EXCEEDED,
    STATE_SLEEP
} device_state_t;

/**
 * Définition du timer
 */
extern TimerEvent_t sleepWakeTimer;
/**
 * Définition d'une variable ne variant que sur les différents états possible
 */
extern device_state_t mcuStatus;

/**
 * Configurer le timer
 */
void configureTimer();

/**
 * Passage en basse consommation
 */
void lowPowerSleep();

/**
 * Gestion du réveil de la carte
 */
void onSleepWake();


#endif