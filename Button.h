#ifndef BUTTON_H
#define BUTTON_H

// Button states
enum class ButtonState {released, pressed, pressed_and_released, long_pressed};

/*

   Controls the button state

*/

class Button
{

  public:

    // Initializes
    bool begin(int pin);

    // Returns the button state
    ButtonState getState();

  private:

    // Arduino pin associated to the button
    int pin;

    // Previous button state (pressed=1 or released=0)
    int pstate{0};

    // Current button state (pressed=1 or released=0)
    int cstate{0};

    // Stores the time when button was first pressed after a release
    unsigned long T0;

};

#endif // BUTTON_H
