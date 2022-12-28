

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


    // Changes the recovery system state to 'flying'.
    void changeStateToFlying(); 

    /*
      Calculates the vertical component of the velocity
      vector based on average values of the vertical 
      position.
    */
    float vAverage();

    /*
      Checks for the conditions of the following events:
        - liftoff 
        - apogee 
        - fall 
        - parachute deployment altitude 
        - landing 
    */
    void checkFlyEvents();

  private:

    RecoverySystemState               state; // Current state of recovery system
    Barometer                     barometer; // Barometer manager
    Memory                           memory; // EEPROM Memory manager
    Button                           button; // Button for interaction with user
    HumanInterface           humanInterface; // Interface for communication with humans
    Actuator                       actuator; // Actuator for deployment of drogue and parachute

    static constexpr uint8_t       N =  12; // Number of time steps
    static constexpr uint8_t   halfN = N/2; // Half the number of time steps
    static constexpr uint16_t deltaT = 250; // Time step between measurements (ms)
    float                    altitude[N+1]; // Register of the last N+1 measurements

    // Event flags (1 if condition is satisfied, 0 otherwise)
    uint8_t             liftoffCondition {0};
    uint8_t                fallCondition {0};
    uint8_t              apogeeCondition {0};
    uint8_t parachuteDeploymentCondition {0};
    uint8_t             landingCondition {0};

};

#endif // RECOVERYSYSTEM_H
