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
   
    DFLTCON = 0b00001000; // noise filter for CAP1 input
    CAP1CON = 0b01000111; //PWM measurement (falling to rising), time base reset
    
    unsigned int throwaway = (unsigned int)((CAP1BUFH << 8) | CAP1BUFL);
}

// This function takes the smoothed data and classifies it into a status
// 0 means that the beacon has not been found
// 1 means the beacon is straight ahead
char classify_data(unsigned int raw_data, unsigned int *smoothed)
{  
    
    unsigned int previous = *smoothed;
    *smoothed = *smoothed + ((raw_data - *smoothed) >> 1);
    
    if(*smoothed - previous >= 2000)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}