#ifndef LORAWAN_H
#define LORAWAN_H

#include "Arduino.h"

extern uint16_t userChannelsMask[6];

void configureJoinTTN();
void sendData(uint8_t *fullArray, size_t length);

#endif