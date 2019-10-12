/**************************************************************************\
  /            rRocket: An Arduino powered rocketry recovery system            \
  /              Federal University of Technology - Parana - Brazil              \
  \                by Guilherme Bertoldo and Jonas Joacir Radtke                 /
  \                         updated September 13, 2019                         /
  \**************************************************************************/
#include "Parameters.h"

namespace Parameters
{

const int                                 pinLed  {13};
const int                              pinBuzzer   {3};
const int                              pinButton  {12};
const int                           pinParachute   {5};
const int                          pinDrogeChute   {7};
const float      displacementForLiftoffDetection  {10};
const float displacementForDrogueChuteDeployment   {5};
const float          parachuteDeploymentAltitude {200};
const float      displacementForRecoverDetection {0.5};

}
