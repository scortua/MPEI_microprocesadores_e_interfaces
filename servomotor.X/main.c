#pragma config FNOSC = FRC  // Selecciona el oscilador interno r?pido (FRC)
#pragma config IESO = ON    // Habilita el cambio de oscilador interno/externo
#pragma config FWDTEN = OFF // Deshabilita el watchdog timer
#pragma config ICS = PGD3   // Selecciona el canal de comunicaci?n PGD3

#include "xc.h"
#include "p33FJ128MC802.h"
#define FCY 7372800
#include <libpic30.h>

#define analogo PORTBbits.RB0

void conf_pwm();
void ADC_conf();
void adquirir();

int lectura;
double m = 1843/255;
int b = 1843;

int main(void) {
    AD1PCFGL = 0xFFFB;
    TRISB = 0x0000;
    ADC_conf();
    conf_pwm();
    while(1){
        adquirir();
        P2DC1 = m * lectura + b;
    }
    return 0;
}

void conf_pwm(){
    P2TCONbits.PTCKPS = 1;         // periodo base prescaler de tiempo T = 4Tcy
    P2TCONbits.PTMOD = 0;        // pwm opera en modo free running
    P2TMRbits.PTMR = 0;             // 
    P2TPERbits.PTPER = 0x4800;  // tiempo T =  20ms      20/2*4*Tcy = 18432
    PWM2CON1bits.PMOD1 = 0;  // habilita pin H y L para ser complementarios
    PWM2CON1bits.PEN1H = 1;    // se habilitan pin pwm H
    PWM2CON1bits.PEN1L = 1;     // se habilita pin pwm L
    P2DTCON1bits.DTAPS = 0;     // prescalador Tiempo muerto de pwm
    P2DTCON1bits.DTA = 59;       // tiempo muerto 4us
    P2DC1 = 0x0733;                    // Duty Cycle    4800=50%    2ms : E66 = 20%   1ms : 733 = 10%    480=6.25%   64C = 8.75%    = xms/20ms * 18432 * 2
    P2TCONbits.PTEN = 1;            // enable PWM timerbase
}

void ADC_conf(){
    AD1CON1 = 0X0000;
    AD1CON2 = 0;
    AD1CON3 = 0X0000;
    AD1CSSL = 0;
    AD1CHS0 = 0X0000;
    AD1CON1bits.ADON = 1;
    AD1CON3bits.ADRC = 1;
    AD1CHS0bits.CH0SA = 2;
    AD1CHS0bits.CH0SB = 2;
}

void adquirir(){
    AD1CON1bits.SAMP = 1;
    __delay_us(10);
    AD1CON1bits.SAMP = 0;
    while(!AD1CON1bits.DONE);
    lectura = ADCBUF0 >> 2;
    __delay_ms(5);
} 
