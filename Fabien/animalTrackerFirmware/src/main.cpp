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
