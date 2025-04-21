#include "secret.h"
#include "config.h"
#include "LoRaWanMinimal_APP.h"
#include "gps.h"
#include "main.h"

extern uint16_t userChannelsMask[6] = {0x00FF, 0, 0, 0, 0, 0};

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