/* 
 * File:   subroutines.h
 * Author: charl
 *
 * Created on 05 March 2020, 10:36
 */

#ifndef SUBROUTINES_H
#define	SUBROUTINES_H

#ifdef	__cplusplus
extern "C" {
#endif

// structure to store the moves and time taken when going towards bomb
struct Movements {
    // move type: 0 is forwards,1 is left 
    char move_type[20];
    // time taken in units of 32 ms, the int will only overflow every 15 mins
    int time_taken[20];
    // stores which move number we are on (max 20)
    char move_number;
};
    
// routine to scan for the beacon
volatile char scanForBeacon(struct DC_motor *mL, struct DC_motor *mR, int speed,
                        struct Movements *move, volatile char *exit_flag);

// routine to move to beacon
volatile char moveToBeacon(struct DC_motor *mL, struct DC_motor *mR, int speed,
                        struct Movements *move, volatile char *exit_flag);

// routine to return home
volatile char returnHome(struct DC_motor *mL, struct DC_motor *mR, int move_speed,
                            int search_speed, struct Movements *move);

// routine to stop and display the result
volatile char stopAndDisplay(struct DC_motor *mL, struct DC_motor *mR, int speed,
volatile char RFID_buffer[]);

// routine to wait for input at the start
void waitForInput(void);

#ifdef	__cplusplus
}
#endif

#endif	/* SUBROUTINES_H */

