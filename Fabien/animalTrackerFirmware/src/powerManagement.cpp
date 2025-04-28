#include "main.h"
#include "hw.h"
#include "low_power.h"

uint32_t starttime;
device_state_t mcu_status = STATE_BOOT;
TimerEvent_t sleepWakeTimer;

void lowPowerSleep()
{
    #ifdef DEBUG
        Serial.println("DEBUG: Entering low power mode...");
    #endif
    TimerStart(&sleepWakeTimer);
    lowPowerHandler();

    #ifdef DEBUG
        Serial.println("DEBUG: Exiting low power mode, starting next cycle.");
    #endif
    
    //delay(LPM_SLEEP_TIME);
}

void onSleepWake()
{
    mcu_status = STATE_GPS_ACQUIRE;
}

