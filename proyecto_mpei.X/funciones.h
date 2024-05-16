
#ifndef FUNCIONES
#define FUNCIONES_H

#include "p33FJ128MC802.h"
#define FCY 7372800
#include "xc.h"
#include "ssd1306_oled.h"
#include "i2c.h"
#include <libpic30.h>

extern int lectura;
extern double m;
extern int duty;
extern unsigned int velocidad;
extern int estado;

float error;
float error1;
float error2;
float cv;
float cv1;
float sp = lectura;
float pv = velocidad;

float kp = 1;
float ki = 1;
float kd = 0.01;
float ts = 100;

//-------------------------------------------------------ADQUIRIR-------------------------------------------------------------------
void adquirir(void);
//------------------------------------------------------CONTROLADOR PID---------------------------------------------------------
void PID(void);
//-------------------------------------------------------TRANSMITIR----------------------------------------------------------------
void transmitir(void);
//-----------------------------------------INTERRUPCION INT0-------------------------------------------------------------------
void __attribute__((interrupt, auto_psv)) _INT0Interrupt(void);
//-----------------------------------------INTERRUPCION TIMER-------------------------------------------------------------------
void __attribute__((interrupt, auto_psv)) _T1Interrupt(void);

#endif	/* XC_HEADER_TEMPLATE_H */

