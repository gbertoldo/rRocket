

    /**********************************************************************\
   /          rRocket: An Arduino powered rocketry recovery system          \
  /            Federal University of Technology - Parana - Brazil            \
  \              by Guilherme Bertoldo and Jonas Joacir Radtke               /
   \                       updated September 12, 2022                       /
    \**********************************************************************/


#ifndef MEMORY_H
#define MEMORY_H

#include <EEPROM.h>

/*

  Class Memory uses EEPROM to store the data of the flight.

  The EEPROM is organized as follow:
    - Addresses  0 to 1   (2 bytes): reserved to store the log of errors of the last flight
    - Addresses  2 to ... (? bytes): reserved to store the altitude from the launching point (AGL) during the flight
                                      The first position is reserved to store the apogee.

  Since measurements are made in known time steps, only the altitude AGL is stored. The altitude
  is converted from float (4 bytes) to unsigned integers uint16_t (2 bytes) to reduce memory consumption.
  The stored altitude h' is related to the original altitude h as follows:
    h' = ( h + 500 ) * 10
  This relation adds 500 meters to the original altitude, to avoid negative values, and converts it from 
  meter to decimeters, to keep the precision of the barometer.
  The range of uint16_t is 0 to 2^16=65536.
  If h' is less than zero, then the new value stored is
    h" = - h'  
  If h' > 65000, then the new value stored is
    h" = h' % 65000
*/

class Memory
{

  public:

    // Initializes
    bool begin();

    // Returns the total amount of memory from EEPROM (bytes)
    uint16_t getMemorySize(){return EEPROM.length();};

    /* 
      Returns the maximum number of slots to store the altitudes 
      during the flight. Each slot occupies 2 bytes.
    */
    uint16_t getNumberOfSlots(){return numberOfSlots;};

    // Returns the number of slots written so far
    uint16_t getNumberOfSlotsWritten(){return numberOfSlotsWritten;};

    // Reads the altitude from EEPROM memory at slot position i (0 <= i <= getNumberOfSlots())
    // Position i = 0, returns the apogee.
    float readAltitude(const uint16_t& i);

    // Writes the altitude at EEPROM memory at slot position i (0 <= i <= getNumberOfSlots())
    // Position i = 0 is reserved to the apogee.
    bool writeAltitude(const uint16_t& i, float altitude);

    // Appends the altitude to EEPROM memory
    bool appendAltitude(float altitude);

    // Get apogee from EEPROM memory
    float readApogee(){return readAltitude(0);};

    // Write apogee to EEPROM memory
    bool writeApogee(float apogee){return writeAltitude(0, apogee);};

    // Returns the log of errors of the last flight
    uint16_t readErrorLog()
    {
      uint16_t errorLog = 0;

      EEPROM.get(addrErrorLog, errorLog);

      return errorLog;
    }

    // Writes an error to the error log
    void writeErrorLog(uint16_t errorLog)
    {
      /*
        In the following line, it is used a bitwise  
        manipulation (OR operator) to encode a list of errors. 
        For example
        error 1, code = 00000001 (binary)
        error 2, code = 00000010 (binary)
        error 3, code = 00000100 (binary)

        error1 | error3 = 00000101 (binary)
      */
      errorLog = readErrorLog() | errorLog;

      EEPROM.put(addrErrorLog, errorLog);
    }

    // Erase memory
    void erase();

  private:

    // Position of the memory where the data are written
    static constexpr uint16_t addrErrorLog                 {0};
    static constexpr uint16_t addrAltitudesBegin           {2};

    // Number of slots written in the memory
    uint16_t numberOfSlotsWritten {0};

    // Number of slots available in the memory
    uint16_t numberOfSlots {0};
};

#endif // MEMORY_H
