/* 
 * File:   RFID.h
 * Author: charl
 *
 * Created on 18 February 2020, 10:54
 */

#ifndef RFID_H
#define	RFID_H

#ifdef	__cplusplus
extern "C" {
#endif

void init_RFID(void);
char getCharSerial(void);
void processRFID(char latestChar);
int check_data(char dataBuf[]);


#ifdef	__cplusplus
}
#endif

#endif	/* RFID_H */

