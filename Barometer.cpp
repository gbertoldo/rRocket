#include "Arduino.h"
#include "Barometer.h"
#include "Adafruit_BMP280.h"

bool Barometer::begin()
{

  if ( searchBarometerAddress ) barometerAddress = getBarometerAddress();

  while (!barometer.begin(barometerAddress)) {}

  baseline = barometer.readAltitude(1013.25);

}


float Barometer::getBaseHeight()
{

  

}


bool Barometer::setBaseHeight(float)
{

  int i;

}


float Barometer::getAltitude()
{


}


bool Barometer::setApogee(float)
{

  int i;

}


float Barometer::getApogee()
{
  
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
