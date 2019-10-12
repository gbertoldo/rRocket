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

};

#endif // ACTUATOR_H
