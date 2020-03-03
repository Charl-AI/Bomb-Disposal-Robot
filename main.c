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
    init_sensors();
    initPWM(199);
    
    TRISBbits.RB0 = 0; // motor direction pins
    TRISBbits.RB2 = 0;
    TRISDbits.RD2 = 1; // button attached to D2, used for reset
}
/*****************************************************************************/
// High priority interrupt service routine
void __interrupt(high_priority) InterruptHandlerHigh (void)
{
    // Trigger interrupt when a character is read from the RFID
    // this can only occur when the robot is in searching mode
    if((PIR1bits.RCIF) && (robot_mode == 0))
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
// Low priority interrupt service routine
void __interrupt(low_priority) InterruptHandlerLow (void)
{
    
    
}
/*****************************************************************************/
// main function
void main(void)
{
  // first we call the setup function to initialise all the necessary registers
  setup();
  
  //now, we declare the structures that need to be visible to the main function
  struct DC_motor motorL, motorR; //declare 2 motor structures
  init_motors(&motorL, &motorR); // initialise values in each struct
  
  struct Sensor sensor; // declare structures for both sensors
  
  // loop, this runs forever
  while(1)
  {
      // Subroutine to search for bomb
      while(robot_mode == 0)
      {
          static char beacon_location;
          
          // First, acquire the PWM duty cycle using the motion feedback module
          sensor.raw_data = (int)((CAP2BUFH << 8) | CAP2BUFL);
          
          
          // Next, process the signal by passing through a smoothing algorithm
          process_signal(&sensor);
          
          // Temporarily store the previous location of the beacon
          char previous_location = beacon_location;
          
          // Now, classify the signals to find the beacon location
          beacon_location = classify_data(sensor.smoothed_signal) 
                             
          // if the beacon is straight ahead, move towards it, otherwise, stop
          // and align the robot with the beacon direction
          moveToBeacon(beacon_location, previous_location, &motorL, &motorR);
           
          //print to LCD for debugging (remove later)
          ClearLCD();
          SetLine(1);
          char temp2[16];
          sprintf(temp2,"smoothed %u ",sensor.smoothed_signal);
          LCD_String(temp2);
          SetLine(2);
          char temp1[16];
          sprintf(temp1,"raw %u ",sensor.raw_data);
          LCD_String(temp1);
          __delay_ms(100);
          
          // once RFID fully read, check against checksum, display it and change
          // robot mode to return home
          if(RFID_flag == 1)
          {
              display_RFID(RFIDbuf);
              check_RFID(RFIDbuf);
              robot_mode = 1;
              RFID_flag = 0;
          }
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
