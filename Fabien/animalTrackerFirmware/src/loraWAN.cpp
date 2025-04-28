#include "secret.h"
#include "config.h"
#include "LoRaWanMinimal_APP.h"
#include "gps.h"
#include "main.h"

extern uint16_t userChannelsMask[6] = {0x00FF, 0, 0, 0, 0, 0};

#ifndef NOLORAWAN 
void configureJoinTTN()
{

    #ifdef DEBUG
        Serial.println("DEBUG: Attempting to Join TTN...");
    #endif

    LoRaWAN.begin(CLASS_A, LORAMAC_REGION_EU868);
    LoRaWAN.setAdaptiveDR(false);
    LoRaWAN.setFixedDR(DR_3);
    #ifdef DEBUG
    Serial.println("DEBUG: LoRaWAN begin, DR set");
    #endif

    if(!LoRaWAN.isJoined())
    {
    while(!LoRaWAN.joinOTAA(appEui, appKey, devEui)){
        #ifdef DEBUG
            Serial.println("DEBUG: Failed to join TTN.");
            Serial.printf("\nDEBUG: Retrying to join in %d seconds...", TTN_JOIN_FAIL_WAIT / 1000);
        #endif
        delay(TTN_JOIN_FAIL_WAIT);
    }
    }
    else { return;}


}
#endif

void sendData(uint8_t *fullArray, size_t length) {

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