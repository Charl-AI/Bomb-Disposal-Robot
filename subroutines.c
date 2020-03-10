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
                        struct Movements *move, volatile char *exit_flag)
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
        
        // if RFID read, return home
        if(*exit_flag == 1)
        {
            return 2; // return home
        }
        // if beacon is straight ahead, exit this subroutine
        else if(beacon_location == 1)
        {
            move-> move_number += 1; // increment move number
            return 1;
        }  
    }
}

// This subroutine moves the robot forwards until it either finds the beacon
// or loses the bomb - in which case it starts searching again
volatile char moveToBeacon(struct DC_motor *mL, struct DC_motor *mR, int speed,
                        struct Movements *move, volatile char *exit_flag)
{
    move->move_type[move->move_number] = 0; // store move type as forwards
    moveForward(mL,mR,speed); // move robot forwards
    ClearLCD();
    LCD_String("MOVING TO BOMB");
    
    // this stores the number of consecutive times the robot misses the bomb
    // once it is over a threshold value, the robot starts searching again
    int error_counter = 0;
    
    // Runs until RFID has been scanned and break statement executes
    while(1)
    {
        // First, acquire the raw signal using the motion feedback module
        unsigned int raw_data = (unsigned int)((CAP1BUFH << 8) | CAP1BUFL);
        
        // Now, classify the signals to find if we are looking at the beacon
        char beacon_location = classify_data(raw_data);
        
        // if RFID read, return home
        if(*exit_flag == 1)
        {
            return 2; // return home
        }
        
        // if beacon lost, add 1 to error counter
        else if(beacon_location == 0)
        {
            error_counter += 1;
        }
        // if beacon found, reset error counter
        else
        {
            error_counter = 0;
        }
        // if the beacon has been lost for a set amount of time, start searching
        // 17000 is a parameter to adjust the sensitivity of how often it will 
        // start searching again
        if(error_counter >=17500)
        {
            move-> move_number += 1; // increment move number
            
            // if we run out of space to store the moves, return home
            if(move->move_number >= 19)
            {
                return 2;
            }
            else
            {
                 return 0; // go back to sweep mode
            }
        }
    }
}

// This subroutine makes the robot return to its starting position
volatile char returnHome(struct DC_motor *mL, struct DC_motor *mR, int move_speed,
                            int search_speed, struct Movements *move)
{
    // stop and display returning on LCD
    stop(mL,mR,move_speed);
    ClearLCD();
    LCD_String("RETURNING HOME");
    
    // Go back through array of moves and undo each previous move
    for(int i=move->move_number;move->move_number >0; move->move_number--)
    {
        // if move was forward
        if(move->move_type[move->move_number] == 0)
        {
            // reverse for same time as initial move
            moveBackward(mL,mR,move_speed); 
            while(move->time_taken[move->move_number] > 0);
        }
        // if move was left
        else if(move->move_type[move->move_number] == 1)
        {
            // turn right for same time as initial move
            turnRight(mL,mR,search_speed);
            while(move->time_taken[move->move_number] > 0);
        }
    }
    return 3; // exit subroutine
}

// This subroutine makes the robot stop and wait for further inputs at the end
volatile char stopAndDisplay(struct DC_motor *mL,struct DC_motor *mR, int speed,
                                volatile char RFID_buffer[])
{
    stop(mL, mR,speed); // stop moving
    
    // Display RFID and checksum result
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
    