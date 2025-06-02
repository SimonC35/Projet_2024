#include "debug.h"
#include "utils.h"
#include "Arduino.h"

void printDebugInfo(uint8_t datalen, uint8_t *data)
{
    if (data == nullptr || datalen == 0) {
        Serial.println("Error: Invalid data pointer or empty data length.");
        return;
    }

    Serial.println("\n--- DEBUG PAYLOAD ---\n");

    Serial.printf("RAW  : ");
    for (int i = 0; i < datalen; ++i) {
        Serial.printf("%02X ", data[i]);
    }
    Serial.println();

    uint8_t *ptr = data;

    while (ptr < data + datalen) {
        if (ptr >= data + datalen) {
            Serial.println("Error: Pointer out of bounds.");
            break;
        }

        uint8_t identifier = *(ptr++);
        Serial.printf("\nIdentifier: 0x%02X\n", identifier);

        switch (identifier) {
        case 0x01: { // coordonnées
            if (ptr + 2 * sizeof(uint32_t) > data + datalen) {
                Serial.println("Error: Insufficient data for GPS coordinates.");
                return;
            }

            int32_t latitude, longitude;
            memcpy(&latitude, ptr, sizeof(int32_t));
            ptr += sizeof(int32_t);

            memcpy(&longitude, ptr, sizeof(int32_t));
            ptr += sizeof(int32_t);

            Serial.printf("Latitude: %d\n", latitude);
            Serial.printf("Longitude: %d\n", longitude);
            break;
        }
        case 0x02: { // Altitude
            if (ptr + sizeof(uint16_t) > data + datalen) {
                Serial.println("Error: Insufficient data for altitude.");
                return;
            }

            uint16_t altitude;
            memcpy(&altitude, ptr, sizeof(uint16_t));
            ptr += sizeof(uint16_t);

            Serial.printf("Altitude: %d meters\n", altitude);
            break;
        }
        case 0x03: { // HDOP
            if (ptr + sizeof(uint8_t) > data + datalen) {
                Serial.println("Error: Insufficient data for HDOP.");
                return;
            }

            uint8_t hdop = *(ptr++);
            Serial.printf("HDOP: %.2f\n", (float) ((float)hdop / 10.0));
            break;
        }
        case 0x04: {
            if (ptr + sizeof(uint8_t) > data + datalen) {
                Serial.println("Error: Insufficient data for speed.");
                return;
            }

            uint8_t speed = *(ptr++);
            Serial.printf("Speed: %.2f m/s\n", (float) ((float)speed / digitPrecision1));
            break;
        }
        case 0x05: { // Course
            if (ptr + sizeof(uint8_t) > data + datalen) {
                Serial.println("Error: Insufficient data for course.");
                return;
            }

            uint8_t course = *(ptr++);
            Serial.printf("Course: %.2f degrees\n", (course * 360.0) / 255.0 );
            break;
        }
        case 0x06: { // Satellites
            if (ptr + sizeof(uint8_t) > data + datalen) {
                Serial.println("Error: Insufficient data for satellites.");
                return;
            }

            uint8_t satellites = *(ptr++);
            Serial.printf("Satellites: %d\n", satellites);
            break;
        }
        default: {
            Serial.printf("Unknown Identifier: 0x%02X\n", identifier);
            return;
        }
        }
    }

    Serial.println("\n--------------------------");
    Serial.println("End of DEBUG PAYLOAD");
    delay(1000);
}