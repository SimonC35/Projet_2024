#include "main.h"

void VextON(void) {
    pinMode(Vext, OUTPUT);
    digitalWrite(Vext, LOW);
}

void VextOFF(void) {
    pinMode(Vext, OUTPUT);
    digitalWrite(Vext, HIGH);
}

void readGPSStoreAsBytes(uint8_t *fullArray) {
    uint8_t *ptr = fullArray;

    uint8_t tempUint8_t = 0;
    uint16_t tempUint16_t = 0;
    uint32_t tempUint32_t = 0;

    // Add CARD_ID (always present)
    memcpy(ptr, &CARD_ID, sizeof(uint16_t));
    ptr += sizeof(uint16_t);

#ifdef DEBUG
    Serial.printf("\n---\nCard ID copied: %04X\n---\n", CARD_ID);
#endif

    // Add GPS coordinates (latitude and longitude)
#ifdef GPS_COORDS
    if (GPS.location.isValid()) {
        tempUint8_t = 0x01; // Identifier for GPS coordinates
        memcpy(ptr, &tempUint8_t, sizeof(uint8_t));
        ptr += sizeof(uint8_t);

        tempUint32_t = (uint32_t)(GPS.location.lat() * digitPrecision5);
        memcpy(ptr, &tempUint32_t, sizeof(uint32_t));
        ptr += sizeof(uint32_t);

#ifdef DEBUG
    Serial.printf("GPS Coordinates added: Lat=%d", tempUint32_t);
#endif

        tempUint32_t = (uint32_t)(GPS.location.lng() * digitPrecision5);
        memcpy(ptr, &tempUint32_t, sizeof(uint32_t));
        ptr += sizeof(uint32_t);

#ifdef DEBUG
        Serial.printf(" Lng=%d\n", tempUint32_t);
#endif
    }
#endif

#ifdef GPS_ALT
    if (GPS.altitude.isValid()) {
        tempUint8_t = 0x02;
        memcpy(ptr, &tempUint8_t, sizeof(uint8_t));
        ptr += sizeof(uint8_t);

        if (GPS.altitude.meters() >= 65535) tempUint16_t = 65535;
        else {
        tempUint16_t = (uint16_t)(GPS.altitude.meters() * digitPrecision2);
        }

        memcpy(ptr, &tempUint16_t, sizeof(uint16_t));
        ptr += sizeof(uint16_t);

#ifdef DEBUG
        Serial.printf("Altitude added: %d meters\n", tempUint16_t);
#endif
    }
#endif

    // Add HDOP
#ifdef GPS_HDOP
    if (GPS.hdop.isValid()) {
        tempUint8_t = 0x03; // Identifier for HDOP
        memcpy(ptr, &tempUint8_t, sizeof(uint8_t));
        ptr += sizeof(uint8_t);

        Serial.printf("/!\\ HDOP  value: %d\n", GPS.hdop.value());

        if (GPS.hdop.value() >= 255) tempUint8_t = 255;
        else if (GPS.hdop.value() <= 0) tempUint8_t = 0;
        else {
        tempUint8_t = (uint8_t)(GPS.hdop.value());
        }

        memcpy(ptr, &tempUint8_t, sizeof(uint8_t));
        ptr += sizeof(uint8_t);

#ifdef DEBUG
        Serial.printf("HDOP added: %d\n", tempUint8_t);
#endif
    }
#endif

    // Add speed
#ifdef GPS_SPEED
    if (GPS.speed.isValid()) {
        tempUint8_t = 0x04; // Identifier for speed
        memcpy(ptr, &tempUint8_t, sizeof(uint8_t));
        ptr += sizeof(uint8_t);

        if (GPS.speed.mps() >= 255) tempUint8_t = 255;
        else if (GPS.speed.mps() <= 0) tempUint8_t = 0;
        else {
        tempUint8_t = (uint8_t)(GPS.speed.mps() * digitPrecision2);
        }

        memcpy(ptr, &tempUint8_t, sizeof(uint8_t));
        ptr += sizeof(uint8_t);

#ifdef DEBUG
        Serial.printf("Speed added: %d m/s\n", tempUint8_t);
#endif
    }
#endif

    // Add course
#ifdef GPS_COURSE
    if (GPS.course.isValid()) {
        tempUint8_t = 0x05; // Identifier for course
        memcpy(ptr, &tempUint8_t, sizeof(uint8_t));
        ptr += sizeof(uint8_t);

        tempUint8_t = (uint8_t) (((GPS.course.deg() * 255) / 360) * digitPrecision2);

        memcpy(ptr, &tempUint8_t, sizeof(uint8_t));
        ptr += sizeof(uint8_t);

#ifdef DEBUG
        Serial.printf("Course added: %d degrees\n", tempUint8_t);
#endif
    }
#endif


#ifdef GPS_SATS
    if (GPS.satellites.isValid()) {
        tempUint8_t = 0x06;
        memcpy(ptr, &tempUint8_t, sizeof(uint8_t));
        ptr += sizeof(uint8_t);

        if (GPS.satellites.value() >= 255) tempUint8_t = 255;
        else { tempUint8_t = 0;
        }

        memcpy(ptr, &tempUint8_t, sizeof(uint8_t));
        ptr += sizeof(uint8_t);

#ifdef DEBUG
        Serial.printf("Satellites added: %d\n", tempUint8_t);
#endif
    }
#endif

#ifdef DEBUG
    printDebugInfo(ptr - fullArray, fullArray);
#endif
}

void sendData(uint8_t *fullArray, size_t length) {
#ifdef DEBUG
    Serial.println("DEBUG: Sending GPS data...");
#endif

    // Exemple : envoyer les données via Serial ou un autre protocole
    Serial.printf("Latitude: %.6f, Longitude: %.6f\n", GPS.location.lat(), GPS.location.lng());
#ifndef NOLORAWAN
    bool confirmed = false;
    int retryCount = 3;

    while (retryCount > 0) {
        if (LoRaWAN.send(12, fullArray, length, confirmed)) {
            Serial.println("Send OK");
            break;
        } else {
            Serial.println("Send FAILED, retrying...");
            retryCount--;
        }
    }

    if (retryCount == 0) {
        Serial.println("Send FAILED after retries");
    }
#endif
}

void readSendData() {
    readGPSStoreAsBytes(fullArray);
    sendData(fullArray, sizeof(fullArray));
}

void gpsUpdate(uint32_t timeout, uint32_t continuetime) {
    VextON();
    delay(10);

    GPS.begin();
    starttime = millis();
    while ((millis() - starttime) < timeout) {
        while (GPS.available() > 0) {
            GPS.encode(GPS.read());
        }

        if (GPS.location.age() < 1000) {
            break;
        }
    }

    if (GPS.location.age() < 1000) {
        starttime = millis();
        while ((millis() - starttime) < continuetime) {
            while (GPS.available() > 0) {
                GPS.encode(GPS.read());
            }
        }
    }

    GPS.end();
    VextOFF();

    TimerSetValue(&autoGPS, GPS_SLEEPTIME);
    TimerStart(&autoGPS);
}

void lowPowerHandler() {
#ifdef DEBUG
    Serial.println("DEBUG: Entering low power mode...");
#endif

    delay(GPS_SLEEPTIME);

    mcu_status = STATUS_UPDATE_GPS;

#ifdef DEBUG
    Serial.println("DEBUG: Exiting low power mode, transitioning to STATUS_UPDATE_GPS.");
#endif
}

void onAutoGPS() {
#ifdef DEBUG
    Serial.println("DEBUG: AutoGPS timer triggered. Updating mcu_status to STATUS_UPDATE_GPS.");
#endif

    TimerStop(&autoGPS);
    mcu_status = STATUS_UPDATE_GPS;
}

void setup() {
    Serial.begin(115200);
    Serial.println("DEBUG: Setup started.");

    gpsUpdate(GPS_INIT_TIMEOUT, GPS_CONTINUE_TIMEOUT);

    TimerInit(&autoGPS, onAutoGPS);
    TimerSetValue(&autoGPS, GPS_SLEEPTIME);
    TimerStart(&autoGPS);

    mcu_status = STATUS_LPM;

    if (GPS.location.isValid()) {
        previousLatitude = GPS.location.lat();
        previousLongitude = GPS.location.lng();
    }
}

void loop() {
    if (mcu_status == STATUS_UPDATE_GPS) {
        if (GPS.location.isValid()) {

            Serial.printf("DEBUG: Previous Latitude: %.6f, Longitude: %.6f\n", previousLatitude, previousLongitude);
            Serial.printf("DEBUG: Current Latitude: %.6f, Longitude: %.6f\n", GPS.location.lat(), GPS.location.lng());

            double distance = GPS.distanceBetween(previousLatitude, previousLongitude, GPS.location.lat(), GPS.location.lng());

            Serial.printf("DEBUG: Distance calculated: %.2f meters\n", distance);

            // Vérification explicite du seuil de distance
            if (DISTANCE_THRESHOLD == 0.0 || distance > DISTANCE_THRESHOLD) {
                Serial.println("DEBUG: Distance threshold exceeded or DISTANCE_THRESHOLD is 0.0. Sending data...");
                readSendData();
                previousLatitude = GPS.location.lat();
                previousLongitude = GPS.location.lng();
                
                TimerStop(&autoGPS);
                TimerSetValue(&autoGPS, LPM_SLEEP_TIME);
                TimerStart(&autoGPS);
                mcu_status = STATUS_LPM;
            } else {
                Serial.println("DEBUG: Distance threshold not exceeded. Entering LPM...");
                TimerStop(&autoGPS);
                TimerSetValue(&autoGPS, LPM_SLEEP_TIME);
                TimerStart(&autoGPS);
                mcu_status = STATUS_LPM;
            }
        } else {
            Serial.println("DEBUG: GPS location is not valid. Entering LPM...");
            TimerStop(&autoGPS);
            TimerSetValue(&autoGPS, LPM_SLEEP_TIME);
            TimerStart(&autoGPS);
            mcu_status = STATUS_LPM;
        }
    }
}