#pragma config FNOSC = FRC  // oscillator mode
#pragma config IESO = ON    // internal external switch
#pragma config FWDTEN = OFF // watchdog timer enable
#pragma config ICS = PGD3   // comm channel select

#include "xc.h"
#include "p33FJ128MC802.h"

#define FCY 7370000UL // reloj micro
#include <libpic30.h>

#define puls PORTBbits.RB0
#define out1 LATAbits.LATA0
#define out2 LATAbits.LATA1
#define out3 LATAbits.LATA2

int up(int);
int down(int);
void sal(int);
int salida=0;

int main(void) {
    AD1PCFGL = 0XFFFF; // 1 son digitales (Rx) 0 es analogo (ANx))
    TRISA = 0X0003; // 0000 0000 0000 0111
    
    while(1){
        if(puls == 0){
            salida = up(salida);
            __delay_ms(1000);
            sal(salida);
        }else {
            salida = down(salida);
            __delay_ms(1000);
            sal(salida);
        }
    }
    
    return 0;
}

int up(int num){
    num++;
}

int down(int num){
    num--;
}

void sal(int num){
    switch(num){
        case 0:
            out1 = 0; out2 = 0; out3 =0; break;
        case 1:
            out1 = 1; out2 = 0; out3 =0; break;
        case 2:
            out1 = 0; out2 = 1; out3 =0; break;
        case 3:
            out1 = 1; out2 = 1; out3 =0; break;
        case 4:
            out1 = 0; out2 = 0; out3 =1; break;
        case 5:
            out1 = 1; out2 = 0; out3 =1; break;
        case 6:
            out1 = 0; out2 =1; out3 =1; break;
        case 7:
            out1 = 1; out2 = 1; out3 =1; break;
    }
}