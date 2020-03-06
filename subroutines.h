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

volatile char scanForBeacon(struct DC_motor *mL, struct DC_motor *mR, int speed);

volatile char moveToBeacon(struct DC_motor *mL, struct DC_motor *mR, int speed,
unsigned long *micros, volatile char RFID_buffer[], volatile char *exit_flag);

volatile char returnHome(struct DC_motor *mL, struct DC_motor *mR, int speed,
                        volatile unsigned long *time);

volatile char stopAndDisplay(struct DC_motor *mL, struct DC_motor *mR, int speed);

void debug(void);

void waitForInput(void);

#ifdef	__cplusplus
}
#endif

#endif	/* SUBROUTINES_H */

