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
// 0 represents the initial sweep to look for bomb
// 1 represents moving towards bomb
// 2 represents returning to starting position
// 3 represents finished (robot has found bomb and returned)
volatile char robot_mode = 0;

// stores characters read from RFID (10 data bits and 2 checksum)
volatile char RFIDbuf[12];

// This flag is set to 1 once the RFID has been completely read
volatile char RFID_flag = 0;
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
    init_sensor();
    initPWM(199);
    
    TRISBbits.RB0 = 0; // motor direction pins
    TRISBbits.RB2 = 0;
    TRISDbits.RD2 = 1; // button attached to D2, used for reset         
}
/*****************************************************************************/
// High priority interrupt service routine for RFID reading
void __interrupt(high_priority) InterruptHandlerHigh (void)
{
    // Trigger interrupt when a character is read from the RFID
    // this can only occur when the robot is in searching mode
    if((PIR1bits.RCIF) && (robot_mode == 1))
    {
        //read RFID data into buffer, once all the data has been read set flag=1
        RFID_flag = processRFID(RFIDbuf, RCREG);
    }
    // this runs if you try to scan an RFID when the robot is not searching
    else
    {
        // simply reads and discards RCREG to reset interrupt flag
        char throwaway = RCREG;
    }
}
/*****************************************************************************/
// main function
void main(void)
{
  // first we call the setup function to initialise all the necessary registers
  setup();
  
  //now, we declare the structures that need to be visible to the main function
  struct DC_motor motorL, motorR; //declare 2 motor structures
  init_motor_struct(&motorL, &motorR); // initialise values in each struct
  
  // these define how fast the robot moves in each operation
  int searching_speed = 50;
  int moving_speed = 75;
  
  unsigned long movementMicros=0; // stores time taken moving forward
  
  // loop, this runs forever
  while(1)
  {
      // Subroutine for initial sweep to search for bomb
      if(robot_mode == 0)
      {
          turnRight(&motorL, &motorR,searching_speed); // continuously turn right
      
          // Runs until the beacon is found and the break statement executes
          while(robot_mode == 0)
          {
            // First, acquire the PWM duty cycle using the motion feedback module
            unsigned int raw_data = (unsigned int)((CAP1BUFH << 8) | CAP1BUFL);

            // Now, classify the signals to find if we are looking at the beacon
            char beacon_location = classify_data(raw_data); 

            // if beacon is straight ahead, exit this subroutine
            if(beacon_location == 1)
            {
                robot_mode = 1;
            }
         }
      }
      
      // Subroutine to move towards bomb
      if(robot_mode == 1)
      {
          moveForward(&motorL, &motorR,moving_speed); // move robot forwards
      
          // Runs until RFID has been scanned and break statement executes
          while(robot_mode == 1)
          {
              __delay_us(1);
              movementMicros += 1;

              // once RFID fully read, check against checksum, display it,
              // break loop and set robot mode to return home
              if(RFID_flag == 1)
              {
                  display_RFID(RFIDbuf);
                  check_RFID(RFIDbuf);
                  robot_mode = 2;
                  RFID_flag = 0;
              }
          }
      }
    
      // Subroutine to return to starting position
      if(robot_mode == 2)
      {
          moveBackward(&motorL,&motorR,moving_speed); // move robot backwards
          
          for(unsigned long i=0; i<movementMicros;i++)
          {
              __delay_us(1);
          }
          robot_mode = 3;
      }
      
      // Subroutine for once bomb has been found and robot has returned
      if(robot_mode == 3)
      {
          stop(&motorL, &motorR,moving_speed); // stop moving
          
          while(robot_mode == 3)
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
}
