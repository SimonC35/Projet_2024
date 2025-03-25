#include <stdint.h>

extern uint8_t flagSettings;
extern uint8_t dataSumSize;

// definitions of flags position in the array below
// E.G. : FLAG_coords would be used like : FLAGS[flags_coords] -> returning 0b0000'0001
constexpr uint8_t FLAG_coords = 0;
constexpr uint8_t FLAG_alt = 1;
constexpr uint8_t FLAG_hdop = 2;
constexpr uint8_t FLAG_speed = 3;
constexpr uint8_t FLAG_course = 4;
constexpr uint8_t FLAG_sats = 5;
constexpr uint8_t FLAG_age = 6;
constexpr uint8_t FLAG_time = 7;

constexpr uint8_t FLAGS[8] = {0b00000001, 0b00000010, 0b00000100, 0b00001000, 0b00010000, 0b00100000, 0b01000000, 0b10000000};

constexpr uint8_t idDataSize      = 2;
constexpr uint8_t coordsDataSize  = 8;
constexpr uint8_t altDataSize     = 2;
constexpr uint8_t hdopDataSize = 1;
constexpr uint8_t speedDataSize = 1;
constexpr uint8_t courseDataSize = 1;
constexpr uint8_t SatsDataSize = 1;
constexpr uint8_t ageDataSize = 1;
constexpr uint8_t timeDataSize = 2;

// Size in byte for every Data that are possible to send, order is below
constexpr uint8_t dataSizes[] = {2, 8, 2, 1, 1 ,1, 1, 1, 2};

/* Positions in array = 
idDataSize      0 || Size of id, unsigned short = 2 bytes 
coordsDataSize  1 || Size of lat + lng in BYTES || 2 int = 2 * 4 bytes
altDataSize     2 || Size of altitude unsigned short = 2 bytes 
                     Total array size = 12 Bytes : [id, coords, alt]
                                                   [2 , 8     , 2  ] (bytes) 
                     Same logic appliues for every next size
hdopDataSize    3 || 
speedDataSize   4
courseDataSize  5
SatsDataSize    6
ageDataSize     7
timeDataSize    8
*/                             