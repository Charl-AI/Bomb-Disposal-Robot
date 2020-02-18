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
    //TRISCbits.RC6 = 1;
    
    SPBRG = 203; //baud rate 9600
    SPBRGH = 0;
  
    BAUDCONbits.BRG16=1; //set baud scaling to 16 bit mode
    TXSTAbits.BRGH = 1; //high baud rate select bit
    RCSTAbits.CREN = 1; //continuous receive mode
    RCSTAbits.SPEN = 1; // enable serial port, default settings
    TXSTAbits.TXEN = 1; // enable transmitter, defualt settings
    
    PIE1bits.RC1IE = 1; // enable serial interrupt
}

// this function is called when the RFID serial interrupt is triggered
void processRFID(char latestChar)
{
    // we declare these as static so they do not change between function calls
    // this is better than declaring globally because they have limited scope
    static char RFIDbuf[12];
    static char position_in_buf;
    
    // if we see the final character in RFID, do the checksum and display result
    if(latestChar == 0x03)
    {
        if(check_data(RFIDbuf) == 1)
        {
            // if the checksum is passed, print the RFID
            ClearLCD();
            SetLine(1);
            LCD_String("CHECKSUM PASSED");
            SetLine(2);
            for(int i=0;i<10;i++)
            {
                SendLCD(RFIDbuf[i],1);
            }
        }
        else
        {
            // if the checksum is failed, print the RFID and checksum for
            // further inspection
            ClearLCD();
            SetLine(1);
            LCD_String("CHECKSUM FAILED");
            SetLine(2);
            for(int i=0;i<10;i++)
            {
                 SendLCD(RFIDbuf[i],1);
            }
            LCD_String("  ");
            SendLCD(RFIDbuf[10],1);
            SendLCD(RFIDbuf[11],1);
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
    
    // if checksum passes, return 1, otherwise 0
    if(byte1^byte2^byte3^byte4^byte5 == checksum)
    {
        return 1;
    }
    else{
        return 0;
    }
}