#ifndef MAIN_H
#define MAIN_H

#include "GPS_Air530Z.h"
#include "Arduino.h"
#include "LoRaWanMinimal_APP.h"

#include "gps.h"
#include "powerManagement.h"
#include "loraWAN.h"
#include "config.h"
#include "debug.h"
#include "utils.h"
#include "secret.h"

extern Air530ZClass GPS;

extern double previousLatitude;
extern double previousLongitude;

extern uint8_t fullArray[PAYLOAD_SIZE];

#endif
