  /**************************************************************************\
 /            rRocket: An Arduino powered rocketry recovery system            \
/              Federal University of Technology - Parana - Brazil              \
\                by Guilherme Bertoldo and Jonas Joacir Radtke                 /
 \                         updated September 13, 2019                         /
  \**************************************************************************/
#include "RecoverySystem.h"
#include "Parameters.h"


bool RecoverySystem::begin()
{

  Wire.begin();

  // Clear Serial Monitor
  for (int j = 0; j < 100; ++j) Serial.print("\n");

  // Printing header
  Serial.println(" rRocket: An Arduino powered rocketry recovery system\n");
  Serial.println(" Federal University of Technology - Parana - Brazil\n");
  Serial.println(" Initializing Rocket Recovery System...\n");

  for (int i = 0; i < n; i++)
  {
    height[i] = 0;
  }
};


void RecoverySystem::run()
{

  //if (buttonState == ButtonState::pressed_and_released)
  {

    Serial.println("\nRecovering data. Please wait...");
   // screen.showApogee();
   // screen.showTrajectory(timeStep, memory);
    Serial.println("\nEnd of data Recovery System.\n");

  };

 // if (memory.getState() == MemoryState::empty) // scanning flight
  {

   // screen.blink();

 //   currentTime = millis();

//    if (currentTime / timeStep > currentStep)
    {

//      currentStep = currentStep + 1;

      for (int i = 0; i < n-1; i++)
      {
        height[i] = height[i + 1];
      }

      //height[n-1] = bmp.readAltitude(1013.25) - baseline;

      Serial.print(height[n-1], 1);
      Serial.println(" m");

    }

    if (height[n-1] - height[0] > deltaHmin) // flying
    {

      Serial.println("Recording flight...");

 //     for (int i = 0; i < n; i++) memory.writeAltitude(height[i]);

 //     recordFlight(baseline);

    }

  }

};


byte RecoverySystem::getBMPAddress()
{

  byte address;
  byte err;
  int  counter {0};

  for (byte addr = 1; addr < 127; addr++ )
  {

    Wire.beginTransmission(addr);

    err = Wire.endTransmission();

    if (err == 0)
    {
      counter++;

      address = addr;
    }
  }

  if ( counter != 1 )
  {
    Serial.println("BMP I2C address not identified. Fatal error! Stopping... \n");
    while (true);
  }

  return address;

};
/*
void RecoverySystem::recordFlight(float baseHeight)
{

  float currentHeight;

  if (memory.getState() == MemoryState::full) Serial.println("Full memory!");

  while (memory.getState() != MemoryState::full)
  {

    currentTime = millis();

    if (currentTime / timeStep > currentStep)
    {

      currentStep = currentStep + 1;
      //currentHeight = bmp.readAltitude(1013.25) - baseHeight;
      memory.writeAltitude(currentHeight);

      Serial.print("Rec.: ");
      Serial.print(currentHeight, 1);
      Serial.println(" m");

    }

  }

  memory.setApogee();

  memory.setState(MemoryState::full);

};*/
