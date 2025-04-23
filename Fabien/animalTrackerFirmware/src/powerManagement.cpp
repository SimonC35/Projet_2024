#include "main.h"
#include "hw.h"
#include "low_power.h"

uint32_t starttime;
device_state_t mcu_status = STATE_BOOT;
TimerEvent_t sleepWakeTimer;

/*
void lowPowerHandler() {
#ifdef DEBUG
    Serial.println("DEBUG: Entering low power mode...");
#endif

    delay(GPS_SLEEPTIME);  // ou un vrai sleep() si tu veux

    mcu_status = STATUS_UPDATE_GPS;

#ifdef DEBUG
    Serial.println("DEBUG: Exiting low power mode, transitioning to STATUS_UPDATE_GPS.");
#endif
}


*/
