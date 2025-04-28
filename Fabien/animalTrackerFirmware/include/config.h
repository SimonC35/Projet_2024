#ifndef CONFIG_H
#define CONFIG_H

#include "Arduino.h"

constexpr uint16_t GPS_INIT_TIMEOUT = 60000; // 1 minute
constexpr uint16_t GPS_SLEEPTIME = 180000; // 1 minute
constexpr uint16_t GPS_UPDATE_TIMEOUT = 90000; // 1.5 minutes
constexpr uint16_t GPS_CONTINUE_TIMEOUT = 50000; // 50 seconds
constexpr uint32_t LPM_SLEEP_TIME = 180000; // 3 minute
constexpr uint16_t TTN_JOIN_FAIL_WAIT = 5000; // 5 seconds

constexpr float DISTANCE_THRESHOLD = 5.0; // 0.0 meters for no threshold

constexpr uint16_t RETRY_COUNT = 5;

#define BASE_SIZE 1

#ifdef GPS_COORDS
constexpr uint8_t GPS_COORDS_SIZE = 9;
#else
constexpr uint8_t GPS_COORDS_SIZE = 0;
#endif

#ifdef GPS_ALT
constexpr uint8_t GPS_ALT_SIZE = 3;
#else
constexpr uint8_t GPS_ALT_SIZE = 0;
#endif

#ifdef GPS_HDOP
constexpr uint8_t GPS_HDOP_SIZE = 2;
#else
constexpr uint8_t GPS_HDOP_SIZE = 0;
#endif

#ifdef GPS_SPEED
constexpr uint8_t GPS_SPEED_SIZE = 2;
#else
constexpr uint8_t GPS_SPEED_SIZE = 0;
#endif

#ifdef GPS_COURSE
constexpr uint8_t GPS_COURSE_SIZE = 2;
#else
constexpr uint8_t GPS_COURSE_SIZE = 0;
#endif

#ifdef GPS_SATS
constexpr uint8_t GPS_SATS_SIZE = 2;
#else
constexpr uint8_t GPS_SATS_SIZE = 0;
#endif

constexpr uint8_t PAYLOAD_SIZE_CALC =
    BASE_SIZE +
    GPS_COORDS_SIZE +
    GPS_ALT_SIZE +
    GPS_HDOP_SIZE +
    GPS_SPEED_SIZE +
    GPS_COURSE_SIZE +
    GPS_SATS_SIZE;

constexpr uint8_t PAYLOAD_SIZE = PAYLOAD_SIZE_CALC;


#endif