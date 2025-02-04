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

///////////////////////////////////////////////////
Air530ZClass GPS;

void setup() {
	Serial.begin(115200);

  if (ACTIVE_REGION==LORAMAC_REGION_AU915) {
    //TTN uses sub-band 2 in AU915
    LoRaWAN.setSubBand2();
  }
 
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

///////////////////////////////////////////////////
void loop()
{
    while (GPS.available() > 0)
    {
      GPS.encode(GPS.read());
    }

  sendData();

  lowPowerSleep(120000);  
}

///////////////////////////////////////////////////
//Example of handling downlink data
void downLinkDataHandle(McpsIndication_t *mcpsIndication)
{
  Serial.printf("Received downlink: %s, RXSIZE %d, PORT %d, DATA: ",mcpsIndication->RxSlot?"RXWIN2":"RXWIN1",mcpsIndication->BufferSize,mcpsIndication->Port);
  for(uint8_t i=0;i<mcpsIndication->BufferSize;i++) {
    Serial.printf("%02X",mcpsIndication->Buffer[i]);
  }
  Serial.println();
}

void sendData()
{
  Serial.println();
  Serial.print("LAT: ");
  Serial.print(GPS.location.lat(),6);
  float lat = (float) GPS.location.lat();
  int lat1 = (int) (lat * pow(10,5));
  Serial.println();
  Serial.print(lat1);
  Serial.println();
  uint8_t latbytes[4];
  Serial.println();

  latbytes[0] = (lat1 >> 24) & 0xFF;
  latbytes[1] = (lat1 >> 16) & 0xFF;
  latbytes[2] = (lat1 >> 8) & 0xFF;
  latbytes[3] = (lat1 >> 0) & 0xFF;

  char latstr[32];
  sprintf(latstr,"\t\t%0b %0b %0b %0b",latbytes[0], latbytes[1], latbytes[2], latbytes[3]);
  Serial.println(latstr);
  sprintf(latstr, "%08b", lat1);
  Serial.println(latstr);
  sprintf(latstr,"%02X %02X %02X %02X",latbytes[0], latbytes[1], latbytes[2], latbytes[3]);
  Serial.print(latstr);
/*
  char latstr[32];
  sprintf(latstr,"%0b %0b%0b%0b%0b",latbytes[0], latbytes[1], latbytes[2], latbytes[3]);
  Serial.println(latstr);
  sprintf(latstr, "%08b", lat1);
  Serial.println(latstr);
  sprintf(latstr,"%02X %02X %02X %02X %02X",latbytes[0], latbytes[1], latbytes[2], latbytes[3]);
//  sprintf(latstr,"%02X", lat1);
  Serial.print(latstr);
*/
/*
  Serial.println();
  Serial.println();
*/
  Serial.println();
  Serial.println();
  Serial.print("LNG: ");
  Serial.print(GPS.location.lng(),6);
  float lng = (float) GPS.location.lng();
  int lng1 = (int) (lng * pow(10,5));
  Serial.println();
  Serial.print(lng1);
  Serial.println();
  uint8_t lngbytes[4];
  Serial.println();

  lngbytes[0] = (lng1 >> 24) & 0xFF;
  lngbytes[1] = (lng1 >> 16) & 0xFF;
  lngbytes[2] = (lng1 >> 8) & 0xFF;
  lngbytes[3] = (lng1 >> 0) & 0xFF;

  char lngstr[32];
  sprintf(latstr,"\t\t%0b %0b %0b %0b", lngbytes[0], lngbytes[1], lngbytes[2], lngbytes[3]);
  Serial.println(lngstr);
  sprintf(lngstr, "%08b", lng1);
  Serial.println(lngstr);
  sprintf(lngstr,"%02X %02X %02X %02X",lngbytes[0], lngbytes[1], lngbytes[2], lngbytes[3]);
  Serial.print(lngstr);

  Serial.println();
  Serial.print("ALT: ");
  Serial.print(GPS.altitude.meters());
  float alt = (float) GPS.altitude.meters();
  int alt1 = (int) (alt * pow(10,2));
  Serial.println();
  Serial.print(alt1);
  Serial.println();
  uint8_t altbytes[4];

  altbytes[0] = (alt1 >> 24) & 0xFF;
  altbytes[1] = (alt1 >> 16) & 0xFF;
  altbytes[2] = (alt1 >> 8) & 0xFF;
  altbytes[3] = (alt1 >> 0) & 0xFF;

  uint8_t fullArray[12];
  fullArray[0]  = latbytes[0];
  fullArray[1]  = latbytes[1];
  fullArray[2]  = latbytes[2];
  fullArray[3]  = latbytes[3];

  fullArray[4]  = lngbytes[0];
  fullArray[5]  = lngbytes[1];
  fullArray[6]  = lngbytes[2];
  fullArray[7]  = lngbytes[3];

  fullArray[8]  = altbytes[0];
  fullArray[9]  = altbytes[1];
  fullArray[10] = altbytes[2];
  fullArray[11] = altbytes[3];

  char fArraystr[128];
  Serial.println();
  Serial.println();
  sprintf(fArraystr,"Valeur full Array binaire : %0b %0b %0b %0b | %0b %0b %0b %0b %0b | %0b %0b %0b %0b", fullArray[0], fullArray[1], fullArray[2], fullArray[3], fullArray[4], fullArray[5], fullArray[6], fullArray[7],  fullArray[8], fullArray[9], fullArray[10], fullArray[11]);
  Serial.println(fArraystr);
  Serial.println();
  sprintf(fArraystr,"Valeur full Array hex :     %02X %02X %02X %02X | %02X %02X %02X %02X | %02X %02X %02X %02X", fullArray[0], fullArray[1], fullArray[2], fullArray[3], fullArray[4], fullArray[5], fullArray[6], fullArray[7], fullArray[8], fullArray[9], fullArray[10], fullArray[11]);
  Serial.print(fArraystr);
  Serial.println();
  bool confirmed;



  if (LoRaWAN.send(12, fullArray, 1, confirmed)) {
    Serial.println("Send OK");
  } else {
    Serial.println("Send FAILED");
  }

}