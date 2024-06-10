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

#ifndef KALMANALPHAFILTERFLIGHTSTATISTICSOPT_H
#define KALMANALPHAFILTERFLIGHTSTATISTICSOPT_H

/*
  KalmanAlphaFilterConstAccelerationFull applies the Kalman filter
  to the following physical model:

  s = s0 + v0 * t + a/2 * t*t,

  where s is the position, v0 is the initial velocity, a is the 
  constant acceleration and t is time. It also applies a dynamic
  alpha filter for smoothing
*/
class KalmanAlphaFilterFlightStatistics
{
public:
  /*
    Initializes the filter
        s0: initial position (m)
        dT: time step (s)
    stdExp: standard deviation of position measurements (m)
    stdMod: standard deviation of acceleration of the physical model (m/s2)
  */ 
  void begin(float s0, float dT, float stdExp, float stdModSub, float stdModTra, float dadt_ref);

  /*
    Process the new state (position, velocity and acceleration) given 
    the measurement of the current position
  */ 
  void process(const float& sMeasured);

public:
  // Variables of public access
  float s; // Position
  float v; // Velocity
  float a; // Acceleration

  float vs; // Smoothed velocity (alpha filter)

private:

  // Kalman filter parameters
  float         T; // Time step
  float        T2; // T*T/2
  float      Vexp; // Variance of the measured values
  float      Vmod; // Variance of the physical model
  float   VmodSub; // Variance of the physical model for subsonic speed
  float   VmodTra; // Variance of the physical model for transonic speed (v > 200m/s)
  float        P00, P01, P02, P11, P12, P22; // Covariance matrix (a measure of the estimated accuracy of the state estimate)
  float  Ph00, Ph01, Ph02, Ph11, Ph12, Ph22; // Predicted covariance matrix
  float                          K0, K1, K2; // Kalman gain

  // Alpha filter parameters
  float da_ref_inv; // Inverse of the reference variation of acceleration within T: 1/(da/dt|ref * T)
};

#endif