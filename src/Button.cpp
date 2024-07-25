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
#include "Arduino.h"
#include "Button.h"

bool Button::begin(unsigned short pin, unsigned long int refTime)
{

  pinMode(pin, INPUT_PULLUP);

  this->pin = pin;

  this->refTime = refTime;
  
  return true;
}


ButtonState Button::getState()
{ 

    // Reading the current pin state (pressed=ON or released=OFF)
    cstate = digitalRead(pin);

    // If button is currently realeased
    if ( cstate == OFF ) {

        // If button was released before
        if ( pstate == OFF ) {

            pstate = cstate;

            return ButtonState::released;
            
        } 
        // If button was pressed before
        else 
        {
        
            pstate = cstate;

            unsigned long int T1 = millis()-T0;

            // If short pressed
            if ( 5 < T1 && T1 < refTime ) {

                return ButtonState::pressedAndReleased;
              
            }
            else
            {
                return ButtonState::released;
            }               
        }
    }
    // If button is currently pressed
    else
    {
        // If button was released before
        if ( pstate == OFF ) {

            pstate = cstate;

            T0 = millis();

            return ButtonState::pressed;
            
        } 
        // If button was pressed before
        else 
        {
        
            pstate = cstate;

            if ( (millis()-T0) >= refTime ) {

                return ButtonState::longPressed;
              
            }
            else
            {
                return ButtonState::pressed;
            }
                            
        }
        
    }
  
};
