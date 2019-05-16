#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include<stdio.h>
#include"i2c_master_noint.h"

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

void initExpander(void);

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
    
    // Turn off AN2 and AN3 pins (make B2 and B3 available for I2C)
    ANSELBbits.ANSB2 = 0;
    ANSELBbits.ANSB3 = 0;

    // do your TRIS and LAT commands here
    TRISBbits.TRISB4 = 1;
    TRISAbits.TRISA4 = 1;
    //LATAbits.LATA4 = 0;
    /*
    TRISBbits.TRISB2 = 0;
    RPB2Rbits.RPB2R = 0b0100;
    TRISBbits.TRISB3 = 0;
    */
    __builtin_enable_interrupts();
    
    i2c_master_setup();
    i2c_master_start();
    i2c_master_send(SLAVE_ADDR<<1);
    i2c_master_send(0x00);
    i2c_master_send(0xF0);
    i2c_master_stop();
    
    i2c_master_start();
    i2c_master_send(SLAVE_ADDR<<1);
    i2c_master_send(0x0A);
    i2c_master_send(0x1);
    i2c_master_stop();
/*
    while(1) {
        _CP0_SET_COUNT(0);      // Setting Core Timer count to 0
        LATAbits.LATA4 = !LATAbits.LATA4;       // Toggling the Green LED ON or OFF
        while(_CP0_GET_COUNT() < 11999999) { ; }       // Toggle it ON or OFF for 0.5 ms
        
        while(!PORTBbits.RB4) {     // If the button is pushed turn LED OFF and wait 
            LATAbits.LATA4 = 0; }
    }*/
}

void initExpander(void) {
    i2c_master_setup();
    i2c_master_start();
    i2c_master_send(SLAVE_ADDR<<1);
    i2c_master_send(0x00);
    i2c_master_send(0xF0);
    i2c_master_stop();
}