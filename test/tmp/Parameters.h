#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <cinttypes>

namespace Parameters
{
    static constexpr uint8_t           speedForLiftoffDetection  {30}; // Speed for liftoff detection (m/s)
    static constexpr uint8_t              speedForFallDetection  {30}; // Speed for fall detection (m/s)
    static constexpr uint8_t            speedForApogeeDetection   {7}; // Speed for apogee detection (m/s)
    static constexpr uint16_t       parachuteDeploymentAltitude {200}; // Altitude to deploy the main parachute (m)
    static constexpr uint8_t    displacementForLandingDetection   {3}; // Displacement for landing detection (meter)
}

#endif