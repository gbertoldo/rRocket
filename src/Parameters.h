

    /**********************************************************************\
   /          rRocket: An Arduino powered rocketry recovery system          \
  /            Federal University of Technology - Parana - Brazil            \
  \              by Guilherme Bertoldo and Jonas Joacir Radtke               /
   \                       updated September 12, 2022                       /
    \**********************************************************************/


#ifndef PARAMETERS_H
#define PARAMETERS_H

namespace Parameters
{
  static constexpr char       softwareVersion[] {"rRocket v.1.4.5"}; // Version of this software
  static constexpr int                                 pinLed  {13}; // Screen uses pin 13 to blink Arduino internal led
  static constexpr int                              pinBuzzer  {11}; // Pin of buzzer
  static constexpr int                              pinButton  {10}; // Pin of button
  static constexpr int                         pinDrogueChute  {12}; // Pin to trigger the auxiliary recovery system (at apogee-displacementForRecoveryDetection)
  static constexpr int                           pinParachute   {3}; // Pin to trigger the main recovery system (at parachuteDeploymentAltitude)
  static constexpr float      displacementForLiftoffDetection  {30}; // Displacement from the ground for liftoff detection (meter)
  static constexpr float displacementForDrogueChuteDeployment   {5}; // Displacement from apogee for activation of drogue chute (meter)
  static constexpr float          parachuteDeploymentAltitude {200}; // Altitude to deploy main parachute (meter)
  static constexpr float     displacementForRecoveryDetection {2.0}; // Displacement for recovery detection (meter)
  static constexpr unsigned long int    timeDrogueChuteActive {500}; // The drogue chute is activated by timeDrogueChuteActive milliseconds. After that, the signal is turned off.
  static constexpr unsigned long int      timeParachuteActive {500}; // The drogue chute is activated by timeParachuteActive milliseconds. After that, the signal is turned off.
}

#endif // PARAMETERS_H
