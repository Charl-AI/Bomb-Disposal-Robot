/*
 signal_processing.c
 
 This file contains the signal acquisition, processing and classification 
 functions for ECM.
 
 21.02.20
 */

#include <pic18f4331.h>
#include "signal_processing.h"

void init_sensors(void)
{
    
    T5CON =0b00011001; // initialise timer 5 with 1:8 prescaler
    TRISAbits.RA2 = 1; // Set CAP1 as an input
    // TRISAbits.RA3 = l; // Set CAP2 as an input
    
    // by default these pins are analog, so we set to digital
    ANSEL0 =0; 
    ANSEL1 =0;
   
    DFLTCON = 0b00011000; // noise filter for CAP1,2,3 inputs
    CAP1CON = 0b01000110; // 

    
    int throwaway = (CAP1BUFH<<8) | CAP1BUFL;
    

}