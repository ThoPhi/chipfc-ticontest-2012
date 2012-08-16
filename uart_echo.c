//*****************************************************************************
//
// uart_echo.c - Example for reading data from and writing data to the UART in
//               an interrupt driven fashion.
//
// Copyright (c) 2005-2012 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 8555 of the EK-LM3S811 Firmware Package.
//
//*****************************************************************************

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "drivers/display96x16x1.h"
#include "inc/lm3s811.h"
//#include "inc/MeetAndroid/MeetAndroid.h"
#include "Amarino.h"
#include "Test2/Serial.h"
//*****************************************************************************
//
//! \addtogroup example_list
//! <h1>UART Echo (uart_echo)</h1>
//!
//! This example application utilizes the UART to echo text.  The first UART
//! (connected to the FTDI virtual serial port on the Stellaris LM3S811
//! Evaluation Board) will be configured in 115,200 baud, 8-n-1 mode.  All
//! characters received on the UART are transmitted back to the UART.
//
//*****************************************************************************

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, unsigned long ulLine)
{
}
#endif
//
////
//void
//UARTIntHandler(void)
//{
//    unsigned long ulStatus;
//
//    //
//    // Get the interrrupt status.
//    //
//    ulStatus = UARTIntStatus(UART1_BASE, true);
//
//    //
//    // Clear the asserted interrupts.
//    //
//    UARTIntClear(UART1_BASE, ulStatus);
//
//    //
//    // Loop while there are characters in the receive FIFO.
//    //
//    while(UARTCharsAvail(UART1_BASE))
//    {
//        //
//        // Read the next character from the UART and write it back to the UART.
//        //
//        UARTCharPutNonBlocking(UART1_BASE, UARTCharGetNonBlocking(UART1_BASE));
//    }
//}
////
//////*****************************************************************************
//////
////// Send a string to the UART.
//////
//////*****************************************************************************
//void
//UARTSend(const unsigned char *pucBuffer, unsigned long ulCount)
//{
//    //
//    // Loop while there are more characters to send.
//    //
//    while(ulCount--)
//    {
//        //
//        // Write the next character to the UART.
//        //
//        UARTCharPutNonBlocking(UART1_BASE, *pucBuffer++);
//    }
//}


//*****************************************************************************
//
// This example demonstrates how to send a string of data to the UART.
//
//*****************************************************************************

void getFirst(unsigned char flag, unsigned char numOfValues);


int
main(void)
{
    unsigned int i,j;
    //
    // Set the clocking to run directly from the crystal.
    //
//    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
//                   SYSCTL_XTAL_6MHZ);

    //
    // Initialize the OLED display and write status.
    //
    Display96x16x1Init(false);
    Display96x16x1StringDraw("Amarino Test", 0, 0);
   Display96x16x1StringDraw("115200, Chipfc", 6, 1); 

     serial_setup(115200);
     serial_print("enter text: ");

    //
    // Loop forever echoing data through the UART.
    //
         //

            // Enable the GPIO port that is used for the on-board LED.
    //
    SYSCTL_RCGC2_R = SYSCTL_RCGC2_GPIOC;

    //
    // Do a dummy read to insert a few cycles after enabling the peripheral.
    //
    i = SYSCTL_RCGC2_R;

    //
    // Enable the GPIO pin for the LED (PC5).  Set the direction as output, and
    // enable the GPIO pin for digital function.  Care is taken to not disrupt
    // the operation of the JTAG pins on PC0-PC3.
    //
    GPIO_PORTC_DIR_R |= 0x20;
    GPIO_PORTC_DEN_R |= 0x20;
    
    MeetAndroid_init();
    MeetAndroid_registerFunction(getFirst,'a');
    

   for(i=0;i<10000;i++)
   {
     GPIO_PORTC_DATA_R |= 0x20;
    for (j=0;j<10000;j++);
            GPIO_PORTC_DATA_R &= ~(0x20);
    for (j=0;j<10000;j++);
    
   }
      Display96x16x1Clear();
   Display96x16x1StringDraw("now begin",0,0);
   
    while(1)
    {
      
      MeetAndroid_receive();
    }
}
//
void getFirst(unsigned char  flag, unsigned char numOfValues)
{
    int tmp = MeetAndroid_getInt();
    if (tmp == 65){
      Display96x16x1StringDraw("bs",6,1);
    }
}