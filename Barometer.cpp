

    /**********************************************************************\
   /          rRocket: An Arduino powered rocketry recovery system          \
  /            Federal University of Technology - Parana - Brazil            \
  \              by Guilherme Bertoldo and Jonas Joacir Radtke               /
   \                       updated October 18, 2019                         /
    \**********************************************************************/


#include "Barometer.h"

bool Barometer::begin()
{

  Wire.begin();

  if ( searchBarometerAddress )
  {

    // Searching for barometer address
    if ( ! getBarometerAddress(barometerAddress) ) return false;

  }

  // Initializing BMP
  if (!barometer.begin(barometerAddress)) return false;

  baseline = barometer.readAltitude(1013.25);

  return true;

}


float Barometer::getAltitude()
{

  float altitude = barometer.readAltitude() - baseline;

  if ( altitude > apogee ) apogee = altitude;

  return altitude;

}


float Barometer::getApogee()
{

  return apogee;

}


bool Barometer::getBarometerAddress(byte& address)
{

  byte err;
  int  counter {0};

  for (byte addr = 1; addr < 127; addr++ )
  {

    Wire.beginTransmission(addr);

    err = Wire.endTransmission();

    if (err == 0)
    {
      counter++;

      address = addr;
    }

  }

  if ( counter != 1 ) return false;

  return true;

};
