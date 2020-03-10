/* 
 File:   signal_processing.h
 Author: charl

 This file contains the signal processing and classification 
 function declarations, as well as a function to initialise the sensors and
 stabilise the readings
 
 Created on 21 February 2020, 18:21
 */

#ifndef SIGNAL_PROCESSING_H
#define	SIGNAL_PROCESSING_H

// This function initialises the sensors
void init_sensor(void);

// this function returns 1 if the beacon is straight ahead and 0 otherwise
char classify_data(unsigned int raw_data);

// call this function once at the top of the main function to stabilise the
// moving average value before the start
void stabiliseAverage(void);


#endif	/* SIGNAL_PROCESSING_H */

