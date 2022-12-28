

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
  static constexpr char       softwareVersion[] {"rRocket v.1.5.0"}; // Version of this software
  static constexpr int                                 pinLed  {13}; // Screen uses pin 13 to blink Arduino internal led
  static constexpr int                              pinBuzzer  {11}; // Pin of buzzer
  static constexpr int                              pinButton  {10}; // Pin of button
  static constexpr int                         pinDrogueChute  {12}; // Pin to trigger the auxiliary recovery system (at apogee-displacementForRecoveryDetection)
  static constexpr int                           pinParachute   {3}; // Pin to trigger the main recovery system (at parachuteDeploymentAltitude)
  
  static constexpr uint8_t           speedForLiftoffDetection  {30}; // Speed for liftoff detection (m/s)
  static constexpr uint8_t              speedForFallDetection  {30}; // Speed for fall detection (m/s)
  static constexpr uint8_t            speedForApogeeDetection   {7}; // Speed for apogee detection (m/s)
  static constexpr uint16_t       parachuteDeploymentAltitude {200}; // Altitude to deploy the main parachute (m)
  static constexpr uint8_t    displacementForLandingDetection   {3}; // Displacement for landing detection (meter)
  static constexpr unsigned long int    timeDrogueChuteActive {500}; // The drogue chute is activated by timeDrogueChuteActive milliseconds. After that, the signal is turned off.
  static constexpr unsigned long int      timeParachuteActive {500}; // The drogue chute is activated by timeParachuteActive milliseconds. After that, the signal is turned off.

}

#endif // PARAMETERS_H
