

    /**********************************************************************\
   /          rRocket: An Arduino powered rocketry recovery system          \
  /            Federal University of Technology - Parana - Brazil            \
  \              by Guilherme Bertoldo and Jonas Joacir Radtke               /
   \                       updated September 12, 2022                       /
    \**********************************************************************/


#include "Barometer.h"
#include "Parameters.h"

#ifdef DEBUGMODE
#include "MessageParser.h"
MessageParser parser;
#endif

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

  #ifdef DEBUGMODE
  baseline = 0.0;
  #else
  baseline = barometer.readAltitude(1013.25);
  #endif
  
  apogee = -500.0;

  return true;

}


float Barometer::getAltitude()
{

  float altitude = 0.0;
  
  #ifdef DEBUGMODE
  // Asking the user (via serial) for the altitude of the current instant 
  unsigned long time = millis();
  Serial.print("<1,");
  Serial.print(time);
  Serial.println(">");

  while( true )
  {
    size_t sz = Serial.available();
    for (size_t i = 0; i < sz; i++) 
    {
      parser.append(Serial.read());
    }
    if ( parser.hasMessage() ) break;
  }
  altitude = parser.getEntryFloat(1);
  #else
  altitude = barometer.readAltitude();
  #endif

  altitude -= baseline;

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
