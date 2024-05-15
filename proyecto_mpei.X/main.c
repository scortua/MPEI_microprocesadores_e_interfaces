//Configuracion del microcontrolador
#pragma config FNOSC = FRC            // Oscillator Mode
#pragma config IESO = ON                // Internal External Switch Over Mode
#pragma config FWDTEN = OFF         // Watchdog Timer Enable
#pragma config ICS = PGD3               // Comm Channel Select

#include "perifericos.h"                        // libreria de configuracion para los perifericos
#include "funciones.h"                          // libreria de funciones usadas en el programa
#include "i2c.h"                                    // libreria de uso del i2c
#include "ssd1306_oled.h"                   // libreria de uso para la pantalla OLED

int lectura = 0;                                     // lectura para el analogo (potenciometro))
double m = 2.0 * 1843.0 / 255.0;        // funcion para seleccionar el ciclo util de la señal para el motor dc
int duty = 0;                                        // definicion de ciclo util
unsigned int velocidad = 0;                // variable de qei para leer el encoder e interpretar la velocidad del motor
int estado = 0;                                    // estado de la interrupcion 1 -> detenido 0-> movimiento

int main(void) {

    AD1PCFGL = 0xFFFB;                   // 1111 1111 1111 1011
    TRISB = 0x038D;                         // 0000 0011 1000 1101
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

