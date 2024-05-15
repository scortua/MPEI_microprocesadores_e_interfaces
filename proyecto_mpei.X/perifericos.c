#include "perifericos.h"

//----------------------------------------------------------INT---------------------------------------------------------------------
void conf_INT() {
    INTCON2 = 0x0000;
    IFS0bits.INT0IF = 0;
    IEC0bits.INT0IE = 1;
}
//----------------------------------------------------------ADC--------------------------------------------------------------------
void conf_ADC() {
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
//----------------------------------------------------------UART-------------------------------------------------------------------
void conf_UART() {
    RPOR0bits.RP1R = 3; // Salida UART1 por RP1
    U1MODEbits.STSEL = 0; // 1-Stop bit        
    U1MODEbits.PDSEL = 0; // No Parity, 8-Data bits
    U1MODEbits.ABAUD = 0; // Auto-Baud disabled
    U1MODEbits.BRGH = 0; // Standard-Speed mode
    U1BRG = BRGVAL; // Baud Rate setting for monda
    U1MODEbits.UARTEN = 1; // Enable UART
    U1STAbits.UTXEN = 1; // Enable Transmition UART    
}
//----------------------------------------------------------TIMER------------------------------------------------------------------
void conf_timer_1() {
    //T2CONbits.T32 = 0; // coloca el registro en 32 bits
    T1CONbits. TCKPS = 2;
    T1CONbits. TGATE = 0; //Disable Gate Time Accumulation Mode
    T1CONbits.TCS = 0; //Select internal clock as the timer clock source
    PR1 = 0xE100; // definimos numero de pulsos para 5 segundo
    T1CONbits. TON = 1; //Activate the timer module
    IEC0bits.T1IE = 1;
    IFS0bits. T1IF = 0;

}
//----------------------------------------------------------PWM-------------------------------------------------------------------
void conf_pwm() {
    P2TCONbits.PTCKPS = 0; // periodo base prescaler de tiempo T = Tcy  limite = 32768
    P2TCONbits.PTMOD = 0; // pwm opera en modo free running
    P2TMRbits.PTMR = 0; // 
    P2TPERbits.PTPER = 0x0733; // tiempo T =  0.5ms      1/2000 /2/7372800  = 1843
    PWM2CON1bits.PMOD1 = 0; // habilita pin H y L para ser complementarios
    PWM2CON1bits.PEN1H = 1; // se habilitan pin pwm H
    PWM2CON1bits.PEN1L = 1; // se habilita pin pwm L
    P2DTCON1bits.DTAPS = 0; // prescalador Tiempo muerto de pwm
    P2DTCON1bits.DTA = 59; // tiempo muerto 16us
    P2DC1 = 0x0000;
    P2TCONbits.PTEN = 1; // enable PWM timerbase
}
//----------------------------------------------------------QEI---------------------------------------------------------------------
void conf_QEI() {
    RPINR14bits.QEA1R = 2;
    RPINR14bits.QEB1R = 3;
    QEI1CON = 0;
    POS1CNT = 0;
    QEI1CONbits.QEIM = 5;
    QEI1CONbits.UPDN = 1;
    MAX1CNT = 0XFFFF; // maximo conteo de pulsos
}
//-------------------------------------------------------------------------------------------------------------------------------
