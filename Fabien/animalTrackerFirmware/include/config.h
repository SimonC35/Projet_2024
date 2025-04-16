#ifndef CONFIG_H
#define CONFIG_H

#include "Arduino.h"

constexpr uint16_t CARD_ID = 0xBEEF; // Example card ID
constexpr uint16_t GPS_INIT_TIMEOUT = 60000; // 1 minute
constexpr uint16_t GPS_SLEEPTIME = 60000; // 1 minute
constexpr uint16_t GPS_UPDATE_TIMEOUT = 60000; // 1 minute
constexpr uint16_t GPS_CONTINUE_TIMEOUT = 10000; // 10 seconds
const uint32_t LPM_SLEEP_TIME = 60000; // 30 seconds

const float DISTANCE_THRESHOLD = 0.0; // 0.0 meters for no threshold

constexpr uint8_t PAYLOAD_SIZE = 2
                                    #ifdef GPS_COORDS + 9 
                                    #endif 
                                    #ifdef GPS_ALT + 3
                                    #endif
                                    #ifdef GPS_HDOP + 2
                                    #endif
                                    #ifdef GPS_SPEED + 2
                                    #endif
                                    #ifdef GPS_COURSE + 2
                                    #endif
                                    #ifdef GPS_SAT + 2
                                    #endif
                                    #ifdef GPS_DATE + 3
                                    #endif
                                    #ifdef GPS_TIME + 3
                                    #endif
                                    #ifdef GPS_SAT + 2
                                    #endif
                                    ; // Payload size in bytes

#endif