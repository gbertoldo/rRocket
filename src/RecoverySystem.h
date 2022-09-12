

    /**********************************************************************\
   /          rRocket: An Arduino powered rocketry recovery system          \
  /            Federal University of Technology - Parana - Brazil            \
  \              by Guilherme Bertoldo and Jonas Joacir Radtke               /
   \                       updated September 12, 2022                       /
    \**********************************************************************/


#ifndef RECOVERYSYSTEM_H
#define RECOVERYSYSTEM_H

#include "Arduino.h"
#include "Barometer.h"
#include "Memory.h"
#include "Button.h"
#include "Actuator.h"
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
    void begin();

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

    /* 
      Checks the flying condition. Used both to detect liftoff as well as 
      to detect falling in the 'recovered' state.
    */ 
    bool isFlying();

    // Changes the recovery system state to 'flying'.
    void changeStateToFlying(); 

  private:

    RecoverySystemState               state; // Current state of recovery system
    Barometer                     barometer; // Barometer manager
    Memory                           memory; // EEPROM Memory manager
    Button                           button; // Button for interaction with user
    HumanInterface           humanInterface; // Interface for communication with humans
    Actuator                       actuator; // Actuator for deployment of drogue and parachute

    static constexpr unsigned long int timeStep  {250}; // Time step for registering altitude (ms)
    uint8_t                                   n   {8};  // Size of 'altitude' vector
    float                           altitude[8];        // Initial altitudes

};

#endif // RECOVERYSYSTEM_H
