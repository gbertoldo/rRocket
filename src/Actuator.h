

    /**********************************************************************\
   /          rRocket: An Arduino powered rocketry recovery system          \
  /            Federal University of Technology - Parana - Brazil            \
  \              by Guilherme Bertoldo and Jonas Joacir Radtke               /
   \                       updated October 18, 2019                         /
    \**********************************************************************/


#ifndef ACTUATOR_H
#define ACTUATOR_H
#include <inttypes.h>

/*

   Actuator effectively deploys chute and drogue chute after

   RecoverySystem command

*/

class Actuator
{
  
  public:

    // Initializes Actuator object
    bool begin();

    // Resets the Actuator
    void reload();

    /* 
      Deploys the main parachute. 
      Returns true if the deployment has been finished.
      Otherwise, returns false. 
    */
    bool deployParachute(bool stopCondition);

    /* 
      Deploys the drogue parachute. 
      Returns true if the deployment has been finished.
      Otherwise, returns false. 
    */
    bool deployDrogueChute(bool stopCondition);

  private:

    /* 
      Generic method to deploys the parachute or the drogue parachute. 
      Returns true if the deployment has been finished.
      Otherwise, returns false. 
    */
    bool deploy(uint8_t pin, bool stopCondition);

public:

    uint8_t deployCounter {0};                // counts the number of activations

  private:

    unsigned long int lastTimeActivated {0};  // last instant where the parachute or drogue chute was activated
};

#endif // ACTUATOR_H
