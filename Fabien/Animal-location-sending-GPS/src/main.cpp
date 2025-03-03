#include "GPS_Air530Z.h"
#include "LoRaWanMinimal_APP.h"
#include "Arduino.h"

#define valueCount 3

void sendData(uint8_t * fullarray);
void readData(uint8_t * fullarray);

uint8_t fullArray[valueCount*4];

static uint8_t devEui[] = { 0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x06, 0xD9, 0x8E };
static uint8_t appEui[] = { 0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x06, 0xD9, 0x8E };
static uint8_t appKey[] = { 0xCE, 0xE4, 0xF3, 0xAA, 0x0D, 0x2A, 0x2A, 0x08, 0xEA, 0xC1, 0x52, 0x95, 0x33, 0xDF, 0x81, 0xB3 };

uint16_t userChannelsMask[6]={ 0x00FF,0x0000,0x0000,0x0000,0x0000,0x0000 };

///////////////////////////////////////////////////
//Some utilities for going into low power mode
TimerEvent_t sleepTimer;
//Records whether our sleep/low power timer expired
bool sleepTimerExpired;

static void wakeUp()
{
  sleepTimerExpired=true;
}

static void lowPowerSleep(uint32_t sleeptime)
{
  sleepTimerExpired=false;
  TimerInit( &sleepTimer, &wakeUp );
  TimerSetValue( &sleepTimer, sleeptime );
  TimerStart( &sleepTimer );
  //Low power handler also gets interrupted by other timers
  //So wait until our timer had expired
  while (!sleepTimerExpired) lowPowerHandler();
  TimerStop( &sleepTimer );
}

void send(uint8_t datalen, uint8_t *data, uint8_t port, bool confirmed)
{
    printf("\n\n");
    for (size_t i = 0; i < datalen;i++)
    {
        if (i == 4 || i == 8) printf("\n");
        printf("%02X", *data);
        data++;  
      }


}

Air530ZClass GPS;

void setup() {
	Serial.begin(115200);

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

  GPS.begin();

  while(GPS.location.lat() == 0.0 || GPS.location.lng() == 0.0)
  {
    Serial.print("Non valid coordinates ! ");
    while (GPS.available() > 0)
    {
      GPS.encode(GPS.read());
    }
      delay(2000);  
  }

}

void loop()
{
    while (GPS.available() > 0)
    {
      GPS.encode(GPS.read());
    }

  readData(fullArray);

  sendData(fullArray);

  send(12, fullArray, 1, 1);

  lowPowerSleep(120000);  
}

void readData(uint8_t * fullarray)
{

  int valueArray[valueCount];

  valueArray[0] = (int) ((float) GPS.location.lat() * pow(10,6));
  valueArray[1] = (int) ((float) GPS.location.lng() * pow(10,6));
  valueArray[2] = (int) ((float) GPS.altitude.meters() * pow(10,2));
  
  Serial.print(valueArray[0]);
  Serial.println();
  Serial.print(valueArray[1]);
  Serial.println();
  Serial.print(valueArray[2]);
  Serial.println();

  for (size_t i = 0; i < valueCount; i++)
  {
      fullArray[i*4]   = (valueArray[i] >> 24) & 0xFF;
      fullArray[i*4+1] = (valueArray[i] >> 16) & 0xFF;
      fullArray[i*4+2] = (valueArray[i] >> 8)  & 0xFF;
      fullArray[i*4+3] = (valueArray[i])       & 0xFF;
  }

}

void sendData(uint8_t * fullArray)
{

  bool confirmed;

  if (LoRaWAN.send(valueCount*4, fullArray, 1, confirmed)) {
    Serial.println("Send OK");
  } else {
    Serial.println("Send FAILED");
  }

}