#pragma config FNOSC = FRC  // oscillator mode
#pragma config IESO = ON    // internal external switch
#pragma config FWDTEN = OFF // watchdog timer enable
#pragma config ICS = PGD3   // comm channel select

#include "xc.h"
#include "p33FJ128MC802.h"

#define FCY 7370000UL // reloj micro
#include <libpic30.h>

// salida digital para matriz horizontal
#define fila1 LATAbits.LATA0 
#define fila2 LATAbits.LATA1 
#define fila3 LATAbits.LATA2 
#define fila4 LATAbits.LATA3 
// entrada digital para dato de teclado
#define dato1 PORTBbits.RB0
#define dato2 PORTBbits.RB1
#define dato3 PORTBbits.RB2
#define dato4 PORTBbits.RB3
// salida a decodificador a displays
#define deco1 LATBbits.LATB12
#define deco2 LATBbits.LATB13
#define deco3 LATBbits.LATB14
#define deco4 LATBbits.LATB15
// salida de los enables a los displays 
#define enable1 LATBbits.LATB9
#define enable2 LATBbits.LATB8

/*
    { '1', '2', '3', 'A' },
    { '4', '5', '6', 'B' },
    { '7', '8', '9', 'C' },
    { '*', '0', '#', 'D' }
  
    0 = 0000
    1 = 0001
    2 = 0010
    3 = 0011
    4 = 0100 
    5 = 0101
    6 = 0110
    7 = 0111
    8 = 1000
    9 = 1001
   10 = 1010
   11 = 1011
   12 = 1100
   13 = 1101
   14 = 1110 
   15 = 1111
 */

int calcular(int, int, int);
int leer_teclado();
void mostrar(int);

int toque;
int num1 = 0;
int num2 = 0;
int operacion;
int veces_in = 0;
int resultado;

int main(void) {
    AD1PCFGL = 0XFFFF; // 1 son digitales (Rx) 0 es analogo (ANx))
    TRISA = 0x0000; // 0000 0000 0000 1111 pues uso en salidas(0) digitales ...RA3 RA2 RA1 RA0
    TRISB = 0x000F; // 0000 0000 0000 0000 pues uso en entradas(1) digitales ...RB3 RB2 RB1 RB0

    while (1) {
        do {
            toque = leer_teclado();
            mostrar(num1);
            mostrar(num2);
            if (toque == 10 || toque == 11 || toque == 12 || toque == 13 || toque == 14 || toque == 15) {
                veces_in = veces_in;
            } else {
                veces_in = veces_in + 1;
                if (veces_in == 1) {
                    num1 = toque;
                } else if (veces_in == 2) {
                    num2 = toque;
                }
            }
        } while (veces_in != 2);

        mostrar(0);

        veces_in = 0;
        do {
            toque = leer_teclado();
            if (toque == 1 || toque == 2 || toque == 3 || toque == 4 || toque == 5 || toque == 6 || toque == 7 || toque == 8 || toque == 9 || toque == 0) {
                veces_in = veces_in;
            } else {
                veces_in = veces_in + 1;
                operacion = toque;
            }
        } while (veces_in != 1);
        resultado = calcular(num1, num2, operacion);
        mostrar(resultado);
    }

    return 0;
}

// Función para realizar operaciones matemáticas básicas

int calcular(int a, int b, int operacion) {
    switch (operacion) {
        case 10: // es suma
            return a + b;
        case 11: // es resta
            return a - b;
        case 12: // es multiplicacion
            return a * b;
        case 13: // es divicion
            if (b != 0) {
                return a / b;
            } else {
                // Manejar el error de división por cero
                return 0;
            }
        default:
            // Manejar el caso de operación no válida
            return 0;
    }
}

int leer_teclado() {
    int estado = 0;
    int act_fila = 0;
    int tecla = 0;
    do {
        // estados para la salida de las filas
        if (act_fila == 0) {
            fila1 = 1;
            fila2 = fila3 = fila4 = 0;
        } else if (act_fila == 1) {
            fila2 = 1;
            fila1 = fila3 = fila4 = 0;
        } else if (act_fila == 2) {
            fila3 = 1;
            fila1 = fila2 = fila4 = 0;
        } else if (act_fila == 3) {
            fila4 = 1;
            fila1 = fila2 = fila3 = 0;
        }
        // busqueda del boton pulsado
        if (fila1 == 1) {
            if (dato1 == 1) {
                tecla = 1;
                estado = 1;
            } else if (dato2 == 1) {
                tecla = 2;
                estado = 1;
            } else if (dato3 == 1) {
                tecla = 3;
                estado = 1;
            } else {
                tecla = 10;
                estado = 1;
            }
        } else if (fila2 == 1) {
            if (dato1 == 1) {
                tecla = 4;
                estado = 1;
            } else if (dato2 == 1) {
                tecla = 5;
                estado = 1;
            } else if (dato3 == 1) {
                tecla = 6;
                estado = 1;
            } else {
                tecla = 11;
                estado = 1;
            }
        } else if (fila3 == 1) {
            if (dato1 == 1) {
                tecla = 7;
                estado = 1;
            } else if (dato2 == 1) {
                tecla = 8;
                estado = 1;
            } else if (dato3 == 1) {
                tecla = 9;
                estado = 1;
            } else {
                tecla = 12;
                estado = 1;
            }
        } else if (fila4 == 1) {
            if (dato1 == 1) {
                tecla = 14;
                estado = 1;
            } else if (dato2 == 1) {
                tecla = 0;
                estado = 1;
            } else if (dato3 == 1) {
                tecla = 15;
                estado = 1;
            } else {
                tecla = 13;
                estado = 1;
            }
        }
        // delay para el paso de fila a fila
        __delay_ms(0.00001);
        // estado para pasar a la siguiente fila
        act_fila = act_fila++;
    } while (estado == 0);

    return tecla;
}

void mostrar(int num) {
    if (num <= 9) {
        switch (num) {
            case 0: enable1 = 1;
                enable2 = 0;
                deco1 = 0;
                deco2 = 0;
                deco3 = 0;
                deco4 = 0;
                __delay_ms(0.005);
                enable1 = 0;
                enable2 = 1;
                deco1 = 0;
                deco2 = 0;
                deco3 = 0;
                deco4 = 0;
                __delay_ms(0.005);
                break;
            case 1: enable1 = 1;
                enable2 = 0;
                deco1 = 1;
                deco2 = 0;
                deco3 = 0;
                deco4 = 0;
                __delay_ms(0.005);
                enable1 = 0;
                enable2 = 1;
                deco1 = 0;
                deco2 = 0;
                deco3 = 0;
                deco4 = 0;
                __delay_ms(0.005);
                break;
            case 2: enable1 = 1;
                enable2 = 0;
                deco1 = 0;
                deco2 = 1;
                deco3 = 0;
                deco4 = 0;
                __delay_ms(0.005);
                enable1 = 0;
                enable2 = 1;
                deco1 = 0;
                deco2 = 0;
                deco3 = 0;
                deco4 = 0;
                __delay_ms(0.005);
                break;
            case 3: enable1 = 1;
                enable2 = 0;
                deco1 = 1;
                deco2 = 1;
                deco3 = 0;
                deco4 = 0;
                __delay_ms(0.005);
                enable1 = 0;
                enable2 = 1;
                deco1 = 0;
                deco2 = 0;
                deco3 = 0;
                deco4 = 0;
                __delay_ms(0.005);
                break;
            case 4: enable1 = 1;
                enable2 = 0;
                deco1 = 0;
                deco2 = 0;
                deco3 = 1;
                deco4 = 0;
                __delay_ms(0.005);
                enable1 = 0;
                enable2 = 1;
                deco1 = 0;
                deco2 = 0;
                deco3 = 0;
                deco4 = 0;
                __delay_ms(0.005);
                break;
            case 5: enable1 = 1;
                enable2 = 0;
                deco1 = 1;
                deco2 = 0;
                deco3 = 1;
                deco4 = 0;
                __delay_ms(0.005);
                enable1 = 0;
                enable2 = 1;
                deco1 = 0;
                deco2 = 0;
                deco3 = 0;
                deco4 = 0;
                __delay_ms(0.005);
                break;
            case 6: enable1 = 1;
                enable2 = 0;
                deco1 = 0;
                deco2 = 1;
                deco3 = 1;
                deco4 = 0;
                __delay_ms(0.005);
                enable1 = 0;
                enable2 = 1;
                deco1 = 0;
                deco2 = 0;
                deco3 = 0;
                deco4 = 0;
                __delay_ms(0.005);
                break;
            case 7: enable1 = 1;
                enable2 = 0;
                deco1 = 1;
                deco2 = 1;
                deco3 = 1;
                deco4 = 0;
                __delay_ms(0.005);
                enable1 = 0;
                enable2 = 1;
                deco1 = 0;
                deco2 = 0;
                deco3 = 0;
                deco4 = 0;
                __delay_ms(0.005);
                break;
            case 8: enable1 = 1;
                enable2 = 0;
                deco1 = 0;
                deco2 = 0;
                deco3 = 0;
                deco4 = 1;
                __delay_ms(0.005);
                enable1 = 0;
                enable2 = 1;
                deco1 = 0;
                deco2 = 0;
                deco3 = 0;
                deco4 = 0;
                __delay_ms(0.005);
                break;
            case 9: enable1 = 1;
                enable2 = 0;
                deco1 = 1;
                deco2 = 0;
                deco3 = 0;
                deco4 = 1;
                __delay_ms(0.005);
                enable1 = 0;
                enable2 = 1;
                deco1 = 0;
                deco2 = 0;
                deco3 = 0;
                deco4 = 0;
                __delay_ms(0.005);
                break;
        }
    } else {
        switch (num) {
            case 10: enable1 = 1;
                enable2 = 0;
                deco1 = 0;
                deco2 = 0;
                deco3 = 0;
                deco4 = 0;
                __delay_ms(0.005);
                enable1 = 0;
                enable2 = 1;
                deco1 = 1;
                deco2 = 0;
                deco3 = 0;
                deco4 = 0;
                __delay_ms(0.005);
                break;
            case 11: enable1 = 1;
                enable2 = 0;
                deco1 = 1;
                deco2 = 0;
                deco3 = 0;
                deco4 = 0;
                __delay_ms(0.005);
                enable1 = 0;
                enable2 = 1;
                deco1 = 1;
                deco2 = 0;
                deco3 = 0;
                deco4 = 0;
                __delay_ms(0.005);
                break;
            case 12: enable1 = 1;
                enable2 = 0;
                deco1 = 0;
                deco2 = 1;
                deco3 = 0;
                deco4 = 0;
                __delay_ms(0.005);
                enable1 = 0;
                enable2 = 1;
                deco1 = 1;
                deco2 = 0;
                deco3 = 0;
                deco4 = 0;
                __delay_ms(0.005);
                break;
            case 13: enable1 = 1;
                enable2 = 0;
                deco1 = 1;
                deco2 = 1;
                deco3 = 0;
                deco4 = 0;
                __delay_ms(0.005);
                enable1 = 0;
                enable2 = 1;
                deco1 = 1;
                deco2 = 0;
                deco3 = 0;
                deco4 = 0;
                __delay_ms(0.005);
                break;
            case 14: enable1 = 1;
                enable2 = 0;
                deco1 = 0;
                deco2 = 0;
                deco3 = 1;
                deco4 = 0;
                __delay_ms(0.005);
                enable1 = 0;
                enable2 = 1;
                deco1 = 1;
                deco2 = 0;
                deco3 = 0;
                deco4 = 0;
                __delay_ms(0.005);
                break;
            case 15: enable1 = 1;
                enable2 = 0;
                deco1 = 1;
                deco2 = 0;
                deco3 = 1;
                deco4 = 0;
                __delay_ms(0.005);
                enable1 = 0;
                enable2 = 1;
                deco1 = 1;
                deco2 = 0;
                deco3 = 0;
                deco4 = 0;
                __delay_ms(0.005);
                break;
            case 16: enable1 = 1;
                enable2 = 0;
                deco1 = 0;
                deco2 = 1;
                deco3 = 1;
                deco4 = 0;
                __delay_ms(0.005);
                enable1 = 0;
                enable2 = 1;
                deco1 = 1;
                deco2 = 0;
                deco3 = 0;
                deco4 = 0;
                __delay_ms(0.005);
                break;
            case 17: enable1 = 1;
                enable2 = 0;
                deco1 = 1;
                deco2 = 1;
                deco3 = 1;
                deco4 = 0;
                __delay_ms(0.005);
                enable1 = 0;
                enable2 = 1;
                deco1 = 1;
                deco2 = 0;
                deco3 = 0;
                deco4 = 0;
                __delay_ms(0.005);
                break;
            case 18: enable1 = 1;
                enable2 = 0;
                deco1 = 0;
                deco2 = 0;
                deco3 = 0;
                deco4 = 1;
                __delay_ms(0.005);
                enable1 = 0;
                enable2 = 1;
                deco1 = 1;
                deco2 = 0;
                deco3 = 0;
                deco4 = 0;
                __delay_ms(0.005);
                break;
        }
    }
}
