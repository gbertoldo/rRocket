

    /**********************************************************************\
   /          rRocket: An Arduino powered rocketry recovery system          \
  /            Federal University of Technology - Parana - Brazil            \
  \              by Guilherme Bertoldo and Jonas Joacir Radtke               /
   \                       updated December 27, 2022                        /
    \**********************************************************************/


#ifndef PARAMETERS_H
#define PARAMETERS_H

namespace Parameters
{
  static constexpr char       softwareVersion[] {"rRocket v.1.5.3"}; // Version of this software
  static constexpr int                                 pinLed  {13}; // Screen uses pin 13 to blink Arduino internal led
  static constexpr int                              pinBuzzer  {11}; // Pin of buzzer
  static constexpr int                              pinButton  {10}; // Pin of button
  static constexpr int                         pinDrogueChute  {12}; // Pin to trigger the auxiliary recovery system (at apogee-displacementForRecoveryDetection)
  static constexpr int                           pinParachute   {3}; // Pin to trigger the main recovery system (at parachuteDeploymentAltitude)
  
  static constexpr float             speedForLiftoffDetection  {30}; // Speed for liftoff detection (m/s)
  static constexpr float                speedForFallDetection  {30}; // Speed for fall detection (m/s)
  static constexpr float              speedForApogeeDetection   {7}; // Speed for apogee detection (m/s)
  static constexpr float          parachuteDeploymentAltitude {200}; // Altitude to deploy the main parachute (m)
  static constexpr float      displacementForLandingDetection   {3}; // Displacement for landing detection (meter)
  static constexpr uint8_t      maxNumberOfDeploymentAttempts   {3}; // Maximum number of deployment attempts
  static constexpr uint32_t             actuatorDischargeTime {500}; // Time to discharge the capacitor of the actuator to deploy the parachute and the drogue (milliseconds)
  static constexpr uint32_t             capacitorRechargeTime {500}; // Time to recharge the capacitor of the actuator (milliseconds)

}

#endif // PARAMETERS_H
