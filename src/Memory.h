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

#ifndef MEMORY_H
#define MEMORY_H

#include <EEPROM.h>
#include "ParametersDynamic.h"

/*

  Class Memory uses EEPROM to store the data of the flight.

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

    /* 
      Returns the maximum number of slots to store the altitudes 
      during the flight. Each slot occupies 2 bytes.
    */
    uint16_t getNumberOfSlots(){return numberOfSlots;};

    // Returns the number of the last slot written
    uint16_t getNumberOfSlotsWritten(){return numberOfSlotsWritten;};

    // Reads the altitude from EEPROM memory at slot position i (0 <= i <= getNumberOfSlots())
    float readAltitude(const uint16_t& i);

    // Writes the altitude at EEPROM memory at slot position i (0 <= i <= getNumberOfSlots())
    bool writeAltitude(const uint16_t& i, float altitude);

    // Appends the altitude to EEPROM memory
    bool appendAltitude(float altitude);

    // Get apogee from EEPROM memory
    float readApogee();

    /* 
      Writes the deltaTMultiplier of the event c to the memory
      'F': flight detected
      'D': drogue activated
      'P': parachute activated
      'L': landed
       t = deltaTMultiplier x deltaT (milliseconds) 
    */
    void writeEvent(const char& c, const uint16_t& deltaTMultiplier);

    // Reads deltaTMultiplier of the event c from the memory (see writeEvent for more details)
    uint16_t readEvent(const char& c);

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

    // Write flight parameters
    void writeFlightParameters(const FlightParameters& p);

    // Write flight parameters
    FlightParameters readFlightParameters();

  private:

    // Position of the memory where the data are written
    static constexpr uint16_t addrFlightParameters         {0};
    static constexpr uint16_t addrErrorLog                 {sizeof(FlightParameters)/sizeof(byte)};
    static constexpr uint16_t addrliftoffEvent              {addrErrorLog+2};
    static constexpr uint16_t addrDrogueEvent              {addrliftoffEvent+2};
    static constexpr uint16_t addrParachuteEvent           {addrDrogueEvent+2};
    static constexpr uint16_t addrLandedEvent              {addrParachuteEvent+2};
    static constexpr uint16_t addrAltitudesBegin           {addrLandedEvent+2};

    // Number of slots written in the memory (refers to the last slot written)
    uint16_t numberOfSlotsWritten {0};

    // Number of slots available in the memory
    uint16_t numberOfSlots {0};
};

#endif // MEMORY_H
