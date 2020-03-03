/* 
 * File:   signal_processing.h
 * Author: charl
 *
 * Created on 21 February 2020, 18:21
 *
 */

#ifndef SIGNAL_PROCESSING_H
#define	SIGNAL_PROCESSING_H

// structure to store properties for each sensor
struct Sensor { 
    int raw_data; // raw PWM duty cycle
    int smoothed_signal; // signal after smoothing and processing
};

void init_sensor(void);
void process_signal(struct Sensor *S);
char classify_data(int left_smoothed, int right_smoothed);

#endif	/* SIGNAL_PROCESSING_H */

