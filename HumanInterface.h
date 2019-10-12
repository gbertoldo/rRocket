#ifndef HUMANINTERFACE_H
#define HUMANINTERFACE_H

#include <Arduino.h>
#include "Memory.h"

/*

   HumanInterface provides communication between humans and recovery system

*/

class HumanInterface
{

  public:

    // Initializer
    void begin();

    // Shows initialization messages
    void showInitialization();

    // Shows ready to launch messages
    void blinkReadyToLaunch();

    // Blinks apogee
    void blinkApogee(Memory& memory);

    // Shows apogee
    void showApogee(Memory& memory);

    // Shows trajectory
    void showTrajectory(unsigned long int iTimeStep, Memory& memory);

    // Prints on screen
    template <typename T>
    void print(T msg)
    {
      Serial.print(msg);
    }

    // Prints on screen (ends with a new line)
    template <typename T>
    void println(T msg)
    {
      Serial.println(msg);
    }

    // Prints on screen (ends with a new line)
    template <typename T, typename Q>
    void println(T msg, Q par)
    {
      Serial.println(msg, par);
    }
  private:
    // Blinks an altitude
    bool blinkAltitude(int n);
};

#endif // HUMANINTERFACE_H
