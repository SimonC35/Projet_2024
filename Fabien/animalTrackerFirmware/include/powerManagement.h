#ifndef POWER_MANAGEMENT_H
#define POWER_MANAGEMENT_H

#include "Arduino.h"


typedef enum {
    STATUS_LPM = 0,
    STATUS_UPDATE_GPS
} dev_status;

static TimerEvent_t autoGPS;
extern dev_status mcu_status;
dev_status mcu_status = STATUS_LPM;
uint32_t starttime;

#endif