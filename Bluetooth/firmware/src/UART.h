#ifndef _UART_H
#define _UART_H

#define PIC_FREQ 48000000ul    // 48 million Hz
#define BAUD_RATE 96000    // Baudrate

void ReadUART(char * string, int maxLength);
void WriteUART(const char * string);

#endif