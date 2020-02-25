/*
 signal_processing.c
 
 This file contains the signal acquisition, processing and classification 
 functions for ECM.
 
 21.02.20
 */

#include <pic18f4331.h>
#include "signal_processing.h"

// This function is called in the setup to initialise the sensors
void init_sensors(void)
{
    T5CON =0b00011001; // initialise timer 5 with 1:8 prescaler
    TRISAbits.RA2 = 1; // Set CAP1 as an input
    TRISAbits.RA3 = 1; // Set CAP2 as an input
    
    // by default these pins are analog, so we set to digital
    ANSEL0 = 0; 
    ANSEL1 = 0;
   
    DFLTCON = 0b00011000; // noise filter for CAP1,2,3 inputs
    CAP1CON = 0b01000110; //PWM measurement (falling to rising), time base reset
}

// This function takes the raw data and smooths it to produce a better signal
int process_signal(int raw_data)
{
    
}

// This function takes the smoothed data and classifies it into a status
// 0 means that the beacon has not been found
// 1 means that the beacon is to the left
// 2 means that the beacon is to the right
// 3 means that the beacon is straight ahead
char classify_data(int left_smoothed, int right_smoothed)
{
    
}