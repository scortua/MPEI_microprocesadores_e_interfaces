#pragma config FNOSC = FRC  // oscillator mode
#pragma config IESO = ON    // internal external switch
#pragma config FWDTEN = OFF // watchdog timer enable
#pragma config ICS = PGD3   // comm channel select

#include "xc.h"
#include "p33FJ128MC802.h"

#define FCY 7370000UL // reloj micro
#include <libpic30.h>

#define LED LATBbits.LATB7
#define LED1 LATBbits.LATB8

void conf_timer_5();
void Interrupt_Init2();
void __attribute__((interrupt, auto_psv)) _T1Interrupt();

int main(void) {
    AD1PCFGL = 0XFFFF; // 1 son digitales (Rx) 0 es analogo (ANx))
    TRISB = 0X0000; // RA0  0000 0000 0000 0000
    conf_timer_5();
    LED1 = 1;
    IFS0bits. T1IF = 0;
    Interrupt_Init2();
    while (1) {
    }
    return 0;
}

void conf_timer_5() {
    //T2CONbits.T32 = 0; // coloca el registro en 32 bits
    T1CONbits. TCKPS = 3; //Select input clock prescaler as 1:1
    T1CONbits. TGATE = 0; //Disable Gate Time Accumulation Mode
    T1CONbits.TCS = 0; //Select internal clock as the timer clock source
    PR1 = 0xFFFF; // definimos numero de pulsos para 5 segundo
    T1CONbits. TON = 1; //Activate the timer module
}

void Interrupt_Init2(void) {
    IEC0bits.T1IE = 1;
    IPC0bits.T1IP = 6;
}

void __attribute__((interrupt, auto_psv)) _T1Interrupt(void) {
    LED = 1;
    __delay_ms(10);
    LED = 0;
    IFS0bits. T1IF = 0;
}

