#ifndef TRAJECTORYMODELS_H
#define TRAJECTORYMODELS_H

#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

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
        Tpropulsion   = 4.0; 
        accPropulsion = 8.0 * g;

        // Other input
        tA =   0.0; // Initial time
        tB =  10.0; // Ignition time
        yG =  40.0; // Final vertical position
        Uh =   1.0; // Uncertainty of y (+-1 m)
        Aperturbation = 50.0; // Amplitude of the perturbation (m)
        Tperturbation =  0.8; // Period of the perturbation (s)

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
	        //srand((unsigned) time(NULL));
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
        return (isNoiseActive ? -Uh+2.0*Uh*1E-4*((float)(rand() % 10000)) : 0.0);
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

class TrajectoryModel2
{
public:
    float tA, tB;
    float tG, yG;
    vector<double> time, height;
    double period; // period of readings
    double unitConversion; // conversion factor from ?? to meter
    string filename;
    int flightNumber;
public:

    TrajectoryModel2()
    {      
        tB =  0.0; // Ignition time
        flightNumber = 12;
        // Other input
        switch (flightNumber)
        {
        case 1:
        {
            filename = "lancamento-01.txt";
            period = 0.05;
            tA = -100.0 * period; // Initial time
            unitConversion = 0.3048; // feet to meter
            break;
        }    
        case 2:
        {
            filename = "lancamento-02.txt";
            period = 0.05;
            tA = -100.0 * period; // Initial time
            unitConversion = 0.3048; // feet to meter
            break;
        }    
        case 3:
        { 
            filename = "lancamento-03.txt";
            period = 0.05;
            tA = -100.0 * period; // Initial time
            unitConversion = 0.3048; // feet to meter
            break;
        }    
        case 4:
        {
            filename = "lancamento-04.txt";
            period = 0.05;
            tA = -100.0 * period; // Initial time
            unitConversion = 0.3048; // feet to meter
            break;
        }    
        case 5:
        {
            filename = "lancamento-05.txt";
            period = 0.05;
            tA = -100.0 * period; // Initial time
            unitConversion = 0.3048; // feet to meter
            break;
        }    
        case 6:
        {
            filename = "lancamento-06.txt";
            period = 0.05;
            tA = -100.0 * period; // Initial time
            unitConversion = 0.3048; // feet to meter
            break;
        }    
        case 7:
        {
            filename = "lancamento-07.txt";
            period = 0.192;
            tA = -20.0 * period; // Initial time
            unitConversion = 1.0; // feet to meter
            break;
        }    
        case 8:
        {
            filename = "lancamento-08.txt";
            period = 0.05;
            tA = -100.0 * period; // Initial time
            unitConversion = 0.3048; // feet to meter
            break;
        }    
        case 9:
        {
            filename = "lancamento-09.txt";
            period = 0.05;
            tA = -100.0 * period; // Initial time
            unitConversion = 0.3048; // feet to meter
            break;
        }    
        case 10:
        {
            filename = "lancamento-10.txt";
            period = 0.25;
            tA = -100.0 * period; // Initial time
            unitConversion = 1.0; // feet to meter
            break;
        }    
        case 11:
        {
            filename = "lancamento-11.txt";
            period = 0.05;
            tA = -100.0 * period; // Initial time
            unitConversion = 0.3048; // feet to meter
            break;
        }           
        case 12:
        {
            filename = "lancamento-12.txt";
            period = 0.05;
            tA = -100.0 * period; // Initial time
            unitConversion = 0.3048; // feet to meter
            break;
        }       
        default:
            break;
        }

        double t = tA;
        while ( t < tB )
        {
            time.push_back(t);
            height.push_back(0.0);
            t = t+ period;
        }
        //cout << time.back() << " " << height.back() << endl;

        ifstream ifile;

        ifile.open(filename);

        string line;
        int counter = 0;
        while ( getline(ifile, line) )
        {
            if ( counter >= 2)
            {
                if ( line.empty() ) continue;

                stringstream ss(line);
                float t, h;

                ss >> t;
                ss >> h;
                h = h * unitConversion;

                time.push_back(t);
                height.push_back(h);

                //cout << time.back() << " " << height.back()/unitConversion << endl;
            }
            counter++;
        }
        //cout << time.back() << " " << height.back()/unitConversion << endl;
 
        tG = time.back();

    }

    double y(double t)
    {
        int idx = (int)((t-tA)/period);
        double result;

        if ( idx < 0 )
        {
            result = 0.0;
        }
        else if ( idx >= (time.size()-1) )
        {
            result = height.back();
        }
        else
        {
            result = height[idx]+(height[idx+1]-height[idx])/period*(t-time[idx]);
        }

        return result;
    }
};


#endif