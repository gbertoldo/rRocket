#include "Timer.h"
#include "Arduino.h"


unsigned long int Timer::elapsedTime()
{
  return millis() - initialTime;
}


void Timer::reset()
{
  initialTime = millis();
}
