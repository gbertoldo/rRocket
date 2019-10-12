#ifndef TIMERCOUNTDOWN_H
#define TIMERCOUNTDOWN_H

/*

   Countdown timer

*/

class TimerCountdown
{

  public:

    // Initializes timer with a target time (milliseconds).
    void begin(unsigned long int targetTime);

    // Returns true if target time was reached
    bool isRinging();

    // Resets countdown
    void reset();

    // If ringing, restarts countdown, but considering time elapsed since ringing started
    void rearm();

  private:

    unsigned long int      targetTime; // Countdown time (milliseconds)
    unsigned long int     initialTime; // Initial time (at initialization, reset or rearm)

};

#endif // TIMERCOUNTDOWN_H
