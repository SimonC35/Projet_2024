#include "GPS_Air530Z.h"
#include "LoRaWanMinimal_APP.h"
#include "Arduino.h"

#include "flagSettings.h"

constexpr unsigned short int cardID = 1; // TODO : Probably make this a variable value.

void sendData(uint8_t * fullarray);      // Prototypes, TODO (MAYBE) : put function higher than main loop & setup... Or make a prototype for every function
void readData(uint8_t * fullarray);

uint8_t fullArray[16];


#ifndef NOLORAWAN                                 // Credentials
static uint8_t devEui[] = { 0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x06, 0xD9, 0x8E };
static uint8_t appEui[] = { 0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x06, 0xD9, 0x8E };
static uint8_t appKey[] = { 0xC7, 0x6E, 0xD6, 0x89, 0xD4, 0x89, 0x64, 0x32, 0xB3, 0x76, 0xFD, 0xF2, 0xA3, 0xC1, 0x9B, 0x96 };

uint16_t userChannelsMask[6]={ 0x00FF,0x0000,0x0000,0x0000,0x0000,0x0000 };
#endif

TimerEvent_t sleepTimer;    // USELESS ?
bool sleepTimerExpired;     // USELESS ?
Air530ZClass GPS;           // declaration of the GPS

#ifdef DEBUG // Function in order to see in the serial monitor the main array data and the value associated with them
void printDebugInfo(uint8_t datalen, uint8_t *data)
{
  Serial.print("\n\n");
  uint8_t *ptr = data;
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
  Serial.printf("%02X%02X", *ptr, *(ptr+1));
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

  readData(fullArray);

#ifdef DEBUG
  printDebugInfo(12, fullArray);
#endif

#ifndef NOLORAWAN
  sendData(fullArray);
#endif

  lowPowerSleep(300000); //300000 - 5 minutes
}

void readData(uint8_t * fullarray)
{
  uint8_t *ptr = fullArray;
  fullArray[0] = (cardID >> 8) & 0xFF; fullArray[1] = cardID & 0xFF; ptr+=2;

  if (flagSettings & FLAGS[FLAG_coords]) {
      int valueArray[2];
      valueArray[0] = (int) (((float) GPS.location.lat()) * pow(10,5));
      valueArray[1] = (int) (((float) GPS.location.lng()) * pow(10,5));
      for (int i = 0; i < coordsDataSize; i++){
        if (i < 4){
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
      unsigned short altValue = (unsigned short int) ((float) GPS.altitude.meters() * pow(10,2));
      *ptr = (altValue << 8) & 0xFF;
      *(ptr++) = altValue & 0xFF;
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