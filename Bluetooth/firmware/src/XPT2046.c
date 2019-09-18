#include<xc.h>
#include"ili9341.h"
#include"XPT2046.h"
#include<string.h>

void XPT2046_read(unsigned short *x, unsigned short *y, unsigned int *z)  {
    unsigned short left, right;
    
    CSnew = 0;
    spi_io(x_ADDR);
    unsigned short xtemp = (spi_io(0x00)<<5) | (spi_io(0x00)>>3);
    CSnew = 1;
    
    CSnew = 0;
    spi_io(y_ADDR);
    unsigned short ytemp = (spi_io(0x00)<<5) | (spi_io(0x00)>>3);
    CSnew = 1;
   
    CSnew = 0;
    spi_io(z1_ADDR);
    unsigned short z1temp = (spi_io(0x00)<<5) | (spi_io(0x00)>>3);
    CSnew = 1;
    
    CSnew = 0;
    spi_io(z2_ADDR);
    unsigned short z2temp = (spi_io(0x00)<<5) | (spi_io(0x00)>>3);
    CSnew = 1;
    
    *x = xtemp;
    *y = ytemp;
    *z = z1temp - z2temp + 4095;
}

void get_pixel(unsigned short *x_pixel, unsigned short *y_pixel, unsigned short *x, unsigned short *y, unsigned int *z, unsigned char *pressed) {
    *pressed = 0;
    if(*z > 500) {
        *pressed = 1;
    }
    *x_pixel = 0;
    *y_pixel = 0;
    if(*pressed) {
        *x_pixel = (*x) * (240.0/3900);
        *y_pixel = 320-((*y) * (320.0/3900));
    }
}

void draw_buttons(unsigned short color) {
    char x, y;
    
    for(y = 0; y<20; y++) {
        for(x = 0; x<20; x++) {
            LCD_drawPixel(200+x, 100+y, color);     // Boxes drawn at centers  (200, 100) and (200,220)
            LCD_drawPixel(200+x, 100-y, color);     // count to be displayed at center (200,160)
            LCD_drawPixel(200-x, 100+y, color);
            LCD_drawPixel(200-x, 100-y, color);
            LCD_drawPixel(200+x, 220+y, color);
            LCD_drawPixel(200+x, 220-y, color);
            LCD_drawPixel(200-x, 220+y, color);
            LCD_drawPixel(200-x, 220-y, color);
        }
    }
    
    for(y = 0; y<15; y++) {
        for(x = 0; x<2; x++) {
            LCD_drawPixel(200+x, 100+y, BACKGROUND);     // Boxes drawn at centers  (200, 100) and (200,220)
            LCD_drawPixel(200+x, 100-y, BACKGROUND);     // count to be displayed at center (200,160)
            LCD_drawPixel(200-x, 100+y, BACKGROUND);
            LCD_drawPixel(200-x, 100-y, BACKGROUND);
            LCD_drawPixel(200+y, 100+x, BACKGROUND);     // Boxes drawn at centers  (200, 100) and (200,220)
            LCD_drawPixel(200+y, 100-x, BACKGROUND);     // count to be displayed at center (200,160)
            LCD_drawPixel(200-y, 100+x, BACKGROUND);
            LCD_drawPixel(200-y, 100-x, BACKGROUND);
            LCD_drawPixel(200+y, 220+x, BACKGROUND);     // Boxes drawn at centers  (200, 100) and (200,220)
            LCD_drawPixel(200+y, 220-x, BACKGROUND);     // count to be displayed at center (200,160)
            LCD_drawPixel(200-y, 220+x, BACKGROUND);
            LCD_drawPixel(200-y, 220-x, BACKGROUND);
        }
    }
}

char buttonStat(unsigned short *x, unsigned short *y, unsigned char *pressed) {
    if(pressed) {
        if(*x>185 && *x<215 && *y>80 && *y<120) {
            return 1;
        }
        else if(*x>185 && *x<215 && *y>185 && *y<215) {
            return -1;
        }
    }
    return 0;
}