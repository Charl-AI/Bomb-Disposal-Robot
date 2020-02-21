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
#include <stdio.h>
#pragma config OSC = IRCIO, MCLRE=OFF, LVP=OFF

#include "LCDIO.h"
#include "dc_motor.h"
#include "RFID.h"
#include "signal_processing.h"

/*****************************************************************************/
//Global variables defined here

// this defines the subroutine that the robot will operate on:
// 0 represents searching for bomb
// 1 represents returning to starting position
// 2 represents finished (robot has found bomb and returned)
volatile char robot_mode = 0;

volatile int raw_data;
/*****************************************************************************/
// setup function, initialise registers here
void setup(void)
{   
    // Board-specific setup
    OSCCON = 0x72; // set clock to 8 MHz
    while(!OSCCONbits.IOFS); // wait for clock to stabilise
    INTCONbits.GIEH = 1; //Global high priority interrupt enable
    INTCONbits.GIEL = 1;// enable low priority interrupts
    RCONbits.IPEN=1; // enable interrupt prioritisation
    
    // Initialise pins to enable LCD, RFID, motors, etc.
    init_LCD();
    init_RFID();
    init_sensors();
    
    TRISDbits.RD2 = 1;
}
/*****************************************************************************/
// High priority interrupt service routine
void __interrupt(high_priority) InterruptHandlerHigh (void)
{
    // Trigger interrupt when a character is read from the RFID
    // this can only occur when the robot is in searching mode
    if((PIR1bits.RCIF) && (robot_mode == 0))
    {
        // stores the characters read from RFID (10 data bits and 2 checksum)
        // this is static so it does not change between ISR calls
        // (better than declaring globally because it is only scoped in the ISR)
        static char RFIDbuf[12];
        
        // process the RFID data, once all the data has been read, it is
        // displayed and RFID_flag is set to 1
        char RFID_flag = processRFID(RFIDbuf, RCREG);
        
        // once the RFID has been read, check against checksum and set 
        // robot to return home
        if(RFID_flag == 1)
        {
            check_RFID(RFIDbuf);
            robot_mode = 1;
        }
    }
    // this runs if you try to scan an RFID when the robot is not searching
    else
    {
        // simply reads and discards RCREG to reset interrupt flag
        char throwaway = RCREG;
    }
}
/*****************************************************************************/
// Low priority interrupt service routine
void __interrupt(low_priority) InterruptHandlerLow (void)
{
    // Triggers when CCP1 detects a rising or falling edge and search mode is on
    if((PIR1bits.CCP1IF) && robot_mode == 0)
    {
        // stores the most recent falling edge
        static int falling_edge;
        // if falling edge detected
        
        if(CCP1CON == 00000100)
        {   
            falling_edge = (CCPR1H << 8) | CCPR1L;
            CCP1CON = 00000101; // set CCP1CON to detect next rising edge
            PIE1bits.CCP1IE = 0; // clear to avoid false interrupt
            PIR1bits.CCP1IF = 0; // clear interrupt flag

        }
        else if(CCP1CON == 00000101)
        {
            raw_data = (CCPR1H << 8) | CCPR1L - falling_edge;
            CCP1CON = 00000100; // set CCP1CON to detect next falling edge
            PIE1bits.CCP1IE = 0; // clear to avoid false interrupt
            PIR1bits.CCP1IF = 0; // clear interrupt flag
        }
    }
    // when the robot is not in searching mode, do nothing apart from clear flag
    else
    {
        PIR1bits.CCP1IF = 0; // clear interrupt flag
    }
}
/*****************************************************************************/
// main function
void main(void)
{
  // first we call the setup function
  setup();
  
  
  // loop, this runs forever
  while(1)
  {
      // Subroutine to search for bomb
      while(robot_mode == 0)
      {
          SetLine(1);
          LCD_String("raw duty cycle");
          SetLine(2);
          char temp[16];
          sprintf(temp,"%d",raw_data);
      }
    
      // Subroutine to return to starting position
      while(robot_mode == 1)
      {
          // for debugging only, remove later
          robot_mode = 2;
      }
      
      // Subroutine for once bomb has been found and robot has returned
      while(robot_mode == 2)
      {
          while(PORTDbits.RD2 == 1)
          {
              ClearLCD();
              LCD_String("RESETTING ROBOT");
              for(int i=0; i<10;i++)
              {
                  __delay_ms(100);
              }
              ClearLCD();
              robot_mode = 0;
          }
      }
  }
}
