#pragma config FNOSC = FRC  // oscillator mode
#pragma config IESO = ON    // internal external switch
#pragma config FWDTEN = OFF // watchdog timer enable
#pragma config ICS = PGD3   // comm channel select

#include "xc.h"
#include "p33FJ128MC802.h"

#define FCY 7370000UL // reloj micro
#include <libpic30.h>

#define LED LATAbits.LATA0
#define PUSH PORTAbits.RA3

/*
 hex 0X000
 bin 0b000
 dec 000
 */

int estado;

int main(void) {
    //Habilita las entradas AN0 y AN1, los demas pines donde hay entradas análogicas son utilizados con I/O digital 
    AD1PCFGL = 0XFFFF; // 1 son digitales (Rx) 0 es analogo (ANx))
    TRISA = 0X0004; // RA3  0000 0000 0000 0100

    /* 
     * numero de lista 13
     * para UART2 tiene RX que es entrada TX que es salida
     * 
     * RX va al RP13
     * U2RX es input y al ver la tabla 11.1 el registro es RPIN19 que se llaman U2RXR<4:0>
     * se lee el RP el numero del registro     * 
     * 
     * TX va al RP14 
     * U2TX es el output y al ver la tabla 11.2 el registro es RPOR7 que se llaman RPnR<4:0> con n el registro de salida
     * y el codigo de U2TX es 00101
     */
    RPINR19bits.U2RXR = 13; // 01101 = 13
    RPOR7bits.RP15R = 5; // 00101 = 5
    /* 
     mucho mejor meter una interrupcion externa al haber un 1
     cuando pasa esto se coloca una subrutina
     con los pines INTx donde se manda lo que viene del boton 
     despues se coloca una funcion que atiene la interrupcion
     */
    estado = 0;
    while (1) {
        if (PUSH == 1) {
            __delay_ms(500);
            if (estado == 0) estado = 1;
            else estado = 0;
        }
        switch (estado) {
            case 0: LED = 1;
                break;
            case 1: LED = 0;
                break;
        }
    }

    return 0;
}

/*
 * void __attribute__((interrupt,auto_psv)) _INT0Interrupt(void){
 *  if(estado == 0) estado = 1;
 *  else estado = 0;
 *  IFSOxbits.INTxIF = 0; borrado de bandera porque se crea al generar una int
 * 
 * Interruption Flag State Xregistro
 * }
 */