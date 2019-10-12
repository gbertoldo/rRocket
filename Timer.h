

    /**************************************************************************\
   /            rRocket: An Arduino powered rocketry recovery system            \
  /              Federal University of Technology - Parana - Brazil              \
  \                by Guilherme Bertoldo and Jonas Joacir Radtke                 /
   \                         updated October 12, 2019                           /
    \**************************************************************************/


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
