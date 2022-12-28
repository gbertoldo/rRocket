

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

  timeParachuteWasActivated = 0;

  timeDrogueChuteWasActivated = 0;

  parachuteDeployCounter = 0;

  drogueDeployCounter = 0;
}

void Actuator::deployParachute()
{
  // If not deployed yet, deploy it
  if ( parachuteDeployCounter == 0 )
  {
    digitalWrite(Parameters::pinParachute, HIGH);
    timeParachuteWasActivated = millis();
    parachuteDeployCounter++;
  }
  else
  {
    if ( abs(millis() - timeParachuteWasActivated) > Parameters::timeParachuteActive )
    {
      digitalWrite(Parameters::pinParachute, LOW);
    }
  }
}


void Actuator::deployDrogueChute()
{
  // If not deployed yet, deploy it
  if ( drogueDeployCounter == 0 )
  {
    digitalWrite(Parameters::pinDrogueChute, HIGH);
    timeDrogueChuteWasActivated = millis();
    drogueDeployCounter++;
  }
  else
  {
    if ( abs(millis() - timeDrogueChuteWasActivated) > Parameters::timeDrogueChuteActive )
    {
      digitalWrite(Parameters::pinDrogueChute, LOW);
    }
  }
};
