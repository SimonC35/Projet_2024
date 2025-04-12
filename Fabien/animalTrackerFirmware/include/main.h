#ifndef MAIN_H
#define MAIN_H

#include "GPS_Air530Z.h"
#include "LoRaWanMinimal_APP.h"
#include "Arduino.h"
#include "flagSettings.h"
#include "secret.h"

constexpr unsigned short int CARD_ID = 1;
constexpr uint32_t SLEEP_TIME = 300000; // 5 minutes
constexpr uint32_t RETRY_DELAY = 30000; // 30 seconds

void sendData(uint8_t *fullArray);
void readData(uint8_t *fullArray);
void setupLoRaWAN();
void setupGPS();
void readGPSStoreAsBytes(uint8_t *fullArray);

#endif // MAIN_H
