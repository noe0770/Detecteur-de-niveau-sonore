// Copyright (c) 2023 Estylos, caneuze
// This code is licensed under MIT license (see LICENSE file for details)

#ifndef __LEDSs_H__
#define __LEDSs_H__

#include <stdint.h>

// Initialize the leds
extern void initLeds(void);

// Illuminate the leds according to the sound level
extern void reactToDb(uint8_t);

#endif
