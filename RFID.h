/* 
 * File:   RFID.h
 * Author: charl
 *
 * Created on 18 February 2020, 10:54
 */

#ifndef RFID_H
#define	RFID_H

void init_RFID(void);
char getCharSerial(void);
char processRFID(char RFIDbuf[], char latestChar);
void check_RFID(char dataBuf[]);

#endif	/* RFID_H */

