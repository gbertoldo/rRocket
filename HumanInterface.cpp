#include "HumanInterface.h"
#include "Parameters.h"


void HumanInterface::begin()
{
  Serial.begin(9600);

  pinMode(Parameters::pinLed, OUTPUT);
}


void HumanInterface::showInitMessage()
{
  // Clear Serial Monitor
  for (int j = 0; j < 100; ++j) Serial.print("\n");

  // Printing header
  Serial.println(" rRocket: An Arduino powered rocketry recovery system\n");
  Serial.println(" Federal University of Technology - Parana - Brazil\n");
  Serial.println(" Initializing Rocket Recovery System...\n");
}


void HumanInterface::blinkReadyToLaunch()
{
  unsigned long time;
  unsigned long timeStep = 750; // Time step to blink

  time = millis();

  if (time % (2 * timeStep) < timeStep)
  {
    digitalWrite(Parameters::pinLed, HIGH);
  }
  else
  {
    digitalWrite(Parameters::pinLed, LOW);
  }
}


void HumanInterface::blinkApogee(Memory& memory)
{

  int          thousands;
  int           hundreds;
  int             dozens;
  int              units;
  unsigned int   iHeight;
  float          fHeight;

  fHeight = memory.readApogee();

  iHeight   = (int)fHeight;
  thousands = iHeight / 1000;
  hundreds  = iHeight % 1000;
  hundreds  = hundreds / 100;
  dozens    = iHeight % 100;
  dozens    = dozens / 10;
  units     = iHeight % 10;

  if (fHeight >= 1000) blinkNumber(thousands);

  if (fHeight >= 100)  blinkNumber(hundreds);

  if (fHeight >= 10)   blinkNumber(dozens);

  blinkNumber(units);

}


void HumanInterface::showApogee(Memory& memory)
{
  Serial.println("");
  Serial.println("Reading apogee. Please wait...");
  Serial.print("Apogee (m) = ");
  Serial.println(memory.readApogee());
  Serial.println("");
}


void HumanInterface::showTrajectory(unsigned long int iTimeStep, Memory& memory)
{

  
  float fTimeStep;

  fTimeStep = (float)iTimeStep;
  fTimeStep = fTimeStep / 1000;

  memory.restartPosition();

  Serial.println("Time (s); Altitude (m)");

  int i = 0;
  while ( memory.hasNext() )
  {
    Serial.print(i * fTimeStep);
    Serial.print("    ");
    Serial.println(memory.readAltitude());
    ++i;
  }

  memory.restartPosition();
}


void HumanInterface::blinkNumber(int n)
{

  int i;

  if (n == 0)
  {
    digitalWrite(Parameters::pinLed, LOW);
    delay(300);
    digitalWrite(Parameters::pinLed, HIGH);
    delay(1000);
    digitalWrite(Parameters::pinLed, LOW);
  }

  for (i = 1; i <= n; i++)
  {
    digitalWrite(Parameters::pinLed, LOW);
    delay(300);
    digitalWrite(Parameters::pinLed, HIGH);
    delay(300);
    digitalWrite(Parameters::pinLed, LOW);
  }

  delay(500);

};