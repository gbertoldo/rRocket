

    /**************************************************************************\
   /            rRocket: An Arduino powered rocketry recovery system            \
  /              Federal University of Technology - Parana - Brazil              \
  \                by Guilherme Bertoldo and Jonas Joacir Radtke                 /
   \                         updated October 12, 2019                           /
    \**************************************************************************/


#ifndef MEMORY_H
#define MEMORY_H

// Memory states
enum class MemoryState {empty, full};

/*

   Read from and write to EEPROM memory

*/

class Memory
{

  public:

    // Initializes
    bool begin();

    // Get memory size
    int getSize();

    // Returns the memory state
    MemoryState getState();

    // Read altitude from EEPROM memory
    float readAltitude();

    // Write altitude to EEPROM memory
    bool writeAltitude(float);

    // Get apogee from EEPROM memory
    float readApogee();

    // Write apogee to EEPROM memory
    bool writeApogee();

    // Erase memory
    bool erase();

    // Set the memory state
    bool setState(MemoryState);

    // Restart position
    bool restartPosition();

  private:

    // Position of the memory
    unsigned short position;

    // Apogee
    unsigned short apogee;

    // Memory state
    MemoryState state;

    // EEPROM total size (bytes)
    unsigned int memorySize;

};

#endif // MEMORY_H
