#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include<string.h>
#include<stdio.h>
#include"common.h"
#include"ili9341.h"
#include"i2c_master_noint.h"
#include"imu.h"

int main() {

    __builtin_disable_interrupts();

    // set the CP0 CONFIG register to indicate that kseg0 is cacheable (0x3)
    __builtin_mtc0(_CP0_CONFIG, _CP0_CONFIG_SELECT, 0xa4210583);

    // 0 data RAM access wait states
    BMXCONbits.BMXWSDRM = 0x0;

    // enable multi vector interrupts
    INTCONbits.MVEC = 0x1;

    // disable JTAG to get pins back
    DDPCONbits.JTAGEN = 0;

    // do your TRIS and LAT commands here
    TRISBbits.TRISB4 = 1;
    TRISAbits.TRISA4 = 0;
    LATAbits.LATA4 = 1;
    
    ANSELBbits.ANSB2 = 0;
    ANSELBbits.ANSB3 = 0;
    
    SPI1_init();
    LCD_init();
    initIMU();
    
    __builtin_enable_interrupts();
    
    LCD_clearScreen(ILI9341_MAROON); 
    
    unsigned char data[14], message[10], whoami[15] = "WHO AM I = ", X[10] = "acc_X = ", Y[10] = "acc_Y = ";
    
    print_message(5, 5, whoami, ILI9341_GREEN);
    print_message(5, 13, X, ILI9341_GREEN);
    print_message(5, 21, Y, ILI9341_GREEN);
    sprintf(message, "%d", getWHO_AM_I(), ILI9341_GREEN);
    clear_space(60, 5, 75);
    print_message(60, 5, message, ILI9341_GREEN);
    
    while(1) {
        _CP0_SET_COUNT(0);
                
        I2C_read_multiple(SLAVE_ADDR, 0x20, data, 14);
        
        float x = getXLX(data)*SCALE_FACTOR;
        sprintf(message, "%1.2f",x);
        clear_space(45, 13, 70);
        print_message(45, 13, message, ILI9341_GREEN);
        
        float y = getXLY(data)*SCALE_FACTOR;
        sprintf(message, "%1.2f",y);
        clear_space(45, 21, 70);
        print_message(45, 21, message, ILI9341_GREEN);
        
        draw_whitebars(ILI9341_BLACK);
        draw_xprogress(x, ILI9341_GREEN);
        draw_yprogress(y, ILI9341_GREEN);
        
        LATAbits.LATA4 = !LATAbits.LATA4;
        
        while(_CP0_GET_COUNT()<1200000) {;}     // 20 Hz delay
    }
}