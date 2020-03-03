/* 
 File:   LCDIO.h
 Author: charl
 
 Created on 27 January 2020, 14:34
 
 This header file contains function prototypes from LCDIO.c and should be 
 used alongside that file whenever the LCD needs to be used
 */

#ifndef LCDIO_H
#define	LCDIO_H

#define _XTAL_FREQ 8000000 //define _XTAL_FREQ so delay routines work

// Define Pins attached to LCD
// if changing board or moving the LCD, we only need to change these definitions
#define LCD_RS LATAbits.LATA7
#define LCD_E LATAbits.LATA6
#define LCD_DB4 LATCbits.LATC0
#define LCD_DB5 LATCbits.LATC3
#define LCD_DB6 LATDbits.LATD0
#define LCD_DB7 LATDbits.LATD1

//function to toggle enable bit on then off
void E_TOG(void);

//function to send four bits to the LCD
void LCDout(unsigned char number);

//function to send data/commands over a 4bit interface
void SendLCD(unsigned char Byte, char type);

// function to initialise LCD display, always call this before doing anything
void init_LCD(void);

//function to put cursor to start of line
void SetLine (char line);

// output a string on the LCD
void LCD_String(char *string);

// function to clear the LCD
void ClearLCD(void);


#endif	/* LCDIO_H */

 