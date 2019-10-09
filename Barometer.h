#ifndef BAROMETER_H
#define BAROMETER_H

#include "Adafruit_BMP280.h"

/*

   Get altitude of rocket

*/

class Barometer
{

  public:
    // Initializes
    bool begin();

    // Get base height
    float getBaseHeight();

    // Set base height
    bool setBaseHeight(float);

    // get current altitude
    float getAltitude();

    // set apogee
    bool setApogee(float);

    // get apogee
    float getApogee();

  private:

    Adafruit_BMP280 barometer;      // BMP280 sensor manager
    float baseline {0};             // height of launch ramp
    byte barometerAddress;                // BMP I2C address
    bool searchBarometerAddress {true};   // If BMP address is known, fill it in the previous line and mark this variable as false


    // Base height
    float baseHeight;

    // Apogee
    int apogee;

    // Search for barometer address
    byte getBarometerAddress();

};

#endif // BAROMETER_H
