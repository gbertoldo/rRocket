  /**************************************************************************\
 /            rRocket: An Arduino powered rocketry recovery system            \
/              Federal University of Technology - Parana - Brazil              \
\                by Guilherme Bertoldo and Jonas Joacir Radtke                 /
 \                         updated September 13, 2019                         /
  \**************************************************************************/
#ifndef RECOVERYSYSTEM_H
#define RECOVERYSYSTEM_H

#include "Arduino.h"
#include <Wire.h>
#include "Adafruit_BMP280.h"

/*

  RecoverySystem class manages data from sensors and 
  
  triggers the recovery system.

*/

class RecoverySystem
{
  public:

    //  Recovery initializer
    bool begin();

    // Reads sensors and execute recovery algorithm
    void run();

  private:

    // Search for BMP address
    byte getBMPAddress();

  private:

// ???    Barometer       barometer; // Barometer manager

    static constexpr float deltaHmin    {10}; // Minimum height variation (m) to activate recovery system
    int                            n     {4}; // Size of height vector
    float                  height[4];         // Initial heights
    float                   baseline     {0}; // height of launch ramp
    byte                  bmpAddress;         // BMP I2C address
    bool            searchBMPAddress  {true}; // If BMP address is known, fill it in the previous line and mark this variable as false

};

#endif // RECOVERYSYSTEM_H
