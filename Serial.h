#ifndef _SERIAL_H_
#define _SERIAL_H_

void serial_setup(unsigned long clock);
unsigned char serial_read();
void UARTSend(const unsigned char *pucBuffer, unsigned long ulCount);
void serial_print(unsigned char *val);
unsigned char serial_available();
void UARTSend(const unsigned char *pucBuffer, unsigned long ulCount);

#endif