/*
 signal_processing.c
 
 This file contains the signal acquisition, processing and classification 
 functions for ECM.
 
 21.02.20
 */

#include <pic18f4331.h>


void init_sensors(void)
{
    // initialise pins as inputs
    TRISCbits.RC1 = 1;
    TRISCbits.RC2 = 1;
    
    // set event to trigger on falling edge for first reading
    CCP1CON = 00000100;
    CCP2CON = 00000100;
    
    T1CONbits.TMR1ON = 1; // enable timer 1
    

}