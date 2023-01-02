

    /**********************************************************************\
   /          rRocket: An Arduino powered rocketry recovery system          \
  /            Federal University of Technology - Parana - Brazil            \
  \              by Guilherme Bertoldo and Jonas Joacir Radtke               /
   \                       updated December 19, 2022                        /
    \**********************************************************************/


#include "Actuator.h"
#include "Arduino.h"
#include "Parameters.h"

bool Actuator::begin()
{

  pinMode(Parameters::pinDrogueChute, OUTPUT);

  pinMode(Parameters::pinParachute, OUTPUT);

  reload();

  return true;

}

void Actuator::reload()
{
  digitalWrite(Parameters::pinDrogueChute, LOW);

  digitalWrite(Parameters::pinParachute, LOW);

  lastTimeActivated = 0;

  deployCounter = 0;
}

bool Actuator::deployParachute(bool stopCondition)
{
  return deploy(Parameters::pinParachute, stopCondition);
};

bool Actuator::deployDrogueChute(bool stopCondition)
{
  return deploy(Parameters::pinDrogueChute, stopCondition);
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
    if ( currentTime > ( lastTimeActivated + Parameters::actuatorDischargeTime + Parameters::capacitorRechargeTime ) )
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
    else if ( currentTime > ( lastTimeActivated + Parameters::actuatorDischargeTime ) )
    {
      digitalWrite(pin, LOW);
    }
  }
  return false;
}
