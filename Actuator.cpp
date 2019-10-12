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

  digitalWrite(Parameters::pinDrogueChute, HIGH);

}

void Actuator::deployDrogueChute()
{

  digitalWrite(Parameters::pinParachute, HIGH);

}
