/**
 * This is an example of joining, sending and receiving data via LoRaWAN using a more minimal interface.
 * 
 * The example is configured for OTAA, set your keys into the variables below.
 * 
 * The example will upload a counter value periodically, and will print any downlink messages.
 * 
 * please disable AT_SUPPORT in tools menu
 *
 * David Brodrick.
 */
#include "GPS_Air530Z.h"
#include "LoRaWanMinimal_APP.h"
#include "Arduino.h"

#define valueCount 3

void sendData();

/*
 * set LoraWan_RGB to Active,the RGB active in loraWan
 * RGB red means sending;
 * RGB purple means joined done;
 * RGB blue means RxWindow1;
 * RGB yellow means RxWindow2;
 * RGB green means received done;
 */

//Set these OTAA parameters to match your app/node in TTN
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

Air530ZClass GPS;

void setup() {
	Serial.begin(115200);

  LoRaWAN.begin(LORAWAN_CLASS, ACTIVE_REGION);
  
  //Enable ADR
  LoRaWAN.setAdaptiveDR(true);

  while (1) {
    Serial.print("Joining... ");
    LoRaWAN.joinOTAA(appEui, appKey, devEui);
    if (!LoRaWAN.isJoined()) {
      //In this example we just loop until we're joined, but you could
      //also go and start doing other things and try again later
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
    while (GPS.available() > 0)
    {
      GPS.encode(GPS.read());
    }
      Serial.print(GPS.location.lat(),6);
      Serial.println();
      Serial.print(GPS.location.lng(),6);
      Serial.println();
      Serial.print(GPS.satellites.value());
      Serial.println();
      delay(2000);  
  }

}

void loop()
{
    while (GPS.available() > 0)
    {
      GPS.encode(GPS.read());
    }

  sendData();

  lowPowerSleep(120000);  
}

//Example of handling downlink data
void downLinkDataHandle(McpsIndication_t *mcpsIndication)
{
  Serial.printf("Received downlink: %s, RXSIZE %d, PORT %d, DATA: ",mcpsIndication->RxSlot?"RXWIN2":"RXWIN1",mcpsIndication->BufferSize,mcpsIndication->Port);
  for(uint8_t i=0;i<mcpsIndication->BufferSize;i++) {
    Serial.printf("%02X",mcpsIndication->Buffer[i]);
  }
  Serial.println();
}

uint8_t * intToUint8_tArray(int *array, size_t arraySize)
{
  uint8_t uBytesArray[4 * arraySize];


  return uBytesArray;

}

void sendData()
{

  int valueArray[valueCount];

  valueArray[0] = (int) ((float)GPS.location.lat() * pow(10,6));
  valueArray[1] = (int) ((float)GPS.location.lng() * pow(10,6));
  valueArray[3] = (int) ((float)GPS.altitude.meters() * pow(10,2));
  
  uint8_t fullArray[12];
  for (size_t i = 0; i < valueCount; i++)
  {
      fullArray[i*4]   = (valueArray[i] >> 24) & 0xFF;
      fullArray[i*4+1] = (valueArray[i] >> 16) & 0xFF;
      fullArray[i*4+2] = (valueArray[i] >> 8)  & 0xFF;
      fullArray[i*4+3] = (valueArray[i])       & 0xFF;
  }

  bool confirmed;

  if (LoRaWAN.send(12, fullArray, 1, confirmed)) {
    Serial.println("Send OK");
  } else {
    Serial.println("Send FAILED");
  }

}