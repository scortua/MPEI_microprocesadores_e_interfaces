#pragma config FNOSC = FRC  // Selecciona el oscilador interno r?pido (FRC)
#pragma config IESO = ON    // Habilita el cambio de oscilador interno/externo
#pragma config FWDTEN = OFF // Deshabilita el watchdog timer
#pragma config ICS = PGD3   // Selecciona el canal de comunicaci?n PGD3

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

int dato = 95; // N?mero a mostrar en los displays

int main(void) {
    // Configuraciou8n de pines como digitales
    AD1PCFGL = 0xFFFF; // Todos los pines como digitales
    TRISA = 0x0000;    // Puerto A como salida para los displays
    TRISB = 0x0000;    // Puerto B como salida para los segmentos
    while(1) {
        // Visualizaci?n din?mica del n?mero en los displays
        // Calculamos las unidades y decenas del n?mero
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
    __delay_ms(4);
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
    return 0;
}