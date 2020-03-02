/*
 RFID.c
 
 This file contains the functions for reading the RFID tag on the device
 
 18.02.20
 */
#include <pic18f4331.h>
#include "RFID.h"
#include "LCDIO.h"

// call this in the setup to initialise everything for the RFID reader
void init_RFID(void)
{
    // Set pins RC6 and RC7 as inputs
    TRISCbits.RC7 = 1;
    //TRISCbits.RC6 = 1;
    
    SPBRG = 203; //baud rate 9600
    SPBRGH = 0;
  
    BAUDCONbits.BRG16=1; //set baud scaling to 16 bit mode
    TXSTAbits.BRGH = 1; //high baud rate select bit
    RCSTAbits.CREN = 1; //continuous receive mode
    RCSTAbits.SPEN = 1; // enable serial port, default settings
    TXSTAbits.TXEN = 1; // enable transmitter, defualt settings
    
    PIE1bits.RC1IE = 1; // enable serial interrupt
    IPR1bits.RC1IP = 1; // Make interrupt high priority
}

// this function is called when the RFID serial interrupt is triggered
// it returns 1 when the RFID has been completely read and 0 otherwise
char processRFID(char RFIDbuf[], char latestChar)
{
    // here we store the position we are at in the buffer, it is declared
    // as static so it doesn't change between function calls
    // (this is better than declaring globally because it limits the scope)
    static char position_in_buf;
    
    // if we see the final character in RFID, return 1
    if(latestChar == 0x03)
    {
        return 1;
    }
    else
    {
       // if starting character is received, reset position to 0
       // and clear RFID buffer
       if(latestChar == 0x02)
        {
           position_in_buf = 0;
           for(char i=0 ;i<12 ;i++)
           {
               RFIDbuf[i] = 0;
           }
           // not finished reading RFID, so return 0
           return 0;
        } 
       // if any other data is received, add it to the buffer
       else
        {
            RFIDbuf[position_in_buf] = latestChar;
            position_in_buf++;
            // not finished reading RFID, so return 0
            return 0;
        }
    } 
}

// do checksum calculation, display result on LCD
void check_RFID(char dataBuf[])
{
    // array to store hex values
    char hexBuf[12];
    // convert each ASCII value to hex and store in buffer
    for(int i=0; i<12;i++)
    {
        if ((dataBuf[i] >= '0') && (dataBuf[i] <= '9')) 
        {
            hexBuf[i] = dataBuf[i] - '0';
        } else if ((dataBuf[i] >= 'A') && (dataBuf[i] <= 'F')) 
        {
            hexBuf[i] = 10 + dataBuf[i] - 'A';
        }  
    }
    // convert buffer into 5 hex bytes and a checksum
    char byte1 = 0x00 | (hexBuf[0] < 1) | hexBuf[1];
    char byte2 = 0x00 | (hexBuf[2] < 1) | hexBuf[3];
    char byte3 = 0x00 | (hexBuf[4] < 1) | hexBuf[5];
    char byte4 = 0x00 | (hexBuf[6] < 1) | hexBuf[7];
    char byte5 = 0x00 | (hexBuf[8] < 1) | hexBuf[9];
    char checksum = 0x00 | (hexBuf[10] < 1) | hexBuf[11];
    
    // check if XOR of data is equal to checksum
    if(byte1^byte2^byte3^byte4^byte5 == checksum)
    {
        // if good, display checksum passed on screen
        SetLine(2);
        LCD_String("CHECKSUM PASSED");
    }
    else{
        // if bad, display checksum failed on screen
        SetLine(2);
        LCD_String("CHECKSUM FAILED");
    }
}

void display_RFID(char dataBuf[])
{
    ClearLCD();
    SetLine(1);
    for(int i=0;i<10;i++)
    {
        SendLCD(dataBuf[i],1);
    }
}