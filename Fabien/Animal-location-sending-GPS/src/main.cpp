#include "GPS_Air530Z.h"
#include "LoRaWanMinimal_APP.h"
#include "Arduino.h"

constexpr unsigned short int valueCount = 3;
constexpr unsigned short int cardID = 1;

void sendData(uint8_t * fullarray);
void readData(uint8_t * fullarray);

uint8_t fullArray[valueCount*4 + 2];

static uint8_t devEui[] = { 0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x06, 0xD9, 0x8E };
static uint8_t appEui[] = { 0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x06, 0xD9, 0x8E };
static uint8_t appKey[] = { 0xC7, 0x6E, 0xD6, 0x89, 0xD4, 0x89, 0x64, 0x32, 0xB3, 0x76, 0xFD, 0xF2, 0xA3, 0xC1, 0x9B, 0x96 };

uint16_t userChannelsMask[6]={ 0x00FF,0x0000,0x0000,0x0000,0x0000,0x0000 };

TimerEvent_t sleepTimer;
bool sleepTimerExpired;




void printDebugInfo(uint8_t datalen, uint8_t *data)
{
    Serial.print("\n\n");
    uint8_t * ptr = data;
    for (size_t i = 0; i < datalen; i++)
    {
        Serial.print(*ptr, HEX);
        ptr++;
        if ((i + 3) % 4 == 0 || i == 1) Serial.print("\n");
}
}

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
  while (!sleepTimerExpired) lowPowerHandler();
  TimerStop( &sleepTimer );
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
    Serial.print(getBatteryVoltage());
    Serial.println();
    while (GPS.available() > 0)
    {
      GPS.encode(GPS.read());
    }
      delay(15000);  
  }
}

void signWithId(uint8_t * fullArray)
{
  fullArray[0] = (cardID >> 8) & 0xFF;
  fullArray[1] = (cardID) & 0xFF;
}

void loop()
{

  while (GPS.available() > 0)
  {
    GPS.encode(GPS.read());
  }

  signWithId(fullArray);

  readData(&fullArray[2]);

  printDebugInfo(14, fullArray);
  sendData(fullArray);

  delay(300000);
}



void readData(uint8_t * fullarray)
{

  int valueArray[valueCount];

  valueArray[0] = (int) (((float) GPS.location.lat()) * pow(10,5));
  valueArray[1] = (int) (((float) GPS.location.lng()) * pow(10,5));
  valueArray[2] = (int) (((float) GPS.altitude.meters()) * pow(10,2));

  for (size_t i = 0; i < valueCount; i++)
  {
    fullArray[2+i*4]   = (valueArray[i] >> 24) & 0xFF;
    fullArray[2+i*4+1] = (valueArray[i] >> 16) & 0xFF;
    fullArray[2+i*4+2] = (valueArray[i] >> 8)  & 0xFF;
    fullArray[2+i*4+3] = (valueArray[i])       & 0xFF;
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