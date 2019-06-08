#ifndef IMU_H__
#define IMU_H__

#define SLAVE_ADDR 0x6B

void initIMU(void);
char getWHO_AM_I(void);

#endif