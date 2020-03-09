#ifndef _DC_MOTOR_H
#define _DC_MOTOR_H

#define _XTAL_FREQ 8000000

//definition of DC_motor structure
struct DC_motor { 
    char power;         //motor power, out of 100
    char direction;     //motor direction, forward(1), reverse(0)
    unsigned char *dutyLowByte; //PWM duty low byte address
    unsigned char *dutyHighByte; //PWM duty high byte address
    char dir_pin; // pin that controls direction on PORTB
    int PWMperiod; //base period of PWM cycle
};

//function prototypes
void initPWM(int PWMperiod); // function to setup PWM
void setMotorPWM(struct DC_motor *m); // function to set PWM

// functions for moving robot
void stop(struct DC_motor *mL, struct DC_motor *mR, int initial_speed);
void turnLeft(struct DC_motor *mL, struct DC_motor *mR, int max_power);
void turnRight(struct DC_motor *mL, struct DC_motor *mR, int max_power);

void moveForward(struct DC_motor *mL, struct DC_motor *mR, int max_power);
void moveBackward(struct DC_motor *mL, struct DC_motor *mR, int max_power);

// function for initialising values in each structure
void init_motor_struct(struct DC_motor *mL, struct DC_motor *mR);



#endif