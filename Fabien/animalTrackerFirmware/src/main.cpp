#include "GPS_Air530Z.h"
#include "LoRaWanMinimal_APP.h"
#include "Arduino.h"

#include "flagSettings.h"
#include "secret.h"

constexpr unsigned short int cardID = 1; // TODO : Probably make this a variable value.

void sendData(uint8_t * fullarray);      // Prototypes, TODO (MAYBE) : put function higher than main loop & setup... Or make a prototype for every function
void readGPSStoreAsBytes(uint8_t * fullarray);

uint8_t fullArray[16];


#ifndef NOLORAWAN
uint16_t userChannelsMask[6]={ 0x00FF,0x0000,0x0000,0x0000,0x0000,0x0000 };
#endif

TimerEvent_t sleepTimer;    // USELESS ?
bool sleepTimerExpired;     // USELESS ?
Air530ZClass GPS;           // declaration of the GPS

#ifdef DEBUG // Function in order to see in the serial monitor the main array data and the value associated with them
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
  Serial.printf("\n\nID  : %02X%02X = %d", data[0], data[1], cardID);
  ptr = &data[2];
  for (size_t i = 0; i < 8; ++i)
  {
    if (i == 0) Serial.print("\nLAT : "); 
    if (i == 4) Serial.print("\nLNG : ");
    Serial.printf("%02X", *ptr);
    ptr++;
    if (i == 3){ Serial.printf(" = %.5f ", GPS.location.lat());}
    if (i == 7){ Serial.printf(" = %.5f ", GPS.location.lng());}
  }
  Serial.print("\nALT : ");
  Serial.printf("%02X%02X", *ptr, *(++ptr));
  Serial.printf(" = %.2f \n\n", GPS.altitude.meters());
}
#endif

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

void setup() {
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
    #ifdef DEBUG
    Serial.print("Non valid coordinates ! ");
    Serial.print(getBatteryVoltage());
    Serial.println();
    #endif
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

  lowPowerSleep(30000); //300000 - 5 minutes
}

void readGPSStoreAsBytes(uint8_t * fullarray)
{
  uint8_t totalDataSize = 2;

  uint8_t *ptr = fullArray;
  fullArray[0] = (cardID >> 8) & 0xFF; fullArray[1] = cardID & 0xFF; ptr+=2;

  if (flagSettings & FLAGS[FLAG_coords]) {
      int valueArray[2];
      totalDataSize += dataSizes[FLAG_coords];
      valueArray[0] = (int) (((float) GPS.location.lat()) * pow(10,5));
      valueArray[1] = (int) (((float) GPS.location.lng()) * pow(10,5));
      for (int i = 0; i < coordsDataSize; i++){
        if (i < 4 /* 4 = coordsDataSize/2*/){
        *ptr = (valueArray[0] >> (24 - (i%4) * 8)) & 0xFF;
        }
        else {
        *ptr = (valueArray[1] >> (24 - (i%4) * 8)) & 0xFF;  
        }
        ptr++;
      }
    }
    if (flagSettings & FLAGS[FLAG_alt])
    {
      short unsigned int altValue = (short unsigned int) (((float) GPS.altitude.meters()) * pow(10,2));
      *ptr = (altValue >> 8);
      *(++ptr) = (altValue) & 0xFF;
    }
}

void sendData(uint8_t * fullArray)
{

  bool confirmed;

#ifndef NOLORAWAN
  if (LoRaWAN.send(12, fullArray, 1, confirmed)) {
    Serial.println("Send OK");
  } else {
    Serial.println("Send FAILED");
  }
#endif

}