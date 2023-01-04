

    /**********************************************************************\
   /          rRocket: An Arduino powered rocketry recovery system          \
  /            Federal University of Technology - Parana - Brazil            \
  \              by Guilherme Bertoldo and Jonas Joacir Radtke               /
   \                       updated September 12, 2022                       /
    \**********************************************************************/


#ifndef ERRORTABLE_H
#define ERRORTABLE_H

/*
  The error table is encoded in such a way that the errors can be
  composed and decomposed later. This is acomplished using a binary
  base. Each error must be an independent vector of the base. For 
  instance:
    Error 1, code 2 << 0 (2 in decimals or 00000001 in binary)
    Error 2, code 2 << 1 (4 in decimals or 00000010 in binary)
    Error 3, code 2 << 2 (8 in decimals or 00000100 in binary)
  Suppose that errors 1 and 3 must be enconded in a unique
  variable errorLog of 2 bytes in memory. This is performed 
  using the bitwise OR operator:
  errorLog = Error1 | Error3 = 00000101.
  To verify if some error ErrorN occured, one may use the bitwise
  AND operator:
    errorLog & ErrorN
  If the result is not zero, the error ErrorN is present. For instance,
     00000101 & 00000100 = 00000100.
*/

namespace error
{
  static constexpr uint16_t BarometerInitializationFailure  {2 << 0}; // Error 1
  static constexpr uint16_t ActuatorInitializationFailure   {2 << 1}; // Error 2
  static constexpr uint16_t AltitudeNegativeOverflow        {2 << 2}; // Error 3
  static constexpr uint16_t AltitudePositiveOverflow        {2 << 3}; // Error 4
  static constexpr uint16_t FlightStartedWithNonEmptyMemory {2 << 4}; // Error 5
}

#endif
