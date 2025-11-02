#include <SD.h>

#define SD_MISO 19
#define SD_MOSI 23
#define SD_SCK 18
#define SD_CS 5

SPIClass spiSD(VSPI);

void setupSd() {
  spiSD.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);

  if (SD.begin(SD_CS, spiSD)) {
    tft.println(F("Initialized SD card."));
    delay(1000);
  }
  else {
    tft.println(F("Failed to initialize SD card."));
    delay(1000);
    return;
  } 

  gps_file = SD.open("/gps.csv", FILE_APPEND);
  ble_file = SD.open("/ble.csv", FILE_APPEND);
  wifi_file = SD.open("/wifi.csv", FILE_APPEND);

  tft.print("GPS file: ");
  tft.println(gps_file ? "OK" : "FAIL");
  tft.print("BLE file: ");
  tft.println(ble_file ? "OK" : "FAIL");
  tft.print("WiFi file: ");
  tft.println(wifi_file ? "OK" : "FAIL");
  delay(2000);
}

