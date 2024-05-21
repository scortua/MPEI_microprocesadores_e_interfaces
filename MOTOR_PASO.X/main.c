#pragma config FNOSC = FRC  // oscillator mode
#pragma config IESO = ON    // internal external switch
#pragma config FWDTEN = OFF // watchdog timer enable
#pragma config ICS = PGD3   // comm channel select

#include "xc.h"
#include "p33FJ128MC802.h"

#define FCY 7370000UL // reloj micro
#include <libpic30.h>

#define BUTTON PORTBbits.RB7 // registro digital RB7 de INT
#define in1 LATBbits.LATB0 // registros salida B
#define in2 LATBbits.LATB1
#define in3 LATBbits.LATB2
#define in4 LATBbits.LATB3

void __attribute__((interrupt, auto_psv)) _INT0Interrupt(void);
int estado = 0;
int TIME = 0.1;

int main(void) {
    AD1PCFGL = 0XFFFF; // 1 son digitales (Rx) 0 es analogo (ANx))
    TRISB = 0X0080;
    // configuracion periferico de interrupcion
    INTCON2 = 0X0000;
    IFS0bits.INT0IF = 0;
    IEC0bits.INT0IE = 1;

    while (1) {
        switch (estado) {
            case 0: in1 = 0;
                in2 = 0;
                in3 = 0;
                in4 = 0;
                break;
            case 1: in1 = 1;
                in2 = 0;
                in3 = 0;
                in4 = 0;
                __delay_ms(TIME);
                in1 = 0;
                in2 = 1;
                in3 = 0;
                in4 = 0;
                __delay_ms(TIME);
                in1 = 0;
                in2 = 0;
                in3 = 1;
                in4 = 0;
                __delay_ms(TIME);
                in1 = 0;
                in2 = 0;
                in3 = 0;
                in4 = 1;
                __delay_ms(TIME);
                break;
            case 2: in1 = 0;
                in2 = 0;
                in3 = 0;
                in4 = 1;
                __delay_ms(TIME);
                in1 = 0;
                in2 = 0;
                in3 = 1;
                in4 = 0;
                __delay_ms(TIME);
                in1 = 0;
                in2 = 1;
                in3 = 0;
                in4 = 0;
                __delay_ms(TIME);
                in1 = 1;
                in2 = 0;
                in3 = 0;
                in4 = 0;
                __delay_ms(TIME);
                break;
            case 3:
                // 256 pulsos para dar 1 vuelta
                for (int i = 0; i < 256; i++) {
                    in1 = 1;
                    in2 = 0;
                    in3 = 0;
                    in4 = 0;
                    __delay_ms(TIME);
                    in1 = 0;
                    in2 = 1;
                    in3 = 0;
                    in4 = 0;
                    __delay_ms(TIME);
                    in1 = 0;
                    in2 = 0;
                    in3 = 1;
                    in4 = 0;
                    __delay_ms(TIME);
                    in1 = 0;
                    in2 = 0;
                    in3 = 0;
                    in4 = 1;
                    __delay_ms(TIME);
                }
                for (int i = 0; i < 256; i++) {
                    in1 = 0;
                    in2 = 0;
                    in3 = 0;
                    in4 = 1;
                    __delay_ms(TIME);
                    in1 = 0;
                    in2 = 0;
                    in3 = 1;
                    in4 = 0;
                    __delay_ms(TIME);
                    in1 = 0;
                    in2 = 1;
                    in3 = 0;
                    in4 = 0;
                    __delay_ms(TIME);
                    in1 = 1;
                    in2 = 0;
                    in3 = 0;
                    in4 = 0;
                    __delay_ms(TIME);
                }
                break;
        }

    }

    return 0;
}

void __attribute__((interrupt, auto_psv)) _INT0Interrupt(void) {
    IFS0bits.INT0IF = 0;
    /*
    if (estado == 0){
        estado = 1;
    }else if(estado == 1){
        estado = 2;
    }else if(estado == 2){
        estado = 0;
    }
     */
    estado = estado++;
    if (estado == 4) {
        estado = 0;
    }

}
