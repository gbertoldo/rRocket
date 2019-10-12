

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
    void erase();

    // Restart position
    void restartPosition();
    
    // Is there a next value?
    bool hasNext();

  private:

    // Set the memory state
    void setState(MemoryState);

  private:

    // Position of the memory
    unsigned int position;

    // Apogee
    unsigned int apogee;

    // Memory state
    MemoryState state;

    // EEPROM total size (bytes)
    unsigned int memorySize;

};

#endif // MEMORY_H
