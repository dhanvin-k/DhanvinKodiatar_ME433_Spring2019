#include<xc.h>
#include"imu.h"
#include"i2c_master_noint.h"
#include"ili9341.h"

void initIMU(void) {
    i2c_master_setup();     // function to set BAUD of I2C at 400 kHz
    
    // initialization of CTRL1_XL register for the accelerometer
    i2c_master_start();     // make the start bit
    i2c_master_send(SLAVE_ADDR << 1 | 0);       // write the address, shifted left by 1, or'ed with a 0 to indicate writing
    i2c_master_send(0x10);      // write to CTRL1_XL register addressed at 0x10 (0b00010000)
    i2c_master_send(0x82);       // set register to 0b10000010 for sample rate to 1.66 kHz, with 2g sensitivity, and 100 Hz filter
    i2c_master_stop();      // make the stop bit
    
    // initialization of CTRL2_G register for the gyroscope
    i2c_master_start();     // make the start bit
    i2c_master_send(SLAVE_ADDR << 1 | 0);       // write the address, shifted left by 1, or'ed with a 0 to indicate writing
    i2c_master_send(0x11);      // write to CTRL2_G register addressed at 0x11 (0b00010001)
    i2c_master_send(0x88);       // set register to 0b10001000 for sample rate to sample rate to 1.66 kHz, with 1000 dps sensitivity
    i2c_master_stop();      // make the stop bit
    
    // initialization of CTRL3_C register
    i2c_master_start();     // make the start bit
    i2c_master_send(SLAVE_ADDR << 1 | 0);       // write the address, shifted left by 1, or'ed with a 0 to indicate writing
    i2c_master_send(0x12);      // write to CTRL3_C register addressed at 0x12 (0b00010010)
    i2c_master_send(0x04);       // set register to 0b00000100 for sample rate to 1.66 kHz, with 2g sensitivity, and 100 Hz filter
    i2c_master_stop();      // make the stop bit
}

char getWHO_AM_I(void) {
    i2c_master_start();     // make the start bit
    i2c_master_send(SLAVE_ADDR << 1 | 0);       // write the address, shifted left by 1, or'ed with a 0 to indicate writing
    i2c_master_send(0x0F);      // read from WHO_AM_I register addressed at 0x0F
    i2c_master_restart();       // make the restart bit
    i2c_master_send((SLAVE_ADDR << 1) | 1);     // write the address, shifted left by 1, or'ed with a 1 to indicate reading
    char r = i2c_master_recv();     // save the value returned
    i2c_master_ack(1);      // make the ack bit
    i2c_master_stop();      // make the stop bit
    return r;
}

void I2C_read_multiple(unsigned char address, unsigned char registerAdd, unsigned char *data, int length) {
    int i = 0;
    i2c_master_start();     // make the start bit
    i2c_master_send(SLAVE_ADDR << 1 | 0);       // write the address, shifted left by 1, or'ed with a 0 to indicate writing
    i2c_master_send(registerAdd);      // read from WHO_AM_I register addressed at 0x0F
    i2c_master_restart();       // make the restart bit
    i2c_master_send((SLAVE_ADDR << 1) | 1);     // write the address, shifted left by 1, or'ed with a 1 to indicate reading
    while(i<length) {
        data[i] = i2c_master_recv();     // save the value returned
        if(i == length-1) {
            i2c_master_ack(1);      // make the ack bit
        }
        else {
            i2c_master_ack(0);      // make the ack bit
        }
        i = i + 1;
    }
    i2c_master_stop();      // make the stop bit
}

short getTemp(unsigned char *data) {
    short temp = (data[1]<<8 | data[0]);
    return temp;
}

short getGyroX(unsigned char *data) {
    short gyrox = data[3]<<8 | data[2];
    return gyrox;
}

short getGyroY(unsigned char *data) {
    short gyroy = data[5]<<8 | data[4];
    return gyroy;
}

short getGyroZ(unsigned char *data) {
    short gyroz = data[7]<<8 | data[6];
    return gyroz;
}

short getXLX(unsigned char *data) {
    short getXLX = data[9]<<8 | data[8];
    return getXLX;
}

short getXLY(unsigned char *data) {
    short getXLY = data[11]<<8 | data[10];
    return getXLY;
}

short getXLZ(unsigned char *data) {
    short getXLZ = data[13]<<8 | data[12];
    return getXLZ;
}

void hello(int x) {
    unsigned char message[5];
    sprintf(message, "hello");
    print_message(28, 32+x, message);
}