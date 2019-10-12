#ifndef TIMER_H
#define TIMER_H

/*

   Timer

*/

class Timer
{

  public:

    // Elapsed time since last reset (milliseconds)
    unsigned long int elapsedTime();

    // Resets elapsed time
    void reset();

  private:

    unsigned long int     initialTime; // Initial time for calculating time elapsed
};

#endif // TIMER_H
