//Configuracion del microcontrolador
#pragma config FNOSC = FRC              // Oscillator Mode
#pragma config IESO = ON                // Internal External Switch Over Mode
#pragma config FWDTEN = OFF             // Watchdog Timer Enable
#pragma config ICS = PGD3               // Comm Channel Select

#include "perifericos.h"
#include "funciones.h"
#include "i2c.h"

int lectura = 0;
double m = 2.0 * 1843.0 / 255.0;
int duty = 0;
unsigned int velocidad = 0;
int estado = 0;

int main(void) {

    AD1PCFGL = 0xFFFB; // 1111 1111 1111 1011
    TRISB = 0x008D; // 0000 0000 1000 1101
    //------------------------inicializacion de configuracion-------------------------
    conf_ADC();
    conf_pwm();
    conf_QEI();
    conf_UART();
    conf_timer_1();
    conf_INT();
    //------------------------Inicio del ciclo infinito---------------------------------
    while (1) {
        switch (estado) {
            case 0:
                adquirir();
                duty = m*lectura; //0X0E66-lectura*0X000E;
                P2DC1 = duty;
                break;
            case 1:
                adquirir();
                duty = m*lectura; //0X0E66-lectura*0X000E;
                P2DC1 = 0;
                break;
        }
    }
    return 0;
}

