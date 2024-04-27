 #pragma config FNOSC = FRC  // Selecciona el oscilador interno r?pido (FRC)
#pragma config IESO = ON    // Habilita el cambio de oscilador interno/externo
#pragma config FWDTEN = OFF // Deshabilita el watchdog timer
#pragma config ICS = PGD3   // Selecciona el canal de comunicaci?n PGD3


#include "p33FJ128MC802.h"
#define FCY 7372800
#include <libpic30.h>
#include "xc.h"

#define FP  FCY/2
#define BAUDRATE 19200
#define BRGVAL ((FP/BAUDRATE)/16)-1

#define analogo PORTBbits.RB0

void ADC_conf();
void adquirir();
void UART_conf();
void conf_pwm();
void QEI_conf();

int lectura=0;
double m = 14.4549;
int duty=0;

int main(void) {
    AD1PCFGL = 0xFFFB; // 1111 1111 1111 1011
    TRISB = 0x0005; // 0000 0000 0000 0101
    RPINR14bits.QEA1R = 2; // entrada por RP2
    RPOR0bits.RP1R = 3; // Salida UART1 por RP1
    ADC_conf();
    conf_pwm();
    QEI_conf();
    //UART_conf();
    
    while(1){
        adquirir();
        duty = m*lectura;//0X0E66-lectura*0X000E;
        P2DC1 = duty;
        U1TXREG = POS1CNT;
    }
    
    return 0;
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
    __delay_ms(1);
    AD1CON1bits.SAMP = 0;
    while (!AD1CON1bits.DONE);
    lectura = ADCBUF0 >> 2; 
    lectura = lectura & (0x03FF);
}

void conf_pwm(){
    P2TCONbits.PTCKPS = 0;         // periodo base prescaler de tiempo T = Tcy  limite = 32768
    P2TCONbits.PTMOD = 0;        // pwm opera en modo free running
    P2TMRbits.PTMR = 0;             // 
    P2TPERbits.PTPER = 0x0733;  // tiempo T =  0.5ms      1/2000 /2/7372800  = 1843
    PWM2CON1bits.PMOD1 = 0;  // habilita pin H y L para ser complementarios
    PWM2CON1bits.PEN1H = 1;    // se habilitan pin pwm H
    PWM2CON1bits.PEN1L = 1;     // se habilita pin pwm L
    P2DTCON1bits.DTAPS = 0;     // prescalador Tiempo muerto de pwm
    P2DTCON1bits.DTA = 59;       // tiempo muerto 16us
    P2DC1 = 0x0000; 
    P2TCONbits.PTEN = 1;            // enable PWM timerbase
}

void QEI_conf(){
    QEI1CON = 0;
    QEI1CONbits.UPDN = 1;
    QEI1CONbits.QEIM = 5;
    QEI1CONbits.TQGATE = 1;
    DFLT1CON = 0;
    DFLT1CONbits.QEOUT = 1;
    MAX1CNT = 256;
}

 void UART_conf() {
    U1MODEbits.STSEL = 0; // 1-Stop bit        
    U1MODEbits.PDSEL = 0; // No Parity, 8-Data bits
    U1MODEbits.ABAUD = 0; // Auto-Baud disabled
    U1MODEbits.BRGH = 0; // Standard-Speed mode

    U1BRG = BRGVAL; // Baud Rate setting for monda

    //U1STAbits.UTXISEL1 = 0; // Interrupt after one RX character is received   
    //IEC0bits.U1RXIE = 1; // Enable UART RX interrupt

    U1MODEbits.UARTEN = 1; // Enable UART
    U1STAbits.UTXEN = 1; // Enable Transmition UART    

}
