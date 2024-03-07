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
};

#endif // BAROMETER_H
