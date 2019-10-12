


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

  // Initializing human interface
  humanInterface.begin();

  // Show initialization message
  humanInterface.showInitialization();

  // Initializing altitude vector
  for (int i = 0; i < n; i++)
  {
    altitude[i] = 0;
  }

  // Initializing button
  button.begin(Parameters::pinButton);

  // Initializing EEPROM memory, barometer and actuator (these modules are critical, so their initialization must be checked)
  if ( memory.begin() && barometer.begin() && actuator.begin() )
  {

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
  } else
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
      Why using absolute value for altitude variation? Suppose recovery system suffer a failure during flight,
      turn off and turn on again. If it is falling down, altitude variation will be negative and the above conditional
      will be fullfiled. Recovery system will change state to 'flying' and soon to 'drogrueChuteActive' or
      'parachuteActive'.
  */



  // If flying, stores data to memory and changes recovery system's state
  if ( isFlying ) {
    // Storing data to memory
    for (int i = 0; i < n; i++) memory.writeAltitude(altitude[i]);

    // Changing recovery system's state
    state = RecoverySystemState::flying;
  }

  // Blink recovery system is ready to launch
  humanInterface.blinkReadyToLaunch();
}


void RecoverySystem::flyingRun()
{
  // Registering altitude and writing to memory
  registerAltitude(true);

  // Checks for falling down
  bool isFallingDown = false;

  if ( abs( altitude[n - 1] - barometer.getApogee() ) > Parameters::displacementForDrogueChuteDeployment )  isFallingDown = true;

  // If rocket is falling down, activates drogue chute and changes recovery system's state
  if ( isFallingDown )
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

  // Checks for main parachute activation
  bool parachuteActivation = false;

  if ( altitude[n - 1] <= Parameters::parachuteDeploymentAltitude ) parachuteActivation = true;

  // If rocket is falling down bellow parachute activation altitude, activates parachute and changes recovery system's state
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

  // Checks for recovery
  bool isRecovered = false;

  if ( abs( altitude[n - 1] - altitude[0] ) < Parameters::displacementForRecoverDetection ) isRecovered = true;

  // If rocket is recovered, changes recovery system's state
  if ( isRecovered )
  {
    // Changing recovery system's state
    state = RecoverySystemState::recovered;
  }
}


void RecoverySystem::recoveredRun()
{
  // Blinking apogee
  humanInterface.blinkApogee(memory);

  // Reading button
  switch (button.getState())
  {
    case ButtonState::pressedAndReleased:
      humanInterface.showApogee(memory);
      humanInterface.showTrajectory(timeStep, memory);
      break;

    case ButtonState::longPressed:
      // Erasing memory
      memory.erase();
      // Changing recovery system's state
      state = RecoverySystemState::readyToLaunch;
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
}
