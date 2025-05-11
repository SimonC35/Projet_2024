/**
 * @file powerManagement.cpp
 * @brief Gestion de l'energie.
 * @author Prenom
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

device_state_t mcuStatus = STATE_BOOT;
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
    TimerInit(&sleepWakeTimer, onSleepWake);
    TimerSetValue(&sleepWakeTimer, LPM_SLEEP_TIME);
}

void lowPowerSleep()
{
#ifdef DEBUG
    Serial.println("DEBUG: Entering low power mode...");
#endif

    if (LPM_SLEEP_TIME > 90000 && LPM_SLEEP_TIME < 180000)
    {
        #ifdef DEBUG
            Serial.println("DEBUG: Sending command to GPS module to enter sleep mode. SLEEP TIME < 3 minutes.");
        #endif
        GPS.sendcmd("$PGKC051,1*36<CR><LF>"); ///> $PGKC051,1*36<CR><LF> Enter standby low power consumption mode Arguments: Arg1: "0", stop mode “1", sleep mode
    } 
    else if (LPM_SLEEP_TIME >= 180000)
    {
        #ifdef DEBUG
            Serial.println("DEBUG: Fully stoping GPS module. SLEEP TIME >= 3 minutes.");
        #endif
        GPS.end();
    }

    #ifdef DEBUG
        Serial.printf("\nDEBUG: Starting to sleep for %d seconds...", LPM_SLEEP_TIME / 1000);
    #endif
    TimerStart(&sleepWakeTimer);
    lowPowerHandler();



    //delay(LPM_SLEEP_TIME);
}

void onSleepWake()
{
    #ifdef DEBUG
    Serial.println("DEBUG: Exiting low power mode, starting next cycle.");
    #endif
    if (LPM_SLEEP_TIME > 90000 && LPM_SLEEP_TIME < 180000)
    {
        #ifdef DEBUG
            Serial.println("DEBUG: Waking GPS module from sleep mode, sending byte to module. 1.5 minutes < SLEEP TIME < 3 minutes.");
        #endif
        GPS.sendcmd("A");
    }
    else if (LPM_SLEEP_TIME >= 180000)
    {
        #ifdef DEBUG
            Serial.println("DEBUG: Fully starting GPS module. SLEEP TIME >= 3 minutes.");
        #endif
        GPS.begin();
    }
    mcuStatus = STATE_GPS_ACQUIRE;
}

