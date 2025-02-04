#include "Arduino.h"
#include "GPS_Air530.h"
#include "GPS_Air530Z.h"

//if GPS module is Air530, use this
//Air530Class GPS;

//if GPS module is Air530Z, use this
Air530ZClass GPS;

void displayInfo()
{
/*  Serial.print("Date/Time: ");
  if (GPS.date.isValid())
  {
    Serial.printf("%d/%02d/%02d",GPS.date.year(),GPS.date.day(),GPS.date.month());
  }
  else
  {
    Serial.print("INVALID");
  }

  if (GPS.time.isValid())
  {
    Serial.printf(" %02d:%02d:%02d.%02d",GPS.time.hour(),GPS.time.minute(),GPS.time.second(),GPS.time.centisecond());
  }
  else
  {
    Serial.print(" INVALID");
  }*/
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

  Serial.println();
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

  uint8_t fullArray[8];
  fullArray[0] = latbytes[0];
  fullArray[1] = latbytes[1];
  fullArray[2] = latbytes[2];
  fullArray[3] = latbytes[3];

  fullArray[4] = lngbytes[0];
  fullArray[5] = lngbytes[1];
  fullArray[6] = lngbytes[2];
  fullArray[7] = lngbytes[3];

  char fArraystr[32];
  Serial.println();
  Serial.println();
  sprintf(fArraystr,"\t\t%0b %0b %0b %0b %0b %0b %0b %0b", fullArray[0], fullArray[1], fullArray[2], fullArray[3], fullArray[4], fullArray[5], fullArray[6], fullArray[7]);
  Serial.println(fArraystr);
  Serial.println();
  sprintf(fArraystr,"%02X %02X %02X %02X %02X %02X %02X %02X", fullArray[0], fullArray[1], fullArray[2], fullArray[3], fullArray[4], fullArray[5], fullArray[6], fullArray[7]);
  Serial.print(fArraystr);
  Serial.println();

  
  Serial.println();
  Serial.print("ALT: ");
  Serial.print(GPS.altitude.meters());
  float alt = (float) GPS.altitude.meters();
  int alt1 = (int) (lat * pow(10,2));
  Serial.println();
  Serial.print(alt1);
  Serial.println();
  uint8_t altbytes[4];

  altbytes[0] = (alt1 >> 24) & 0xFF;
  altbytes[1] = (alt1 >> 16) & 0xFF;
  altbytes[2] = (alt1 >> 8) & 0xFF;
  altbytes[3] = (alt1 >> 0) & 0xFF;

  Serial.println();

  char altstr[32];
  sprintf(altstr,"\t\t%0b %0b %0b %0b", altbytes[0], altbytes[1], altbytes[2], altbytes[3]);
  Serial.println(altstr);
  sprintf(lngstr, "%08b", alt1);
  Serial.println(altstr);
  sprintf(altstr,"%02X %02X %02X %02X", altbytes[0], altbytes[1], altbytes[2], altbytes[3]);
  Serial.print(altstr);
  
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

  Serial.println(F("A simple demonstration of Air530 module"));
  Serial.println();

  displayInfo();
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
  displayInfo();
  if (millis() > 5000 && GPS.charsProcessed() < 10)
  {
    Serial.println("No GPS detected: check wiring.");
    while(true);
  }
}

