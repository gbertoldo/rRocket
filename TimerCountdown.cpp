#include "TimerCountdown.h"
#include "Arduino.h"

void TimerCountdown::begin(unsigned long int targetTime)
{
  this->targetTime = targetTime;

  initialTime = millis();

}


bool TimerCountdown::isRinging()
{
  if ( (millis() - initialTime) >= targetTime )
  {
    return true;
  }
  else
  {
    return false;
  }
}


void TimerCountdown::reset()
{
  initialTime = millis();
}


void TimerCountdown::rearm()
{
  if ( isRinging() )
  {
    unsigned long int time = millis();

    initialTime = time - (time - initialTime) % targetTime;
  }
}
