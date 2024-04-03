#pragma config FNOSC = FRC  // Selecciona el oscilador interno r?pido (FRC)
#pragma config IESO = ON    // Habilita el cambio de oscilador interno/externo
#pragma config FWDTEN = OFF // Deshabilita el watchdog timer
#pragma config ICS = PGD3   // Selecciona el canal de comunicaci?n PGD3

#include "xc.h"
#include "p33FJ128MC802.h"
#define FCY 7372800
#include <libpic30.h>

#define analogo PORTBbits.RB0
#define LED LATBbits.LATB3

#define in1 LATBbits.LATB15 // registros salida B
#define in2 LATBbits.LATB14
#define in3 LATBbits.LATB13
#define in4 LATBbits.LATB12

#define FP  FCY/2
#define BAUDRATE 19200
#define BRGVAL ((FP/BAUDRATE)/16)-1

void ADC_conf();
void adquirir();
void UART_conf();
void __attribute__((interrupt, auto_psv)) _U1TXInterrupt(void);
void __attribute__((interrupt, auto_psv)) _U1RXInterrupt(void);

int estado = 0;
int lectura = 0;
float TIME = 0.1;

int main(void) {
    AD1PCFGL = 0xFFFB; // 1111 1111 1111 1011
    // 0 salida 1 entrada
    TRISB = 0X0005; // 0000 0000 0000 0101 
    RPOR0bits.RP1R = 3; // Salida UART1 por RP1
    RPINR18bits.U1RXR = 2; //Entrada de la UART1 por RP2
    ADC_conf();
    UART_conf();
    while (1) {
        switch (estado) {
            case 0: 
                in1 = 0; in2 = 0; in3 = 0; in4 = 0;
                break;
            case 1: 
                in1 = 1; in2 = 0; in3 = 0; in4 = 0; __delay_ms(TIME);
                in1 = 0; in2 = 1; in3 = 0; in4 = 0; __delay_ms(TIME);
                in1 = 0; in2 = 0; in3 = 1; in4 = 0; __delay_ms(TIME);
                in1 = 0; in2 = 0; in3 = 0; in4 = 1; __delay_ms(TIME);
                break;
            case 2: 
                in1 = 0; in2 = 0; in3 = 0; in4 = 1; __delay_ms(TIME);
                in1 = 0; in2 = 0; in3 = 1; in4 = 0; __delay_ms(TIME);
                in1 = 0; in2 = 1; in3 = 0; in4 = 0; __delay_ms(TIME);
                in1 = 1; in2 = 0; in3 = 0; in4 = 0; __delay_ms(TIME);
                break;
    }
    return 0;
}

void UART_conf() {
    U1MODEbits.STSEL = 0; // 1-Stop bit        
    U1MODEbits.PDSEL = 0; // No Parity, 8-Data bits
    U1MODEbits.ABAUD = 0; // Auto-Baud disabled
    U1MODEbits.BRGH = 0; // Standard-Speed mode

    U1BRG = BRGVAL; // Baud Rate setting for monda

    U1STAbits.UTXISEL0 = 0; // Interrupt after one TX character is transmitted
    U1STAbits.UTXISEL1 = 0; // Interrupt after one RX character is received   
    IEC0bits.U1TXIE = 1; // Enable UART TX interrupt
    IEC0bits.U1RXIE = 1; // Enable UART RX interrupt

    IPC3bits.U1TXIP = 5; // Set the transmition priority in 5 Lower than Reception 
    IPC2bits.U1RXIP = 6; // Set the reception priority in 6 Higher than Transmition

    U1MODEbits.UARTEN = 1; // Enable UART
    U1STAbits.UTXEN = 1; // Enable Transmition UART    

}

void ADC_conf() {
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

void adquirir() {
    AD1CON1bits.SAMP = 1;
    __delay_ms(20);
    AD1CON1bits.SAMP = 0;
    while (!AD1CON1bits.DONE);
}

void __attribute__((interrupt, auto_psv)) _U1TXInterrupt(void) {
    
    adquirir();
    lectura = ADCBUF0 >> 2;
    lectura = lectura & (0X03FF);
    int centenas = lectura / 100;
    centenas += 48;
    U1TXREG = centenas; // Transmit one character
    int decenas = lectura / 10;
    decenas %= 10;
    decenas += 48;
    while (!U1STAbits.TRMT);
    U1TXREG = decenas; // Transmit one character
    int unidades = lectura % 10;
    unidades += 48;
    while (!U1STAbits.TRMT);
    U1TXREG = unidades; // Transmit one character
    while (!U1STAbits.TRMT);
    U1TXREG = '\n'; // Transmit a end line
    while (!U1STAbits.TRMT);
    IFS0bits.U1TXIF = 0; // Clear TX Interrupt flag
}

void __attribute__((interrupt, auto_psv)) _U1RXInterrupt(void) {
    while(U1STAbits.URXDA)
    LED = 1;
    __delay_ms(20);
    LED = 0;
    switch(U1RXREG){
        case 's': // s ascii 115 
            estado = 0;
            break;
        case 'd': //a ascii 97 0 1000110101010
            estado = 1;
            break;
        case 'a': // d ascii 100
            estado = 2;
            break;
    }
    IFS0bits.U1RXIF = 0;
}
