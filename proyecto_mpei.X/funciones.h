
#ifndef FUNCIONES
#define	FUNCIONES_H

#include "p33FJ128MC802.h"
#define FCY 7372800
#include "xc.h"
#include <libpic30.h>

extern int lectura;
extern double m;
extern int duty;
extern unsigned int velocidad;
extern int estado;

//-------------------------------------------------------ADQUIRIR-------------------------------------------------------------------
void adquirir();
//-------------------------------------------------------TRANSMITIR----------------------------------------------------------------
void transmitir();
//-----------------------------------------INTERRUPCION INT0-------------------------------------------------------------------
void __attribute__((interrupt, auto_psv)) _INT0Interrupt(void);
//-----------------------------------------INTERRUPCION TIMER-------------------------------------------------------------------
void __attribute__((interrupt, auto_psv)) _T1Interrupt(void);

#endif	/* XC_HEADER_TEMPLATE_H */

