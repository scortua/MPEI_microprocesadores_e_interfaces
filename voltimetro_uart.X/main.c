
#pragma config FNOSC = FRC  // Selecciona el oscilador interno r?pido (FRC)
#pragma config IESO = ON    // Habilita el cambio de oscilador interno/externo
#pragma config FWDTEN = OFF // Deshabilita el watchdog timer
#pragma config ICS = PGD3   // Selecciona el canal de comunicaci?n PGD3

#include "xc.h"
#include "p33FJ128MC802.h"
#define FCY 7372800
#include <libpic30.h>
#define analogo PORTBbits.RB0

#define FP  FCY/2
#define BAUDRATE 19200
#define BRGVAL ((FP/BAUDRATE)/16)-1

void ADC_conf();
void adquirir();
void UART_conf();

int lectura = 0;

int main(void) {
    AD1PCFGL = 0xFFFB;
    TRISA = 0X0000;
    TRISB = 0X0001;
    RPOR2bits.RP4R = 3; // Salida UART1 por RP4
    ADC_conf();
    UART_conf();
    while(1){
        
    }
    return 0;
}

void UART_conf(){
    U1MODEbits.STSEL = 0; // 1-Stop bit        
    U1MODEbits.PDSEL = 0; // No Parity, 8-Data bits
    U1MODEbits.ABAUD = 0; // Auto-Baud disabled
    U1MODEbits.BRGH = 0; // Standard-Speed mode
    
    U1BRG = BRGVAL; // Baud Rate setting for monda
    
    U1STAbits.UTXISEL0 = 0; // Interrupt after one TX character is transmitted
    U1STAbits.UTXISEL1 = 0;
    IEC0bits.U1TXIE = 1; // Enable UART TX interrupt
    
    U1MODEbits.UARTEN = 1; // Enable UART
    U1STAbits.UTXEN = 1; // Enable UAR
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
    __delay_ms(20);
    AD1CON1bits.SAMP = 0;
    while(!AD1CON1bits.DONE);
} 

void __attribute__((__interrupt__)) _U1TXInterrupt(void) {
    IFS0bits.U1TXIF = 0; // Clear TX Interrupt flag
    adquirir();
    lectura = ADCBUF0 >> 2;
    lectura = lectura & (0X03FF);
    int centenas = lectura/100;
    centenas += 48;
    U1TXREG = centenas; // Transmit one character
    int decenas = lectura/10;
    decenas %= 10;
    decenas += 48;
    while(!U1STAbits.TRMT);
    U1TXREG = decenas; // Transmit one character
    int unidades = lectura % 10; 
    unidades += 48;
    while(!U1STAbits.TRMT);
    U1TXREG = unidades; // Transmit one character
    while(!U1STAbits.TRMT);
    U1TXREG = '\n';// Transmit a end line
    while(!U1STAbits.TRMT);
    
}