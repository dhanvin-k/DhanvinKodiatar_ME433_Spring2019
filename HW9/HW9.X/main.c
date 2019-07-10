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
    
    LCD_clearScreen(BACKGROUND); 
    
    unsigned char message[10];
    unsigned short x, y;
    unsigned int z;
    
    // Print who_am_i register on the top left corner of the LCD screen at (5,5):
    sprintf(message, "Hello World!");
    print_message(10, 10, message, COLOR);
    
    draw_buttons(COLOR);
    
    while(1) {
        _CP0_SET_COUNT(0);
                
        XPT2046_read(&x, &y, &z);
        
        sprintf(message, "%d", x);
        print_message(10, 20, message, COLOR);
        
        
        LATAbits.LATA4 = !LATAbits.LATA4;       // LED blink
        
        while(_CP0_GET_COUNT()<2400000) {;}
    }
}