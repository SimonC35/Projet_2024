#include "gps.h"

void readGPSStoreAsBytes(uint8_t *fullArray) {
    uint8_t *ptr = fullArray;

    uint8_t tempUint8_t = 0;
    uint16_t tempUint16_t = 0;
    uint32_t tempUint32_t = 0;

#ifdef GPS_COORDS
    if (GPS.location.isValid()) {
        tempUint8_t = 0x01;
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
        else if (GPS.altitude.meters() <= 0) tempUint16_t = 0;
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
        if (GPS.hdop.value() >= 255) tempUint8_t = 255;
        else if (GPS.hdop.value() <= 0) tempUint8_t = 0;
        else {
        tempUint8_t = (uint8_t)(GPS.hdop.value() * digitPrecision1);
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
        tempUint8_t = (uint8_t)(GPS.speed.mps() * digitPrecision1);
        }
    
        Serial.printf("/!\\ SPEED  value: %.5f\n", GPS.speed.mps());


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

        tempUint8_t = (uint8_t) (((GPS.course.deg() * 255) / 360));

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
        else if (GPS.satellites.value() <= 0) { tempUint8_t = 0;
        }
        else { 
        tempUint8_t = (uint8_t) GPS.satellites.value();
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
