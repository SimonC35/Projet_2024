#ifndef POWER_MANAGEMENT_H
#define POWER_MANAGEMENT_H

#include "Arduino.h"

typedef enum {
    STATE_GPS_ACQUIRE,
    STATE_SEND_THRESHOLD_EXCEEDED,
    STATE_SLEEP
} device_state_t;

extern TimerEvent_t sleepWakeTimer;
extern device_state_t mcuStatus;

void configureTimer();
void lowPowerSleep();
void onSleepWake();


#endif