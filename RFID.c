/*
 This file contains the functions for reading the RFID tag on the device
 
 
 18.02.20
 */
#include <pic18f4331.h>
#include "RFID.h"
#include "LCDIO.h"

// call this in the setup to initialise everything
void init_RFID(void)
{
    // Set pins RC6 and RC7 as inputs
    TRISCbits.RC7 = 1;
    TRISCbits.RC6 = 1;
    
    SPBRG = 203; //baud rate 9600
    SPBRGH = 0;
  
    BAUDCONbits.BRG16=1; //set baud scaling to 16 bit mode
    TXSTAbits.BRGH = 1; //high baud rate select bit
    RCSTAbits.CREN = 1; //continuous receive mode
    RCSTAbits.SPEN = 1; // enable serial port, default settings
    TXSTAbits.TXEN = 1; // enable transmitter, defualt settings
    
    PIE1bits.RC1IE = 1; // enable serial interrupt
}


void processRFID(char latestChar)
{
    // we declare these as static so they do not change between function calls
    static char RFIDbuf[14];
    static char position_in_buf;
    
    // if final character in RFID, do the checksum and display result
    if(latestChar == 0x03)
    {
        
        ClearLCD();
        SetLine(1);
        LCD_String("CHECKSUM PASSED");
        SetLine(2);
        for(int i=1;i<11;i++)
        {
            SendLCD(RFIDbuf[i],1);
        }
    }
    else
    {
       // if starting character is received, reset position to 0
       if(latestChar == 0x02)
        {
            position_in_buf = 0;
        } 
       // if any other data is received, add it to the buffer
       else
        {
            RFIDbuf[position_in_buf] = latestChar;
            position_in_buf++;
        }
    } 
}

// do checksum calculation, return 1 if good, 0 otherwise
int check_data(char dataBuf[])
{
    
}