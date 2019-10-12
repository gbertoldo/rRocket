/**************************************************************************\
  /            rRocket: An Arduino powered rocketry recovery system            \
  /              Federal University of Technology - Parana - Brazil              \
  \                by Guilherme Bertoldo and Jonas Joacir Radtke                 /
  \                         updated October 08, 2019                           /
  \**************************************************************************/
#ifndef BAROMETER_H
#define BAROMETER_H

#include "Adafruit_BMP280.h"
#include <Wire.h>

/*

   Get altitude of rocket

*/

class Barometer
{

  public:
    // Initializes
    bool begin();

    // Get current altitude
    float getAltitude();

    // Get apogee
    float getApogee();

  private:

    // Search for barometer address
    byte getBarometerAddress();


  private:

    Adafruit_BMP280   barometer;                       // BMP280 sensor manager
    float             baseline {0};                    // Height at launch ramp
    byte              barometerAddress;                // BMP I2C address
    bool              searchBarometerAddress {true};   // If BMP address is known, fill it in the previous line and mark this variable as false
    float             apogee{ -500};                   // Apogee

};

#endif // BAROMETER_H
