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
