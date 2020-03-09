/*
 signal_processing.c
 
 This file contains the signal processing and classification 
 functions for ECM, as well as a function to initialise the sensors.
 
 21.02.20
 */

#include <pic18f4331.h>
#include "signal_processing.h"
#include <stdio.h>
#include <xc.h>
#include "LCDIO.h"

// This function is called in the setup to initialise the sensors
void init_sensor(void)
{
    T5CON =0b00000001; // initialise timer 5 with 1:1 prescaler
    TRISAbits.RA2 = 1; // Set CAP1 as an input
    
    // by default these pins are analog, so we set to digital
    ANSEL0 = 0; 
    ANSEL1 = 0;
   
    CAP1CON = 0b01000111; //PWM measurement, time base reset   
}

// This function takes the smoothed data and classifies it into a status
// 0 means that the beacon has not been found
// 1 means the beacon is straight ahead
// (We use an exponential moving average to smooth the data)
char classify_data(unsigned int raw_data)
{  
    // initialise smoothed data as a static variable so it doesnt reset between
    // function calls
    static unsigned int smoothed;
    
    // Exponentially weighted moving average implementation
    // by subtracting the smoothed from the raw data, we essentially high-pass
    // filter the data, helping to identify the beacon
    smoothed = smoothed + ((raw_data - smoothed) >> 2);
    unsigned int filtered = raw_data - smoothed;  
               
    // Compare filtered data to a threshold to see if beacon is straight ahead
    if(filtered >= 100)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}