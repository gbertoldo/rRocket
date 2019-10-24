

    /**********************************************************************\
   /          rRocket: An Arduino powered rocketry recovery system          \
  /            Federal University of Technology - Parana - Brazil            \
  \              by Guilherme Bertoldo and Jonas Joacir Radtke               /
   \                       updated October 18, 2019                         /
    \**********************************************************************/


#ifndef ACTUATOR_H
#define ACTUATOR_H

/*

   Actuator effectively deploys chute and drogue chute after

   RecoverySystem command

*/

class Actuator
{
  
  public:

    // Initializes Actuator object
    bool begin();

    // Deploys main parachute
    void deployParachute();

    // Deploys drogue chute
    void deployDrogueChute();

  private:

    unsigned long int timeDrogueChuteWasActivated {0};          // time in that drogue chute was activated
    unsigned long int timeParachuteWasActivated {0};            // time in that parachute was activated

};

#endif // ACTUATOR_H
