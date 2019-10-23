

    /**********************************************************************\
   /          rRocket: An Arduino powered rocketry recovery system          \
  /            Federal University of Technology - Parana - Brazil            \
  \              by Guilherme Bertoldo and Jonas Joacir Radtke               /
   \                       updated October 18, 2019                         /
    \**********************************************************************/


#include "Actuator.h"
#include "Arduino.h"
#include "Parameters.h"

bool Actuator::begin()
{

  pinMode(Parameters::pinDrogueChute, OUTPUT);

  digitalWrite(Parameters::pinDrogueChute, LOW);

  pinMode(Parameters::pinParachute, OUTPUT);

  digitalWrite(Parameters::pinParachute, LOW);

  return true;

}


void Actuator::deployParachute()
{

  if (timeParachuteWasActivated == 0) timeParachuteWasActivated = millis();
  
  if ( abs(millis() - timeParachuteWasActivated) < timeParachuteActive)
  {
    
    digitalWrite(Parameters::pinParachute, HIGH);
  
  }
  else
  {
  
    digitalWrite(Parameters::pinDrogueChute, LOW);
    digitalWrite(Parameters::pinParachute, LOW);
  
  }

}


void Actuator::deployDrogueChute()
{

  if (timeDrogueChuteWasActivated == 0) timeDrogueChuteWasActivated = millis();
  
  if ( abs(millis() - timeDrogueChuteWasActivated) < timeDrogueChuteActive)
  {
    
    digitalWrite(Parameters::pinDrogueChute, HIGH);
  
  }
  else
  {
  
    digitalWrite(Parameters::pinDrogueChute, LOW);

  }

};
