#ifndef XPT2046_H_
#define XPT2046_h_

#define CSnew LATBbits.LATB9
#define x_ADDR 0b11010001
#define y_ADDR 0b10010001
#define z1_ADDR 0b10110001
#define z2_ADDR 0b11000001

void XPT2046_read(unsigned short *x, unsigned short *y, unsigned int *z);
void get_pixel(unsigned short *x_pixel, unsigned short *y_pixel, unsigned short *x, unsigned short *y, unsigned int *z, unsigned char *pressed);
void draw_buttons(unsigned short color);
char buttonStat(unsigned short *x, unsigned short *y, unsigned char *pressed);

#endif