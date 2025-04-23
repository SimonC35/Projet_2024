#include "main.h"

Air530ZClass GPS;
uint8_t fullArray[PAYLOAD_SIZE] = {0};

double previousLatitude = 0.0;
double previousLongitude = 0.0;

void readSendData() {
    #ifdef DEBUG
        Serial.println("DEBUG: Distance threshold exceeded, sending data...");
    #endif
    if (!GPS.location.isValid()) {
        Serial.println("DEBUG : Cannot send data, GPS location invalid.");
        mcu_status = STATE_SLEEP;
        return;
    }

    readGPSStoreAsBytes(fullArray);
    sendData(fullArray, PAYLOAD_SIZE);

    previousLatitude = GPS.location.lat();
    previousLongitude = GPS.location.lng();

    mcu_status = STATE_SLEEP;
}

void onSleepWake() {
    mcu_status = STATE_GPS_ACQUIRE;
}

void gpsAcquire() {
    #ifdef DEBUG
        Serial.println("DEBUG: Acquiring GPS...");
    #endif

        uint32_t start = millis();
        while ((millis() - start) < GPS_UPDATE_TIMEOUT) {
            while (GPS.available()) GPS.encode(GPS.read());
            if (GPS.location.age() < 1000) break;
        }

        if (!GPS.location.isValid()) {
    #ifdef DEBUG
            Serial.println("DEBUG: Invalid GPS. Skipping send.");
    #endif
            mcu_status = STATE_SLEEP;
            return;
        }

        double distance = GPS.distanceBetween(previousLatitude, previousLongitude, GPS.location.lat(), GPS.location.lng());

    #ifdef DEBUG
        Serial.printf("Previous: %.6f / %.6f\n", previousLatitude, previousLongitude);
        Serial.printf("Current : %.6f / %.6f\n", GPS.location.lat(), GPS.location.lng());
        Serial.printf("Distance: %.2f meters\n", distance);
    #endif

        if (DISTANCE_THRESHOLD == 0.0 || distance > DISTANCE_THRESHOLD) {
            mcu_status = STATE_SEND_THRESHOLD_EXCEEDED;
        } else {
    #ifdef DEBUG
            Serial.println("DEBUG: No significant movement.");
    #endif
            mcu_status = STATE_SLEEP;
        }
}

void lowPowerSleep()
{
    #ifdef DEBUG
        Serial.println("DEBUG: Entering low power mode...");
    #endif
    TimerStart(&sleepWakeTimer);
    lowPowerHandler();

    #ifdef DEBUG
        Serial.println("DEBUG: Exiting low power mode, starting next cycle.");
    #endif
    
    //delay(LPM_SLEEP_TIME);
}

void configureJoinTTN()
{
    #ifdef DEBUG
        Serial.println("DEBUG: Attempting to Join TTN...");
    #endif
    LoRaWAN.setAdaptiveDR(false);
    LoRaWAN.setFixedDR(DR_3);

    while(!LoRaWAN.joinOTAA(appEui, appKey, devEui)){
        #ifdef DEBUG
            Serial.println("DEBUG: Failed to join TTN.");
            Serial.printf("\nDEBUG: Retrying to join in %d seconds...", TTN_JOIN_FAIL_WAIT / 1000);
            delay(TTN_JOIN_FAIL_WAIT);
        #endif
    }
}


void setup() {
    Serial.begin(115200);
    Serial.println("DEBUG: Setup started.");

#ifndef NOLORAWAN
    configureJoinTTN();
#endif

    GPS.begin();
    gpsAcquire();

    TimerInit(&sleepWakeTimer, onSleepWake);
    TimerSetValue(&sleepWakeTimer, LPM_SLEEP_TIME);

    if (GPS.location.isValid()) {
        previousLatitude = GPS.location.lat();
        previousLongitude = GPS.location.lng();
    }

    mcu_status = STATE_SEND_THRESHOLD_EXCEEDED;
}

void loop() {
    switch (mcu_status) {
        case STATE_GPS_ACQUIRE:
            gpsAcquire();
            break;

        case STATE_SEND_THRESHOLD_EXCEEDED:
            readSendData(); 
            break;

        case STATE_SLEEP:
            lowPowerSleep();
            break;

        default:
            mcu_status = STATE_SLEEP;
            break;
    }
}
