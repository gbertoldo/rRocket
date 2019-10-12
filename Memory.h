#ifndef MEMORY_H
#define MEMORY_H

// Memory states
enum class MemoryState {empty, full, writing};

/*

   Read from and write into EEPROM memory

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

    // Read EEPROM memory
    float readAltitude();

    // Write into EEPROM memory
    bool writeAltitude(float);

    // Get apogee
    float readApogee();

    // Write into EEPROM memory
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
    int memorySize;

};

#endif // MEMORY_H
