

    /**********************************************************************\
   /          rRocket: An Arduino powered rocketry recovery system          \
  /            Federal University of Technology - Parana - Brazil            \
  \              by Guilherme Bertoldo and Jonas Joacir Radtke               /
   \                       updated October 18, 2019                         /
    \**********************************************************************/


#ifndef PARAMETERS_H
#define PARAMETERS_H

namespace Parameters
{

extern const int                                 pinLed; // Screen uses pin 13 in order to blink Arduino internal led
extern const int                              pinBuzzer; // Pin of buzzer
extern const int                              pinButton; // Pin of button
extern const int                     pinButtonVCCSource; // Pin used in the button as 5V
extern const int                         pinDrogueChute; // Pin to trigger the auxiliary recovery system (at apogee-displacementForRecoveryDetection)
extern const int                           pinParachute; // Pin to trigger the main recovery system (at parachuteDeploymentAltitude)
extern const float      displacementForLiftoffDetection; // Displacement from ground for liftoff detection (meter)
extern const float displacementForDrogueChuteDeployment; // Displacement from apogee for activation of drogue chute (meter)
extern const float          parachuteDeploymentAltitude; // Altitude to deploy main parachute (meter)
extern const float     displacementForRecoveryDetection; // Displacement for recovery detection (meter)
extern const unsigned long int    timeDrogueChuteActive; // Drogue chute is activated by timeDrogueChuteActive milliseconds. After that, signal is turned off.
extern const unsigned long int      timeParachuteActive; // Drogue chute is activated by timeParachuteActive milliseconds. After that, signal is turned off.

}

#endif // PARAMETERS_H
