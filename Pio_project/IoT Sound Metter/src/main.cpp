// Copyright (c) 2023 Estylos, caneuze
// This code is licensed under MIT license (see LICENSE file for details)

#include <Arduino.h>

#include "sensors.h"
#include "influx.h"
#include "leds.h"

#define SERIAL_MODE 0
#define IOT_MODE 1

uint8_t mode = SERIAL_MODE;

void setup()
{
    Serial.begin(115200);

    initLeds();

    configureWifi();
    configureInfluxDB();
    wifiSleep();
}

void loop()
{
    if (mode == SERIAL_MODE)
    {
        printADCLevels(1);
        reactToDb(convertGrooveADCToDB(mesureGrooveLevelADC()));
        delay(10);
    }
    else if (mode == IOT_MODE)
    {
        int avgMicCusADC = mesureCustomLevelADC();
        int avgMicGrooveADC = mesureGrooveLevelADC();
        sendReport(avgMicCusADC, convertCustomADCToDB(avgMicCusADC), avgMicGrooveADC, convertGrooveADCToDB(avgMicGrooveADC));
        wifiSleep();
    }

    if (Serial.available())
    {
        char c = Serial.read();
        if (c == 's')
        {
            Serial.println("Serial mode");
            mode = SERIAL_MODE;
        }
        else if (c == 'i')
        {
            Serial.println("IoT mode");
            mode = IOT_MODE;
        }
    }
}