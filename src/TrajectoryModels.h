#ifndef TRAJECTORYMODELS_H
#define TRAJECTORYMODELS_H

#include <Arduino.h>

class TrajectoryModel1
{
public:
    float g = 9.8;       // Gravitational acceleration (m/s2)
    float accPropulsion; // Acceleration for propulsion (m/s2)
    float Tpropulsion;   // Propulsion time (s)
    float tA;
    float tB;
    float tC, yC, vyC;
    float tD;
    float tG, yG;

    bool isNoiseActive;         // if true, activates the noise +- Uh
    bool isPerturbationActive;  // if true, activates the perturbation in the trajectory
    
    float Uh; // Uncertainty of y   
    float Aperturbation; // Amplitude of the perturbation (m)
    float Tperturbation; // Period of the perturbation (s)


public:

    TrajectoryModel1()
    {
        // Propulsion input
        Tpropulsion   = 3.0; 
        accPropulsion = 2.0 * g;

        // Other input
        tA =   0.0; // Initial time
        tB =  20.0; // Ignition time
        yG =  40.0; // Final vertical position
        Uh =   1.0; // Uncertainty of y (+-1 m)
        Aperturbation = 50.0; // Amplitude of the perturbation (m)
        Tperturbation =  1.2; // Period of the perturbation (s)

        isNoiseActive = true;
        isPerturbationActive = true;

        // Calculated parameters
        tC = tB + Tpropulsion;
        yC  = accPropulsion / 2.0 * (tC-tB)*(tC-tB);
        vyC = accPropulsion * (tC-tB); 
        tD  = tC + vyC/g;
        tG  = tC + (vyC+sqrt(vyC*vyC+2.0*g*(yC-yG)))/g; 

        if ( isNoiseActive )
        {
            // Providing a seed value
	        randomSeed(analogRead(0));
        }
    }

    /*
        Returns the altitude (m) according to the
        baseline model
    */
    float ybaseline(float t)
    {
        if ( t < tB )
        {
            return 0.0;
        }
        else if ( t < tC )
        {
            return accPropulsion / 2.0 * (t-tB)*(t-tB);
        }
        else if ( t < tG )
        {
            return yC + vyC*(t-tC) - 0.5*g*(t-tC)*(t-tC);
        }
        else
        {
            return yG;
        }
    }

    float vybaseline(float t)
    {
        if ( t < tB )
        {
            return 0.0;
        }
        else if ( t < tC )
        {
            return accPropulsion * (t-tB);
        }
        else if ( t < tG )
        {
            return vyC - g * (t-tC);
        }
        else
        {
            return 0.0;
        }
    }

    float yrandom()
    {
        return (isNoiseActive ? -Uh+2.0*Uh*(1E-3*random(1000)) : 0.0);
    }

    float perturbation(float t, float A, float t0, float tbeg, float tend, float T)
    {
        if ( tbeg <= t && t <= tend )
        {
            return A*sin(2.0*3.1416*(t-t0)/T);
        }
        else
        {
            return 0.0;
        }
    }

    float yperturbation(float t)
    {
        if ( isPerturbationActive )
        {
            return perturbation(t, Aperturbation, tB/3.0, tB/3.0, tB/3.0+Tperturbation, Tperturbation)
            + perturbation(t, 1.00*Aperturbation, tB, tB, 0.8*tD, Tperturbation)
            + perturbation(t, 1.00*Aperturbation, 1.2*tD, 1.2*tD, tG, Tperturbation);
        }
        else
        {
            return 0.0;
        }
    }

    float y(float t)
    {
        return ybaseline(t)+yrandom()+yperturbation(t);
    }
};


#endif