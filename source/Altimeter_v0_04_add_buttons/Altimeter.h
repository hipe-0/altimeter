#include <Adafruit_BMP085.h>
#include <EEPROM.h>

#define EEPROM_ADRESS 0
#define EEPROM_SIZE 16
#define MBAR_PER_METER_ALTITUDE 8.0
#define START_PRESSURE_AT_SEA_LEVEL 100830


/*************************************************** 
  This is an example for the BMP085 Barometric Pressure & Temp Sensor

  Designed specifically to work with the Adafruit BMP085 Breakout 
  ----> https://www.adafruit.com/products/391

  These pressure and temperature sensors use I2C to communicate, 2 pins
  are required to interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

// Connect VCC of the BMP085 sensor to 3.3V (NOT 5.0V!)
// Connect GND to Ground
// Connect SCL to i2c clock - on '168/'328 Arduino Uno/Duemilanove/etc thats Analog 5
// Connect SDA to i2c data - on '168/'328 Arduino Uno/Duemilanove/etc thats Analog 4
// EOC is not used, it signifies an end of conversion
// XCLR is a reset pin, also not used here

class Altimeter {

  Adafruit_BMP085 bmp_;
  float temperature_;
  float pressure_;
  float pressureCalculatedSeaLevel_;
  float altitude_;
  float previousAltitude_;
  char* weather_;
  int pressureReference_ = START_PRESSURE_AT_SEA_LEVEL;

public:
  void init() {

    if (!bmp_.begin()) {
      Serial.println("Could not find a valid BMP085 sensor, check wiring!");
      while (1) {}
    }

    EEPROM.begin(EEPROM_SIZE);
    int pressure;
    EEPROM.get(EEPROM_ADRESS, pressure);
    DEBUGVAL("pressure read from ESP Flash EEPROM", pressure)

    if (pressure > 90000 && pressure < 110000)  // within plausible boundaries?
      pressureReference_ = pressure;

    DEBUGVAL("pressureReference_", pressureReference_)

    previousAltitude_ = bmp_.readAltitude(pressureReference_) ;
  }

#define FILTER_NEW_READING 0.1

  void read() {
    temperature_ = bmp_.readTemperature();
    pressure_ = bmp_.readPressure() / 100.0;

    altitude_ = bmp_.readAltitude(pressureReference_)  * FILTER_NEW_READING + previousAltitude_ * (1.0 - FILTER_NEW_READING);;
    previousAltitude_ = altitude_;
    pressureCalculatedSeaLevel_ = pressure_ + altitude_ / MBAR_PER_METER_ALTITUDE;

    if (pressureCalculatedSeaLevel_ > 1100) weather_ = "bug P+";
    else if (pressureCalculatedSeaLevel_ > 1040) weather_ = "Canicule";
    else if (pressureCalculatedSeaLevel_ > 1025) weather_ = "Ensoleille";
    else if (pressureCalculatedSeaLevel_ > 1005) weather_ = "Nuageux";
    else if (pressureCalculatedSeaLevel_ > 985) weather_ = "Pluvieux";
    else if (pressureCalculatedSeaLevel_ > 900) weather_ = "Orageux";
    else weather_ = "bug P-";
  }

  float getTemperature() {
    return temperature_;
  }

  float getPressureMBar() {
    return pressure_;
  }

  float getPressureAtSeaLevelMBar() {
    return pressureCalculatedSeaLevel_;
  }

  float getAltitude() {
    return altitude_;
  }

  char* getWeatherText() {
    return weather_;
  }

  void correctPressureReferenceAtSeaLevel(int deltaP) {
    pressureReference_ += deltaP;
    EEPROM.put(EEPROM_ADRESS, pressureReference_);
    bool isOk = EEPROM.commit();

    DEBUGVAL("EEPROM Write", isOk)
    DEBUGVAL("pressureReference_", pressureReference_)
  }
};