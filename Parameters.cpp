

    /**********************************************************************\
   /          rRocket: An Arduino powered rocketry recovery system          \
  /            Federal University of Technology - Parana - Brazil            \
  \              by Guilherme Bertoldo and Jonas Joacir Radtke               /
   \                       updated October 18, 2019                         /
    \**********************************************************************/


#include "Parameters.h"

namespace Parameters
{

  const char*                      softwareVersion {"rRocket v.1.2"};
  const int                                 pinLed {13};
  const int                              pinBuzzer {6};
  const int                              pinButton {10};
  const int                     pinButtonVCCSource {12};
  const int                         pinDrogueChute {3};
  const int                           pinParachute {5};
  const float      displacementForLiftoffDetection {10};
  const float displacementForDrogueChuteDeployment {5};
  const float          parachuteDeploymentAltitude {200};
  const float     displacementForRecoveryDetection {2.0};
  const unsigned long int    timeDrogueChuteActive {500}; 
  const unsigned long int      timeParachuteActive {500};

}
