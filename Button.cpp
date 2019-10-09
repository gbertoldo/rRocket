#include "Arduino.h"
#include "Button.h"


bool Button::begin(int pin)
{

  this->pin = pin;

  pinMode(pin, INPUT);

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

      unsigned long T1 = millis() - T0;

      // If short pressed
      if ( 5 < T1 && T1 < 3000 ) {

        return ButtonState::pressed_and_released;

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

      if ( (millis() - T0) >= 3000 ) {

        return ButtonState::long_pressed;

      }
      else
      {
        return ButtonState::pressed;
      }

    }

  }

};
