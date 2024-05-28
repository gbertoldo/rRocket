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

#ifndef PARAMETERSSTATIC_H
#define PARAMETERSSTATIC_H
#include "Arduino.h"

namespace ParametersStatic
{
  static constexpr char              softwareVersion[] {"1.6.2-KF"}; // Version of this software
  static constexpr int                                 pinLed  {13}; // Screen uses pin 13 to blink Arduino internal led
  static constexpr int                              pinBuzzer  {11}; // Pin of buzzer
  static constexpr int                              pinButton  {10}; // Pin of button
  static constexpr int                         pinDrogueChute  {12}; // Pin to trigger the auxiliary recovery system (at apogee-displacementForRecoveryDetection)
  static constexpr int                           pinParachute   {3}; // Pin to trigger the main recovery system (at parachuteDeploymentAltitude)
  static constexpr uint32_t             actuatorDischargeTime {500}; // Time to discharge the capacitor of the actuator to deploy the parachute and the drogue (milliseconds)
  static constexpr uint32_t            capacitorRechargeTime {1000}; // Time to recharge the capacitor of the actuator (milliseconds)
  static constexpr uint8_t                                  N  {32}; // Number of time steps to calculate the flight statistics (must be a multiple of 4)
  static constexpr uint16_t                            deltaT {100}; // Time step between measurements (ms)
}


/*
  The error table is encoded in such a way that the errors can be
  composed and decomposed later. This is acomplished using a binary
  base. Each error must be an independent vector of the base. For 
  instance:
    Error 1, code 2 << 0 (2 in decimals or 00000001 in binary)
    Error 2, code 2 << 1 (4 in decimals or 00000010 in binary)
    Error 3, code 2 << 2 (8 in decimals or 00000100 in binary)
  Suppose that errors 1 and 3 must be enconded in a unique
  variable errorLog of 2 bytes in memory. This is performed 
  using the bitwise OR operator:
  errorLog = Error1 | Error3 = 00000101.
  To verify if some error ErrorN occured, one may use the bitwise
  AND operator:
    errorLog & ErrorN
  If the result is not zero, the error ErrorN is present. For instance,
     00000101 & 00000100 = 00000100.
*/

namespace error
{
  static constexpr uint16_t BarometerInitializationFailure  {2 << 0}; // Error 1
  static constexpr uint16_t ActuatorInitializationFailure   {2 << 1}; // Error 2
  static constexpr uint16_t AltitudeNegativeOverflow        {2 << 2}; // Error 3
  static constexpr uint16_t AltitudePositiveOverflow        {2 << 3}; // Error 4
  static constexpr uint16_t FlightStartedWithNonEmptyMemory {2 << 4}; // Error 5
}


/*
  Input communication codes 
*/
namespace icode
{
  static constexpr uint8_t readStaticParameters                {0};
  static constexpr uint8_t readDynamicParameters               {1};
  static constexpr uint8_t writeDynamicParameters              {2};
  static constexpr uint8_t restoreToFactoryParameters          {3};
  static constexpr uint8_t clearFlightMemory                   {4};
  static constexpr uint8_t readFlightReport                    {5};
  static constexpr uint8_t setSimulationMode                   {6};
  static constexpr uint8_t setSimulatedFlightAltitude          {7};
  static constexpr uint8_t setSpeedForLiftoffDetection         {8};
  static constexpr uint8_t setSpeedForFallDetection            {9};
  static constexpr uint8_t setSpeedForApogeeDetection         {10};
  static constexpr uint8_t setParachuteDeploymentAltitude     {11};
  static constexpr uint8_t setDisplacementForLandingDetection {12};
  static constexpr uint8_t setMaxNumberOfDeploymentAttempts   {13};
  static constexpr uint8_t setTimeStepScaler                  {14};
}

/*
  Output communication codes 
*/
namespace ocode
{
  static constexpr uint8_t errorLog                         {0};
  static constexpr uint8_t requestSimulatedAltitude         {1};
  static constexpr uint8_t simulatedFlightState             {2};
  static constexpr uint8_t flightPath                       {3};
  static constexpr uint8_t firmwareVersion                  {4};
  static constexpr uint8_t simulatedMode                    {5};
  static constexpr uint8_t startedInitialization            {6};
  static constexpr uint8_t finishedInitialization           {7};
  static constexpr uint8_t stardedSendingMemoryReport       {8};
  static constexpr uint8_t finishedSendingMemoryReport      {9};
  static constexpr uint8_t liftoffEvent                    {10};
  static constexpr uint8_t drogueEvent                     {11};
  static constexpr uint8_t parachuteEvent                  {12};
  static constexpr uint8_t landedEvent                     {13};
  static constexpr uint8_t actuatorDischargeTime           {14};
  static constexpr uint8_t capacitorRechargeTime           {15};
  static constexpr uint8_t N                               {16};
  static constexpr uint8_t deltaT                          {17}; 
  static constexpr uint8_t speedForLiftoffDetection        {18};
  static constexpr uint8_t speedForFallDetection           {19};
  static constexpr uint8_t speedForApogeeDetection         {20};
  static constexpr uint8_t parachuteDeploymentAltitude     {21}; 
  static constexpr uint8_t displacementForLandingDetection {22};
  static constexpr uint8_t maxNumberOfDeploymentAttempts   {23};
  static constexpr uint8_t timeStepScaler                  {24};
}

#endif // PARAMETERSSTATIC_H
