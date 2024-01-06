// Copyright (c) 2023 Estylos, caneuze
// This code is licensed under MIT license (see LICENSE file for details)

#include <Arduino.h>
#include <WiFiMulti.h>
#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>

#include "influx.h"

// WiFi AP SSID
#define WIFI_SSID "SSID"
// WiFi password
#define WIFI_PASSWORD "PASSWORD"

// InfluxDB credentials
#define INFLUXDB_URL "https://eu-central-1-1.aws.cloud2.influxdata.com"
#define INFLUXDB_TOKEN "TOKEN"
#define INFLUXDB_ORG "ORG"
#define INFLUXDB_BUCKET "Sound Metter"

// Time zone info
#define TZ_INFO "UTC1"

WiFiMulti wifiMulti;

// Declare InfluxDB client instance with preconfigured InfluxCloud certificate
InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);

// Declare Data point
Point sensor("measurements");

// Configure WiFi network
void configureWifi(void)
{
    WiFi.mode(WIFI_STA);
    wifiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);

    Serial.print("Connecting to wifi");
    while (wifiMulti.run() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(100);
    }
    Serial.println();
}

void wifiSleep(void)
{
    WiFi.mode(WIFI_OFF);
    delay(2);
    WiFi.setSleep(true);
    delay(1000);
}

// Configure InfluxDB
void configureInfluxDB(void)
{
    // Accurate time is necessary for certificate validation and writing in batches
    // We use the NTP servers in your area as provided by: https://www.pool.ntp.org/zone/
    // Syncing progress and the time will be printed to Serial.
    timeSync(TZ_INFO, "pool.ntp.org", "time.nis.gov");

    // Check server connection
    if (client.validateConnection())
    {
        Serial.print("Connected to InfluxDB: ");
        Serial.println(client.getServerUrl());
    }
    else
    {
        Serial.print("InfluxDB connection failed: ");
        Serial.println(client.getLastErrorMessage());
    }

    // Add tags to the data point
    sensor.addTag("device", "ESP32 Sound Metter");
    sensor.addTag("SSID", WiFi.SSID());
}

// Send a report to InfluxDB
void sendReport(uint16_t avgMicCustomADC, uint16_t avgMicCustomDB, uint16_t avgMicGrooveADC, uint16_t avgMicGrooveDB)
{
    // Clear fields for reusing the point. Tags will remain the same as set above.
    sensor.clearFields();

    // Store measured value into point
    sensor.addField("adc_custom", avgMicCustomADC);
    sensor.addField("db_custom", avgMicCustomDB);
    sensor.addField("adc_groove", avgMicGrooveADC);
    sensor.addField("db_groove", avgMicGrooveDB);

    // Print what are we exactly writing
    Serial.print("Writing: ");
    Serial.println(sensor.toLineProtocol());

    // Check WiFi connection and reconnect if needed
    if (wifiMulti.run() != WL_CONNECTED)
    {
        Serial.println("Wifi connection lost");
    }

    // Write point
    if (!client.writePoint(sensor))
    {
        Serial.print("InfluxDB write failed: ");
        Serial.println(client.getLastErrorMessage());
    }
}
