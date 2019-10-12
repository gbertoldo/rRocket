#include "Arduino.h"
#include "Memory.h"
#include <EEPROM.h>


bool Memory::begin()
{

  restartPosition(); // initial position to read or write the height
  apogee = 0; // initial value to apogee
  memorySize = EEPROM.length(); // EEPROM size of the Arduino (bytes)
  //memorySize = 124;

  if (readApogee() > 0)
  {
    setState(MemoryState::full);
  }
  else
  {
    setState(MemoryState::empty);
  }

  return true;

}


bool Memory::setState(MemoryState currentState)
{

  state = currentState;

}


int Memory::getSize()
{

  return memorySize;

}


MemoryState Memory::getState()
{

  return state;

}


bool Memory::restartPosition()
{

  position = 2;

}


float Memory::readAltitude()
{

  float fHeight;
  unsigned int iHeight;
  int quotient;
  int rest;

  quotient = EEPROM.read(position);
  rest = EEPROM.read(position + 1);
  position = position + 2;

  iHeight = (255 * quotient + rest);
  fHeight = (float)iHeight;
  fHeight = fHeight / 10;   // transform the measures of decimeters to meters
  fHeight = fHeight - 500;

  return fHeight;

}


bool Memory::writeAltitude(float fHeight)
{

  unsigned int iHeight;
  int quotient;
  int rest;

  if (getState() == MemoryState::full) return 1;

  if (position > memorySize - 2)
  {
    setState(MemoryState::full);
    return 1; // memory full
  }

  if (fHeight > apogee) apogee = fHeight;

  fHeight = fHeight + 500; // increase 500 meters to write positive heights

  if (fHeight < 0) fHeight = 0;

  if (fHeight > 6500) fHeight = 6500; // height limit

  fHeight = 10 * fHeight; // transform the measures of meters to decimeters
  iHeight = (int)fHeight;
  quotient = iHeight / 255;
  rest = iHeight % 255;

  EEPROM.write(position, quotient);
  EEPROM.write(position + 1, rest);

  position = position + 2;

  return 0;

}


bool Memory::writeApogee()
{

  float fHeight;
  unsigned int iHeight;
  int quotient;
  int rest;

  fHeight = apogee;

  if (fHeight > 6500) fHeight = 6500; // height limit

  fHeight = 10 * fHeight; // transform the measures of meters to decimeters
  iHeight = (int)fHeight;
  quotient = iHeight / 255;
  rest = iHeight % 255;

  EEPROM.write(0, quotient);
  EEPROM.write(1, rest);

}


float Memory::readApogee()
{

  float fHeight;
  unsigned int iHeight;
  int quotient;
  int rest;

  quotient = EEPROM.read(0);
  rest = EEPROM.read(1);

  iHeight = (255 * quotient + rest);
  fHeight = (float)iHeight;
  fHeight = fHeight / 10; // transform the measures of decimeters to meters

  return fHeight;

}


bool Memory::erase()
{

  int i;

  apogee = 0;
  writeApogee();

  restartPosition();

  for (i = 0; i < memorySize / 2 - 1; i++) writeAltitude(0);

  restartPosition();

  setState(MemoryState::empty);

};
