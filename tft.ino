#define TFT_CS      15
#define TFT_DC       2
#define TFT_MOSI    13
#define TFT_CLK     14
#define TFT_RST     -1
#define TFT_MISO    12
#define TFT_BL      21

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

void setupTft() {
  pinMode(TFT_BL, OUTPUT); 
  digitalWrite(TFT_BL, HIGH); //TFT Backlight ON
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK); 
  tft.setTextSize(2);
  tft.println(F("Setup Complete")); 
  delay(1500);
  tft.fillScreen(ILI9341_BLACK);
}