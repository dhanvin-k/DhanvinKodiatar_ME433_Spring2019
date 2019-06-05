#include<xc.h>
#include"imu.h"
#include"i2c_master_noint.h"

void initIMU(void) {
    i2c_master_setup();     // function to set BAUD of I2C at 400 kHz
    
    // initialization of CTRL1_XL register for the accelerometer
    i2c_master_start();     // make the start bit
    i2c_master_send(SLAVE_ADDR << 1 | 0);       // write the address, shifted left by 1, or'ed with a 0 to indicate writing
    i2c_master_send(0x10);      // write to CTRL1_XL register addressed at 0x10 (0b00010000)
    i2c_master_send(0x82)       // set register to 0b10000010 for sample rate to 1.66 kHz, with 2g sensitivity, and 100 Hz filter
    i2c_master_stop();      // make the stop bit
    
    // initialization of CTRL2_G register for the gyroscope
    i2c_master_start();     // make the start bit
    i2c_master_send(SLAVE_ADDR << 1 | 0);       // write the address, shifted left by 1, or'ed with a 0 to indicate writing
    i2c_master_send(0x11);      // write to CTRL2_G register addressed at 0x11 (0b00010001)
    i2c_master_send(0x88)       // set register to 0b10001000 for sample rate to sample rate to 1.66 kHz, with 1000 dps sensitivity
    i2c_master_stop();      // make the stop bit
    
    // initialization of CTRL3_C register
    i2c_master_start();     // make the start bit
    i2c_master_send(SLAVE_ADDR << 1 | 0);       // write the address, shifted left by 1, or'ed with a 0 to indicate writing
    i2c_master_send(0x12);      // write to CTRL3_C register addressed at 0x12 (0b00010010)
    i2c_master_send(0x04)       // set register to 0b00000100 for sample rate to 1.66 kHz, with 2g sensitivity, and 100 Hz filter
    i2c_master_stop();      // make the stop bit
}