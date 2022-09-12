

    /**********************************************************************\
   /          rRocket: An Arduino powered rocketry recovery system          \
  /            Federal University of Technology - Parana - Brazil            \
  \              by Guilherme Bertoldo and Jonas Joacir Radtke               /
   \                       updated September 12, 2022                       /
    \**********************************************************************/


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
    // Initializes barometer. Returns true if successful initialization.
    bool begin();

    // Get current altitude
    float getAltitude();

    // Get apogee
    float getApogee();
    
    // Set baseline
    void setBaseline(float baseline);

  private:

    // Search for barometer address
    bool getBarometerAddress(byte& address);


  private:

    Adafruit_BMP280   barometer;                       // BMP280 sensor manager
    float             baseline {0};                    // Altitude at launch ramp
    byte              barometerAddress;                // BMP I2C address
    bool              searchBarometerAddress {true};   // If BMP address is known, fill it in the previous line and mark this variable as false
    float             apogee{ -500};                   // Apogee (m)

};

#endif // BAROMETER_H
