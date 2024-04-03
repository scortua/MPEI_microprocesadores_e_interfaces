/*
 * File:   main.c
 * Author: grupo mpei
 *
 * Created on 31 de enero de 2024, 10:44 AM
 */

/*
 * PARTE 2
 * inclusion de librerias, constantes y demas
 */
#include "xc.h"
#include <p33FJ128MC802.h>
#define FCY 7370000UL // reloj micro
#include <libpic30.h>
// configuracion del microcontrolador
/*
 * PARTE 1
 * configuracion generales del MCU 
*/
#pragma config FNOSC = FRC  // oscillator mode
#pragma config IESO = ON    // internal external switch
#pragma config FWDTEN = OFF // watchdog timer enable
#pragma config ICS = PGD3   // comm channel select
/*
 * PARTE 3 
 * declaracion de funciones, variables y demas
 */
#define LED LATAbits.LATA0
// se guarda en el LAT A0 el nombre del LED
// LAT A bits, latsh es un registro necesario a la conexion digital
// para modificar el puerto como si lo escribiera
// para leer es una lectura port 

/*
 * PARTE 4
 * definicion funcion principal
 */
int main(void) {
    /*
     * configuracion de perifericos
     * inicialisador de pines y asignacion de los mismos
     */
    AD1PCFGL = 0xFFFF;  // pines digitales
    // registos de 1x16 bits
    TRISA = 0x0000;     // puerto A como salidas digitales
    // TRIS es un registro de configuracion
    /*
     * repeticiones o loops para ejecutar el programa
     * internamente el programa de funcionamiento
     
     * el argumento 1 en el while permite tener el bucle
     * se puede manejar con interrupciones  
     */
    while(1){
        LED = 1;
        __delay32(2000000);
        LED = 0;
        __delay32(2000000);
    }
    return 0;
}
// window->Target Memory View->Configuration Bits

