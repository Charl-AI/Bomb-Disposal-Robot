/* 
 File:   RFID.h
 Author: charl

 This file contains the functions for reading the RFID tag on the device
 
 Created on 18 February 2020, 10:54
 */

#ifndef RFID_H
#define	RFID_H

// initialise registers for RFID reader
void init_RFID(void);

// Take the latest character from the RFID and add to buffer. If it is 0x03,
// return 1
char processRFID(volatile char RFIDbuf[], char latestChar);

// check RFID with checksum
void check_RFID(volatile char dataBuf[]);

// display RFID on LCD
void display_RFID(volatile char RFIDBuf[]);

#endif	/* RFID_H */

