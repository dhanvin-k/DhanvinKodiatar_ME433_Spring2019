#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include<string.h>
#include<stdio.h>
#include "ili9341.h"

// DEVCFG0
#pragma config DEBUG = OFF // no debugging
#pragma config JTAGEN = OFF // no jtag
#pragma config ICESEL = ICS_PGx1 // use PGED1 and PGEC1
#pragma config PWP = OFF // no write protect
#pragma config BWP = OFF // no boot write protect
#pragma config CP = OFF // no code protect

// DEVCFG1
#pragma config FNOSC = PRIPLL // use primary oscillator with pll
#pragma config FSOSCEN = OFF // turn off secondary oscillator
#pragma config IESO = OFF // no switching clocks
#pragma config POSCMOD = HS // high speed crystal mode
#pragma config OSCIOFNC = OFF // disable secondary osc
#pragma config FPBDIV = DIV_1 // divide sysclk freq by 1 for peripheral bus clock
#pragma config FCKSM = CSDCMD // do not enable clock switch
#pragma config WDTPS = PS1048576 // use slowest wdt
#pragma config WINDIS = OFF // wdt no window mode
#pragma config FWDTEN = OFF // wdt disabled
#pragma config FWDTWINSZ = WINSZ_25 // wdt window at 25%

// DEVCFG2 - get the sysclk clock to 48MHz from the 8MHz crystal
#pragma config FPLLIDIV = DIV_2 // divide input clock to be in range 4-5MHz
#pragma config FPLLMUL = MUL_24 // multiply clock after FPLLIDIV
#pragma config FPLLODIV = DIV_2 // divide clock after FPLLMUL to get 48MHz
#pragma config UPLLIDIV = DIV_2 // divider for the 8MHz input clock, then multiplied by 12 to get 48MHz for USB
#pragma config UPLLEN = ON // USB clock on

// DEVCFG3
#pragma config USERID = 0 // some 16bit userid, doesn't matter what
#pragma config PMDL1WAY = OFF // allow multiple reconfigurations
#pragma config IOL1WAY = OFF // allow multiple reconfigurations
#pragma config FUSBIDIO = ON // USB pins controlled by USB module
#pragma config FVBUSONIO = ON // USB BUSON controlled by USB module


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
    
    SPI1_init();
    LCD_init();
    
    __builtin_enable_interrupts();
    
    LCD_clearScreen(ILI9341_MAROON);
    char message[20], fps_message[10], number[3], huns, tens, ones;
    int a = 0, b = 0, c = 0, k = 0, m = 0, index = 0, count = 0;
    
    sprintf(message, "Hello world");
    sprintf(fps_message, "FPS: ");
    
    while(message[index]) {
        print_char(28 + 5*index, 32, message[index]);
        index++;
    }
    
    index = 0;
    while(fps_message[index]) {
        print_char(28 + 5*index, 80, fps_message[index]);
        index++;
    }
    
    while(1) {
        _CP0_SET_COUNT(0);
        
        clear_space(28 + 5*12, 32, 108);
        sprintf(number, "%d%d%d", a, b, c);
        
        while(number[k]) {
            print_char(28 + 5*(12+k), 32, number[k]);
            k++;
        }
        
        print_char(28 + 5*(12+k), 32, '!');
        draw_progress(count);
        k = 0;
        c++;
        count++;
        
        if(a == 1){
            a=0;b=0;c=0;
            clear_space(28, 50, 28 + count);
            count = 0;
        }
        
        if(c==10){
            b++;
            c=0;
        }
        
        if(b==10){
            a++;
            b=0;
        }
        
        char fps[5];
        clear_space(28 + 25, 80, 85);
        sprintf(fps, "%1.2f", (1.0*24000000)/_CP0_GET_COUNT());
        while(fps[m]) {
            print_char(28 + 5*(5+m), 80, fps[m]);
            m++;
        }
        m = 0;
        
        while(_CP0_GET_COUNT()<2400000) {;}
    }
}