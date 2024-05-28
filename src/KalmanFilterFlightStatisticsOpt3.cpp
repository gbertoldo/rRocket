#include "KalmanFilterFlightStatisticsOpt3.h"

void KalmanFilterFlightStatistics::begin(float dT, float stdExp, float stdModSub, float stdModTra)
{
  T    =              dT; // Time step
  T2   =       dT*dT*0.5; // Auxiliary variable
  Vexp = stdExp * stdExp; // Variance of the measurements
  VmodSub = stdModSub * stdModSub; // Variance of the physical model for subsonic speed
  VmodTra = stdModTra * stdModTra; // Variance of the physical model for subsonic speed
  reset();
};


void KalmanFilterFlightStatistics::reset()
{
  s=0.0;
  v=0.0;
  a=0.0;
  
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


void KalmanFilterFlightStatistics::process(const float& sMeasured)
{
  /****************************
      Prediction step
  ****************************/
  s += v*T + a*T2;
  v +=       a*T;
  //a = a;
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
};