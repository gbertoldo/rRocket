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

#include "KalmanAlphaFilterFlightStatistics.h"
#include <math.h>

void KalmanAlphaFilterFlightStatistics::begin(float s0, float dT, float stdExp, float stdModSub, float stdModTra, float dadt_ref)
{
  T    =              dT; // Time step
  T2   =       dT*dT*0.5; // Auxiliary variable
  Vexp = stdExp * stdExp; // Variance of the measurements
  VmodSub = stdModSub * stdModSub; // Variance of the physical model for subsonic speed
  VmodTra = stdModTra * stdModTra; // Variance of the physical model for subsonic speed

  da_ref_inv = 1.0 / ( dadt_ref * T);

  s=s0;
  v=0.0;
  a=0.0;

  vs = v;
  
  Vmod = VmodSub;
  
  P00 = Vexp+Vmod*T*T*T*T*T*T/36.0;
  P11 = Vexp/T+Vmod*T*T*T*T/4.0;
  P22 = Vexp/(T*T);
  P01 = Vmod*T*T*T*T*T/12.0;
  P02 = Vmod*T*T*T*T/6.0;
  P12 = Vmod*T*T*T/2.0;
  
  K0 = 0.0;
  K1 = 0.0;
  K2 = 0.0;
}


void KalmanAlphaFilterFlightStatistics::process(const float& sMeasured)
{
  /****************************
      Prediction step
  ****************************/
  s += v*T + a*T2;
  v +=       a*T;
  //a = a;
  float a0 = a;

  if ( v > 170 )
  {
    Vmod = VmodTra;
  }
  else
  {
    Vmod = VmodSub;
  }
  
  float c1 = P22*T2;
  float c2 = P22*T;

  Ph00 = (c1+2.0*P12*T+2.0*P02)*T2+(P11*T+2.0*P01)*T+P00;
  Ph11 = (c2+2.0*P12)*T+P11;
  Ph22 = P22+Vmod*T*T;
  Ph01 = (c2+P12)*T2+(P12*T+P02+P11)*T+P01;
  Ph02 = c1+P12*T+P02;
  Ph12 = c2+P12;
  
  
  /****************************
      Update step
  ****************************/
  float Sinv = 1.0/(Ph00+Vexp);

  // Updating the Kalman gain
  K0=Ph00*Sinv;
  K1=Ph01*Sinv;
  K2=Ph02*Sinv;

  // Calculating the innovation (measured value - predicted value)
  float innovation = sMeasured - s;

  // Calculating the new estimate state
  s += K0*innovation;
  v += K1*innovation;
  a += K2*innovation;
  
  // Calculating the new estimate covariance
  float raux = 1.0-K0; 
  P00 = raux*Ph00;
  P01 = raux*Ph01;
  P02 = raux*Ph02;
  P11 = Ph11-K1*Ph01;
  P12 = Ph12-K1*Ph02;
  P22 = Ph22-K2*Ph02;

  // Alpha filter
  vs += (v-vs)/(1.0+fabs(a-a0)*da_ref_inv);
};