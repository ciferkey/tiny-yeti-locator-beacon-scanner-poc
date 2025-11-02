#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
#include <sys/time.h>

#define GPS_RX       1 
#define GPS_TX       3 

File gps_file;
EspSoftwareSerial::UART gpsSerial;
TinyGPSPlus gps;
extern Adafruit_ILI9341 tft;

bool isGpsReady() {
  return gpsSerial.available() > 0;
}

void setupGps() {
  gpsSerial.begin(9600, EspSoftwareSerial::SWSERIAL_8N1, GPS_RX, GPS_TX);

  while (!isGpsReady()){
    tft.setCursor(0, 0);
    tft.println(F("Waiting for GPS"));
    delay(1000);
  }
  tft.println(F("GPS Found."));
  updateClock();
}

bool isNewLocationAvailable() {
  return gps.encode(gpsSerial.read());
}

void displayLocation()
{
  if (gps.location.isValid())
  {
    tft.fillRect(0, 0, 320, 100, ILI9341_BLACK);
    tft.setCursor(0, 0);
    tft.println(F("Location: "));
    tft.print(gps.location.lat(), 6);
    tft.print(F(" , "));
    tft.print(gps.location.lng(), 6);
  }
  else
  {
    // Invalid GPS Location
    tft.setCursor(0, 0);
    tft.print(F("Acquiring Satellites..."));
  }
}


int gpsTime() {
    time_t t_of_day; 
    struct tm t;   

    t.tm_year = gps.date.year()-1900;
    t.tm_mon = gps.date.month()-1;           // Month, 0 - jan
    t.tm_mday = gps.date.day();          // Day of the month
    t.tm_hour = gps.time.hour();
    t.tm_min =  gps.time.minute();
    t.tm_sec = gps.time.second();
    t_of_day = mktime(&t);
  
    return t_of_day;
}

void updateClock() {
  timeval currentTime = {gpsTime(), 0};

  const timeval *tv = &currentTime;
  timezone utc = {0,0};
  const timezone *tz = &utc;
  settimeofday(tv, tz);

  tft.setCursor(0, 0);
  tft.println(F("Clock updated."));
}

void writeObservationToSD() {
  gps_file.print(millis());
  gps_file.print(",");
  gps_file.print(gps.location.lat(), 9);
  gps_file.print(",");
  gps_file.print(gps.location.lng(), 9);
  gps_file.print(",");
  gps_file.print(gps.location.age());
  gps_file.println();
  gps_file.flush();
}