/*
 signal_processing.c
 
 This file contains the signal processing and classification 
 functions, as well as a function to initialise the sensors and
 stabilise the readings
 
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

/* This function takes the smoothed data and classifies it into a status
 0 means that the beacon has not been found
 1 means the beacon is straight ahead
 (We use an exponential moving average to smooth the data and subtract this
 from the raw data to high pass filter it, this result can then be compared
 to a threshold to classify)
 */
char classify_data(unsigned int raw_data)
{  
    // initialise smoothed data as a static variable so it doesn't reset between
    // function calls
    static unsigned int smoothed;
    
    /* Exponentially weighted moving average implementation
     by subtracting the smoothed from the raw data, we essentially high-pass
     filter the data, helping to identify the beacon 'alpha' = 0.25, because
     alpha can be expressed as 1/(2^k), we can replace the multiplication with
     a bit shift
     */
    smoothed = smoothed + ((raw_data - smoothed) >> 2);
    unsigned int filtered = raw_data - smoothed;  
               
    // Compare filtered data to a threshold to see if beacon is straight ahead
    // the choice of threshold is largely irrelevant, it is most reliable
    // if it's between 50 and 1000
    if(filtered >= 100)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

// call this function once at the top of the main function to stabilise the
// moving average value before the start
void stabiliseAverage(void)
{
    // simply read and throw away the first 500 outputs
    for(int i =0;i<500;i++)
    {
        unsigned int raw_data = (unsigned int)((CAP1BUFH << 8) | CAP1BUFL);
        char throwaway = classify_data(raw_data); // throw away the result
    }
}