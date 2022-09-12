

    /**********************************************************************\
   /          rRocket: An Arduino powered rocketry recovery system          \
  /            Federal University of Technology - Parana - Brazil            \
  \              by Guilherme Bertoldo and Jonas Joacir Radtke               /
   \                       updated September 12, 2022                       /
    \**********************************************************************/


#include "Arduino.h"
#include "Memory.h"
#include "ErrorTable.h"

bool Memory::begin()
{
  // Number of available slots to save altitudes during flight
  // The first slots is reserved for the apogee.
  numberOfSlots = (uint16_t)(((float)getMemorySize()-addrAltitudesBegin)/2.0) - 1;

  // Reading from memory the number of altitude registers
  numberOfSlotsWritten = 0;
  for ( uint16_t i = 1; i <= numberOfSlots; ++i)
  {
    uint16_t iAltitude, address;

    address = 2*i+addrAltitudesBegin;

    EEPROM.get(address, iAltitude);

    if ( iAltitude == 0 ) 
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
  if ( i > numberOfSlots ) return 0.0;

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
  fAltitude = ((float)iAltitude)/10.0-500.0;

  return fAltitude;
}

bool Memory::writeAltitude(const uint16_t& i, float fAltitude)
{
  // If the position is out of range, returns 0
  if ( i > numberOfSlots ) return false;

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

  return true;
}

bool Memory::appendAltitude(float altitude)
{
  if ( numberOfSlotsWritten < numberOfSlots )
  {
    if ( writeAltitude(numberOfSlotsWritten+1, altitude) )
    {
      numberOfSlotsWritten++;
      return true;
    }
    else
    {
      return false;
    }
  }
  else
  {
    return false;
  }
}

void Memory::erase()
{
  for (uint16_t i = 0; i < EEPROM.length(); i++) EEPROM.write(i, 0);
};
