/*
 signal_processing.c
 
 This file contains the signal acquisition, processing and classification 
 functions for ECM.
 
 21.02.20
 */

#include <pic18f4331.h>
#include "signal_processing.h"

// This function is called in the setup to initialise the sensors
void init_sensor(void)
{
    T5CON =0b00000001; // initialise timer 5 with 1:8 prescaler
    TRISAbits.RA2 = 1; // Set CAP1 as an input
    
    // by default these pins are analog, so we set to digital
    ANSEL0 = 0; 
    ANSEL1 = 0;
   
    //DFLTCON = 0b00011000; // noise filter for CAP1,2,3 inputs
    CAP1CON = 0b01000111; //PWM measurement (falling to rising), time base reset
}

// This function takes the raw data and smooths it to produce a better signal
// we use an exponential moving average, which is good because it acts as a
// low-pass filter, is very fast and uses very little memory
void process_signal(struct Sensor *S)
{
    int smoothing_constant = 10; //"alpha" constant for smoothing algorithm (%)
    // use exponential moving average to smooth data
    S->smoothed_signal *= (100-smoothing_constant)/100;
    S->smoothed_signal += (S->raw_data * smoothing_constant)/100;
}

// This function takes the smoothed data and classifies it into a status
// 0 means that the beacon has not been found
// 1 means the beacon is straight ahead
char classify_data(int smoothed_data)
{
    if(smoothed_data > 10000)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}