#ifndef KALMANFILTERFLIGHTSTATISTICSOPT3_H
#define KALMANFILTERFLIGHTSTATISTICSOPT3_H

/*
  KalmanFilterConstAccelerationFull applies the Kalman filter
  to the following physical model:

  s = s0 + v0 * t + a/2 * t*t,

  where s is the position, v0 is the initial velocity, a is the 
  constant acceleration and t is time.
*/
class KalmanFilterFlightStatistics
{
public:
  /*
    Initializes the filter
        dT: time step (s)
    stdExp: standard deviation of position measurements (m)
    stdMod: standard deviation of acceleration of the physical model (m/s2)
  */ 
  void begin(float dT, float stdExp, float stdModSub, float stdModTra);

  // Resets the filter to the initial condition
  void reset();

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

private:

  float         T; // Time step
  float        T2; // T*T/2
  float      Vexp; // Variance of the measured values
  float      Vmod; // Variance of the physical model
  float   VmodSub; // Variance of the physical model for subsonic speed
  float   VmodTra; // Variance of the physical model for transonic speed (v > 200m/s)
  float        P00, P01, P02, P11, P12, P22; // Covariance matrix (a measure of the estimated accuracy of the state estimate)
  float  Ph00, Ph01, Ph02, Ph11, Ph12, Ph22; // Predicted covariance matrix
  float                          K0, K1, K2; // Kalman gain
};

#endif