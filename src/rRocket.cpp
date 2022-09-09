/*

       ^
       |
       |                                        ****
       |                                  ******    *****
       |                              *****             ******
       |                           ***                        ****
       |                        ***                              ***
       |                      **                                   ****
       |                    **                                        ***
       |                 ***                  rRocket                    *
       |                **                                                ***
       |              **     An Arduino powered rocketry recovery system    **
       |            ***                                                       **
       |           **                                                          *
       |         **
       |         *                             UTFPR
       |       **
       |      *               Federal University of Technology - Parana
       |    **
       |   *                   FRANCISCO BELTRAO  -  PARANA  -  BRAZIL
       |  **
       | *                  by Guilherme Bertoldo and Jonas Joacir Radtke
       |*
       +-------------------------------------------------------------------------------->

*/

#include "RecoverySystem.h"


// Creates an instance of RecoverySystem class
RecoverySystem recoverySystem;


void setup()
{

  // Initializing recovery system
  recoverySystem.begin();

}


void loop() {

  // Running recovery system
  recoverySystem.run();

}
