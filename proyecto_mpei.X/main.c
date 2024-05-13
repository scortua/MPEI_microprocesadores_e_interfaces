//Configuracion del microcontrolador
#pragma config FNOSC = FRC              // Oscillator Mode
#pragma config IESO = ON                // Internal External Switch Over Mode
#pragma config FWDTEN = OFF             // Watchdog Timer Enable
#pragma config ICS = PGD3               // Comm Channel Select

#include "perifericos.h"

int lectura = 0;
double m = 2.0*1843.0/255.0;
int duty = 0;
unsigned int velocidad = 0;
int estado = 0;

void adquirir();
void transmitir();
void __attribute__((interrupt, auto_psv)) _INT0Interrupt(void);
void __attribute__((interrupt, auto_psv)) _T1Interrupt(void);

int main(void) {
    
    AD1PCFGL = 0xFFFB; // 1111 1111 1111 1011
    TRISB = 0x008D; // 0000 0000 1000 1101
    conf_ADC();
    conf_pwm();
    conf_QEI();
    conf_UART();
    conf_timer_1();
    conf_INT();
    
    while(1){
        switch(estado){
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
        
        /*
        transmitir();
        __delay_ms(4);     //Se lee el numero de pulsos por cada 10 milisegundos y se resetea el registro
        POS1CNT = 0;
         */
    }
    
    return 0;
}


void adquirir() {
    AD1CON1bits.SAMP = 1;
    __delay_us(10);
    AD1CON1bits.SAMP = 0;
    while (!AD1CON1bits.DONE);
    lectura = ADCBUF0 >> 2; 
    lectura = lectura & (0x03FF);
}

void transmitir(){
    U1TXREG = 'L';// Transmit a end line
    while(!U1STAbits.TRMT);
    U1TXREG = 'a';// Transmit a end line
    while(!U1STAbits.TRMT);
    U1TXREG = ' ';// Transmit a end line
    while(!U1STAbits.TRMT);
    U1TXREG = 'v';// Transmit a end line
    while(!U1STAbits.TRMT);
    U1TXREG = 'e';// Transmit a end line
    while(!U1STAbits.TRMT);
    U1TXREG = 'l';// Transmit a end line
    while(!U1STAbits.TRMT);
    U1TXREG = 'o';// Transmit a end line
    while(!U1STAbits.TRMT);
    U1TXREG = 'c';// Transmit a end line
    while(!U1STAbits.TRMT);
    U1TXREG = 'i';// Transmit a end line
    while(!U1STAbits.TRMT);
    U1TXREG = 'd';// Transmit a end line
    while(!U1STAbits.TRMT);
    U1TXREG = 'a';// Transmit a end line
    while(!U1STAbits.TRMT);
    U1TXREG = 'd';// Transmit a end line
    while(!U1STAbits.TRMT);
    U1TXREG = ':';// Transmit a end line
    while(!U1STAbits.TRMT);
    velocidad = POS1CNT*0.5882 ;
    int diez_miles = velocidad / 10000;
    diez_miles += 48;
    U1TXREG = diez_miles;
    while(!U1STAbits.TRMT);
    int miles = velocidad / 1000;
    miles %= 10;
    miles += 48;
    U1TXREG = miles;
    while(!U1STAbits.TRMT);
    int centenas = velocidad/100;
    centenas %= 10;
    centenas += 48;
    U1TXREG = centenas; // Transmit one character
    while(!U1STAbits.TRMT);
    int decenas = velocidad/10;
    decenas %= 10;
    decenas += 48;
    U1TXREG = decenas; // Transmit one character
    while(!U1STAbits.TRMT);
    int unidades = velocidad % 10; 
    unidades += 48;
    U1TXREG = unidades; // Transmit one character
    while(!U1STAbits.TRMT);
    U1TXREG = '\n';// Transmit a end line
    while(!U1STAbits.TRMT);
 }

void __attribute__((interrupt, auto_psv)) _INT0Interrupt(void){
     
     IFS0bits.INT0IF = 0; // aclarar la bandera de interrupcion
     switch (estado){
         case 0:
             estado = 1;
             break;
         case 1:
             estado = 0;
             break;
     }
    
    
 }

void __attribute__((interrupt, auto_psv)) _T1Interrupt(void) {
    transmitir(); // transmision de encoder por uart
    POS1CNT = 0; // se resetea el contador del qei
    IFS0bits. T1IF = 0; // aclarar la bandera de interrupcion
}