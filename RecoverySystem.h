

    /**************************************************************************\
   /            rRocket: An Arduino powered rocketry recovery system            \
  /              Federal University of Technology - Parana - Brazil              \
  \                by Guilherme Bertoldo and Jonas Joacir Radtke                 /
   \                         updated October 12, 2019                           /
    \**************************************************************************/


#ifndef RECOVERYSYSTEM_H
#define RECOVERYSYSTEM_H

#include "Arduino.h"
#include "Barometer.h"
#include "Memory.h"
#include "Button.h"
#include "Actuator.h"
#include "Timer.h"
#include "HumanInterface.h"

/*

  RecoverySystem class manages data from sensors and

  triggers the recovery system.

*/

/*

                                      Recovery System States

*/

enum class RecoverySystemState {readyToLaunch, flying, drogueChuteActive, parachuteActive, recovered};

/*

                                             apogee
        --------------------------------------____----------------------------------------
         |                                 ***    ****                                  |
         |                             ****            ****                             | flying
         |                          ***                    ***                          |
         |                       ***                          *** ------------------------
         |                      **                              **                      |
         |                    **                                  **                    |
         |                  **                                      **                  | drogueChuteActive
         |                 *                                          *                 |
  flying |               **                                            **               |
         |              **                                              **              |
         |            **                                                  ** -------------
         |           **                                                    **           |
         |          *                                                        *          |
         |         *                                                          *         |
         |        *                                                            *        | parachuteActive
         |      **                                                              **      |
         |      *                                                                *      |
         |     *                                                                  *     |
        ---- **                                                                    ** ----
                  --->   readyToLaunch                         recovered   <---
      ------------------------------------------------------------------------------------

*/


class RecoverySystem
{
  public:

    //  Recovery initializer
    bool begin();

    // Reads sensors and execute recovery algorithm
    void run();

  private:

    // The following functions define the behavior of recovery system,
    // that change dynamically in accordance to recovery system's state
    void     readyToLaunchRun();
    void            flyingRun();
    void drogueChuteActiveRun();
    void   parachuteActiveRun();
    void         recoveredRun();

    // Updates altitude vector
    void registerAltitude(bool writeToMemory);

  private:

    RecoverySystemState               state; // Current state of recovery system
    Barometer                     barometer; // Barometer manager
    Memory                           memory; // EEPROM Memory manager
    Button                           button; // Button for interaction with user
    HumanInterface           humanInterface; // Interface for communication with humans
    Actuator                       actuator; // Actuator for deployment of drogue and parachute

    static constexpr unsigned long int timeStep  {500}; // Time step for registering altitude (ms)
    int                                       n   {4};  // Size of 'altitude' vector
    float                           altitude[4];        // Initial altitudes

};

#endif // RECOVERYSYSTEM_H
