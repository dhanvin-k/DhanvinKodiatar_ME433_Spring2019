#include <xc.h>
#include "UART.h"

#define BAUD_RATE 96000    // Baudrate

void UART_Startup() {
  // disable interrupts
  __builtin_disable_interrupts();
  
  // TRIS operations to make B2 & B3 make Rx and Tx pins repectively
  TRISBbits.TRISB2 = 1;
  TRISBbits.TRISB3 = 0;


  // configure TX & RX pins as output & input pins
  U1RXRbits.U1RXR = 4;
  RPB3Rbits.RPB3R = 1;   
//  U3STAbits.UTXEN = 1;
//  U3STAbits.URXEN = 1;

  // enable the UART
  U1MODEbits.ON = 1;

  __builtin_enable_interrupts();
}

void readUART(char * message, int maxLength) {
  char data = 0;
  int complete = 0, num_bytes = 0;
  // loop until you get a '\r' or '\n'
  while (!complete) {
    if (U3STAbits.URXDA) { // if data is available
      data = U3RXREG;      // read the data
      if ((data == '\n') || (data == '\r')) {
        complete = 1;
      } else {
        message[num_bytes] = data;
        ++num_bytes;
        // roll over if the array is too small
        if (num_bytes >= maxLength) {
          num_bytes = 0;
        }
      }
    }
  }
  // end the string
  message[num_bytes] = '\0';
}

void writeUART(const char * string) {
  while (*string != '\0') {
    while (U3STAbits.UTXBF) {
      ; // wait until tx buffer isn't full
    }
    U3TXREG = *string;
    ++string;
  }
}