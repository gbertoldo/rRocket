

    /**************************************************************************\
   /            rRocket: An Arduino powered rocketry recovery system            \
  /              Federal University of Technology - Parana - Brazil              \
  \                by Guilherme Bertoldo and Jonas Joacir Radtke                 /
   \                         updated October 12, 2019                           /
    \**************************************************************************/


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
