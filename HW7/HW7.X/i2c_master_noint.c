// I2C Master utilities, 100 kHz, using polling rather than interrupts
// The functions must be callled in the correct order as per the I2C protocol
// Change I2C1 to the I2C channel you are using
// I2C pins need pull-up resistors, 2k-10k

#include<xc.h>
#include"i2c_master_noint.h"

void i2c_master_setup(void) {
  I2C2BRG = 53;            // I2CBRG = [1/(2*Fsck) - PGD]*Pblck - 2 
                                    // look up PGD for your PIC32
  I2C2CONbits.ON = 1;               // turn on the I2C1 module
}

// Start a transmission on the I2C bus
void i2c_master_start(void) {
    I2C2CONbits.SEN = 1;            // send the start bit
    while(I2C2CONbits.SEN) { ; }    // wait for the start bit to be sent
}

void i2c_master_restart(void) {     
    I2C2CONbits.RSEN = 1;           // send a restart 
    while(I2C2CONbits.RSEN) { ; }   // wait for the restart to clear
}

void i2c_master_send(unsigned char byte) { // send a byte to slave
  I2C2TRN = byte;                   // if an address, bit 0 = 0 for write, 1 for read
  while(I2C2STATbits.TRSTAT) { ; }  // wait for the transmission to finish
  if(I2C2STATbits.ACKSTAT) {        // if this is high, slave has not acknowledged
    // ("I2C2 Master: failed to receive ACK\r\n");
  }
}

unsigned char i2c_master_recv(void) { // receive a byte from the slave
    I2C2CONbits.RCEN = 1;             // start receiving data
    while(!I2C2STATbits.RBF) { ; }    // wait to receive the data
    return I2C2RCV;                   // read and return the data
}

void i2c_master_ack(int val) {        // sends ACK = 0 (slave should send another byte)
                                      // or NACK = 1 (no more bytes requested from slave)
    I2C2CONbits.ACKDT = val;          // store ACK/NACK in ACKDT
    I2C2CONbits.ACKEN = 1;            // send ACKDT
    while(I2C2CONbits.ACKEN) { ; }    // wait for ACK/NACK to be sent
}

void i2c_master_stop(void) {          // send a STOP:
  I2C2CONbits.PEN = 1;                // comm is complete and master relinquishes bus
  while(I2C2CONbits.PEN) { ; }        // wait for STOP to complete
}

void initExpander(void) {
    i2c_master_setup();     // function to set BAUD of I2C at 400 kHz
    i2c_master_start();     // make the start bit
    i2c_master_send(SLAVE_ADDR << 1 | 0);       // write the address, shifted left by 1, or'ed with a 0 to indicate writing
    i2c_master_send(0x00);      // set the direction of the pins with IODIR register addressed at 0x00
    i2c_master_send(0xF0);      // set GP0 - 3 as OUTPUT pins and GP4 - 7 as INPUT pins
    i2c_master_stop();      // make the stop bit
}

void setExpander(char pin, char level) {
    i2c_master_start();     // make the start bit
    i2c_master_send(SLAVE_ADDR << 1  | 0);      // write the address, shifted left by 1, or'ed with a 0 to indicate writing
    i2c_master_send(0x0A);      // write to OLAT register addressed at 0x0A
    i2c_master_send(level << pin);      // set 'value'  'pin' number of OLAT registers
    i2c_master_stop();      // make the stop bit
}

char getExpander(void) {
    i2c_master_start();     // make the start bit
    i2c_master_send(SLAVE_ADDR << 1 | 0);       // write the address, shifted left by 1, or'ed with a 0 to indicate writing
    i2c_master_send(0x09);      // read from GPIO register addressed at 0x09
    i2c_master_restart();       // make the restart bit
    i2c_master_send((SLAVE_ADDR << 1) | 1);     // write the address, shifted left by 1, or'ed with a 1 to indicate reading
    char r = i2c_master_recv();     // save the value returned
    i2c_master_ack(1);      // make the ack bit
    i2c_master_stop();      // make the stop bit
    return r;
}

void initIMU(void) {
    // initialization of accelerometer register
    i2c_master_setup();     // function to set BAUD of I2C at 400 kHz
    i2c_master_start();     // make the start bit
    i2c_master_send(SLAVE_ADDR << 1 | 0);       // write the address, shifted left by 1, or'ed with a 0 to indicate writing
    i2c_master_send(0x10);      // write to CTRL1_XL register addressed at 0x10 (0b00010000)
    i2c_master_send(0x82)       // set register to 0b10000010 for sample rate to 1.66 kHz, with 2g sensitivity, and 100 Hz filter
    i2c_master_stop();      // make the stop bit
    
}