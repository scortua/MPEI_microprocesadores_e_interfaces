#ifndef PERIFERICOS_H
#define	PERIFERICOS_H

#include "p33FJ128MC802.h"
#define FCY 7372800
#include "xc.h"
#include <libpic30.h>

#define FP  FCY/2
#define BAUDRATE 19200
#define BRGVAL ((FP/BAUDRATE)/16)-1

//----------------------------------------------------------INT-------------------------------------------------------------------
void conf_INT();
//----------------------------------------------------------ADC-------------------------------------------------------------------
void conf_ADC();
//----------------------------------------------------------UART------------------------------------------------------------------
void conf_UART();
//----------------------------------------------------------TIMER-----------------------------------------------------------------
void conf_timer_1();
//----------------------------------------------------------PWM-------------------------------------------------------------------
void conf_pwm();
//----------------------------------------------------------QEI-------------------------------------------------------------------
void conf_QEI();

#endif	/* PERIFERICOS_H */

