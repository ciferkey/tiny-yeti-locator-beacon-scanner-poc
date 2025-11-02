// Use Library Manager to Search for and Install:
//   Adafruit ILI9341 Library (with dependancies)
//   EspSoftwareSerial by Dirk Kaar
//   TinyGPSPlus-ESP32 by Mikal Hart

#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

#include <Arduino.h>

#include <SD.h>

extern File gps_file;
extern File ble_file;
extern File wifi_file;
extern Adafruit_ILI9341 tft;

void setup() {
  setupTft();
  setupSd();
  setupGps();
  setupBle();
  setupWifi();
}

void loop()
{
  while (isGpsReady()){
    if (isNewLocationAvailable()) {
      writeObservationToSD();
      displayLocation();
      reportBle();
      reportWifi();
    }
  }
}
