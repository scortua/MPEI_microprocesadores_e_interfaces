// configuraciones iniciales
    #pragma config FNOSC = FRC       // Selecciona el oscilador interno r?pido (FRC)
    #pragma config IESO = ON            // Habilita el cambio de oscilador interno/externo
    #pragma config FWDTEN = OFF     // Deshabilita el watchdog timer
    #pragma config ICS = PGD3           // Selecciona el canal de comunicaci?n PGD3

#include "xc.h"
#include "p33FJ128MC802.h"

#define FCY 7372800 // Frecuencia del microcontrolador
#include <libpic30.h>

#define FP  FCY/2
#define BAUDRATE 19200
#define BRGVAL ((FP/BAUDRATE)/16)-1
unsigned int i;

int main(void) {
    
    AD1PCFGL = 0XFFFF;      // activando AN2 como entrada analoga
    TRISB = 0x0000;             // Puerto B como salida para los segmentos
    RPOR2bits.RP4R = 3; // Salida UART1 por RP4

    U1MODEbits.STSEL = 0; // 1-Stop bit        
    U1MODEbits.PDSEL = 0; // No Parity, 8-Data bits
    U1MODEbits.ABAUD = 0; // Auto-Baud disabled
    U1MODEbits.BRGH = 0; // Standard-Speed mode
    
    U1BRG = BRGVAL; // Baud Rate setting for 9600
    
    U1STAbits.UTXISEL0 = 0; // Interrupt after one TX character is transmitted
    U1STAbits.UTXISEL1 = 0;
    IEC0bits.U1TXIE = 1; // Enable UART TX interrupt
    
    U1MODEbits.UARTEN = 1; // Enable UART
    U1STAbits.UTXEN = 1; // Enable UAR

    /* Wait at least 105 microseconds (1/9600) before sending first char */
    __delay_us(105);
    U1TXREG = 'a'; // Transmit one character
    __delay_us(105);
    U1TXREG = '\n';// Transmit a end line
    
    while (1) {
    }
}

void __attribute__((__interrupt__)) _U1TXInterrupt(void) {
    IFS0bits.U1TXIF = 0; // Clear TX Interrupt flag
    U1TXREG = 'a'; // Transmit one character
    while(!U1STAbits.TRMT);
    U1TXREG = '\n';// Transmit a end line
    while(!U1STAbits.TRMT);
}