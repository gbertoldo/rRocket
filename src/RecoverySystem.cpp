

    /**********************************************************************\
   /          rRocket: An Arduino powered rocketry recovery system          \
  /            Federal University of Technology - Parana - Brazil            \
  \              by Guilherme Bertoldo and Jonas Joacir Radtke               /
   \                       updated September 12, 2022                       /
    \**********************************************************************/


#include "RecoverySystem.h"
#include "Parameters.h"
#include "ErrorTable.h"


void RecoverySystem::begin()
{
  // Initializing human interface
  humanInterface.begin();

  // Show initialization message
  humanInterface.showInitMessage();

  // Initializing button
  button.begin(Parameters::pinButton);

  // Initializing EEPROM memory
  memory.begin();  

  // Initializing the barometer (this module is critical, so its initialization must be garanteed)
  while ( ! barometer.begin() )
  {
    // Registering error
    memory.writeErrorLog(error::BarometerInitializationFailure);
    delay(100);
  }

  // Initializing the actuator (this module is critical, so its initialization must be garanteed)
  while ( ! actuator.begin() )
  {
    // Registering error
    memory.writeErrorLog(error::ActuatorInitializationFailure);
    delay(100);
  }

  // Initializing the altitude vector
  for ( uint8_t i = 0; i < n; ++i)
  {
    altitude[i] = 0.0;
  }
  for ( uint8_t i = 0; i < n; ++i)
  {
    // Read the altitude, but do not write it to the memory
    registerAltitude(false);
    delay(timeStep+5);
  }

  /*
    Choose the initial state according to the data in memory.
    If memory is empty, the initial state is readyToLaunch. 
    Otherwise, the initial state is recovered. If the altimeter
    is reset during the flight, it will be initialized in
    the recovered state and soon its state will be changed
    to flying, due to the fullfilment of the flying condition.
  */
  if ( memory.getNumberOfSlotsWritten() > 0 )
  {
    state = RecoverySystemState::recovered;
  }
  else
  {
    state = RecoverySystemState::readyToLaunch;
  }
}


void RecoverySystem::run()
{

  // Recovery system's actions depend on recovery system's state.
  switch (state)
  {

    case RecoverySystemState::readyToLaunch:
      readyToLaunchRun();
      break;

    case RecoverySystemState::flying:
      flyingRun();
      break;

    case RecoverySystemState::drogueChuteActive:
      drogueChuteActiveRun();
      break;

    case RecoverySystemState::parachuteActive:
      parachuteActiveRun();
      break;

    case RecoverySystemState::recovered:
      recoveredRun();
      break;

    default:;

  }

}


void RecoverySystem::readyToLaunchRun()
{

  // Registering altitude without writing to memory
  // Writing to memory is performed only after liftoff
  registerAltitude(false);

  /*

           Scanning for liftoff

  */
  
  // If flying, stores data to memory and changes recovery system's state
  if ( isFlying() ) {
    changeStateToFlying();    
  }
  else
  {
    // Showing recovery system is ready to launch
    humanInterface.showReadyToLaunchStatus();
  }
}


void RecoverySystem::flyingRun()
{

  // Registering altitude and writing to memory
  registerAltitude(true);

  // Shows to user the flying status
  humanInterface.showFlyingStatus();

  // Checks for falling state
  bool isFalling = false;

  if ( abs( altitude[n - 1] - barometer.getApogee() ) > Parameters::displacementForDrogueChuteDeployment ) isFalling = true;

  // If rocket is falling, activates drogue chute and changes recovery system's state
  if ( isFalling )
  {

    // Deploying drogue chute
    actuator.deployDrogueChute();

    // Changing recovery system's state
    state = RecoverySystemState::drogueChuteActive;
        
    // Saving apogee
    memory.writeApogee( barometer.getApogee() );

  }

}


void RecoverySystem::drogueChuteActiveRun()
{

  // Registers altitude and writes to memory
  registerAltitude(true);

  // Deploying drogue chute
  actuator.deployDrogueChute();

  // Checks for main parachute activation
  bool parachuteActivation = false;

  if ( altitude[n - 1] <= Parameters::parachuteDeploymentAltitude ) parachuteActivation = true;

  // If rocket is falling bellow parachute activation altitude, activates parachute and changes recovery system's state
  if ( parachuteActivation )
  {

    // Deploying parachute
    actuator.deployParachute();

    // Changing recovery system's state
    state = RecoverySystemState::parachuteActive;

  }

}


void RecoverySystem::parachuteActiveRun()
{

  // Registers altitude and writes to memory
  registerAltitude(true);

  // Deploying parachute
  actuator.deployParachute();

  // Checks for recovery
  bool isRecovered = false;

  if ( abs( altitude[n - 1] - altitude[0] ) < Parameters::displacementForRecoveryDetection ) isRecovered = true;

  // If rocket is recovered, changes recovery system's state
  if ( isRecovered )
  {

    // Changing recovery system's state
    state = RecoverySystemState::recovered;

  }

}


void RecoverySystem::recoveredRun()
{

  // Registering altitude without writing to memory
  registerAltitude(false);
 
  // If flying, stores data to memory and changes recovery system's state.
  // This condition should not occur in a regular flight. But, if the altimeter resets
  // during the flight, and there is some data stored, the initial state will be 'recovered'.
  // To give the altimeter a chance to open the parachute, the flying condition is monitored.
  // If the condition is fullfiled, the state changes to 'flying'.
  if ( isFlying() ) {
    // Saving the exception
    memory.writeErrorLog(error::RestartedDuringFlight);
    
    // Changing state
    changeStateToFlying();    
  }

  // Shows to user the recovered status
  humanInterface.showRecoveredStatus();

  // Reading button
  switch (button.getState())
  {

    case ButtonState::pressedAndReleased:
      humanInterface.blinkApogee(memory);
      humanInterface.showReport(timeStep, memory);
      break;

    case ButtonState::longPressed:
      // Erasing memory
      memory.erase();
      
      // Restarting recovery system
      begin();

      break;

    default:;

  };

}


void RecoverySystem::registerAltitude(bool writeToMemory)
{

  static unsigned long int currentStep = 0;

  if (millis() / timeStep > currentStep)
  {
    currentStep = currentStep + 1;

    for (int i = 0; i < n - 1; i++)
    {
      altitude[i] = altitude[i + 1];
    }

    altitude[n - 1] = barometer.getAltitude();

    if (writeToMemory) memory.appendAltitude(altitude[n - 1]);
  }

};

bool RecoverySystem::isFlying()
{
  /*
      Why using absolute value for altitude variation? Suppose the recovery system suffers a failure during flight,
      turn off and turn on again. If it is falling, altitude variation will be negative and the above conditional
      will be fulfilled. Recovery system will change state to 'flying' and soon to 'drogrueChuteActive' or
      'parachuteActive'.
  */
  return ( abs(altitude[n - 1] - altitude[0]) > Parameters::displacementForLiftoffDetection);
}

void RecoverySystem::changeStateToFlying()
{
    // Setting barometer new baseline
    barometer.setBaseline(altitude[0]);
    /* 
      Why resetting the barometer height baseline? If altimeter stays switched on for a long time, environment pressure
      and temperature may change and, hence, change local baseline height. To avoid this problem, it is necessary to 
      reset the barometer height baseline when liftoff is detected.
    */

    // Storing data to memory using the corrected altitude
    for (int i = 0; i < n; i++) 
    {
      altitude[i] = altitude[i]-altitude[0];
      
      memory.appendAltitude(altitude[i]);
    }
    
    // Changing recovery system's state
    state = RecoverySystemState::flying;
}