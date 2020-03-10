/* 
 dc_motor.c
 
 This file contains the functions required to move the robot
 */

#include <xc.h>
#include "dc_motor.h"

// PWMperiod input in seconds. This function is called in setup to initialise
// the motor PWM registers
void initPWM(int PWMperiod){
    
    PTCON0 = 0b00000000; // free running mode, 1:1 prescaler = 0.5us
    PTCON1 = 0b10000000; // enable PWM timer
    
    PWMCON0 = 0b01101111; // PWM0/1 enabled, independent mode
    PWMCON1 = 0x00; //special features, all 0
    
    PTPERL = (0b11111111 & PWMperiod); // base PWM period low byte
    PTPERH = (0b1111111100000000 & PWMperiod) >> 8; // base PWM period high byte
}

// function to set PWM output from the values in the motor structure
void setMotorPWM(struct DC_motor *m)
{
    int PWMduty; //temp variable to store PWM duty cycle

    PWMduty = (m->power*m->PWMperiod)/100;  //calculate duty cycle (value between 0 and PWMperiod)
    
    if (m->direction) //if forward direction
    {
        LATB=LATB|(1<<(m->dir_pin)); //set dir_pin high in LATB
		PWMduty=m->PWMperiod-PWMduty; //need to invert duty cycle as direction is high (100% power is a duty cycle of 0)
    }
    else //if reverse direction
    {
        LATB=LATB&(~(1<<(m->dir_pin))); //set dir_pin low in LATB
    }

	//write duty cycle value to appropriate registers
    *(m->dutyLowByte)=PWMduty<<2;
    *(m->dutyHighByte)=PWMduty>>6;
}

//function to stop the robot gradually 
void stop(struct DC_motor *mL, struct DC_motor *mR, int initial_speed)
{
	for(int i = initial_speed; i > 0; i--)
    {
        mL->power = i;
        mR->power = i;
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_ms(1);
    }
}

//function to make the robot turn on the spot 
void turnLeft(struct DC_motor *mL, struct DC_motor *mR, int max_power)
{
	//remember to change the power gradually
    mL->direction = 0;
    mR->direction = 1;
	//remember to change the power gradually
    for(int i = 0; i<max_power;i++){
        mL->power = i;
        mR->power = i;
        
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_ms(1);
    }
}

void turnRight(struct DC_motor *mL, struct DC_motor *mR, int max_power)
{
	//remember to change the power gradually
    mL->direction = 1;
    mR->direction = 0;
	//remember to change the power gradually
    for(int i = 0; i<max_power;i++){
        mL->power = i;
        mR->power = i;
        
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_ms(1);
    }
}

/* 
 function to make the robot go straight
 Note: we deliberately bias the movement to curve slightly right becuase we
 would prefer to miss right than left if we miss the beacon
*/
void moveForward(struct DC_motor *mL, struct DC_motor *mR, int max_power)
{
    mL->direction = 1;
    mR->direction = 1;
    for(int i = 5; i < max_power; i++)
    {
        mL->power = i; 
        mR->power = i - 5; // slightly underpower right motor to curve the path
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_ms(1);
    }
}

/*
 function to make the robot go straight
 Note: we also bias the robot in this function because we need to 'undo'
 the effect of the curvilinear path in the forwards one
*/

void moveBackward(struct DC_motor *mL, struct DC_motor *mR, int max_power)
{
    mL->direction = 0;
    mR->direction = 0;
    for(int i = 5; i < max_power; i++)
    {
        mL->power = i;
        mR->power = i - 5; // slightly underpower right motor to curve the path
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_ms(1);
    }
}

void initMotorValues(struct DC_motor *mL, struct DC_motor *mR)
{
    //some code to set initial values of each structure
    mL->power = 0;
    mL->direction = 1;
    mL->dutyLowByte = (unsigned char *)(&PDC0L);
    mL->dutyHighByte = (unsigned char *)(&PDC0H);
    mL->dir_pin=0;
    mL->PWMperiod=199;

    mR->power = 0;
    mR->direction = 1;
    mR->dutyLowByte = (unsigned char *)(&PDC1L);
    mR->dutyHighByte = (unsigned char *)(&PDC1H);
    mR->dir_pin=2;
    mR->PWMperiod=199;

    // set PWM for each motor to prevent random startup values
    setMotorPWM(mL);
    setMotorPWM(mR);
}