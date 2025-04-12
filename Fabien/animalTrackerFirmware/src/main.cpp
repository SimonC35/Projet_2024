#include "main.h"

uint8_t fullArray[16];

#ifndef NOLORAWAN
uint16_t userChannelsMask[6] = { 0x00FF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 };
#endif

TimerEvent_t sleepTimer;
bool sleepTimerExpired;
Air530ZClass GPS;

void printDebugInfo(uint8_t datalen, uint8_t *data)
{
  Serial.print("\n\n");
  uint8_t *ptr = &data[0];
  for (int i = 0; i < 12; i++)
  {
    Serial.printf("%02X", *ptr);
    ptr++;
  }
  Serial.print("\n");
  Serial.printf("\n\nID  : %02X%02X = %d", data[0], data[1], CARD_ID);
  ptr = &data[2];
  for (short int i = 0; i < 8; ++i)
  {
    if (i == 0) Serial.print("\nLAT : ");
    if (i == 4) Serial.print("\nLNG : ");
    Serial.printf("%02X", *ptr);
    ptr++;
    if (i == 3){ Serial.printf(" = %.5f ", GPS.location.lat());}
    if (i == 7){ Serial.printf(" = %.5f ", GPS.location.lng());}
  }
  Serial.printf("\nALT : ");
  Serial.printf("%02X%02X", *ptr, *(++ptr));
  Serial.printf(" = %.2f \n\n", GPS.altitude.meters());
  Serial.printf("\n\n");

  Serial.printf("FLAG_coords : %u \n", flagSettings & FLAG_coords);
  Serial.printf("FLAG_alt : %u \n", flagSettings & FLAG_alt);
  Serial.printf("FLAG_hdop : %u \n", flagSettings & FLAG_hdop);
  Serial.printf("FLAG_course : %u \n", flagSettings & FLAG_course);
  Serial.printf("FLAG_sats : %u \n", flagSettings & FLAG_sats);
  Serial.printf("FLAG_age : %u \n", flagSettings & FLAG_age);
  Serial.printf("FLAG_time : %u \n", flagSettings & FLAG_time);

  Serial.printf("\n\n");
}

static void wakeUp()
{
  sleepTimerExpired = true;
}

static void lowPowerSleep(uint32_t sleeptime)
{
  sleepTimerExpired = false;
  TimerInit(&sleepTimer, &wakeUp);
  TimerSetValue(&sleepTimer, sleeptime);
  TimerStart(&sleepTimer);
  while (!sleepTimerExpired) lowPowerHandler();
  TimerStop(&sleepTimer);
}

void addToBuffer(uint8_t*& ptr, int32_t value, uint8_t size)
{
  for (int i = 0; i < size; i++) {
    *ptr = (value >> (8 * (size - 1 - i))) & 0xFF;
    ptr++;
  }
}

void setup()
{
  Serial.begin(115200);

#ifndef NOLORAWAN
  LoRaWAN.begin(LORAWAN_CLASS, ACTIVE_REGION);
  LoRaWAN.setAdaptiveDR(true);

  while (1) {
    Serial.print("Joining... ");
    LoRaWAN.joinOTAA(appEui, appKey, devEui);
    if (!LoRaWAN.isJoined()) {
      Serial.println("JOIN FAILED! Sleeping for 30 seconds");
      lowPowerSleep(30000);
    } else {
      Serial.println("JOINED");
      break;
    }
  }
#endif

  GPS.begin();

  while(GPS.location.lat() == 0.0 || GPS.location.lng() == 0.0)
  {
    while (GPS.available() > 0)
    {
      GPS.encode(GPS.read());
    }
    delay(5000);  
  }
}

void loop()
{
  while (GPS.available() > 0)
  {
    GPS.encode(GPS.read());
  }

  readGPSStoreAsBytes(fullArray);

#ifdef DEBUG
  printDebugInfo(12, fullArray);
#endif

#ifndef NOLORAWAN
  sendData(fullArray);
#endif

  lowPowerSleep(SLEEP_TIME); 
}

void readGPSStoreAsBytes(uint8_t *fullArray)
{
  uint8_t totalDataSize = 2;

  uint8_t *ptr = fullArray;
  fullArray[0] = (CARD_ID >> 8) & 0xFF;
  fullArray[1] = CARD_ID & 0xFF;
  ptr += 2;

  if (flagSettings & FLAG_coords) {
    int32_t lat = (int32_t)(GPS.location.lat() * 100000);
    int32_t lng = (int32_t)(GPS.location.lng() * 100000);
    addToBuffer(ptr, lat, 4);
    addToBuffer(ptr, lng, 4); 
    totalDataSize += 8;
  }

  if (flagSettings & FLAG_alt) {
    short unsigned int altValue = (short unsigned int) (((float) GPS.altitude.meters()) * 100);
    *ptr = (altValue >> 8);
    *(++ptr) = (altValue) & 0xFF;
    totalDataSize += 2;
  }

  if (flagSettings & FLAG_time) {
    unsigned int secondsSinceMidnight = (unsigned int)(GPS.time.hour() * 3600 + GPS.time.minute() * 60 + GPS.time.second());
    addToBuffer(ptr, secondsSinceMidnight, 2);  // Ajouter le temps en secondes (2 octets)
    totalDataSize += 2;
  }
}



void sendData(uint8_t *fullArray)
{
#ifndef NOLORAWAN
  bool confirmed = false;

  if (LoRaWAN.send(12, fullArray, 1, confirmed)) {
    Serial.println("Send OK");
  } else {
    Serial.println("Send FAILED");
  }
#endif
}
