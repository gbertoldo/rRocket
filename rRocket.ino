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


// Creates an instance of Recovery class
RecoverySystem recoverySystem;


void setup()
{

  // Initializing recovery
  recoverySystem.begin();

}


void loop() {

  // Running recovery
  recoverySystem.run();

}
