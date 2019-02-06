#define FASTLED_ESP8266_RAW_PIN_ORDER

#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <FastLED.h>
#include <FastLED_NeoMatrix.h>

#define PIN 2
#define mw 15
#define mh 6
#define NUMMATRIX (mw * mh)

const int GMT_time_offset = 7200;
const char *ssid = "ESP8266";       //  your network SSID (name)
const char *password = "123456789"; // your network password

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, GMT_time_offset);

CRGB matrixleds[NUMMATRIX];

FastLED_NeoMatrix *matrix = new FastLED_NeoMatrix(matrixleds, mw, mh, 1, 1,
                                                  NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG);

const uint16_t colors[] = {
    matrix->Color(255, 0, 0),
    matrix->Color(0, 255, 0),
    matrix->Color(0, 0, 255)};

int x = mw;
int pass = 0;

void setup()
{
  WiFiManager wifiManager;
  wifiManager.autoConnect(ssid, password);

  timeClient.forceUpdate();

  FastLED.addLeds<WS2812B, PIN>(matrixleds, NUMMATRIX);
  matrix->begin();
  matrix->setTextWrap(false);
  matrix->setBrightness(255);
  matrix->setTextColor(colors[0]);

  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
}

void loop()
{
  timeClient.update();

  matrix->fillScreen(0);
  matrix->setCursor(x, 0);
  matrix->print(timeClient.getFormattedTime());

  if (--x < -54)
  {
    x = matrix->width();
    if (++pass >= 3)
      pass = 0;
    matrix->setTextColor(colors[pass]);
  }

  matrix->show();

  delay(100);
}