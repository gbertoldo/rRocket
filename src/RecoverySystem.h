/*
  The MIT License (MIT)

  Copyright (C) 2022 Guilherme Bertoldo and Jonas Joacir Radtke
  (UTFPR) Federal University of Technology - Parana

  Permission is hereby granted, free of charge, to any person obtaining a 
  copy of this software and associated documentation files (the “Software”), 
  to deal in the Software without restriction, including without limitation 
  the rights to use, copy, modify, merge, publish, distribute, sublicense, 
  and/or sell copies of the Software, and to permit persons to whom the Software 
  is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all 
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN 
  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#ifndef RECOVERYSYSTEM_H
#define RECOVERYSYSTEM_H

#include "Arduino.h"
#include "Barometer.h"
#include "Memory.h"
#include "Button.h"
#include "Actuator.h"
#include "ParametersStatic.h"
#include "ParametersDynamic.h"
#include "MessageParser.h"

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

/*
  Note about altitude vector delayed record
  -----------------------------------------

  The recovery system keeps a vector of altitude measurements. When the flight is detected,
  it is not possible to write the whole vector to the memory immediately, because writing
  to memory is computationally expensive and may cause a delay greater than the sample period 
  (deltaT) of the altitude. 
  
  To overcome this problem, when the flight is detected, only the last element of the vector 
  (the element N) is written to the N-th slot of the memory. This procedure forces the Memory class
  to reserve the N+1 first slots and to append only after the N-th slot index. Only one element of 
  the vector must be written per main loop cycle. The variable delayedWriteIdx was created to control 
  the recording process. delayedWriteIdx must be initialized to 0 when the flight is detected 
  (in the changeStateToFlying method) and be incremented at every call of registerAltitude method 
  during the flight until all elements are recorded.
*/


class RecoverySystem
{
  public:

    //  Recovery initializer
    void begin(bool simulationMode);

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

    /* 
      Updates altitude vector and writes data to permanent memory
      scaler defines how much the data will be written to memory
      0 = do not write
      1 = once per reading
      N = every N readings
      Returns true if a new reading is available. Otherwise, returns false.
    */
    bool registerAltitude(const uint8_t& scaler);


    // Changes the recovery system state to 'flying'.
    void changeStateToFlying(); 

    /*
      Calculates the vertical component of the velocity
      and acceleration vectors based on average values 
      of the vertical position. The results are stored
      in the variables currentSpeed and currentAcceleration.
    */
    void calculateSpeedAndAcceleration();

    /*
      Checks for the conditions of the following events:
        - liftoff 
        - apogee 
        - fall 
        - parachute deployment altitude 
        - landing 
    */
    void checkFlyEvents();

    // Shows the rRocket static parameters
    void showStaticParameters();

    // Shows the rRocket dynamic parameters
    void showDynamicParameters(const FlightParameters& p);

    // Shows initialization messages
    void showInitMessage(const FlightParameters& flightParameters);

    // Shows initialization finished message
    void showInitFinishedMessage();

    // Shows ready to launch messages
    void showReadyToLaunchStatus();

    // Shows to user the flying status
    void showFlyingStatus();

    // Blinks apogee
    void blinkApogee(Memory& memory);

    // Shows the error log
    void showErrorLog();

    // Shows the report of apogee, trajectory, errors, etc
    void showReport();

    // Blinks a number
    void blinkNumber(int n);

    // Listen to serial
    void listenForMessages();

  private:

    bool                simulationMode {false}; // If true, uses a simulated barometer (through serial port)
    bool   waitingForSimulatedAltitude {false}; // Lock used to wait for simulated altitude after a request in the simulation mode
    
    float                     currentSpeed {0}; // Current speed (m/s)
    float              currentAcceleration {0}; // Current acceleration (m/s2)
    int32_t                    currentStep {0}; // Current time step = int( millis()/deltaT )
    int32_t              flightInitialStep {0}; // Time step when the flight was detected
    int32_t          simulationInitialStep {0}; // Step of the simulation start
    
    RecoverySystemState               state; // Current state of recovery system
    Barometer                     barometer; // Barometer manager
    Memory                           memory; // EEPROM Memory manager
    Button                           button; // Button for interaction with user
    Actuator                       actuator; // Actuator for deployment of drogue and parachute

    static constexpr uint8_t N            {ParametersStatic::N}; // Number of time steps to calculate the flight statistics (must be a multiple of 4)
    static constexpr uint16_t deltaT {ParametersStatic::deltaT}; // Time step between measurements (ms)

    static constexpr uint8_t   halfN = N/2; // Half the number of time steps
    static constexpr uint8_t   quarN = N/4; // The fourth part of the number of time steps
    float                 altitude[N+1] {}; // Register of the last N+1 measurements
    // See the note about altitude vector delayed record in the header
    uint8_t            delayedWriteIdx = 0; // Index to write altitude vector to memory after liftoff

    // Event flags (1 if condition is satisfied, 0 otherwise)
    uint8_t             liftoffCondition {0};
    uint8_t                fallCondition {0};
    uint8_t              apogeeCondition {0};
    uint8_t parachuteDeploymentCondition {0};
    uint8_t             landingCondition {0};

    // Flight parameters
    FlightParameters flightParameters;

    // Last time of the message on the serial monitor
    unsigned long int timeOfTheLastMessage;

    // Input message parser
    MessageParser parser;
};

#endif // RECOVERYSYSTEM_H
