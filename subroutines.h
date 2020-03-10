/* 
 File:   subroutines.h
 Author: charl

 This file contains the declarations for the subroutines that the robot performs 
 when it is in each mode. The code is placed in this source file in order to 
 de-clutter the main function and improve readability.
 
 The robot operates on 4 subroutines, searching, moving forwards, returning and
 displaying the results
 
 A 'wait for inputs' routine is also defined here and is used on startup to
 stabilise the signal processing and to wait for a button press to start 

 Created on 05 March 2020, 10:36
 */

#ifndef SUBROUTINES_H
#define	SUBROUTINES_H

#ifdef	__cplusplus
extern "C" {
#endif

// structure to store the moves and time taken when going towards bomb
struct Movement_storage {
    // move type: 0 is forwards,1 is left 
    char move_type[20];
    // time taken in units of 32 ms, the int will only overflow every 15 mins
    int time_taken[20];
    // stores which move number we are on (max 20)
    char move_number;
};
    
// routine to scan for the beacon
volatile char scanForBeacon(struct DC_motor *mL, struct DC_motor *mR, int speed,
                    struct Movement_storage *move, volatile char *exit_flag);

// routine to move to beacon
volatile char moveToBeacon(struct DC_motor *mL, struct DC_motor *mR, int speed,
                    struct Movement_storage *move, volatile char *exit_flag);

// routine to return home
volatile char returnHome(struct DC_motor *mL, struct DC_motor *mR, int move_speed,
                            int search_speed, struct Movement_storage *move);

// routine to stop and display the result
volatile char stopAndDisplay(struct DC_motor *mL,struct DC_motor *mR, int speed,
                            volatile char RFID_buffer[]);

// routine to wait for input at the start
void waitForInput(void);

#ifdef	__cplusplus
}
#endif

#endif	/* SUBROUTINES_H */

