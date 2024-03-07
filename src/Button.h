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

#ifndef BUTTON_H
#define BUTTON_H


/*
  Button states
              released: at time button state was read, it was released
               pressed: at time button state was read, it was pressed by a time less than refTime (user defined reference time)
    pressedAndReleased: at time button state was read, it was released after had been pressed by a time less than refTime (user defined reference time)
           longPressed: at time button state was read, it was pressed by a time greater or equal to refTime (user defined reference time)
*/


enum class ButtonState {released, pressed, pressedAndReleased, longPressed};


/*

   Controls the button state

*/

class Button
{

  public:

    // Initializes
    bool begin(unsigned short pin, unsigned long int refTime = 1500);

    // Returns the button state
    ButtonState getState();

  private:

    // Arduino pin associated to the button
    unsigned short pin;

    // Reference time to long pressed button state (milliseconds)
    unsigned long int refTime {1500};

    // Previous button state (pressed=1 or released=0)
    unsigned short pstate{0};

    // Current button state (pressed=1 or released=0)
    unsigned short cstate{0};

    // Stores the time when button was first pressed after a release
    unsigned long int T0;

};

#endif // BUTTON_H
