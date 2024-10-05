// GUI.h
// by Hippolyte Mounier
// 2024-05-20
//
// Goal: Manages the display of the Trainer device
// Platform: ESP32WROOM32
// Display: circular 240*240 pixel 4-wire SPI TFT CG9A010 driver
//
// pin wiring is as follows
//     GC9A010 -------- ESP32
//       RST ------------ NC
//       CS ------------- D4
//       DC ------------- D2
//       SDA ------------ D23 (green)
//       SCL ------------ D18 (yellow)
//       GND ------------ GND
//       VCC -------------3V3
//

#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_GC9A01A.h"

#define TFT_DC 2
#define TFT_CS 4

#define COLOR_BACKGROUND GC9A01A_WHITE
#define COLOR_CLEAN_VALUE GC9A01A_WHITE // GC9A01A_YELLOW // for debuging
#define COLOR_TEXT GC9A01A_BLUE
#define COLOR_HIGHLIGHT GC9A01A_ORANGE
#define COLOR_LOWLIGHT GC9A01A_LIGHTGREY

#include "iferror.h"
#include "graphics.h"

class GUI {
private:
  Adafruit_GC9A01A tft_;

public:
  GUI()
    : tft_(TFT_CS, TFT_DC) {
  }

  void init() {
    tft_.begin();
    tft_.setRotation(2);
    clean();
  }

  void clean() {
    tft_.fillScreen(COLOR_BACKGROUND);
  }

  void displayTitle(char* t1, char* t2, char* t3) {
    tft_.setCursor(80, 20);
    tft_.setTextColor(COLOR_TEXT);
    tft_.setTextSize(1);
    tft_.println(t1);

    tft_.setCursor(50, 30);
    tft_.setTextColor(COLOR_LOWLIGHT);
    tft_.setTextSize(1);
    tft_.println(t2);
    
    tft_.setCursor(150, 30);
    tft_.setTextColor(COLOR_LOWLIGHT);
    tft_.setTextSize(1);
    tft_.println(t3);

    tft_.drawBitmap(0, 80, bitmap_mountain, 92, 36, GC9A01A_BLUE);
  }

  void displayAltitude(float altitude) {
    tft_.setCursor(50, 70);
    tft_.setTextColor(COLOR_HIGHLIGHT);
    tft_.setTextSize(2);
    tft_.println("Altitude = ");

    tft_.fillRect(85, 90, 150, 30, COLOR_CLEAN_VALUE);
    
    if (altitude > 0 && altitude < 9000)
    {
      tft_.setTextSize(4);
      tft_.setCursor(90, 90);
      tft_.print(int(round(altitude)));
      tft_.println(" m");
    }
  }

  void displaySymbol(char* txt) {
    tft_.setTextColor(COLOR_HIGHLIGHT);
    tft_.setTextSize(3);
    tft_.fillRect(200, 70, 25, 25, COLOR_CLEAN_VALUE);
    tft_.setCursor(200, 70);
    tft_.print(txt);
  }

  void displayPressure(float pressure) {
    tft_.setCursor(10, 135);
    tft_.setTextColor(COLOR_LOWLIGHT);
    tft_.setTextSize(1);
    tft_.println("Pression locale");

    tft_.fillRect(30, 145, 80, 10, COLOR_CLEAN_VALUE);
    tft_.setCursor(40, 145);
    tft_.print(int(round(pressure)));
    tft_.println(" mbar");
  }

  void displayPressureAtSeaLevel(float pressure) {
    tft_.setCursor(20, 155);
    tft_.setTextColor(COLOR_LOWLIGHT);
    tft_.setTextSize(1);
    tft_.println("P au niveau de la mer");

    tft_.fillRect(40, 165, 80, 10, COLOR_CLEAN_VALUE);
    tft_.setCursor(40, 165);
    tft_.print(int(round(pressure)));
    tft_.println(" mbar");
  }

  void displayTemperature(float temp) {
    tft_.setCursor(140, 135);
    tft_.setTextColor(COLOR_LOWLIGHT);
    tft_.setTextSize(1);
    tft_.println("Temperature");

    tft_.fillRect(150, 145, 80, 10, COLOR_CLEAN_VALUE);
    tft_.setCursor(150, 145);
    tft_.print(int(round(temp)));
    tft_.println(" degrees C");
  }

  void displayWeather(char* text) {
    tft_.fillRect(60, 180, 150, 25, COLOR_CLEAN_VALUE);
    tft_.setCursor(60, 180);
    tft_.setTextColor(COLOR_HIGHLIGHT);
    tft_.setTextSize(3);
    tft_.println(text);
  }
};