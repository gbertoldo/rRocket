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

#include "Arduino.h"
#include "Memory.h"
#include "ParametersStatic.h"

bool Memory::begin()
{
  // Number of available slots to save altitudes during flight
  numberOfSlots = (uint16_t)(((float)EEPROM.length()-addrAltitudesBegin)/2.0)-1;

  // Reading from memory the number of altitude registers
  numberOfSlotsWritten = 0;
  for ( uint16_t i = 0; i < numberOfSlots; ++i)
  {
    uint16_t iAltitude, address;

    address = 2*i+addrAltitudesBegin;

    EEPROM.get(address, iAltitude);

    if ( iAltitude == 0xFF ) 
    {
      break;
    }
    else
    {
      numberOfSlotsWritten++;
    }
  }
  return true;
}

float Memory::readAltitude(const uint16_t& i)
{
  // If the position is out of range, returns 0
  if ( i >= numberOfSlots ) return 0.0;

  // The altitude is written as unsigned integers of 16 bits.
  // It must be converted to float.
  uint16_t iAltitude;
  float    fAltitude;

  // Calculating the address in memory of slot i
  uint16_t address = 2 * i + addrAltitudesBegin;

  // Reading the altitude (decimeters)
  EEPROM.get(address, iAltitude);

  // Converts the altitude from decimeter to meter and removes the 500 m added
  // when the altitude was saved. 
  fAltitude = ((float)iAltitude)*0.1-500.0;

  return fAltitude;
}

bool Memory::writeAltitude(const uint16_t& i, float fAltitude)
{
  // If the position is out of range, returns 0
  if ( i >= numberOfSlots ) return false;

  fAltitude = fAltitude + 500.0; // increase 500 meters to write positive altitudes

  // Checking if the altitude is still negative
  if (fAltitude < 0) 
  {
    fAltitude = -fAltitude;

    // Registering altitude negative overflow error in the log
    writeErrorLog(error::AltitudeNegativeOverflow);
  }

  // Converting meters to decimeters
  fAltitude = 10.0 * fAltitude; 

  // Integer part of fAltitude
  uint16_t iAltitude;

  /* 
    Checking the upper limit of the altitude 
    (it must not exceed the limit of uint16_t = 65536)
  */
  if (fAltitude > 65000.0) 
  {
    // Calculating the mod of fAltitude/65000 and converting it to uint16_t.
    iAltitude = (uint16_t)(fAltitude-65000.0*(int)(fAltitude/65000.0)); // Altitude limit

    // Registering altitude negative overflow error in the log
    writeErrorLog(error::AltitudePositiveOverflow);
  }
  else
  {
    iAltitude = (uint16_t)fAltitude;
  }
  
  // Calculating the address in memory of slot i
  uint16_t address = 2 * i + addrAltitudesBegin;

  // Saving to EEPROM
  EEPROM.put(address, iAltitude);

  // Updating the last slot written and the end of memory mark (0xFF)
  if ( i >= numberOfSlotsWritten ) 
  {
    numberOfSlotsWritten = i+1;
    // Saving to EEPROM
    EEPROM.put(address+2, 0xFF);
  }

  return true;
}

bool Memory::appendAltitude(float altitude)
{
  if ( numberOfSlotsWritten < numberOfSlots )
  {
    return writeAltitude(numberOfSlotsWritten, altitude);
  }
  else
  {
    return false;
  }
}

float Memory::readApogee()
{
  float apogee = 0.0;
  float faux = -1E5;

  for (uint16_t i = 0; i < getNumberOfSlotsWritten(); ++i)
  {
    faux = readAltitude(i);
    if ( faux > apogee ) apogee = faux;
  }
  return apogee;
}

void Memory::writeEvent(const char& c, const uint16_t& deltaTMultiplier)
{
  switch (c)
  {
    case 'F':
    {
      EEPROM.put(addrliftoffEvent,deltaTMultiplier);
      break;
    }
    case 'D':
    {
      EEPROM.put(addrDrogueEvent,deltaTMultiplier);
      break;
    }
    case 'P':
    {
      EEPROM.put(addrParachuteEvent,deltaTMultiplier);
      break;
    }
    case 'L':
    {
      EEPROM.put(addrLandedEvent,deltaTMultiplier);
      break;
    }
  default:
    break;
  }
}

uint16_t Memory::readEvent(const char& c)
{
  uint16_t deltaTMultiplier = 0xFF;
  switch (c)
  {
    case 'F':
    {
      EEPROM.get(addrliftoffEvent,deltaTMultiplier);
      break;
    }
    case 'D':
    {
      EEPROM.get(addrDrogueEvent,deltaTMultiplier);
      break;
    }
    case 'P':
    {
      EEPROM.get(addrParachuteEvent,deltaTMultiplier);
      break;
    }
    case 'L':
    {
      EEPROM.get(addrLandedEvent,deltaTMultiplier);
      break;
    }
  default:
    break;
  }

  return deltaTMultiplier;
}

void Memory::erase()
{
  uint16_t value = 0;
  EEPROM.put(addrErrorLog, value);
  EEPROM.put(addrliftoffEvent, value);
  EEPROM.put(addrDrogueEvent, value);
  EEPROM.put(addrParachuteEvent, value);
  EEPROM.put(addrLandedEvent, value);

  value = 0xFF; // Maximum value of uint16_t (hexadecimal) 
  /*
  for (uint16_t i = 0; i < numberOfSlots; i++){
    uint16_t address = 2*i + addrAltitudesBegin;
    EEPROM.put(address, value);
  } 
  */
  EEPROM.put(addrAltitudesBegin, value);
  numberOfSlotsWritten = 0;
};


void Memory::writeFlightParameters(const FlightParameters& p)
{
  EEPROM.put(addrFlightParameters, p);
}


FlightParameters Memory::readFlightParameters()
{
  FlightParameters p;
  EEPROM.get(addrFlightParameters, p);
  return p;
}