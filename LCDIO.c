/*
 LCDIO.c
 Here we have all the functions necessary for operation of the LCD.
 This file and its corresponding header can be used in any project that
 needs the LCD so the code is as modular as possible
 
 Note:
 Make sure LCD pins have already been defined in LCDIO.h file, e.g:
 //#define _XTAL_FREQ 8000000 //define _XTAL_FREQ so delay routines work
 //#define LCD_RS LATbits.LATA6
 //#define LCD_E LATCbits.LATC0
 //#define LCD_DB4 LATCbits.LATC1
 //#define LCD_DB5 LATCbits.LATC2
 //#define LCD_DB6 LATDbits.LATD0
 //#define LCD_DB7 LATDbits.LATD1
 
 */
#include "LCDIO.h"
#include <xc.h>
#include <pic18f4331.h>

//function to toggle enable bit on then off
void E_TOG(void){
// Toggles E on then off, E is connected to RC0    
LCD_E = 1;
__delay_us(5); // 5us delay 
LCD_E = 0;
}

//function to send four bits to the LCD
void LCDout(unsigned char number){
//set data pins using the four bits from number
 LCD_DB4 = (0b0001 & number); 
 LCD_DB5 = (0b0010 & number) >> 1;
 LCD_DB6 = (0b0100 & number) >> 2;
 LCD_DB7 = (0b1000 & number) >> 3;
 
 //toggle the enable bit to send data
 E_TOG();
__delay_us(5); // 5us delay
}

//function to send data/commands over a 4bit interface
void SendLCD(unsigned char Byte, char type){
 // set RS pin whether it is a Command (0) or Data/Char (1)
 // using type as the argument
 LCD_RS = type;
 // send high bits of Byte using LCDout function
 LCDout((0b11110000 & Byte) >> 4);
 __delay_us(10); // 10us delay
 
 //char low_nibble = (0b00001111 & Byte);
 LCDout(0b00001111 & Byte);
 __delay_us(50); // 10us delay
}
void init_LCD(void){
 
 // Set all pins to outputs
 TRISCbits.RC0 = 0;
 TRISAbits.RA7 = 0;
 TRISCbits.RC3 = 0;
 TRISDbits.RD0 = 0;
 TRISDbits.RD1 = 0;
 TRISAbits.RA6 = 0;
 
 // set values for pins to prevent random errors
 LCD_DB6 = 0;
 LCD_DB7 = 0;
 LCD_DB5 = 0;
 LCD_DB4 = 0;
 LCD_E = 0;
 LCD_RS = 0;
 
 // Initialisation stage (from datasheet)
 __delay_ms(15);
 LCDout(0b0011);
 __delay_ms(5);
 LCDout(0b0011);
 __delay_us(200);
 LCDout(0b0011);
 __delay_us(50);
 LCDout(0b0010);
 __delay_us(50);
 
 // Set function (4-bit, 2-line, 5x10 dots)
 SendLCD(0b00101000,0);
 __delay_us(50);
 
 //display off
 SendLCD(0b00001000,0);
 __delay_us(50);
 
 // clear screen
 SendLCD(0b00000001,0);
 __delay_ms(5);
 
 // Set entry mode (cursor increases, display shifts)
 SendLCD(0b00000110,0);
 __delay_us(50);
 
 // Display on (cursor on, blinking on)
 SendLCD(0b00001100,0);
 __delay_us(50);
}

//function to put cursor to start of line
void SetLine (char line){
//Send 0x80 to set line to 1 (0x00 ddram address)
    if(line == 1){
        SendLCD(0x80,0);
    }
//Send 0xC0 to set line to 2 (0x40 ddram address)
    if(line == 2){
        SendLCD(0xC0,0);
    }
 __delay_us(50); // 50us delay 
}

void LCD_String(char *string){
 //While the data pointed to isn?t a 0x00 do below
while(*string != 0){
 //Send out the current byte pointed to
 // and increment the pointer
SendLCD(*string++,1);
 __delay_us(50); //so we can see each character
 //being printed in turn (remove delay if you want
 //your message to appear almost instantly)
 }
}

void ShiftLeft(void){
    SendLCD(0b00011000,0);
}
void ShiftRight(void){
    SendLCD(0b00011100,0);
}
void ClearLCD(void){
    SendLCD(0b00000001,0);
    __delay_ms(5);
}

