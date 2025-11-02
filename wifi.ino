#include "WiFi.h"

File wifi_file;

void setupWifi() {
  // Enable Station Interface
  WiFi.STA.begin();
}

void scanWiFi() {
  // WiFi.scanNetworks will return the number of networks found.
  int n = WiFi.scanNetworks();
  long scanTime = millis();

  tft.fillRect(0, 100, 320, 100, ILI9341_BLACK);
  tft.setCursor(0, 100);
  tft.print("WiFi Devices found: ");
  tft.println(n);

  if (n != 0) {
    for (int i = 0; i < n; ++i) {
      wifi_file.print(scanTime);
      wifi_file.print(",");
      wifi_file.print(WiFi.SSID(i).c_str());
      wifi_file.print(",");
      wifi_file.print(WiFi.BSSIDstr(i).c_str());
      wifi_file.print(",");
      wifi_file.print(WiFi.RSSI(i));
      wifi_file.print(",");
      wifi_file.print(WiFi.channel(i));
      wifi_file.print(",");
      // Radio type
      // frequency
      // signal to noise ratio
      wifi_file.println("");
      wifi_file.flush();
    }

    WiFi.scanDelete();
  }
}

void reportWifi() {
    WiFi.setBandMode(WIFI_BAND_MODE_2G_ONLY);
    scanWiFi();
  }