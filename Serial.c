#include "Serial.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "drivers/display96x16x1.h"


//*****************************************************************************
//
// The UART initial
//
//*****************************************************************************

void serial_setup(unsigned long clock){
  
    //
    // Set the clocking to run directly from the crystal.
    //
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_6MHZ);

    //
    // Enable the peripherals used by this example.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Enable processor interrupts.
    //
    IntMasterEnable();

    //

    //
    // Set GPIO A0 and A1 as UART pins.
    //
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    

 
//    unsigned long ulLoop;
    // Configure the UART for 115,200, 8-N-1 operation.
    //
      UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), clock,
                        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                         UART_CONFIG_PAR_NONE));

    //
    // Enable the UART interrupt.
    //
 
    
    IntEnable(INT_UART0);
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
    
}

//*****************************************************************************
//
// The UART interrupt handler.
//
//*****************************************************************************

void
UARTIntHandler(void)
{
    unsigned long ulStatus;
  
    //
    // Get the interrrupt status.
    //
    ulStatus = UARTIntStatus(UART0_BASE, true);
 
    //
    // Clear the asserted interrupts.
    //
    UARTIntClear(UART0_BASE, ulStatus);
  
    
    //
    // Loop while there are characters in the receive FIFO.
    //
//    while(UARTCharsAvail(UART0_BASE))
//    {
//        //
//        // Read the next character from the UART and write it back to the UART.
//        //
//        UARTCharPutNonBlocking(UART0_BASE, UARTCharGetNonBlocking(UART0_BASE));
//    }
}

//*****************************************************************************
//
// Send a string to the UART.
//
//*****************************************************************************
void UARTSend(const unsigned char *pucBuffer, unsigned long ulCount)
{
    //
    // Loop while there are more characters to send.
    //
    while(ulCount--)
    {
        //
        // Write the next character to the UART.
        //
        UARTCharPutNonBlocking(UART0_BASE, *pucBuffer++);
    }
}


void serial_print(unsigned char *val)
{
  UARTSend(val,12);//const unsigned char *pucBuffer, unsigned long ulCount);
}

//*****************************************************************************
//
// is serial available
//
//*****************************************************************************
unsigned char serial_available()
{
  
  return UARTCharsAvail(UART0_BASE);
}


unsigned char serial_read()
{
  return UARTCharGetNonBlocking(UART0_BASE);
}
