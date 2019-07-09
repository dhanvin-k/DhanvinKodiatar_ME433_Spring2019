#ifndef IMU_H__
#define IMU_H__

#define SLAVE_ADDR 0x6B
#define SCALE_FACTOR 0.006

void initIMU(void);
unsigned char getWHO_AM_I(void);
void I2C_read_multiple(unsigned char address, unsigned char registerAdd, unsigned char *data, int length);
void draw_progressbars(char color);
void draw_xprogress(signed int x, char color);
void draw_yprogress(signed int y, char color);

#endif