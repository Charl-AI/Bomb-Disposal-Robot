/* 
 File:   dc_motor.h
 Author: charl
 
 
 This header file contains function declarations for moving the robot
 and also contains the definition of the structure used to store the variables
 */

#ifndef _DC_MOTOR_H
#define _DC_MOTOR_H

#define _XTAL_FREQ 8000000

//definition of DC_motor structure, this stores the variables for each motor
struct DC_motor { 
    char power;         //motor power, out of 100
    char direction;     //motor direction, forward(1), reverse(0)
    unsigned char *dutyLowByte; //PWM duty low byte address
    unsigned char *dutyHighByte; //PWM duty high byte address
    char dir_pin; // pin that controls direction on PORTB
    int PWMperiod; //base period of PWM cycle
};

// Call this in the setup to initialise PWM registers
void initPWM(int PWMperiod); // function to setup PWM

// function for initialising values in each structure. Call outside the loop
// at the top of main
void initMotorValues(struct DC_motor *mL, struct DC_motor *mR);

// This is used as part of the movement functions, do not call this from
// the main file. This should be declared privately in future.
void setMotorPWM(struct DC_motor *m); // function to set PWM

/* 
 functions for moving robot
 Note: we deliberately bias the forward and reverse movements to curve slightly 
 right becuase we would prefer to miss right than left if we miss the beacon
*/
void stop(struct DC_motor *mL, struct DC_motor *mR, int initial_speed);
void turnLeft(struct DC_motor *mL, struct DC_motor *mR, int max_power);
void turnRight(struct DC_motor *mL, struct DC_motor *mR, int max_power);
void moveForward(struct DC_motor *mL, struct DC_motor *mR, int max_power);
void moveBackward(struct DC_motor *mL, struct DC_motor *mR, int max_power);





#endif