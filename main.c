/*
 * 12/02/20
 * 
 * ECM Final Project main file
 * 
 * This project is to develop the software and hardware for an autonomous
 * bomb disposal robot.
 */

/*****************************************************************************/
// Include statements and boilerplate code
#include <xc.h>
#include <pic18f4331.h>
#pragma config OSC = IRCIO, MCLRE=OFF, LVP=OFF

#include "LCDIO.h"
#include "dc_motor.h"


/*****************************************************************************/
//Global variables defined here


/*****************************************************************************/
// setup function, initialise registers here
void setup(void)
{   
    // set the clock to 8 MHz and wait for it to stabilise
    OSCCON = 0x72; 
    while(!OSCCONbits.IOFS);
    
    
}
/*****************************************************************************/
// High priority interrupt service routine
void __interrupt(high_priority) InterruptHandlerHigh (void)
{
    
}
/*****************************************************************************/
// Low priority interrupt service routine
void __interrupt(low_priority) InterruptHandlerLow (void)
{
    
}
/*****************************************************************************/
// main function
void main(void)
{
  // first we call the setup function
  setup();
  
  
  // loop, this runs forever
  while(1){
      
  }  
}
