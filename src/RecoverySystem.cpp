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

#include "RecoverySystem.h"
#include "ParametersStatic.h"
#include "ParametersStatic.h"


void RecoverySystem::begin(bool simulationMode)
{
  // Simulation mode
  this->simulationMode = simulationMode;
  waitingForSimulatedAltitude = false;

  pinMode(ParametersStatic::pinLed, OUTPUT);
  timeOfTheLastMessage = millis();
  parser.begin();

  // Initializing button
  button.begin(ParametersStatic::pinButton);

  // Initializing EEPROM memory
  memory.begin();  

  // Reading flight parameters from permanent memory
  flightParameters = memory.readFlightParameters();

  // Show initialization message
  showInitMessage(flightParameters);

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
  if ( ! barometer.begin() )
  {
    // Registering error
    memory.writeErrorLog(error::BarometerInitializationFailure);
    tone(ParametersStatic::pinBuzzer,600);
    showErrorLog();

    while ( ! barometer.begin() )
    { 
      delay(100);
    }
    noTone(ParametersStatic::pinBuzzer);
  }

  // Initializing the actuator (this module is critical, so its initialization must be garanteed)
  if ( ! actuator.begin() ) 
  {
    // Registering error
    memory.writeErrorLog(error::ActuatorInitializationFailure);
    tone(ParametersStatic::pinBuzzer,600);
    showErrorLog();

    while ( ! actuator.begin() )
    {
      delay(100);
    }
    noTone(ParametersStatic::pinBuzzer);
  }
  
  // Initializing the time counter for altitude measurements
  uint32_t t0 = millis();
  currentStep = ( (int32_t) t0 ) / ( (int32_t) deltaT );
  flightInitialStep = currentStep;
  simulationInitialStep = currentStep+N+4;

  // Pre-initializing the remainder elements of the altitude vector
  for ( uint8_t i = 0; i <= N; ++i)
  {
    altitude[i] = 0.0;
  }
  
  // Initializing the Kalman filter
  kalmanFilter.begin(getAltitude(), 
    ParametersStatic::deltaT*1E-3, 
    ParametersStatic::kfStdExp, 
    ParametersStatic::kfStdModSub, 
    ParametersStatic::kfStdModTra, 
    ParametersStatic::kfdadt_ref);

  // Giving the registerAltitude method enough time to fully populate the altitude[] vector
  while ( currentStep <= flightInitialStep + N )
  {
    // Read the altitude, but do not write it to the memory
    registerAltitude(0);
  }
  delayedWriteIdx = 0; 

  // Initialization finished message
  showInitFinishedMessage();

  // Given enought time to print the message of initialization finished
  while ( currentStep <= flightInitialStep + N + 2 )
  {
    // Read the altitude, but do not write it to the memory
    registerAltitude(0);
  }
}


void RecoverySystem::run()
{
  // Checks for a new measurement
  bool hasNewMeasurement = false;

  // Recovery system's actions depend on recovery system's state.
  switch (state)
  {
    case RecoverySystemState::readyToLaunch:
     {
      // Registering altitude without writing it to memory
      // Writing to memory is performed only after liftoff
      hasNewMeasurement = registerAltitude(0);
      readyToLaunchRun();
      break;
     }
    case RecoverySystemState::flying:
    { 
      // Registering altitude and writing it to memory
      hasNewMeasurement = registerAltitude(1);
      flyingRun();
      break;
    }
    case RecoverySystemState::drogueChuteActive:
    {  
      // Registers altitude and writes it to memory every timeStepScaler
      hasNewMeasurement = registerAltitude(flightParameters.timeStepScaler);
      drogueChuteActiveRun();
      break;
    }
    case RecoverySystemState::parachuteActive:
    {  
      // Registers altitude and writes it to memory every timeStepScaler
      hasNewMeasurement = registerAltitude(flightParameters.timeStepScaler);
      parachuteActiveRun();
      break;
    }
    case RecoverySystemState::recovered:
    {
      // Registering altitude without writing it to memory
      hasNewMeasurement = registerAltitude(0);
      recoveredRun();
      break;
    }
    default:
      break;
  }

  // If a new measurement is available and in simulation mode, prints the current state
  if ( hasNewMeasurement && simulationMode )
  {
    Serial.print(F("<"));
    Serial.print(ocode::simulatedFlightState);
    Serial.print(F(","));
    Serial.print((currentStep-simulationInitialStep)*deltaT);
    Serial.print(F(","));
    Serial.print((int32_t)(10.0*altitude[N]));
    Serial.print(F(","));
    Serial.print((int32_t)(10.0*currentSpeed));
    Serial.print(F(","));
    Serial.print((int32_t)(10.0*currentAcceleration));
    switch (state)
    {
    case RecoverySystemState::readyToLaunch:
      Serial.println(F(",R>"));
      break;
    case RecoverySystemState::flying:
      Serial.println(F(",F>"));
      break;
    case RecoverySystemState::drogueChuteActive:
      Serial.println(F(",D>"));
      break;
    case RecoverySystemState::parachuteActive:
      Serial.println(F(",P>"));
      break;
    case RecoverySystemState::recovered:
      Serial.println(F(",L>"));
      break;
    default:
      break;
    }
  }
  listenForMessages();
}


void RecoverySystem::readyToLaunchRun()
{
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
    showReadyToLaunchStatus();
  }
}


void RecoverySystem::flyingRun()
{
  // Shows to user the flying status
  showFlyingStatus();

  // If rocket is falling, activates drogue chute and changes recovery system's state
  if ( ( apogeeCondition + fallCondition ) > 0 )
  {
    // Preparing actuator for drogue deployment
    actuator.reload();

    // Deploying drogue chute (for now, the stopCondition of the deployment cycle is false)
    actuator.deployDrogueChute(false);

    // Recording the drogue activation event
    memory.writeEvent('D', (uint16_t)(currentStep-flightInitialStep));

    // Changing recovery system's state
    state = RecoverySystemState::drogueChuteActive;
  }
}


void RecoverySystem::drogueChuteActiveRun()
{
  // Asks the actuator to deploy the drogue parachute.
  // After finishing one deployment cycle (turning on and off the drogue pin), the
  // actuator evaluates the stop condition, i.e., the parachute deployment condition OR
  // the number of deployment attempts. 
  bool actuatorFinished = actuator.deployDrogueChute( ( parachuteDeploymentCondition > 0 ) ||
                                               ( actuator.deployCounter >= flightParameters.maxNumberOfDeploymentAttempts ) );

  // If the parachute activation condition is true AND the actuator finished the deployment cycle, 
  // activates parachute and changes the state of the recovery system.
  // The conclusion of the deployment cycle is fundamental to ensure that the capacitor
  // is recharged to the parachute deployment of the next state.
  // Also checks the landing condition. If the actuator finished and the landing condition is valid,
  // changes the state to the next one (parachuteActive).
  if ( ( actuatorFinished && ( parachuteDeploymentCondition > 0 ) ) || ( actuatorFinished && ( landingCondition > 0 ) ) )
  {
    // Reloads the actuator (restarts the deploy counter etc...)
    actuator.reload();

    // Deploying the parachute (for now, the stopCondition of the deployment cycle is false)
    actuator.deployParachute(false);

    // Writing parachute activation event to memory 
    memory.writeEvent('P', (uint16_t)(currentStep-flightInitialStep));

    // Changing recovery system's state
    state = RecoverySystemState::parachuteActive;
  }

}


void RecoverySystem::parachuteActiveRun()
{
  // Asks the actuator to deploy the main parachute.
  // After finishing one deployment cycle (turning on and off the parachute pin), the
  // actuator evaluates the stop condition, i.e., the number of deployment attempts.
  // If the condition is not satisfied, another deployment cycle is started. 
  actuator.deployParachute( actuator.deployCounter >= flightParameters.maxNumberOfDeploymentAttempts );

  // If rocket is recovered, changes recovery system's state
  if ( landingCondition > 0 )
  {
    // Reloads the actuator to ensure that the drogue and parachute pins are turned off.
    actuator.reload();

    // Changing recovery system's state
    state = RecoverySystemState::recovered;

    // Recording the landing event
    memory.writeEvent('L', (uint16_t)(currentStep-flightInitialStep));
  }
}


void RecoverySystem::recoveredRun()
{
  // If flying, stores data to memory and changes recovery system's state.
  // This condition should not occur in a regular flight. But, if the altimeter resets
  // during the flight, and there is some data stored, the initial state will be 'recovered'.
  // To give the altimeter a chance to open the parachute, the flying condition is monitored.
  // If the condition is fullfiled, the state changes to 'flying'.
  if ( ( liftoffCondition + fallCondition ) > 0 ) {
    // Erase memory
    memory.erase();

    // Saving the exception
    memory.writeErrorLog(error::FlightStartedWithNonEmptyMemory);
    
    // Changing state
    changeStateToFlying();   
  }
  else
  {
    // Reading button
    switch (button.getState())
    {

      case ButtonState::pressedAndReleased:
        blinkApogee(memory);
        showReport();
        break;

      case ButtonState::longPressed:
        // Erasing memory
        memory.erase();
        showReport();
        
        // Restarting recovery system
        begin(false);

        break;

      default:;

    };
  }
}

float RecoverySystem::getAltitude()
{
    if ( simulationMode )
    {
      /* 
        Requests a simulated altitude for the current instant and
        waits for the response
      */
      waitingForSimulatedAltitude = true;

      Serial.print(F("<"));
      Serial.print(ocode::requestSimulatedAltitude);
      Serial.print(F(","));
      Serial.print((currentStep-simulationInitialStep)*deltaT);
      //Serial.print((int32_t)currentTime-(int32_t)(simulationInitialStep*deltaT));
      //Serial.print(millis());
      Serial.println(F(">"));
      while ( waitingForSimulatedAltitude )
      {
        listenForMessages();
      }
      // If the code of the message is 7, reads the simulated altitude
      if ( parser.getEntryInt(0) == 7 )
      {
        // Converting cm to m
        return 0.01 * parser.getEntryFloat(1);
      }
    }
  return barometer.getAltitude();
}

bool RecoverySystem::registerAltitude(const uint8_t& scaler)
{
  static uint16_t counter = 0;
  uint32_t currentTime = millis();
  bool hasNewMeasurement = false;

  if ( currentTime > ((uint32_t) (currentStep * deltaT)))
  {
    currentStep++;
    hasNewMeasurement = true;

    /*
      Delayed altitude vector recording (see the note about altitude vector delayed record in the header)
    */ 
    if ( scaler > 0 ) 
    {
      if ( delayedWriteIdx < N ) 
      {
        memory.writeAltitude(delayedWriteIdx, altitude[0]);
        delayedWriteIdx++;
      }
    }

    // Shifting the altitude vector and taking another measurement
    for (int i = 0; i < N; i++)
    {
      altitude[i] = altitude[i + 1];
    }

    // Reading the current altitude
    altitude[N] = getAltitude();

    // Updating the Kalman filter
    kalmanFilter.process(altitude[N]);

    if ( scaler > 0 ) 
    {
      counter++;
      if ( counter == scaler )
      {
        memory.appendAltitude(altitude[N]);
        counter = 0;
      }
    }
    else
    {
      counter = 0;
    }
    checkFlyEvents();
  }
  return hasNewMeasurement;
};


void RecoverySystem::changeStateToFlying()
{
    // Storing data to memory using the corrected altitude
    //float newBaseline = altitude[0];

    // Setting barometer new baseline
    //barometer.setBaseline(newBaseline);
    /* 
      Why resetting the barometer height baseline? If altimeter stays switched on for a long time, environment pressure
      and temperature may change and, hence, change local baseline height. To avoid this problem, it is necessary to 
      reset the barometer height baseline when liftoff is detected.
    */
    //for (int i = 0; i <= N; i++) 
    //{
    //  altitude[i] = altitude[i]-newBaseline;
    //}

    /*
      Delayed altitude vector recording (see the note about altitude vector delayed record in the header)
    */ 
    delayedWriteIdx = 0;
    memory.writeAltitude(N,altitude[N]);

    // Reloads the actuator
    actuator.reload();
    
    // Changing recovery system's state
    state = RecoverySystemState::flying;

    // Updating the time step when the flight was detected
    flightInitialStep = currentStep-N;

    // Registering the flight detection
    memory.writeEvent('F',N);
}

/*
void RecoverySystem::calculateSpeedAndAcceleration()
{
    static constexpr float accFactor = 1.0 /((1E-3*quarN*deltaT)*(1E-3*quarN*deltaT)*(halfN+1));
    static constexpr float spdFactor = 1.0 /(1E-3*(halfN+1)*halfN*deltaT);
    static constexpr float spdCorFactor = 1.E-3*deltaT*quarN;
    
    float sum1 = 0.0;
    float sum2 = 0.0;
    float sum3 = 0.0;

    for (uint8_t i = 0; i <= halfN; ++i)
    {
        sum1 += altitude[i];
        sum2 += altitude[i+quarN];
        sum3 += altitude[N-i];
    }
    currentAcceleration = (sum3+sum1-2.0*sum2)*accFactor; // Acceleration using central differencing scheme (shifted in time halfN*deltaT)
    
    // Backward differencing scheme
    currentSpeed = (3.0*sum3-4.0*sum2+sum1)*spdFactor; // Speed using backward differencing scheme (shifted in time quarN*deltaT )
    currentSpeed = currentSpeed + currentAcceleration * spdCorFactor; // Corrected speed (no time shift)
    
    // Central differencing scheme
    // currentSpeed = (sum3-sum1)*spdFactor; // Speed using central differencing scheme (shifted in time quarN*deltaT )
    // currentSpeed = currentSpeed + currentAcceleration * 2.0 * spdCorFactor; // Corrected speed (no time shift)

    return;
};
*/

void RecoverySystem::checkFlyEvents()
{
    // Calculating the average vertical component of the velocity
    //calculateSpeedAndAcceleration();
    currentSpeed = kalmanFilter.vs; // Uses the smoothed velocity vs instead of v
    currentAcceleration = kalmanFilter.a;

    // Ckecking the lift off condition
    liftoffCondition = (  currentSpeed > flightParameters.speedForLiftoffDetection ? 1 : 0 );
    
    // Checking the fall condition
    fallCondition    = ( -currentSpeed > flightParameters.speedForFallDetection ? 1 : 0 );
    
    // Checking the apogee condition 
    apogeeCondition  = (  currentSpeed < flightParameters.speedForApogeeDetection ? 1 : 0 );
    
    // Checking the parachute deployment condition
    parachuteDeploymentCondition = ( altitude[N] <= flightParameters.parachuteDeploymentAltitude ? 1 : 0 );

    // Checking the landing condition
    if ( liftoffCondition || fallCondition )
    { 
      landingCondition = 0;
    }
    else{
      float ymin=1E5, ymax=-1E5;
      for (uint8_t i = 0; i < N+1; ++i)
      {
          if ( altitude[i] < ymin ) ymin = altitude[i];
          if ( altitude[i] > ymax ) ymax = altitude[i];
      }
      landingCondition = ( ymax-ymin < flightParameters.displacementForLandingDetection ? 1 : 0 );
    }
};

void RecoverySystem::showStaticParameters()
{
  Serial.print(F("<"));
  Serial.print(ocode::firmwareVersion);
  Serial.print(F(","));
  Serial.print(ParametersStatic::softwareVersion);
  Serial.println(F(">"));
  Serial.print(F("<"));
  Serial.print(ocode::actuatorDischargeTime);
  Serial.print(F(","));
  Serial.print(ParametersStatic::actuatorDischargeTime);
  Serial.println(F(">"));
  Serial.print(F("<"));
  Serial.print(ocode::capacitorRechargeTime);
  Serial.print(F(","));
  Serial.print(ParametersStatic::capacitorRechargeTime);
  Serial.println(F(">"));
  Serial.print(F("<"));
  Serial.print(ocode::N);
  Serial.print(F(","));
  Serial.print(N);
  Serial.println(F(">"));
  Serial.print(F("<"));
  Serial.print(ocode::deltaT);
  Serial.print(F(","));
  Serial.print(deltaT);
  Serial.println(F(">"));

  // Kalman filter parameters
  Serial.print(F("<"));
  Serial.print(ocode::kfStdExp);
  Serial.print(F(","));
  Serial.print(ParametersStatic::kfStdExp);
  Serial.println(F(">"));
  Serial.print(F("<"));
  Serial.print(ocode::kfStdModSub);
  Serial.print(F(","));
  Serial.print(ParametersStatic::kfStdModSub);
  Serial.println(F(">"));
  Serial.print(F("<"));
  Serial.print(ocode::kfStdModTra);
  Serial.print(F(","));
  Serial.print(ParametersStatic::kfStdModTra);
  Serial.println(F(">"));
  Serial.print(F("<"));
  Serial.print(ocode::kfDadt_ref);
  Serial.print(F(","));
  Serial.print(ParametersStatic::kfdadt_ref);
  Serial.println(F(">"));
}

void RecoverySystem::showDynamicParameters(const FlightParameters& p)
{
  Serial.print(F("<"));
  Serial.print(ocode::simulatedMode);
  Serial.print(F(","));
  Serial.print((simulationMode?1:0));
  Serial.println(F(">"));
  Serial.print(F("<"));
  Serial.print(ocode::speedForLiftoffDetection);
  Serial.print(F(","));
  Serial.print(p.speedForLiftoffDetection);
  Serial.println(F(">"));
  Serial.print(F("<"));
  Serial.print(ocode::speedForFallDetection);
  Serial.print(F(","));
  Serial.print(p.speedForFallDetection);
  Serial.println(F(">"));
  Serial.print(F("<"));
  Serial.print(ocode::speedForApogeeDetection);
  Serial.print(F(","));
  Serial.print(p.speedForApogeeDetection);
  Serial.println(F(">"));
  Serial.print(F("<"));
  Serial.print(ocode::parachuteDeploymentAltitude);
  Serial.print(F(","));
  Serial.print(p.parachuteDeploymentAltitude);
  Serial.println(F(">"));
  Serial.print(F("<"));
  Serial.print(ocode::displacementForLandingDetection);
  Serial.print(F(","));
  Serial.print(p.displacementForLandingDetection);
  Serial.println(F(">"));
  Serial.print(F("<"));
  Serial.print(ocode::maxNumberOfDeploymentAttempts);
  Serial.print(F(","));
  Serial.print(p.maxNumberOfDeploymentAttempts);
  Serial.println(F(">"));
  Serial.print(F("<"));
  Serial.print(ocode::timeStepScaler);
  Serial.print(F(","));
  Serial.print(p.timeStepScaler);
  Serial.println(F(">"));
}

void RecoverySystem::showInitMessage(const FlightParameters& flightParameters)
{
  tone(ParametersStatic::pinBuzzer, 4000, 250);
  Serial.println(F("<"));
  Serial.println(ocode::startedInitialization);
  Serial.println(F(">"));
  showStaticParameters();
  showDynamicParameters(flightParameters);
}


void RecoverySystem::showInitFinishedMessage()
{
  tone(ParametersStatic::pinBuzzer, 4000, 100);
  Serial.println(F("<"));
  Serial.println(ocode::finishedInitialization);
  Serial.println(F(">"));
}


void RecoverySystem::showReadyToLaunchStatus()
{

  unsigned long currentTime;
  unsigned long timeStep = 750; // Time step to blink

  currentTime = millis();

  if ( currentTime > 2000 + timeOfTheLastMessage )
  {
    timeOfTheLastMessage = currentTime; 
  }

  if (currentTime % (2 * timeStep) < timeStep)
  {
    digitalWrite(ParametersStatic::pinLed, HIGH);
  }
  else
  {
    digitalWrite(ParametersStatic::pinLed, LOW);
  }

  if (currentTime % (2 * timeStep) < timeStep / 8)
  {
    tone(ParametersStatic::pinBuzzer, 600);
  }
  else
  {
    noTone(ParametersStatic::pinBuzzer);
  }
}


void RecoverySystem::showFlyingStatus()
{
  digitalWrite(ParametersStatic::pinLed, LOW);
  noTone(ParametersStatic::pinBuzzer);
}

void RecoverySystem::blinkApogee(Memory& memory)
{
  int          thousands;
  int           hundreds;
  int             dozens;
  int              units;
  unsigned int   iHeight;
  float          fHeight;

  fHeight = memory.readApogee();

  iHeight   = (int)fHeight;
  thousands = iHeight / 1000;
  hundreds  = iHeight % 1000;
  hundreds  = hundreds / 100;
  dozens    = iHeight % 100;
  dozens    = dozens / 10;
  units     = iHeight % 10;

  if (fHeight >= 1000) blinkNumber(thousands);

  if (fHeight >= 100)  blinkNumber(hundreds);

  if (fHeight >= 10)   blinkNumber(dozens);

  blinkNumber(units);
}

void RecoverySystem::showErrorLog()
{
  Serial.print(F("<"));
  Serial.print(ocode::errorLog);
  Serial.print(F(","));
  uint16_t errorLog = memory.readErrorLog();
  if ( errorLog > 0 ){
    for (uint8_t i = 0; i < 16; ++i)
    {
      /* 
        If the error is in the log, print it. See the error encode in the ErrorTable.h file.
      */
      if ( (errorLog & 2) == 2 )
      {
        Serial.print(i+1);
        Serial.print(F(";"));
      }
      errorLog = errorLog >> 1;
    }
  }
  else
  {
    Serial.print(F("0"));
  }
  Serial.println(F(">"));
}


void RecoverySystem::showReport()
{
  Serial.print(F("<"));
  Serial.print(ocode::stardedSendingMemoryReport);
  Serial.print(F(">"));

  showErrorLog();
 
  // Slot up to which data is written with higher frequency 
  uint16_t hfSlot = min(memory.readEvent('D'), memory.getNumberOfSlotsWritten());

  int32_t landingInstant = ((int32_t)deltaT)*memory.readEvent('L');

  // Flight events
  if ( memory.getNumberOfSlotsWritten() > 0 ){
    Serial.print(F("<"));
    Serial.print(ocode::liftoffEvent);
    Serial.print(F(","));
    Serial.print(((int32_t)deltaT)*memory.readEvent('F'));
    Serial.println(F(">"));
    Serial.print(F("<"));
    Serial.print(ocode::drogueEvent);
    Serial.print(F(","));
    Serial.print(((int32_t)deltaT)*memory.readEvent('D'));
    Serial.println(F(">"));
    Serial.print(F("<"));
    Serial.print(ocode::parachuteEvent);
    Serial.print(F(","));
    Serial.print(((int32_t)deltaT)*memory.readEvent('P'));
    Serial.println(F(">"));
    Serial.print(F("<"));
    Serial.print(ocode::landedEvent);
    Serial.print(F(","));
    Serial.print(landingInstant);
    Serial.println(F(">"));
  
    int32_t t0 = deltaT * hfSlot;
    int32_t t;
    int32_t h;
    
    for ( uint16_t i = 0; i < memory.getNumberOfSlotsWritten(); ++i)
    {
      if ( i > hfSlot )
      {
        t = (int32_t)(deltaT) * (int32_t)(flightParameters.timeStepScaler) * (i-hfSlot) + t0;
      }
      else
      {
        t = i * deltaT;
      }

      h = (int32_t)(10.0*memory.readAltitude(i)); // m to dm

      Serial.print(F("<"));
      Serial.print(ocode::flightPath);
      Serial.print(F(","));
      Serial.print(t);
      Serial.print(F(","));
      Serial.print(h);
      Serial.println(F(">"));
    }

  }
  Serial.print(F("<"));
  Serial.print(ocode::finishedSendingMemoryReport);
  Serial.print(F(">"));
}


void RecoverySystem::blinkNumber(int n)
{

  int i;

  if (n == 0)
  {
    digitalWrite(ParametersStatic::pinLed, LOW);
    noTone(ParametersStatic::pinBuzzer);
    delay(300);
    digitalWrite(ParametersStatic::pinLed, HIGH);
    tone(ParametersStatic::pinBuzzer, 600);
    delay(1000);
    digitalWrite(ParametersStatic::pinLed, LOW);
    noTone(ParametersStatic::pinBuzzer);
  }

  for (i = 1; i <= n; i++)
  {
    digitalWrite(ParametersStatic::pinLed, LOW);
    noTone(ParametersStatic::pinBuzzer);
    delay(300);
    digitalWrite(ParametersStatic::pinLed, HIGH);
    tone(ParametersStatic::pinBuzzer, 600);
    delay(300);
    digitalWrite(ParametersStatic::pinLed, LOW);
    noTone(ParametersStatic::pinBuzzer);
  }

  delay(500);

};


void RecoverySystem::listenForMessages()
{
  size_t sz = Serial.available();
  for (size_t i = 0; i < sz; i++) 
  {
    parser.append(Serial.read());
  }
  if ( parser.hasMessage() )
  {
    switch ( parser.getEntryInt(0) )
    {
    case icode::readStaticParameters: // Shows static parameters
    {
      showStaticParameters();
      break;
    }
    case icode::readDynamicParameters: // Shows flight parameters
    {
      showDynamicParameters(flightParameters);
      break;
    }
    case icode::writeDynamicParameters: // Writes flight parameters to permanent memory
    {
      memory.writeFlightParameters(flightParameters);
      memory.erase();
      showReport();
      begin(false);
      break;
    }
    case icode::restoreToFactoryParameters: // Clears records of the last flight and saves the parameters of the factory
    {
      FlightParameters p; // Creates flight parameters with default values
      flightParameters = p;
      memory.writeFlightParameters(p);
      memory.erase();
      showReport();
      begin(false);
      break;
    }
    case icode::clearFlightMemory: // Clears records of the last flight
    {
      memory.erase();
      showReport();
      begin(false);
      break;
    }
    case icode::readFlightReport: // Shows the report of the last flight
    {
      showReport();
      break;
    }
    case icode::setSimulationMode: // Sets the simulation mode (0=off, 1=on)
    {
      bool simMode = ( parser.getEntryInt(1) == 1 ? true : false );
      begin(simMode);
      break;
    }
    case icode::setSimulatedFlightAltitude: // Sets the altitude (m) for the requested instant in the simulation mode
    {
      waitingForSimulatedAltitude = false;
      break;
    }
    case icode::setSpeedForLiftoffDetection: // Sets the speed for liftoff detection (m/s)
    {
      flightParameters.speedForLiftoffDetection = parser.getEntryInt(1);
      break;
    }
    case icode::setSpeedForFallDetection: // Sets the speed for fall detection (m/s)
    {
      flightParameters.speedForFallDetection = parser.getEntryInt(1);
      break;
    }
    case icode::setSpeedForApogeeDetection: // Sets the speed for apogee detection (m/s)
    {
      flightParameters.speedForApogeeDetection = parser.getEntryInt(1);
      break;
    }
    case icode::setParachuteDeploymentAltitude: // Sets the altitude to deploy the main parachute (m)
    {
      flightParameters.parachuteDeploymentAltitude = parser.getEntryInt(1);
      break;
    }
    case icode::setDisplacementForLandingDetection: // Sets the displacement for landing detection (m)
    {
      flightParameters.displacementForLandingDetection = parser.getEntryInt(1);
      break;
    }
    case icode::setMaxNumberOfDeploymentAttempts: // Sets the maximum number of deployment attempts
    {
      flightParameters.maxNumberOfDeploymentAttempts = parser.getEntryInt(1);
      break;
    }
    case icode::setTimeStepScaler: // Sets the scaler for adaptive deltaT
    {
      flightParameters.timeStepScaler = parser.getEntryInt(1);
      break;
    }
    default:
      break;
    }
  }
}