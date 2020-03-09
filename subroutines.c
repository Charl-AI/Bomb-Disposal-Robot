/*
 This file contains the subroutines that the robot performs when it is in each 
 mode. The code is placed in this source file in order to de-clutter the main
 function and improve readability.
 
 The robot operates on 4 subroutines, searching, moving forwards, returning and
 displaying the results
 
 A 'wait for inputs' routine is also defined here and is used on startup to
 stabilise the signal processing and to wait for a button press to start
   
 05.03.20
 */
#include <xc.h>
#include <pic18f4331.h>
#include "dc_motor.h"
#include "RFID.h"
#include "signal_processing.h"
#include "subroutines.h"
#include "LCDIO.h"

// This subroutine scans for the beacon initially
volatile char scanForBeacon(struct DC_motor *mL, struct DC_motor *mR, int speed,
                            struct Movements *move)
{
    move-> move_type[move->move_number] = 1; // store move type as left
    turnLeft(mL,mR,speed); // continuously turn on the spot
    ClearLCD();
    LCD_String("SEARCHING"); // Display on LCD
        
    // Runs until the beacon is found and the return statement executes
    while(1)
    {
        // First, acquire the raw signal using the motion feedback module
        unsigned int raw_data = (unsigned int)((CAP1BUFH << 8) | CAP1BUFL);
        
        // Now, classify the signals to find if we are looking at the beacon
        char beacon_location = classify_data(raw_data);
        
        // if beacon is straight ahead, exit this subroutine
        if(beacon_location == 1)
        {
            move-> move_number += 1; // increment move number
            return 1;
        }  
    }
}

// This subroutine moves the robot forwards until it either finds the beacon
// or reaches a time limit
volatile char moveToBeacon(struct DC_motor *mL, struct DC_motor *mR, int speed,
                        struct Movements *move, volatile char *exit_flag)
{
    move->move_type[move->move_number] = 0; // store move type as forwards
    moveForward(mL,mR,speed); // move robot forwards
    ClearLCD();
    LCD_String("MOVING TO BOMB");
    int count = 0;
    // Runs until RFID has been scanned and break statement executes
    while(1)
    {
        // First, acquire the raw signal using the motion feedback module
        unsigned int raw_data = (unsigned int)((CAP1BUFH << 8) | CAP1BUFL);
        
        // Now, classify the signals to find if we are looking at the beacon
        char beacon_location = classify_data(raw_data);
        
        if(*exit_flag == 1)
        {
            exit_flag = 0;
            return 2; // return home
        }
        
        else if(beacon_location == 0)
        {
            count += 1;
        }
        else
        {
            count = 0;
        }
        
        if(count >=18000)
        {
            move-> move_number += 1; // increment move number
            return 0;
        }
    }
}

// This subroutine makes the robot return to its starting position
volatile char returnHome(struct DC_motor *mL, struct DC_motor *mR, int move_speed,
                            int search_speed, struct Movements *move)
{
    stop(mL,mR,move_speed);
    ClearLCD();
    LCD_String("RETURNING HOME");
    
    for(int i=move->move_number;move->move_number >0; move->move_number--)
    {
        if(move->move_type[move->move_number] == 0)
        {
            moveBackward(mL,mR,move_speed);
            while(move->time_taken[move->move_number] > 0);
        }
        else if(move->move_type[move->move_number] == 1)
        {
            turnRight(mL,mR,search_speed);
            while(move->time_taken[move->move_number] > 0);
        }
    }
    
    return 3;
}

// This subroutine makes the robot stop and wait for further inputs at the end
volatile char stopAndDisplay(struct DC_motor *mL,struct DC_motor *mR, int speed,
                                volatile char RFID_buffer[])
{
    stop(mL, mR,speed); // stop moving
    
    display_RFID(RFID_buffer);
    check_RFID(RFID_buffer);
    
    if(RFID_buffer[0] != 0)
    {
        while(1)
        {
            while(PORTDbits.RD2 == 1) // when button is pressed
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
    else 
    {
        return 0;
    }
}

// This function is allows the user to press the button to start the search
void waitForInput(void)
{
    // first, stabilise the smoothing algorithm by reading values
    for(int i =0;i<500;i++)
    {
        unsigned int raw_data = (unsigned int)((CAP1BUFH << 8) | CAP1BUFL);
        char throwaway = classify_data(raw_data); // throw away the result
    }
    
    // display some text for the user
    ClearLCD();
    SetLine(1);
    LCD_String("PRESS BUTTON");
    SetLine(2);
    LCD_String("TO START SEARCH");
    
    // wait until button is pressed
    while(PORTDbits.RD2 == 0);

    // display text to confirm start
    ClearLCD();
    SetLine(1);
    LCD_String("STARTING SEARCH");
    for(int i=0; i<10;i++)
    {
        __delay_ms(100);
    }
    ClearLCD();     
}
    