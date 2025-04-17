#include "debug.h"
#include "utils.h"
#include "Arduino.h"

void printDebugInfo(uint8_t datalen, uint8_t *data) {
    if (data == nullptr || datalen == 0) {
        Serial.println("Error: Invalid data pointer or empty data length.");
        return;
    }

    Serial.println("\n--- DEBUG PAYLOAD ---");

    // Affichage des données brutes
    Serial.printf("RAW  : ");
    for (int i = 0; i < datalen; ++i) {
        Serial.printf("%02X ", data[i]);
    }
    Serial.println();

    uint8_t *ptr = data;

    // Lecture de l'ID (2 octets)
    uint16_t card_id;
    memcpy(&card_id, ptr, sizeof(uint16_t));
    ptr += sizeof(uint16_t);
    Serial.printf("\nID   : %04X = %d\n", card_id, card_id);

    // Lecture des données
    while (ptr < data + datalen) {
        if (ptr >= data + datalen) {
            Serial.println("Error: Pointer out of bounds.");
            break;
        }

        uint8_t identifier = *(ptr++);
        Serial.printf("\nIdentifier: 0x%02X\n", identifier);

        switch (identifier) {
        case 0x01: { // GPS Coordinates
            if (ptr + 2 * sizeof(uint32_t) > data + datalen) {
                Serial.println("Error: Insufficient data for GPS coordinates.");
                return;
            }

            uint32_t latitude, longitude;
            memcpy(&latitude, ptr, sizeof(uint32_t));
            ptr += sizeof(uint32_t);
            memcpy(&longitude, ptr, sizeof(uint32_t));
            ptr += sizeof(uint32_t);

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
            Serial.printf("HDOP: %d\n", hdop / digitPrecision2);
            break;
        }
        case 0x04: {
            if (ptr + sizeof(uint8_t) > data + datalen) {
                Serial.println("Error: Insufficient data for speed.");
                return;
            }

            uint8_t speed = *(ptr++);
            Serial.printf("Speed: %d m/s\n", speed / digitPrecision2);
            break;
        }
        case 0x05: { // Course
            if (ptr + sizeof(uint8_t) > data + datalen) {
                Serial.println("Error: Insufficient data for course.");
                return;
            }

            uint8_t course = *(ptr++);
            Serial.printf("Course: %.2f degrees\n", ((float)(course) / digitPrecision2) * 360 / 255);
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
    delay(30000);
}