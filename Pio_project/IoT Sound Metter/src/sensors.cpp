// Copyright (c) 2023 Estylos, caneuze
// This code is licensed under MIT license (see LICENSE file for details)

#include <Arduino.h>

#include "sensors.h"

// Constants for sound sensors
#define NB_SAMPLES 2000
#define PIN_MIC_CUSTOM 36
#define PIN_MIC_GROOVE 39
// In order to know this value, put the custom mic in a quiet place and run the printADCLevels() function with this constant at 0
#define CUSTOM_AVG_VALUE 1887
#define CUSTOM_OFFSET 0

// Global variables for sound sensors
uint16_t tabSensorCustom[NB_SAMPLES];
long avgMicCus = 0;
uint16_t indexRel = 0;
long avgMicGroove = 0;

// Return the ADC level of the Groove mic
uint16_t mesureGrooveLevelADC(void)
{
    avgMicGroove = 0;

    for (uint16_t i = 0; i < NB_SAMPLES; i++)
        avgMicGroove += analogRead(PIN_MIC_GROOVE);

    avgMicGroove /= NB_SAMPLES;

    return avgMicGroove;
}

// Return the ADC level of the custom mic
uint16_t mesureCustomLevelADC(void)
{
    avgMicCus = 0;
    indexRel = 0;

    for (uint16_t i = 0; i < NB_SAMPLES; i++)
        tabSensorCustom[i] = analogRead(PIN_MIC_CUSTOM);

    /* 
     * Here we need to calculate the average of the values in the table that are greater than CUSTOM_AVG_VALUE
     * because there is a phenomenon of cancellation of the sound waves that we want to avoid 
     */
    for (uint16_t i = 0; i < NB_SAMPLES; i++)
    {
        if (tabSensorCustom[i] > CUSTOM_AVG_VALUE)
        {
            // Sum of the values greater than CUSTOM_AVG_VALUE
            avgMicCus += tabSensorCustom[i];
            indexRel++;
        }
    }
    avgMicCus /= indexRel;

    return avgMicCus;
}

// Convert the ADC level of the custom mic to dB
uint8_t convertCustomADCToDB(uint16_t adcValue) 
{
    adcValue -= CUSTOM_OFFSET;
    if(adcValue < 1965) 
    {
        return 0.7074663403 * (adcValue) - 1319.9;
    } 
    else 
    {
        return 0.01423956016 * (adcValue) + 48.85;
    }
}

// Convert the ADC level of the Groove mic to dB
uint8_t convertGrooveADCToDB(uint16_t adcValue) 
{
    return 0.05588667616 * adcValue + 53.72;
}

// Print the ADC levels of the Groove and Custom mics (for calibration)
void printADCLevels(uint8_t convertToDB)
{
    avgMicCus = 0;
    indexRel = 0;
    avgMicGroove = 0;

    for (uint16_t i = 0; i < NB_SAMPLES; i++)
    {
        tabSensorCustom[i] = analogRead(PIN_MIC_CUSTOM);
        avgMicGroove += analogRead(PIN_MIC_GROOVE);
    }

    for (uint16_t i = 0; i < NB_SAMPLES; i++)
    {
        if (tabSensorCustom[i] > CUSTOM_AVG_VALUE)
        {
            avgMicCus += tabSensorCustom[i];
            indexRel++;
        }
    }
    avgMicCus /= indexRel;

    avgMicGroove /= NB_SAMPLES;

    if(convertToDB) {
        avgMicCus = convertCustomADCToDB(avgMicCus);
        avgMicGroove = convertGrooveADCToDB(avgMicGroove);
    }

    Serial.print(avgMicCus);
    Serial.print(",");
    Serial.println(avgMicGroove);
}