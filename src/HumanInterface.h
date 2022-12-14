

    /**********************************************************************\
   /          rRocket: An Arduino powered rocketry recovery system          \
  /            Federal University of Technology - Parana - Brazil            \
  \              by Guilherme Bertoldo and Jonas Joacir Radtke               /
   \                       updated September 12, 2022                       /
    \**********************************************************************/


#ifndef HUMANINTERFACE_H
#define HUMANINTERFACE_H

#include <Arduino.h>
#include "Memory.h"

/*

   HumanInterface provides communication between humans and the recovery system

*/

class HumanInterface
{

  public:

    // Initializer
    void begin();

    // Shows initialization messages
    void showInitMessage();

    // Shows ready to launch messages
    void showReadyToLaunchStatus();

    // Shows to user the flying status
    void showFlyingStatus();

    // Shows to user the recovered status
    void showRecoveredStatus();

    // Blinks apogee
    void blinkApogee(Memory& memory);

    // Shows the report of apogee, trajectory, errors, etc
    void showReport(unsigned long int iTimeStep, Memory& memory);

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

    // Blinks a number
    void blinkNumber(int n);

    // Last time of the message on the serial monitor
    unsigned long int timeOfTheLastMessage;

};

#endif // HUMANINTERFACE_H
