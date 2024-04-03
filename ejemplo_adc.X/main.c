#pragma config BWRP = WRPROTECT_OFF // Boot Segment Write Protect (Boot Segment may be wri@en)
#pragma config SWRP = WRPROTECT_OFF // Secure Segment Program Write Protect (Secure segment may be wri@en)
#pragma config GWRP = OFF // General Code Segment Write Protect (User program memory is not write-protected)
#pragma config GSS = OFF // General Segment Code ProtecIon (User program memory is not code-protected)

#pragma config FNOSC = FRC // Oscillator Mode (Internal Fast RC (FRC))
#pragma config POSCMD = NONE // Primary Oscillator Source (Primary Oscillator Disabled)
#pragma config FWDTEN = OFF // Watchdog Timer Enable (Watchdog Imer always enabled)
#pragma config ICS = PGD3 // Comm Channel Select (Communicate on PGC1/EMUC1 and PGD1/EMUD1)

#include "xc.h"
#include "p33FJ128MC802.h"

#define Fosc 7372800
#define Fp Fosc/2

// CONFIG
int estado = 1;
int yn, resto;
// Declaración de la funciones
void AdquirirAD();
void delay(unsigned long int);
//==============================================================================

void main(void) {
    AD1PCFGL = 0XFFFC;
    TRISB = 0x0000;
    TRISA = 0x0003;
    PORTB = 0x0000;
    PORTA = 0x0000;
    //Configuración ADC
    ADC_Init();
    while (1) {
        AdquirirAD();
        LATB = yn;
    }
}
//============================================================================

void AdquirirAD(void) {
    AD1CON1bits.SAMP = 1;
    delay(1000);
    AD1CON1bits.SAMP = 0;
    while (!AD1CON1bits.DONE);
    yn = ADCBUF0 >> 2;
    yn = yn & (0x00FF);
}
//============================================================================

void ADC_Init(void) {
    AD1CON1 = 0x0000;
    AD1CON2 = 0;
    AD1CON3 = 0x0000;
    AD1CSSL = 0;
    AD1CHS0 = 0x0000;
    AD1CON1bits.ADON = 1;
    AD1CHS0bits.CH0SA = 0;
   
}
//============================================================================

void delay(unsigned long int contador) {
    while (contador > 0) {
        contador--;
    }
}