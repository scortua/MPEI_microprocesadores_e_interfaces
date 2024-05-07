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
#define BUTTON PORTBbits.RB7 // registro digital RB7 de INT

void ADC_conf();
void adquirir();
void UART_conf();
void conf_pwm();
void QEI_conf();
void transmitir();

void conf_timer_1();
void __attribute__((interrupt, auto_psv)) _T1Interrupt(void);

void __attribute__((interrupt, auto_psv)) _INT0Interrupt(void);

int lectura = 0;
double m = 2.0*1843.0/255.0;
int duty = 0;
unsigned int velocidad = 0;
int estado = 0;

int main(void) {
    AD1PCFGL = 0xFFFB; // 1111 1111 1111 1011
    TRISB = 0x000D; // 0000 0000 0000 1101
    RPINR14bits.QEA1R = 2;
    RPINR14bits.QEB1R = 3; 
    RPOR0bits.RP1R = 3; // Salida UART1 por RP1
    ADC_conf();
    conf_pwm();
    QEI_conf();
    UART_conf();
    conf_timer_1();
    IFS0bits.INT0IF = 0;
    IEC0bits.INT0IE = 1; 
    
    while(1){
        adquirir();
        duty = m*lectura;//0X0E66-lectura*0X000E;
        P2DC1 = duty;
        /*
        transmitir();
        __delay_ms(4);     //Se lee el numero de pulsos por cada 10 milisegundos y se resetea el registro
        POS1CNT = 0;
         */
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
    __delay_us(10);
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
    POS1CNT = 0;
    QEI1CONbits.QEIM = 5;
    QEI1CONbits.UPDN = 1;
    MAX1CNT = 0XFFFF;               // maximo conteo de pulsos
}

 void UART_conf() {
    U1MODEbits.STSEL = 0;           // 1-Stop bit        
    U1MODEbits.PDSEL = 0;           // No Parity, 8-Data bits
    U1MODEbits.ABAUD = 0;         // Auto-Baud disabled
    U1MODEbits.BRGH = 0;            // Standard-Speed mode
    U1BRG = BRGVAL;                     // Baud Rate setting for monda
    U1MODEbits.UARTEN = 1;        // Enable UART
    U1STAbits.UTXEN = 1;               // Enable Transmition UART    
}
 
 void transmitir(){
    U1TXREG = 'L';// Transmit a end line
    while(!U1STAbits.TRMT);
    U1TXREG = 'a';// Transmit a end line
    while(!U1STAbits.TRMT);
    U1TXREG = ' ';// Transmit a end line
    while(!U1STAbits.TRMT);
    U1TXREG = 'v';// Transmit a end line
    while(!U1STAbits.TRMT);
    U1TXREG = 'e';// Transmit a end line
    while(!U1STAbits.TRMT);
    U1TXREG = 'l';// Transmit a end line
    while(!U1STAbits.TRMT);
    U1TXREG = 'o';// Transmit a end line
    while(!U1STAbits.TRMT);
    U1TXREG = 'c';// Transmit a end line
    while(!U1STAbits.TRMT);
    U1TXREG = 'i';// Transmit a end line
    while(!U1STAbits.TRMT);
    U1TXREG = 'd';// Transmit a end line
    while(!U1STAbits.TRMT);
    U1TXREG = 'a';// Transmit a end line
    while(!U1STAbits.TRMT);
    U1TXREG = 'd';// Transmit a end line
    while(!U1STAbits.TRMT);
    U1TXREG = ':';// Transmit a end line
    while(!U1STAbits.TRMT);
    velocidad = POS1CNT*0.5882 ; // pulsos contados en 10 ms [pulsos/10ms] * (1000ms/1seg)*(1rev/100pulsos)*(60seg/1min)= 60 
    int diez_miles = velocidad / 10000;
    diez_miles += 48;
    U1TXREG = diez_miles;
    while(!U1STAbits.TRMT);
    int miles = velocidad / 1000;
    miles %= 10;
    miles += 48;
    U1TXREG = miles;
    while(!U1STAbits.TRMT);
    int centenas = velocidad/100;
    centenas %= 10;
    centenas += 48;
    U1TXREG = centenas; // Transmit one character
    while(!U1STAbits.TRMT);
    int decenas = velocidad/10;
    decenas %= 10;
    decenas += 48;
    U1TXREG = decenas; // Transmit one character
    while(!U1STAbits.TRMT);
    int unidades = velocidad % 10; 
    unidades += 48;
    U1TXREG = unidades; // Transmit one character
    while(!U1STAbits.TRMT);
    U1TXREG = '\n';// Transmit a end line
    while(!U1STAbits.TRMT);
 }

 void conf_timer_1() {
    //T2CONbits.T32 = 0; // coloca el registro en 32 bits
    T1CONbits. TCKPS = 2; //Select input clock prescaler as 1:1
    T1CONbits. TGATE = 0; //Disable Gate Time Accumulation Mode
    T1CONbits.TCS = 0; //Select internal clock as the timer clock source
    PR1 = 0xE100; // definimos numero de pulsos para 5 segundo
    T1CONbits. TON = 1; //Activate the timer module
    IEC0bits.T1IE = 1;
    IFS0bits. T1IF = 0;
        
}

 void __attribute__((interrupt, auto_psv)) _T1Interrupt(void) {
    transmitir(); // transmision de encoder por uart
    POS1CNT = 0; // se resetea el contador del qei
    IFS0bits. T1IF = 0; // aclarar la bandera de interrupcion
}
 
 void __attribute__((interrupt, auto_psv)) _INT0Interrupt(void){

    if (estado = 0) {
        P2TCONbits.PTEN = 1; // enable PWM timerbase
        estado = 1; // el estado quieto pasa a movimiento
    } else {
        P2TCONbits.PTEN = 0; // enable PWM timerbase
        estado = 0; // estado de movimiento pasa a quieto
    }
    IFS0bits.INT0IF = 0; // aclarar la bandera de interrupcion
 }
 
 
 
