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
#include "RFID.h"

/*****************************************************************************/
//Global variables defined here

/*****************************************************************************/
// setup function, initialise registers here
void setup(void)
{   
    // set the clock to 8 MHz and wait for it to stabilise
    OSCCON = 0x72; 
    while(!OSCCONbits.IOFS);
    INTCONbits.GIEH = 1; //Global high priority interrupt enable
    INTCONbits.GIEL = 1;// enable low priority interrupts
    RCONbits.IPEN=1; // enable interrupt prioritisation
    
    LCD_Init();
    init_RFID();
}
/*****************************************************************************/
// High priority interrupt service routine
void __interrupt(high_priority) InterruptHandlerHigh (void)
{
    // Trigger interrupt when a character is read from the RFID
    if(PIR1bits.RCIF)
    {
        char temp = RCREG;
        processRFID(temp);
        
        
    }
    
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
