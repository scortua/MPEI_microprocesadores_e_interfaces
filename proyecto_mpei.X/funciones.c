#include "funciones.h"
#include "i2c.h"


//-------------------------------------------------------ADQUIRIR-------------------------------------------------------------------
void adquirir() {
    AD1CON1bits.SAMP = 1;
    __delay_us(10);
    AD1CON1bits.SAMP = 0;
    while (!AD1CON1bits.DONE);
    lectura = ADCBUF0 >> 2;
    lectura = lectura & (0x03FF);
}
//-------------------------------------------------------TRANSMITIR----------------------------------------------------------------
void transmitir(void) {
    
}
//-----------------------------------------INTERRUPCION INT0-------------------------------------------------------------------
void __attribute__((interrupt, auto_psv)) _INT0Interrupt(void) {

    IFS0bits.INT0IF = 0; // aclarar la bandera de interrupcion
    switch (estado) {
        case 0:
            estado = 1;
            break;
        case 1:
            estado = 0;
            break;
    }
}
//-----------------------------------------INTERRUPCION TIMER-------------------------------------------------------------------
void __attribute__((interrupt, auto_psv)) _T1Interrupt(void) {
    //transmitir(); // transmision de encoder por uart
    pv = POS1CNT * 5.8823 ;
    POS1CNT = 0; // se resetea el contador del qei
    IFS0bits. T1IF = 0; // aclarar la bandera de interrupcion
}
//-----------------------------------------INTERRUPCION TIMER-------------------------------------------------------------------