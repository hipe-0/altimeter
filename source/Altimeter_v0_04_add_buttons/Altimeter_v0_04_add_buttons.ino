// Altimeter
// by Hippolyte Mounier
// 2024-06-09

#define APP_NAME "Altimetre"
#define APP_AUTHOR "par Hippo"
#define APP_VERSION "v0.04"

#include "GUI.h"
GUI myScreen;

#include "toneESP32.h"
Tone myTone;

#include "Altimeter.h"
Altimeter myAltimeter;

#include "ezButton.h"
ezButton buttonMinus(15);
ezButton buttonPlus(16);


void setup() {
  Serial.begin(115200);
  Serial.println(APP_NAME);

  myScreen.init();
  myTone.init();

  myTone.playBeep(0, 100);
  myTone.playBeep(0, 100);

  myAltimeter.init();

  myScreen.clean();
  myScreen.displayTitle(APP_NAME, APP_AUTHOR, APP_VERSION);

  Serial.println("--------End Setup---------");
}

void loop() {
  myAltimeter.read();

  float altitude = myAltimeter.getAltitude();
  float temperature = myAltimeter.getTemperature();
  float pressure = myAltimeter.getPressureMBar();
  float pressureAtSeaLevel = myAltimeter.getPressureAtSeaLevelMBar();
  char* weather = myAltimeter.getWeatherText();

  myScreen.displayAltitude(altitude);
  myScreen.displayPressure(pressure);
  myScreen.displayPressureAtSeaLevel(pressureAtSeaLevel);
  myScreen.displayTemperature(temperature);
  myScreen.displayWeather(weather);

  buttonPlus.loop();
  buttonMinus.loop();

  myScreen.displaySymbol(" ");
  if (buttonMinus.getState()==0) // pressed "-" button
  {
    DEBUGTXT("Minus")
    myAltimeter.correctPressureReferenceAtSeaLevel(-10);
    myScreen.displaySymbol("-");
  }
  if (buttonPlus.getState()==0) // pressed "+" button
  {
    DEBUGTXT("Plus")
    myAltimeter.correctPressureReferenceAtSeaLevel(+10);
    myScreen.displaySymbol("+");
  }

  delay(100);
}
