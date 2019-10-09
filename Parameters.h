  /**************************************************************************\
 /            rRocket: An Arduino powered rocketry recovery system            \
/              Federal University of Technology - Parana - Brazil              \
\                by Guilherme Bertoldo and Jonas Joacir Radtke                 /
 \                         updated September 13, 2019                         /
  \**************************************************************************/
#ifndef PARAMETERS_H
#define PARAMETERS_H

namespace Parameters
{

extern const int pinLed;                // Screen uses pin 13 in order to blink Arduino internal led
extern const int pinBuzzer;             // Pin of buzzer
extern const int pinButton;             // Pin of button
extern const int pinDrogueChute;        // Pin to trigger the auxiliary recovery system (in apogee)
extern const int pinParachute;          // Pin to trigger the main recovery system (in arbitrary height)
extern const int parachuteDeployHeight; // Altitude to trigger of main recovery system

}

#endif // PARAMETERS_H
