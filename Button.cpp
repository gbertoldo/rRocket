#include "Arduino.h"
#include "Button.h"

bool Button::begin(unsigned short pin, unsigned long int refTime)
{

  pinMode(pin, INPUT);

  this->pin = pin;

  this->refTime = refTime;
  
}


ButtonState Button::getState()
{ 

    // Reading the current pin state (pressed=1 or released=0)
    cstate = digitalRead(pin);

    // If button is currently realeased
    if ( cstate == 0 ) {

        // If button was released before
        if ( pstate == 0 ) {

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
        if ( pstate == 0 ) {

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
