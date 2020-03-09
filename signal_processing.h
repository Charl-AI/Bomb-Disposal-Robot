/* 
 * File:   signal_processing.h
 * Author: charl
 *
 * Created on 21 February 2020, 18:21
 */

#ifndef SIGNAL_PROCESSING_H
#define	SIGNAL_PROCESSING_H

// This function initialises the sensors
void init_sensor(void);

// this function returns 1 if the beacon is straight ahead and 0 otherwisw
char classify_data(unsigned int raw_data);

#endif	/* SIGNAL_PROCESSING_H */

