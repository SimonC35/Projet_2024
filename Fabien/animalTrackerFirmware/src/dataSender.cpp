#include "dataSender.h"

uint8_t fullArray[PAYLOAD_SIZE] = {0};


void readSendData()
{
#ifdef DEBUG
    Serial.println("DEBUG: Distance threshold exceeded, sending data...");
#endif

    readGPSStoreAsBytes(fullArray);
    sendData(fullArray, PAYLOAD_SIZE);

    previousLatitude = GPS.location.lat();
    previousLongitude = GPS.location.lng();

    mcuStatus = STATE_SLEEP;
}