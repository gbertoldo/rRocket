#include "Arduino.h"
#include "Barometer.h"
#include "Adafruit_BMP280.h"

bool Barometer::begin()
{

  Wire.begin();

  if ( searchBarometerAddress ) barometerAddress = getBarometerAddress();

  while (!barometer.begin(barometerAddress)) {}

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


byte Barometer::getBarometerAddress()
{

  byte address;
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

  if ( counter != 1 )
  {
    Serial.println(" BMP I2C address not identified. Fatal error! Stopping... \n");
    while (true);
  }

  return address;

};
