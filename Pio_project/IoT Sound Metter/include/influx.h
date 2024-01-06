// Copyright (c) 2023 Estylos, caneuze
// This code is licensed under MIT license (see LICENSE file for details)

#ifndef __INFLUX_H__
#define __INFLUX_H__

#include <stdint.h>

// Configure WiFi network
extern void configureWifi(void);

// Put WiFi to sleep mode
extern void wifiSleep(void);

// Configure InfluxDB
extern void configureInfluxDB(void);

// Send a report to InfluxDB
extern void sendReport(uint16_t, uint16_t, uint16_t, uint16_t);

#endif
