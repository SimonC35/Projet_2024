#include "main.h"

Air530ZClass GPS;
uint8_t fullArray[PAYLOAD_SIZE] = {0};

double previousLatitude = 0.0;
double previousLongitude = 0.0;

void VextON() {
    pinMode(Vext, OUTPUT);
    digitalWrite(Vext, LOW);
}

void VextOFF() {
    pinMode(Vext, OUTPUT);
    digitalWrite(Vext, HIGH);
}

void onAutoGPS() {
    Serial.println("DEBUG: AutoGPS timer triggered. Updating mcu_status to STATUS_UPDATE_GPS.");
    TimerStop(&autoGPS);
    mcu_status = STATUS_UPDATE_GPS;
}

void readSendData() {
    readGPSStoreAsBytes(fullArray);
    sendData(fullArray, PAYLOAD_SIZE);
}

void lowPowerHandler() {
    Serial.println("DEBUG: Entering low power mode...");
    delay(GPS_SLEEPTIME);
    mcu_status = STATUS_UPDATE_GPS;
    Serial.println("DEBUG: Exiting low power mode.");
}

void gpsUpdate(uint32_t timeout, uint32_t continuetime) {
    VextON();
    delay(10);
    GPS.begin();

    starttime = millis();
    while ((millis() - starttime) < timeout) {
        while (GPS.available()) GPS.encode(GPS.read());
        if (GPS.location.age() < 1000) break;
    }

    if (GPS.location.age() < 1000) {
        starttime = millis();
        while ((millis() - starttime) < continuetime) {
            while (GPS.available()) GPS.encode(GPS.read());
        }
    }

    GPS.end();
    VextOFF();

    TimerSetValue(&autoGPS, GPS_SLEEPTIME);
    TimerStart(&autoGPS);
}



void setup() {
    Serial.begin(115200);
    Serial.println("DEBUG: Setup started.");

    gpsUpdate(GPS_INIT_TIMEOUT, GPS_CONTINUE_TIMEOUT);

    TimerInit(&autoGPS, onAutoGPS);
    TimerSetValue(&autoGPS, GPS_SLEEPTIME);
    TimerStart(&autoGPS);

    mcu_status = STATUS_LPM;

    if (GPS.location.isValid()) {
        previousLatitude = GPS.location.lat();
        previousLongitude = GPS.location.lng();
    }
}

void loop() {
        switch (mcu_status) {
        case STATUS_UPDATE_GPS:
            if (GPS.location.isValid()) {
                double distance = GPS.distanceBetween(previousLatitude, previousLongitude, GPS.location.lat(), GPS.location.lng());
                if (DISTANCE_THRESHOLD == 0.0 || distance > DISTANCE_THRESHOLD) {
                    readSendData();
                    previousLatitude = GPS.location.lat();
                    previousLongitude = GPS.location.lng();
                }
            }
            TimerStop(&autoGPS);
            TimerSetValue(&autoGPS, LPM_SLEEP_TIME);
            TimerStart(&autoGPS);
            mcu_status = STATUS_LPM;
            break;

        case STATUS_LPM:
            lowPowerHandler();
            break;

        default:
            mcu_status = STATUS_LPM;
            break;
    }
    if (mcu_status == STATUS_UPDATE_GPS) {
        if (GPS.location.isValid()) {
            Serial.printf("DEBUG: Previous Lat: %.6f, Lng: %.6f\n", previousLatitude, previousLongitude);
            Serial.printf("DEBUG: Current Lat: %.6f, Lng: %.6f\n", GPS.location.lat(), GPS.location.lng());

            double distance = GPS.distanceBetween(previousLatitude, previousLongitude, GPS.location.lat(), GPS.location.lng());
            Serial.printf("DEBUG: Distance = %.2f m\n", distance);

            if (DISTANCE_THRESHOLD == 0.0 || distance > DISTANCE_THRESHOLD) {
                Serial.println("DEBUG: Movement detected. Sending data...");
                readSendData();
                previousLatitude = GPS.location.lat();
                previousLongitude = GPS.location.lng();
            } else {
                Serial.println("DEBUG: No significant movement.");
            }
        } else {
            Serial.println("DEBUG: Invalid GPS. Skipping send.");
        }

        TimerStop(&autoGPS);
        TimerSetValue(&autoGPS, LPM_SLEEP_TIME);
        TimerStart(&autoGPS);
        mcu_status = STATUS_LPM;
    } else if (mcu_status == STATUS_LPM) {
        lowPowerHandler();
    }
}
