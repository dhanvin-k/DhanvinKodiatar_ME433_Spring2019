#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro

#ifndef _spi_H    /* Guard against multiple inclusion */
#define _spi_H

#define CS LATBbits.LATB7
void init_spi(void);
unsigned char spi_io(unsigned char);

#endif