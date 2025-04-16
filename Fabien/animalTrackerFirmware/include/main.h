#ifndef MAIN_H
#define MAIN_H

#include "GPS_Air530Z.h"
#include "LoRaWanMinimal_APP.h"
#include "Arduino.h"
#include "secret.h"
#include "config.h"
#include "debug.h"
#include "utils.h"
#include "powerManagement.h"

Air530ZClass GPS;

double previousLatitude = 0.0;
double previousLongitude = 0.0;

uint8_t fullArray[PAYLOAD_SIZE] = {0};

#endif
