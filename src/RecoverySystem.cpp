

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
  for ( uint8_t i = 0; i <= N; ++i)
  {
    altitude[i] = 0.0;
  }
  uint32_t t0 = millis();
  // Giving the registerAltitude method enough time to fully populate the altitude[] vector
  while ( millis()-t0 < deltaT * (N+3) )
  {
    // Read the altitude, but do not write it to the memory
    registerAltitude(false);
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
  if ( liftoffCondition > 0 ) {
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

  // If rocket is falling, activates drogue chute and changes recovery system's state
  if ( ( apogeeCondition + fallCondition ) > 0 )
  {
    // Preparing actuator for drogue deployment
    actuator.reload();

    // Deploying drogue chute (for now, the stopCondition of the deployment cycle is false)
    actuator.deployDrogueChute(false);

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

  // Asks the actuator to deploy the drogue parachute.
  // After finishing one deployment cycle (turning on and off the drogue pin), the
  // actuator evaluates the stop condition, i.e., the parachute deployment condition OR
  // the number of deployment attempts. 
  bool actuatorFinished = actuator.deployDrogueChute( ( parachuteDeploymentCondition > 0 ) ||
                                               ( actuator.deployCounter >= Parameters::maxNumberOfDeploymentAttempts ) );

  // If the parachute activation condition is true AND the actuator finished the deployment cycle, 
  // activates parachute and changes the state of the recovery system.
  // The conclusion of the deployment cycle is fundamental to ensure that the capacitor
  // is recharged to the parachute deployment of the next state.
  if ( actuatorFinished && ( parachuteDeploymentCondition > 0 ) )
  {
    // Reloads the actuator (restarts the deploy counter etc...)
    actuator.reload();

    // Deploying the parachute (for now, the stopCondition of the deployment cycle is false)
    actuator.deployParachute(false);

    // Changing recovery system's state
    state = RecoverySystemState::parachuteActive;

  }

}


void RecoverySystem::parachuteActiveRun()
{

  // Registers altitude and writes to memory
  registerAltitude(true);

  // Asks the actuator to deploy the main parachute.
  // After finishing one deployment cycle (turning on and off the parachute pin), the
  // actuator evaluates the stop condition, i.e., the number of deployment attempts.
  // If the condition is not satisfied, another deployment cycle is started. 
  actuator.deployParachute( actuator.deployCounter >= Parameters::maxNumberOfDeploymentAttempts );

  // If rocket is recovered, changes recovery system's state
  if ( landingCondition > 0 )
  {
    // Reloads the actuator to ensure that the drogue and parachute pins are turned off.
    actuator.reload();

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
  if ( ( liftoffCondition + fallCondition ) > 0 ) {
    // Saving the exception
    memory.writeErrorLog(error::RestartedDuringFlight);
    
    // Changing state
    changeStateToFlying();  
  
  }
  else
  {
    // Shows to user the recovered status
    humanInterface.showRecoveredStatus();

    // Reading button
    switch (button.getState())
    {

      case ButtonState::pressedAndReleased:
        humanInterface.blinkApogee(memory);
        humanInterface.showReport(deltaT, memory);
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
}


void RecoverySystem::registerAltitude(bool writeToMemory)
{

  static unsigned long int currentStep = 0;

  if (millis() / deltaT > currentStep)
  {
    currentStep = currentStep + 1;

    for (int i = 0; i < N; i++)
    {
      altitude[i] = altitude[i + 1];
    }

    altitude[N] = barometer.getAltitude();

    if (writeToMemory) memory.appendAltitude(altitude[N]);

    checkFlyEvents();
  }

};


void RecoverySystem::changeStateToFlying()
{
    // Storing data to memory using the corrected altitude
    #ifdef DEBUGMODE
    float newBaseline = 0.0;
    #else
    float newBaseline = altitude[0];
    #endif

    // Setting barometer new baseline
    barometer.setBaseline(newBaseline);
    /* 
      Why resetting the barometer height baseline? If altimeter stays switched on for a long time, environment pressure
      and temperature may change and, hence, change local baseline height. To avoid this problem, it is necessary to 
      reset the barometer height baseline when liftoff is detected.
    */
    for (int i = 0; i <= N; i++) 
    {
      altitude[i] = altitude[i]-newBaseline;
      
      memory.appendAltitude(altitude[i]);
    }

    // Reloads the actuator
    actuator.reload();
    
    // Changing recovery system's state
    state = RecoverySystemState::flying;
}


float RecoverySystem::vAverage()
{
    /*
    // Simple mean

    float sum1 = 0.0;
    float sum2 = 0.0;

    for (uint8_t i = 0; i < halfN; ++i)
    {
        sum1 += altitude[i];
        sum2 += altitude[N-i];
    }
    return (sum2-sum1)/(1E-3*(halfN+1)*halfN*deltaT);
    */
    // Integral based average
    float sum1 = 0.5*altitude[0];
    float sum2 = 0.5*altitude[N];

    for (uint8_t i = 1; i < halfN; ++i)
    {
        sum1 += altitude[i];
        sum2 += altitude[N-i];
    }
    return (sum2-sum1)/(1E-3*halfN*halfN*deltaT);
};


void RecoverySystem::checkFlyEvents()
{
    // Calculating the average vertical component of the velocity
    float vavg = vAverage();

    // Ckecking the lift off condition
    liftoffCondition = (  vavg > Parameters::speedForLiftoffDetection ? 1 : 0 );
    
    // Checking the fall condition
    fallCondition    = ( -vavg > Parameters::speedForFallDetection ? 1 : 0 );
    
    // Checking the apogee condition 
    apogeeCondition  = (  vavg < Parameters::speedForApogeeDetection ? 1 : 0 );
    
    // Checking the parachute deployment condition
    parachuteDeploymentCondition = ( altitude[N] <= Parameters::parachuteDeploymentAltitude ? 1 : 0 );

    // Checking the landing condition
    float ymin=1E5, ymax=-1E5;
    for (uint8_t i = 0; i < N+1; ++i)
    {
        if ( altitude[i] < ymin ) ymin = altitude[i];
        if ( altitude[i] > ymax ) ymax = altitude[i];
    }
    landingCondition = ( ymax-ymin < Parameters::displacementForLandingDetection ? 1 : 0 );

    #ifdef DEBUGMODE

    char cstate;

    switch (state)
    {
    case RecoverySystemState::readyToLaunch:
      cstate = 'R';
      break;   
    case RecoverySystemState::flying:
      cstate = 'F';
      break;   
    case RecoverySystemState::drogueChuteActive:
      cstate = 'D';
      break;   
    case RecoverySystemState::parachuteActive:
      cstate = 'P';
      break;   
    case RecoverySystemState::recovered:
      cstate = 'L';
      break;   
    default:
      break;
    }
    Serial.print(1E-3*millis()+STARTBAROMETERTIME,3);
    Serial.print(' ');
    Serial.print(altitude[N]);
    Serial.print(' ');
    Serial.print(vavg);
    Serial.print(' ');
    Serial.print(cstate);
    Serial.print('\n');
    #endif

};
