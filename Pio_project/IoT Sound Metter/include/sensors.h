// Copyright (c) 2023 Estylos, caneuze
// This code is licensed under MIT license (see LICENSE file for details)

#ifndef __SENSORS_H__
#define __SENSORS_H__

#include <stdint.h>

// Return the ADC level of the Groove mic
extern uint16_t mesureGrooveLevelADC(void);

// Return the ADC level of the custom mic
extern uint16_t mesureCustomLevelADC(void);

// Convert the ADC level of the custom mic to dB
extern uint8_t convertCustomADCToDB(uint16_t);

// Convert the ADC level of the Groove mic to dB
extern uint8_t convertGrooveADCToDB(uint16_t);

// Print the ADC levels of the Groove and Custom mics (for calibration)
extern void printADCLevels(uint8_t);

#endif