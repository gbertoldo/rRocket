/*
  The MIT License (MIT)

  Copyright (C) 2022 Guilherme Bertoldo and Jonas Joacir Radtke
  (UTFPR) Federal University of Technology - Parana

  Permission is hereby granted, free of charge, to any person obtaining a 
  copy of this software and associated documentation files (the “Software”), 
  to deal in the Software without restriction, including without limitation 
  the rights to use, copy, modify, merge, publish, distribute, sublicense, 
  and/or sell copies of the Software, and to permit persons to whom the Software 
  is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all 
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN 
  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#include "Barometer.h"
#include "ParametersStatic.h"

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

  float altitude = 0.0;
  
  altitude = barometer.readAltitude();

  altitude -= baseline;

  return altitude;

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
