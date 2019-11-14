

    /**********************************************************************\
   /          rRocket: An Arduino powered rocketry recovery system          \
  /            Federal University of Technology - Parana - Brazil            \
  \              by Guilherme Bertoldo and Jonas Joacir Radtke               /
   \                       updated October 18, 2019                         /
    \**********************************************************************/


#include "RecoverySystem.h"
#include "Parameters.h"


bool RecoverySystem::begin()
{

  // Initializing human interface
  humanInterface.begin();

  // Show initialization message
  humanInterface.showInitMessage();

  // Initializing button
  button.begin(Parameters::pinButton);

  // Pin used in the button as 5V
  pinMode(Parameters::pinButtonVCCSource, OUTPUT);
  digitalWrite(Parameters::pinButtonVCCSource, HIGH);

  // Initializing EEPROM memory, barometer and actuator (these modules are critical, so their initialization must be checked)
  if ( memory.begin() && barometer.begin() && actuator.begin() )
  {

    // Initializing altitude vector
    for (int i = 0; i < n; i++)
    {
      altitude[i] = 0;
    }

    switch (memory.getState())
    {

      case MemoryState::full:
        state = RecoverySystemState::recovered;
        break;

      case MemoryState::empty:
        state = RecoverySystemState::readyToLaunch;
        break;

      default:
        humanInterface.println(" Recovery system fatal error: unknown memory state. Stopping...\n");
        while (true);

    }

  }
  else
  {

    humanInterface.println(" Recovery system fatal error: unable to initialize recovery system...\n");

    while (true) {};

  }

};


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

  bool isFlying = false;

  if ( abs(altitude[n - 1] - altitude[0]) > Parameters::displacementForLiftoffDetection) isFlying = true;
  /*
      Why using absolute value for altitude variation? Suppose the recovery system suffers a failure during flight,
      turn off and turn on again. If it is falling, altitude variation will be negative and the above conditional
      will be fulfilled. Recovery system will change state to 'flying' and soon to 'drogrueChuteActive' or
      'parachuteActive'.
  */

  // If flying, stores data to memory and changes recovery system's state
  if ( isFlying ) {

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
      
      memory.writeAltitude(altitude[i]);
    }
    
    // Changing recovery system's state
    state = RecoverySystemState::flying;

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

    // Saving apogee
    memory.writeApogee();

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

  // Shows to user the recovered status
  humanInterface.showRecoveredStatus();

  // Reading button
  switch (button.getState())
  {

    case ButtonState::pressedAndReleased:
      humanInterface.blinkApogee(memory);
      humanInterface.showApogee(memory);
      humanInterface.showTrajectory(timeStep, memory);
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

    if (writeToMemory) memory.writeAltitude(altitude[n - 1]);
  }

};
