#ifndef POWER_MANAGEMENT_H
#define POWER_MANAGEMENT_H

#include "Arduino.h"

void VextON(void);
void VextOFF(void);

typedef enum {
    STATUS_INIT,
    STATUS_LPM,
    STATUS_UPDATE_GPS,
    STATUS_SEND_DATA
} dev_status;

static TimerEvent_t autoGPS;
extern dev_status mcu_status;
extern uint32_t starttime;

#endif