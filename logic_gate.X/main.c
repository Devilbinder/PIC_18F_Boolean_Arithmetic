#include <xc.h>
#include <p18f4520.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "conbits.h"
#include "uart_layer.h"

void interrupt high_isr(void);
void interrupt low_priority low_isr(void);

uint8_t print_buffer[256] = {0}; // buffer to print stuff to serial

void wait_button(void){
    while(!PORTDbits.RD1);
    __delay_ms(300);
    
}

void main(void){

    OSCCONbits.IDLEN = 0;
    OSCCONbits.IRCF = 0x07;
    OSCCONbits.SCS = 0x03;
    while(OSCCONbits.IOFS!=1); // 8Mhz
    
    TRISB=0;    // b port as output
    LATB=0x00; // b port low
    
    TRISDbits.RD1 = 1;
  
    uart_init(51,0,1,0);//baud 9600

    __delay_ms(2000);
    sprintf(print_buffer,"Program start\n\r");
    uart_send_string(print_buffer); // everything works in setup
    
    for(;;){
        LATB = 0b10000001;
        wait_button();
        LATB = 0b10001000;
        wait_button();
        LATB = 0b10000001 & 0b10001000;
        wait_button();
        
        LATB = 0b10000001;
        wait_button();
        LATB = 0b10001000;
        wait_button();
        LATB = 0b10000001 | 0b10001000;
        wait_button();
        
        LATB = 0b10000001;
        wait_button();
        LATB = 0b10001000;
        wait_button();
        LATB = 0b10000001 ^ 0b10001000;
        wait_button();
        
        LATB = 0b11000011;
        wait_button();
        LATB = ~0b11000011;
        wait_button();
        
        LATB = 0b10101010;
        wait_button();
        LATB = LATB >> 1;
        wait_button();
        LATB = LATB << 1;
        wait_button();
    } 
}



void interrupt high_isr(void){
    INTCONbits.GIEH = 0;
    if(PIR2bits.CCP2IF == 1){
        
        PIR2bits.CCP2IF = 0; 
    }
    INTCONbits.GIEH = 1;
}

void interrupt low_priority low_isr(void){
    INTCONbits.GIEH = 0;
    if(PIR2bits.TMR3IF == 1){
        
        PIR2bits.TMR3IF = 0;
    }  
    INTCONbits.GIEH = 1;
}



