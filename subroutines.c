/*
 This file contains the subroutines that the robot performs when it is in each 
 mode. The code is placed in this source file in order to de-clutter the main
 function and improve readability.
 
 05.03.20
 */
#include <xc.h>
#include <pic18f4331.h>
#include "dc_motor.h"
#include "RFID.h"
#include "signal_processing.h"
#include "subroutines.h"
#include "LCDIO.h"
#include <stdio.h>

volatile char scanForBeacon(struct DC_motor *mL, struct DC_motor *mR, int speed)
{
    turnRight(mL,mR,speed); // continuously turn right
    unsigned int smoothed_data = (unsigned int)((CAP1BUFH << 8) | CAP1BUFL);
    // Runs until the beacon is found and the break statement executes
    while(1)
    {
        // First, acquire the PWM duty cycle using the motion feedback module
        unsigned int raw_data = (unsigned int)((CAP1BUFH << 8) | CAP1BUFL);
        
        // Now, classify the signals to find if we are looking at the beacon
        char beacon_location = classify_data(raw_data, &smoothed_data); 

        // if beacon is straight ahead, exit this subroutine
        if(beacon_location == 1)
        {
            return 1;
        }
    }
}

volatile char moveToBeacon(struct DC_motor *mL, struct DC_motor *mR, int speed,
unsigned long *micros, volatile char RFID_buffer[], volatile char *exit_flag)
{
    moveForward(mL,mR,speed); // move robot forwards
      
          // Runs until RFID has been scanned and break statement executes
          while(1)
          {
              __delay_us(1);
              *micros += 1;
           
              // once RFID fully read, check against checksum, display it,
              // break loop and set robot mode to return home
              if(*exit_flag == 1)
              {
                  display_RFID(RFID_buffer);
                  check_RFID(RFID_buffer);
                  exit_flag = 0;
                  return 2;
              }
          }
}

volatile char returnHome(struct DC_motor *mL, struct DC_motor *mR, int speed,
                        unsigned long *micros)
{
    moveBackward(mL,mR,speed); // move robot backwards
          
    for(unsigned long i=0; i<*micros;i++)
    {
        __delay_us(1);
    }
    return 3;
}

volatile char stopAndDisplay(struct DC_motor *mL, struct DC_motor *mR, int speed)
{
    stop(mL, mR,speed); // stop moving
    
        while(1)
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
                Reset();
            }
        }
}

void debug(void)
{
    // Runs until the beacon is found and the break statement executes
    while(1)
    {
        // First, acquire the PWM duty cycle using the motion feedback module
        unsigned int raw_data = (unsigned int)((CAP1BUFH << 8) | CAP1BUFL);
        
        static unsigned int smoothed_data;
        smoothed_data = smoothed_data + ((raw_data - smoothed_data) >> 1);
        ClearLCD();
        char buf[16];
        SetLine(1);
        sprintf(buf,"%u",raw_data);
        LCD_String(buf);
        SetLine(2);
        char buf2[16];
        sprintf(buf2, "%u",smoothed_data);
        LCD_String(buf2);
        __delay_ms(100);
         
        
        // Now, classify the signals to find if we are looking at the beacon
        //char beacon_location = classify_data(raw_data); 
    
    }
}
    