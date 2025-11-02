#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <BLEEddystoneURL.h>
#include <BLEEddystoneTLM.h>
#include <BLEBeacon.h>

int scanDuration = 3;  //In seconds
BLEScan *pBLEScan;
File ble_file;

class ScannerAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
  bool isBeacon(BLEAdvertisedDevice advertisedDevice) {
    if (advertisedDevice.haveManufacturerData() == true) {
      return false;
    }

    String strManufacturerData = advertisedDevice.getManufacturerData();

    // Buffer to store manufacturer data (BLE max is 255 bytes)
    uint8_t cManufacturerData[255];
    size_t dataLength = strManufacturerData.length();

    if (dataLength > sizeof(cManufacturerData)) {
      return false;
    }

    memcpy(cManufacturerData, strManufacturerData.c_str(), dataLength);

    if (dataLength == 25 && cManufacturerData[0] == 0x4C && cManufacturerData[1] == 0x00) {
      // iBeacon
      return true;
    } else if (advertisedDevice.getFrameType() == BLE_EDDYSTONE_URL_FRAME) {
      return true;
    } else if (advertisedDevice.getFrameType() == BLE_EDDYSTONE_TLM_FRAME) {
      return true;
    }

    return false;
}

  void onResult(BLEAdvertisedDevice advertisedDevice) {
    long scanTime = millis();

    if (!isBeacon(advertisedDevice)) {
      return;
    }

    ble_file.print(scanTime);
    ble_file.print(",");
    ble_file.print(advertisedDevice.getAddress().toString());
    ble_file.print(",");
    if (advertisedDevice.haveName()) {
      ble_file.print(advertisedDevice.getName());
    }
    ble_file.print(",");
    ble_file.print(advertisedDevice.getRSSI());
    ble_file.print(",");
    ble_file.println("");
    ble_file.flush();
  }
};

void setupBle() {
  tft.println("Scanning BLE");

  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan();  //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new ScannerAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);  //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);  // less or equal setInterval value
}

void reportBle() {
  BLEScanResults *foundDevices = pBLEScan->start(scanDuration, false);
  tft.fillRect(0, 100, 320, 100, ILI9341_BLACK);
  tft.setCursor(0, 100);
  tft.print("BLE Devices found: ");
  tft.println(foundDevices->getCount());
  pBLEScan->clearResults();
}