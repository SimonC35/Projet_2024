#include "Arduino.h"
#include "GPS_Air530.h"
#include "GPS_Air530Z.h"

//if GPS module is Air530, use this
//Air530Class GPS;

//if GPS module is Air530Z, use this
Air530ZClass GPS;

void bytesShift(int intData, uint8_t bytesArray[])
{
    bytesArray[0] = (intData >> 24) & 0xFF;
    bytesArray[1] = (intData >> 16) & 0xFF;
    bytesArray[2] = (intData >> 8) & 0xFF;
    bytesArray[3] = (intData >> 0) & 0xFF;
}

void displayInfo()
{

  /* Every conversion displayed here,  */
  Serial.println("___________________");
  Serial.print("LAT: ");
  Serial.print(GPS.location.lat(),6);
  float lat = (float) GPS.location.lat();
  int lat1 = (int) (lat * pow(10,6));
  Serial.println();
  Serial.print(lat1);
  Serial.println();
  uint8_t latbytes[4];
  Serial.println();

  bytesShift(lat1, latbytes);

  char latstr[32];
  sprintf(latstr,"\t\t%0b %0b %0b %0b",latbytes[0], latbytes[1], latbytes[2], latbytes[3]);
  Serial.println(latstr);
  sprintf(latstr, "%08b", lat1);
  Serial.println(latstr);
  sprintf(latstr,"%02X %02X %02X %02X",latbytes[0], latbytes[1], latbytes[2], latbytes[3]);
  Serial.print(latstr);
  Serial.println();
  Serial.print("___________________");

  Serial.println("___________________");
  Serial.println();
  Serial.print("LNG: ");
  Serial.print(GPS.location.lng(),6);
  float lng = (float) GPS.location.lng();
  int lng1 = (int) (lng * pow(10,6));
  Serial.println();
  Serial.print(lng1);
  Serial.println();
  uint8_t lngbytes[4];
  Serial.println();

  bytesShift(lng1, lngbytes);

  char lngstr[32];
  sprintf(latstr,"\t\t%0b %0b %0b %0b", lngbytes[0], lngbytes[1], lngbytes[2], lngbytes[3]);
  Serial.println(lngstr);
  sprintf(lngstr, "%08b", lng1);
  Serial.println(lngstr);
  sprintf(lngstr,"%02X %02X %02X %02X",lngbytes[0], lngbytes[1], lngbytes[2], lngbytes[3]);
  Serial.print(lngstr);
  Serial.println();
  Serial.print("___________________");

  Serial.println("___________________");
  Serial.println();
  Serial.print("ALT: ");
  Serial.print(GPS.altitude.meters());
  float alt = (float) GPS.altitude.meters();
  int alt1 = (int) (alt * pow(10,2));
  Serial.println();
  Serial.print("Altitude transformé : ");
  Serial.print(alt1);
  Serial.println();

  uint8_t altbytes[4];

  bytesShift(alt1, altbytes);

  char altstr[32];
  sprintf(altstr,"\t\t%0b %0b %0b %0b", altbytes[0], altbytes[1], altbytes[2], altbytes[3]);
  Serial.println(altstr);
  sprintf(lngstr, "%08b", alt1);
  Serial.println(altstr);
  sprintf(altstr,"%02X %02X %02X %02X", altbytes[0], altbytes[1], altbytes[2], altbytes[3]);
  Serial.print(altstr);
  Serial.println();
  Serial.print("___________________");


  uint8_t fullArray[12];
  fullArray[0] = latbytes[0];
  fullArray[1] = latbytes[1];
  fullArray[2] = latbytes[2];
  fullArray[3] = latbytes[3];

  fullArray[4] = lngbytes[0];
  fullArray[5] = lngbytes[1];
  fullArray[6] = lngbytes[2];
  fullArray[7] = lngbytes[3];

  fullArray[8] = altbytes[0];
  fullArray[9] = altbytes[1];
  fullArray[10] = altbytes[2];
  fullArray[11] = altbytes[3];

  char fArraystr[32];
  Serial.println();
  Serial.println();
  sprintf(fArraystr,"\t\t%0b %0b %0b %0b %0b %0b %0b %0b", fullArray[0], fullArray[1], fullArray[2], fullArray[3], fullArray[4], fullArray[5], fullArray[6], fullArray[7]);
  Serial.println(fArraystr);
  Serial.println();
  sprintf(fArraystr,"%02X %02X %02X %02X %02X %02X %02X %02X", fullArray[0], fullArray[1], fullArray[2], fullArray[3], fullArray[4], fullArray[5], fullArray[6], fullArray[7]);
  Serial.print(fArraystr);
  Serial.println();


  
/*
  Serial.print("SATS: ");
  Serial.print(GPS.satellites.value());
  Serial.print(", HDOP: ");
  Serial.print(GPS.hdop.hdop());
  Serial.print(", AGE: ");
  Serial.print(GPS.location.age());
  Serial.print(", COURSE: ");
  Serial.print(GPS.course.deg());
  Serial.print(", SPEED: ");
  Serial.println(GPS.speed.kmph());
  Serial.println();
*/
}

void setup()
{
  Serial.begin(115200);
  GPS.begin();


  
}

void loop(){
  uint32_t starttime = millis();
  while( (millis()-starttime) < 1000 )
  {
    while (GPS.available() > 0)
    {
      GPS.encode(GPS.read());
    }
  }

  for (int i = 0; i < 200; i++)
  {
    Serial.println();
  }
  displayInfo();

  if (millis() > 5000 && GPS.charsProcessed() < 10)
  {
    Serial.println("No GPS detected: check wiring.");
    while(true);
  }


}


