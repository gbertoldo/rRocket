

    /**********************************************************************\
   /          rRocket: An Arduino powered rocketry recovery system          \
  /            Federal University of Technology - Parana - Brazil            \
  \              by Guilherme Bertoldo and Jonas Joacir Radtke               /
   \                       updated September 12, 2022                       /
    \**********************************************************************/


#include "HumanInterface.h"
#include "Parameters.h"
#include "ErrorTable.h"

void showParameters()
{
    Serial.println(Parameters::softwareVersion);
    Serial.print(F("Vl(m/s)="));
    Serial.println(Parameters::speedForLiftoffDetection);
    Serial.print(F("Va(m/s)="));
    Serial.println(Parameters::speedForApogeeDetection);
    Serial.print(F("Vf(m/s)="));
    Serial.println(Parameters::speedForFallDetection);
    Serial.print(F("Hp(m)="));
    Serial.println(Parameters::parachuteDeploymentAltitude);
    Serial.print(F("Dl(m)="));
    Serial.println(Parameters::displacementForLandingDetection);
    Serial.print(F("Td(ms)="));
    Serial.println(Parameters::actuatorDischargeTime);
    Serial.print(F("Tr(ms)="));
    Serial.println(Parameters::capacitorRechargeTime);
    Serial.print(F("Nd="));
    Serial.println(Parameters::maxNumberOfDeploymentAttempts);
    #ifdef DEBUGMODE
    Serial.println("DEBUG MODE!");
    #endif
}

void HumanInterface::begin()
{
  #ifdef DEBUGMODE
  Serial.begin(115200);
  #else
  Serial.begin(9600);
  #endif

  pinMode(Parameters::pinLed, OUTPUT);

  timeOfTheLastMessage = millis();

}


void HumanInterface::showInitMessage()
{

  // Clear Serial Monitor
  for (int j = 0; j < 100; ++j) Serial.println();

  // Printing header
  Serial.println(F(" Initializing...\n\n"));
  showParameters();
  Serial.println();
}


void HumanInterface::showReadyToLaunchStatus()
{

  unsigned long currentTime;
  unsigned long timeStep = 750; // Time step to blink

  currentTime = millis();

  if ( currentTime - timeOfTheLastMessage > 2000 )
  {

    Serial.println(F("R"));

    timeOfTheLastMessage = currentTime;
    
  }

  if (currentTime % (2 * timeStep) < timeStep)
  {

    digitalWrite(Parameters::pinLed, HIGH);

  }
  else
  {

    digitalWrite(Parameters::pinLed, LOW);

  }

  if (currentTime % (2 * timeStep) < timeStep / 8)
  {

    tone(Parameters::pinBuzzer, 600);

  }
  else
  {

    noTone(Parameters::pinBuzzer);

  }

}


void HumanInterface::showFlyingStatus()
{

  digitalWrite(Parameters::pinLed, HIGH);
  noTone(Parameters::pinBuzzer);

}


void HumanInterface::showRecoveredStatus()
{

  unsigned long int currentTime;

  currentTime = millis();
  
  if ( currentTime - timeOfTheLastMessage > 2000 )
  {

    Serial.println(F("\n Recovered!\n\n"));
    showParameters();
    Serial.println(F("\n - Short press button to show data"));
    Serial.println(F(" - Long press button to erase\n"));
    timeOfTheLastMessage = currentTime;
    
  }

  digitalWrite(Parameters::pinLed, LOW);

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


void HumanInterface::showReport(unsigned long int iTimeStep, Memory& memory)
{

  Serial.print(F("\nApogee (m) = "));
  Serial.println(memory.readApogee());
  Serial.print(F("\nError code = "));
  uint16_t errorLog = memory.readErrorLog();
  if ( errorLog > 0 ){
    for (uint8_t i = 0; i < 16; ++i)
    {
      /* 
        If the error is in the log, print it. See the error encode in the ErrorTable.h file.
      */
      if ( (errorLog & 2) == 2 )
      {
        Serial.print(i+1);
        Serial.print(F("; "));
      }
      errorLog = errorLog >> 1;
    }
  }
  else
  {
    Serial.print('0');
  }
  Serial.println();

  float fTimeStep = ((float)iTimeStep)/1000.0;
  
  Serial.println(F("Time (s); Altitude (m)"));

  for ( uint16_t i = 1; i <= memory.getNumberOfSlotsWritten(); ++i)
  {
    Serial.print((i-1) * fTimeStep);
    Serial.print(F("; "));
    Serial.println(memory.readAltitude(i));
  }
}


void HumanInterface::blinkNumber(int n)
{

  int i;

  if (n == 0)
  {
    digitalWrite(Parameters::pinLed, LOW);
    noTone(Parameters::pinBuzzer);
    delay(300);
    digitalWrite(Parameters::pinLed, HIGH);
    tone(Parameters::pinBuzzer, 600);
    delay(1000);
    digitalWrite(Parameters::pinLed, LOW);
    noTone(Parameters::pinBuzzer);
  }

  for (i = 1; i <= n; i++)
  {
    digitalWrite(Parameters::pinLed, LOW);
    noTone(Parameters::pinBuzzer);
    delay(300);
    digitalWrite(Parameters::pinLed, HIGH);
    tone(Parameters::pinBuzzer, 600);
    delay(300);
    digitalWrite(Parameters::pinLed, LOW);
    noTone(Parameters::pinBuzzer);
  }

  delay(500);

};
