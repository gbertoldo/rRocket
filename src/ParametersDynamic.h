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

#ifndef DYNAMICPARAMETERS
#define DYNAMICPARAMETERS

#include <inttypes.h>

struct FlightParameters
{
  int16_t             speedForLiftoffDetection  {30}; // Speed for liftoff detection (m/s)
  int16_t                speedForFallDetection  {30}; // Speed for fall detection (m/s)
  int16_t              speedForApogeeDetection   {3}; // Speed for apogee detection (m/s)
  int16_t          parachuteDeploymentAltitude {200}; // Altitude to deploy the main parachute (m)
  int16_t      displacementForLandingDetection   {3}; // Displacement for landing detection (meter)
  int16_t        maxNumberOfDeploymentAttempts   {3}; // Maximum number of deployment attempts
  int16_t                       timeStepScaler  {10}; // Scaler for adaptive deltaT
};

#endif