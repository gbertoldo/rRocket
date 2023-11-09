#ifndef RECOVERYSYSTEM_H
#define RECOVERYSYSTEM_H
#include <cinttypes>
#include <cstdio>
#include "TrajectoryModels.h"
#include "Parameters.h"

/*

    Recovery System States

*/

enum class RecoverySystemState {readyToLaunch, flying, drogueChuteActive, parachuteActive, recovered};


class RecoverySystem
{
public:

    void begin()
    {
        
        ftraj = fopen("trajectory.txt","w");
        fevts = fopen("events.txt","w");

        state = RecoverySystemState::readyToLaunch;

        tStart = trajectory.tA;
        tEnd   = trajectory.tG*1.2;

        t = tStart;
        int counter = 0;

        while ( t < tEnd)
        {
            // Reading altitude
            float h = trajectory.y(t);
            
            // Initializing the altitude vector
            altitude[counter] = h;
            
            fprintf(ftraj, "%9.2f %9.4f\n",t,h);

            counter++;
            t = t + 1e-3*deltaT;

            if ( counter > N ) return;
        }

        checkFlyEvents();
    };


    void run()
    {

        while ( t < tEnd)
        {
            // Reading altitude          
            for (uint8_t i = 0; i < N; ++i)
            {
                altitude[i] = altitude[i+1];
            }
            altitude[N] = trajectory.y(t);

            fprintf(ftraj, "%9.2f %9.4f %9.4f\n",t,altitude[N],vAverage());

            checkFlyEvents();

            switch (state)
            {
                case RecoverySystemState::flying:
                {
                    if ( apogeeCondition )
                    {
                        state = RecoverySystemState::drogueChuteActive;
                        fprintf(fevts, "%9.2f %9.2f %9.2f %s\n",t,altitude[N],vAverage(),"Apogeu detectado");
                    }
                    if ( fallCondition )
                    {
                        state = RecoverySystemState::drogueChuteActive;
                        fprintf(fevts, "%9.2f %9.2f %9.2f %s\n",t,altitude[N],vAverage(),"Queda detectada");
                    }
                    break;
                }
                case RecoverySystemState::drogueChuteActive:
                {
                    if ( parachuteDeploymentCondition )
                    {
                        state = RecoverySystemState::parachuteActive;
                        fprintf(fevts, "%9.2f %9.2f %9.2f %s\n",t,altitude[N],vAverage(),"Paraquedas acionado");
                    }
                    if ( landingCondition )
                    {
                        state = RecoverySystemState::recovered;
                        fprintf(fevts, "%9.2f %9.2f %9.2f %s\n",t,altitude[N],vAverage(),"Pouso detectado");
                    }
                    break;
                }
                case RecoverySystemState::parachuteActive:
                {
                    if ( landingCondition )
                    {
                        state = RecoverySystemState::recovered;
                        fprintf(fevts, "%9.2f %9.2f %9.2f %s\n",t,altitude[N],vAverage(),"Pouso detectado");
                    }
                    break;
                }
                case RecoverySystemState::readyToLaunch:
                {
                    if ( liftoffCondition )
                    {
                        state = RecoverySystemState::flying;
                        fprintf(fevts, "%9.2f %9.2f %9.2f %s\n",t,altitude[N],vAverage(),"Decolagem detectada");
                    }
                    if ( fallCondition )
                    {
                        //state = RecoverySystemState::flying;
                        //fprintf(fevts, "%9.2f %9.2f %9.2f %s\n",t,altitude[N],vAverage(),"Queda detectada");
                    }
                    break;
                }
                case RecoverySystemState::recovered:
                {
                    if ( liftoffCondition )
                    {
                        state = RecoverySystemState::flying;
                        fprintf(fevts, "%9.2f %9.2f %9.2f %s\n",t,altitude[N],vAverage(),"Decolagem detectada");
                    }
                    if ( fallCondition )
                    {
                        state = RecoverySystemState::flying;
                        fprintf(fevts, "%9.2f %9.2f %9.2f %s\n",t,altitude[N],vAverage(),"Queda detectada");
                    }
                    break;
                }
                default:
                    break;
            }

            t = t + 1e-3*deltaT;
        }    
        fclose(ftraj);
        fclose(fevts);
    };

private:

    /*
        Calculates the vertical component of the velocity
        vector based on average values of the vertical 
        position.
    */
    float vAverage()
    {
        /*
        // Simple mean

        float sum1 = 0.0;
        float sum2 = 0.0;

        for (uint8_t i = 0; i < halfN; ++i)
        {
            sum1 += altitude[i];
            sum2 += altitude[N-i];
        }
        return (sum2-sum1)/(1E-3*(halfN+1)*halfN*deltaT);
        */
        // Integral based average
        float sum1 = 0.5*altitude[0];
        float sum2 = 0.5*altitude[N];

        for (uint8_t i = 1; i < halfN; ++i)
        {
            sum1 += altitude[i];
            sum2 += altitude[N-i];
        }
        return (sum2-sum1)/(1E-3*halfN*halfN*deltaT);
    };

    /*
        Checks for the conditions of the following events:
            - liftoff 
            - apogee 
            - fall 
            - parachute deployment altitude 
            - landing 
    */
    void checkFlyEvents()
    {
        // Calculating the average vertical component of the velocity
        float vavg = vAverage();

        // Ckecking the lift off condition
        liftoffCondition = (  vavg > Parameters::speedForLiftoffDetection ? 1 : 0 );
        
        // Checking the fall condition
        fallCondition    = ( -vavg > Parameters::speedForFallDetection ? 1 : 0 );
        
        // Checking the apogee condition 
        apogeeCondition  = (  vavg < Parameters::speedForApogeeDetection ? 1 : 0 );
        
        // Checking the parachute deployment condition
        parachuteDeploymentCondition = ( altitude[N] <= Parameters::parachuteDeploymentAltitude ? 1 : 0 );

        // Checking the landing condition
        float ymin=1E5, ymax=-1E5;
        for (uint8_t i = 0; i < N+1; ++i)
        {
            if ( altitude[i] < ymin ) ymin = altitude[i];
            if ( altitude[i] > ymax ) ymax = altitude[i];
        }
        landingCondition = ( ymax-ymin < Parameters::displacementForLandingDetection ? 1 : 0 );
    };

private:

    //TrajectoryModel1 trajectory;
    TrajectoryModel2 trajectory;
    float t, tStart, tEnd;
    FILE* ftraj;
    FILE* fevts;


    RecoverySystemState state;

    static constexpr uint8_t       N =  12; // Number of time steps
    static constexpr uint8_t   halfN = N/2; // Half the number of time steps
    static constexpr uint16_t deltaT = 250; // Time step between measurements (ms)
    float                    altitude[N+1]; // Register of the last N+1 measurements

    // Event flags (1 if condition is satisfied, 0 otherwise)
    uint8_t             liftoffCondition {0};
    uint8_t                fallCondition {0};
    uint8_t              apogeeCondition {0};
    uint8_t parachuteDeploymentCondition {0};
    uint8_t             landingCondition {0};

};

#endif // RECOVERYSYSTEM_H