#define FASTLED_ESP8266_RAW_PIN_ORDER

#include <Arduino.h>
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
#include "DHT.h"
#include "Adafruit_Sensor.h"
#define DHTPIN 13
#define DHTTYPE DHT22
#define PIN 2
#define mw 15
#define mh 7
#define NUMMATRIX (mw * mh)

const int GMT_time_offset = 7200;
const char *ssid = "ESP";          //  your network SSID (name)
const char *password = "12345679"; // your network password

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, GMT_time_offset);

CRGB matrixleds[NUMMATRIX];

FastLED_NeoMatrix *matrix = new FastLED_NeoMatrix(matrixleds, mw, mh, 1, 1,
                                                  NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG);

const uint16_t colors[] = {
    matrix->Color(255, 0, 0),
matrix->Color(155, 180, 0),
    matrix->Color(0, 125, 195),
matrix->Color(155, 0, 125),
    matrix->Color(0, 200, 155)
    };

int x = mw;
int pass = 0;
float h;
float t;
float f;
float hic;

DHT dht(DHTPIN, DHTTYPE);

void getHT()
{

  h = dht.readHumidity();
  t = dht.readTemperature();
  f = dht.readTemperature(false);

  if (isnan(h) || isnan(t) || isnan(f))
  {
    matrix->print(F("ERROR_DHT"));
    return;
  }
  hic = dht.computeHeatIndex(t, h, false);
}

void setup()
{
  
  WiFiManager wifiManager;
  wifiManager.autoConnect(ssid, password); 
  dht.begin();
  FastLED.addLeds<WS2812B, PIN>(matrixleds, NUMMATRIX);
  matrix->begin();
  matrix->setTextWrap(false);
   // while ( WiFi.status() != WL_CONNECTED )  
  delay(500);
  timeClient.begin();
  //timeClient.forceUpdate();
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
  FastLED.setBrightness(255);
  matrix->show();
}

void loop(){
  timeClient.update();
  EVERY_N_SECONDS(1){
  getHT();}
  matrix->fillScreen(0);
  matrix->setCursor(x, 0);
  String result = timeClient.getFormattedTime()+ " T=" + t+ "*C; H=" + h + "% ";
  matrix->print(result);
  matrix->show();
 
  int frames = result.length() * 6;
  if (--x < -frames){
    x = matrix->width();
    if (++pass >= 5)
    pass = 0;
    matrix->setTextColor(colors[pass]);
  }
  matrix->show();
  delay(50);
}