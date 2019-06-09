#ifndef IMU_H__
#define IMU_H__

#define SLAVE_ADDR 0x6B

void initIMU(void);
char getWHO_AM_I(void);
void I2C_read_multiple(unsigned char address, unsigned char registerAdd, unsigned char *data, int length);
void hello(int);

#endif