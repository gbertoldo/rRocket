/*
  The MIT License (MIT)

  Copyright (C) 2022 Guilherme Bertoldo and Jonas Joacir Radtke
  (UTFPR) Federal University of Technology - Parana

  Permission is hereby granted, free of charge, to any person obtaining a 
  copy of this software and associated documentation files (the “Software”), 
  to deal in the Software without restriction, including without limitation 
  the rights to use, copy, modify, merge, publish, distribute, sublicense, 
  and/or sell copies of the Software, and to permit persons to whom the Software 
  is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all 
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN 
  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

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
       |              **     An Arduino-powered rocketry recovery system    **
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
  // Initializing the communication
  Serial.begin(115200);

  // Initializing recovery system
  recoverySystem.begin(false);
}


void loop() {

  // Running recovery system
  recoverySystem.run();

}
