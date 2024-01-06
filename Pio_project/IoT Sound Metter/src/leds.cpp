// Copyright (c) 2023 Estylos, caneuze
// This code is licensed under MIT license (see LICENSE file for details)

#include <Arduino.h>

#include "leds.h"

const uint8_t LEDS_PINS[6] = { 12, 13, 5, 23, 19, 18 };

// Initialize the leds
void initLeds(void) {
    for (uint8_t i = 0; i < 6; i++) {
        pinMode(LEDS_PINS[i], OUTPUT);
    }
}

// Illuminate the leds according to the sound level
void reactToDb(uint8_t db) {

    uint8_t ledsOn = 0;

    if(db >= 40 && db < 60) {
        ledsOn = 1;
    } else if(db >= 60 && db < 70) {
        ledsOn = 2;
    } else if(db >= 70 && db < 80) {
        ledsOn = 3;
    } else if(db >= 80 && db < 90) {
        ledsOn = 4;
    } else if(db >= 90 && db < 100) {
        ledsOn = 5;
    } else if(db >= 100) {
        ledsOn = 6;
    }

    for(uint8_t led = 0; led < 6; led++) {
        if(led < ledsOn) {
            digitalWrite(LEDS_PINS[led], HIGH);
        } else {
            digitalWrite(LEDS_PINS[led], LOW);
        }
    }
}

