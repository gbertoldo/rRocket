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

#include "Actuator.h"
#include "Arduino.h"
#include "ParametersStatic.h"

bool Actuator::begin()
{

  pinMode(ParametersStatic::pinDrogueChute, OUTPUT);

  pinMode(ParametersStatic::pinParachute, OUTPUT);

  reload();

  return true;

}

void Actuator::reload()
{
  digitalWrite(ParametersStatic::pinDrogueChute, LOW);

  digitalWrite(ParametersStatic::pinParachute, LOW);

  lastTimeActivated = 0;

  deployCounter = 0;
}

bool Actuator::deployParachute(bool stopCondition)
{
  return deploy(ParametersStatic::pinParachute, stopCondition);
};

bool Actuator::deployDrogueChute(bool stopCondition)
{
  return deploy(ParametersStatic::pinDrogueChute, stopCondition);
};

bool Actuator::deploy(uint8_t pin, bool stopCondition)
{
  /*
    If not deployed yet, starts the deployment cycle.
    The deployment cycle is composed by a period for 
    the actuator discharge (pin HIGH) and a period for
    the capacitor recharge time (pin LOW).


    Actuator discharge time
    ______________________
    |                     | Capacitor recharge time
  __|                     |_________________________
  */

  uint32_t currentTime = millis();
  
  // If not deployed yet, starts the deployment cycle
  if ( deployCounter == 0 )
  {
    digitalWrite(pin, HIGH);
    lastTimeActivated = currentTime;
    deployCounter++;
  }
  else
  {
    // If the deployment cycle has been elapsed, check the stop condition before starting another deployment cycle.
    if ( currentTime > ( lastTimeActivated + ParametersStatic::actuatorDischargeTime + ParametersStatic::capacitorRechargeTime ) )
    {
      // If the stop condition is true, finishes the deployment cycle
      if ( stopCondition )
      {
        digitalWrite(pin, LOW);
        return true;
      }
      // Otherwise, starts another deployment cycle
      else
      {
        digitalWrite(pin, HIGH);
        lastTimeActivated = currentTime;
        deployCounter++;
      }
    }
    // If the actuator discharge time has bee elapsed, turn off the pin
    else if ( currentTime > ( lastTimeActivated + ParametersStatic::actuatorDischargeTime ) )
    {
      digitalWrite(pin, LOW);
    }
  }
  return false;
}
