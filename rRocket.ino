/*

       ^
       |
       |                                        ____
       |                                    ****    ****
       |                              *****             ******
       |                           ***                        ****
       |                        ***                              ***
       |                      **                                   ****
       |                    **                rRocket                 ***
       |                 ***                                             *
       |                **       An Arduino powered recovery system       ***
       |              **                                                    **
       |            ***                                                       **
       |           **                                                          *
       |         **
       |         *                             UTFPR
       |       **
       |      *               Federal University of Technology - Parana
       |    **
       |   *                   FRANCISCO BELTRAO  -  PARANA  -  BRAZIL
       |  **
       | *                  by Jonas Joacir Radtke and Guilherme Bertoldo
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
