//Configuracion del microcontrolador
#pragma config FNOSC = FRC            // Oscillator Mode
#pragma config IESO = ON                // Internal External Switch Over Mode
#pragma config FWDTEN = OFF         // Watchdog Timer Enable
#pragma config ICS = PGD3               // Comm Channel Select

#include "p33FJ128MC802.h"              // libreria del microcontrolador
#include "perifericos.h"                        // libreria de configuracion para los perifericos
#include "funciones.h"                          // libreria de funciones usadas en el programa
#include "oled.h"                              // libreria de uso del i2c

int lectura = 0; // lectura para el analogo (potenciometro))
double m = 3294.0 / 255.0; // funcion para seleccionar el ciclo util de la se�al para el motor dc
int duty = 0; // definicion de ciclo util
int estado = 0; // estado de la interrupcion 1 -> detenido 0-> movimiento

float pv = 0.0;
float sp = 0.0;

float cv = 0.0;
float cv1 = 0.0;
float error = 0.0;
float error1 = 0.0;
float error2 = 0.0;

float kp = 1;
float ki = 0.7;
float kd = 0.5;
float tm = 0.1;

int main(void) {

    AD1PCFGL = 0xFFFB; // 1111 1111 1111 1011     1 no 0 si   se usa
    TRISB = 0x838D; // 1000 0011 1000 1101      0 salida 1 entrada
    IFS1bits.MI2C1IF = 0;
    //------------------------inicializacion de configuracion-------------------------
    conf_ADC();
    conf_pwm();
    conf_QEI();
    conf_timer_1();
    I2C_Init_Master();
    conf_INT(); // configuracion de interrupcion
    sh1106_init();
    sh1106_clear();
    //------------------------Inicio del ciclo infinito---------------------------------
    while (1) {
        
        sp = lectura * m;
        
        error = sp - pv;
        
        cv = cv1 +(kp + kd/tm) * error + (-kp + ki * tm - 2 * kd/tm) * error1 + (kd/tm)*error2;
        cv1 = cv;
        error2 = error1;
        error1 = error;
        
        if (cv > 100000.0){
            cv = 100000;
        }
        if(cv < 30000.0){
            cv = 30000;
        }
        
//        sh1106_clear();
//        sh1106_draw_pixel(10,10,WHITE);
//        sh1106_display();
//        
//        __delay_ms(2000);
        
        switch (estado) {
            case 0:
                adquirir();
                duty = cv * 0X0E66 / 100000;
                P1DC1 = duty;
                break;
            case 1:
                adquirir();
                P1DC1 = 0;
                break;
        }

        
        
        
        
    }
    return 0;
}

