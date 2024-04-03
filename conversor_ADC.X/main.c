// configuraciones iniciales
    #pragma config FNOSC = FRC       // Selecciona el oscilador interno r?pido (FRC)
    #pragma config IESO = ON            // Habilita el cambio de oscilador interno/externo
    #pragma config FWDTEN = OFF     // Deshabilita el watchdog timer
    #pragma config ICS = PGD3           // Selecciona el canal de comunicaci?n PGD3
//#pragma config POSCMD = NONE // Primary Oscillator Source (Primary Oscillator Disabled)

//#pragma config BWRP = WRPROTECT_OFF // Boot Segment Write Protect (Boot Segment may be wri@en)
//#pragma config SWRP = WRPROTECT_OFF // Secure Segment Program Write Protect (Secure segment may be wri@en)
//#pragma config GWRP = OFF // General Code Segment Write Protect (User program memory is not write-protected)
//#pragma config GSS = OFF // General Segment Code ProtecIon (User program memory is not code-protected)

#include "xc.h"
#include "p33FJ128MC802.h"

#define FCY 7370000UL // Frecuencia del microcontrolador
#include <libpic30.h>

// Definici?n de pines de habilitacion de displays
#define d1 LATAbits.LATA0
#define d2 LATAbits.LATA1
// Definici?n de pines directos al decodificador de 7 segmentos
#define a LATBbits.LATB15
#define b LATBbits.LATB14
#define c LATBbits.LATB13
#define d LATBbits.LATB12
#define e LATBbits.LATB11
#define f LATBbits.LATB10
#define g LATBbits.LATB9
// puerto de entrada analogo
#define analogo PORTBbits.RB0

float lectura = 0;
int k = 310;

void ADC_configuration();
void adquirir_AD();
void visualizador();

int main(void) {
    AD1PCFGL = 0XFFFB;      // activando AN2 como entrada analoga
    TRISA = 0x0000;             // Puerto A como salida para los displays
    TRISB = 0x0001;             // Puerto B como salida para los segmentos

    ADC_configuration();
    while (1) {
        visualizador();
        adquirir_AD();
        visualizador();
    }

    return 0;
}

void ADC_configuration() {
    AD1CON1 = 0x0000;
    AD1CON2 = 0X0000;
    AD1CON3 = 0x8000;
    AD1CSSL = 0;
    AD1CHS0 = 0x0000;
    AD1CON1bits.ADON = 1;
    AD1CHS0bits.CH0SA = 2;
     AD1CHS0bits.CH0SB = 2;
}

void adquirir_AD() {
    AD1CON1bits.SAMP = 1;
    __delay_ms(1);
    AD1CON1bits.SAMP = 0;
    while (!AD1CON1bits.DONE);
    lectura = ADCBUF0; // >> 2;
    // lectura = lectura & (0x03FF); es inecesario
    __delay_ms(1);
}

void visualizador(){
    
    int dato = (lectura / k) * 10;
    
    int unidades = dato % 10; // el modulo es lo que sobra al dividir un numero
    int decenas = dato / 10; // al dividir el numero 
    // Habilitamos el primer display (unidades)
    d1 = 1;
    d2 = 0;
    // Codificamos y enviamos las se?ales correspondientes para las unidades
    switch(unidades) {
        case 0: a = 0; b = 0; c = 0; d = 0; e = 0; f = 0; g = 1; break;           
        case 1: a = 1; b = 0; c = 0; d = 1; e = 1; f = 1; g = 1; break; 
        case 2: a = 0; b = 0; c = 1; d = 0; e = 0; f = 1; g = 0; break;
        case 3: a = 0; b = 0; c = 0; d = 0; e = 1; f = 1; g = 0; break;
        case 4: a = 1; b = 0; c = 0; d = 1; e = 1; f = 0; g = 0; break;
        case 5: a = 0; b = 1; c = 0; d = 0; e = 1; f = 0; g = 0; break;
        case 6: a = 0; b = 1; c = 0; d = 0; e = 0; f = 0; g = 0; break;
        case 7: a = 0; b = 0; c = 0; d = 1; e = 1; f = 1; g = 1; break;
        case 8: a = 0; b = 0; c = 0; d = 0; e = 0; f = 0; g = 0; break;
        case 9: a = 0; b = 0; c = 0; d = 0; e = 1; f = 0; g = 0; break;
    }
    __delay_ms(4  );
    // Habilitamos el segundo display (decenas)
    d1 = 0;
    d2 = 1;
    // Codificamos y enviamos las se?ales correspondientes para las decenas
    switch(decenas) {
        case 0: a = 0; b = 0; c = 0; d = 0; e = 0; f = 0; g = 1; break;           
        case 1: a = 1; b = 0; c = 0; d = 1; e = 1; f = 1; g = 1; break; 
        case 2: a = 0; b = 0; c = 1; d = 0; e = 0; f = 1; g = 0; break;
        case 3: a = 0; b = 0; c = 0; d = 0; e = 1; f = 1; g = 0; break;
        case 4: a = 1; b = 0; c = 0; d = 1; e = 1; f = 0; g = 0; break;
        case 5: a = 0; b = 1; c = 0; d = 0; e = 1; f = 0; g = 0; break;
        case 6: a = 0; b = 1; c = 0; d = 0; e = 0; f = 0; g = 0; break;
        case 7: a = 0; b = 0; c = 0; d = 1; e = 1; f = 1; g = 1; break;
        case 8: a = 0; b = 0; c = 0; d = 0; e = 0; f = 0; g = 0; break;
        case 9: a = 0; b = 0; c = 0; d = 0; e = 1; f = 0; g = 0; break;
    }
    // Peque?o retardo para visualizaci?n
    __delay_ms(4);
    // Apagar todos los segmentos para evitar fugas de corriente
    // a = b = c = d = 1;
}

