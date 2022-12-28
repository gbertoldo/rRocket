

    /**********************************************************************\
   /          rRocket: An Arduino powered rocketry recovery system          \
  /            Federal University of Technology - Parana - Brazil            \
  \              by Guilherme Bertoldo and Jonas Joacir Radtke               /
   \                       updated September 12, 2022                       /
    \**********************************************************************/


#include "Barometer.h"
#include "Parameters.h"

#ifdef DEBUGMODE
#include "TrajectoryModels.h"
TrajectoryModel1 model;
#endif // DEBUGMODE

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

  apogee = -500.0;

  return true;

}


float Barometer::getAltitude()
{

  #ifdef DEBUGMODE
  float altitude = model.y(1E-3*millis()+STARTBAROMETERTIME);
  #else
  float altitude = barometer.readAltitude() - baseline;
  #endif
  if ( altitude > apogee ) apogee = altitude;

  return altitude;

}


float Barometer::getApogee()
{

  return apogee;

}


void Barometer::setBaseline(float baseline)
{
   
   this->baseline = this->baseline + baseline;
   
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
