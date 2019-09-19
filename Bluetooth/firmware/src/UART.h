#ifndef _UART_H
#define _UART_H

#define PIC_FREQ 48000000ul    // 48 million Hz
#define BAUD_RATE 9600    // Baudrate

void UART_init(void);
void ReadUART(char * string, int maxLength);
void WriteUART(const char * string);

#endif