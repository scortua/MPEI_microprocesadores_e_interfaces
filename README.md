
# MICROPROCESADORES E INTERFACES

##### Este repositorio es un almacen que tiene guardado todos los proyectos y trabajos hechos para sistemas embebidos, porgramados en c, en el IDE de Microchip.

<img src="https://github.com/scortua/MPEI-LAS-AMIGAS/assets/140832465/5c3d58c1-fcc4-4eae-a299-d507adad1c6b" alt="Microchip Logo" width="300"/> <img src="https://github.com/scortua/MPEI-LAS-AMIGAS/assets/140832465/c9450c8f-6ebb-4645-8056-1d28fd988173" alt="LogoEscuelaIng" width="300"/>


## DESCRIPCIÓN 
Este repositorio está dedicado a almacenar y compartir los trabajos, proyectos y ejercicios realizados como parte del programa de desarrollo de sistemas embebidos utilizando el microcontrolador dsPIC33FJ128MC802 de Microchip. Los archivos presentes aquí representan el resultado del esfuerzo en equipo de estudiantes que han trabajado en diversas pruebas y estudios para desarrollar programas específicamente diseñados para este microcontrolador.

El sistema embebido al que se refiere este repositorio se centra en el uso del microcontrolador dsPIC33FJ128MC802 de Microchip. Este microcontrolador es una unidad de procesamiento altamente integrada diseñada para aplicaciones embebidas que requieren procesamiento de señales digitales en tiempo real, como controladores de motores, dispositivos de comunicación y sistemas de control en general. El programa de desarrollo incluye actividades de diseño, programación y prueba de software específicamente adaptado para este microcontrolador, permitiendo a los estudiantes explorar y aplicar conceptos relacionados con el desarrollo de sistemas embebidos en un entorno educativo o profesional.

## TRABAJO

>El repositorio está organizado en las siguientes secciones:

>1. **Tareas de MPEI**: Ejercicios y problemas asignados como tarea en clase relacionados con Métodos Probabilísticos para la Ingeniería Informática.
2. **Proyectos de Microprocesadores e Interfaces**: Proyectos relacionados con el desarrollo de sistemas embebidos utilizando el microcontrolador dsPIC33FJ128MC802 y sus interfaces.
3. **Ejemplos de Código**: Ejemplos de código para entender el funcionamiento del microcontrolador y sus periféricos.
4. **Documentación**: Documentación técnica, manuales y notas de clase relacionadas con MPEI y el desarrollo de sistemas embebidos.
5. **Recursos**: Enlaces útiles, bibliografía recomendada y material adicional para profundizar en los temas tratados.

## DSPIC33FJ128MC802 

<div style="text-align:center;">
    <img src="https://github.com/scortua/MPEI-LAS-AMIGAS/assets/140832465/0d1bc36e-1ca0-447d-8ebd-742a9ed87d61" alt="Captura de pantalla 2024-04-05 172654" width="500"/>
</div>



##### MPEI_Ejemplo0.X
Este proyecto trata de esplicar la simplicidad del sistema de entorno de desarrollo integrado (IDE) de MPLAB. Además, la implementación, la inicialización y la configuración del microcontrolador DSPIC33FJ128MC802.
```c
#pragma config FNOSC = FRC  // oscillator mode
#pragma config IESO = ON    // internal external switch
#pragma config FWDTEN = OFF // watchdog timer enable
#pragma config ICS = PGD3   // comm channel select
```
> Configuración inicial del micro

```c
#include "xc.h"
#include <p33FJ128MC802.h>
#define FCY 7370000UL // reloj micro
#include <libpic30.h>
```
> Configuración del micro con su relog y libreria del delay

```c
#define x LATXbits.LATA0 // X es A o B
#define y PORTXbits.RB0 //

AD1PCFGL = 0xFFFF;  // pines analogos
    // registos de 1x16 bits digital 1 analogo 0
TRISX = 0x0000;     // puerto X / entrada 1 salida 0
```
> Configuración pines de IN/OUT 
	Configuración de periféricos

##### REPROGRAMACIÓN DE PINES RP

Se usan las tablas del datasheet para reprogramar pines.

```c
 RPINRXXbits.U2RXR = 13; // periferico UART2 como entrada en el pin RP13
 RPORYbits.RP15R = 5; // periferico UART como salida en el pin RP15
```
> lo anterior es un ejemplo, pero para hacer la reprogramación de pines se necesita de la siguientes tablas.

<div style="text-align:center;">
    <img src="https://github.com/scortua/MPEI-LAS-AMIGAS/assets/140832465/98bf405c-71cc-445c-aef9-73449652c5e0" alt="Captura de pantalla 2024-04-05 172850" style="max-width: 500px;"/>
</div>

###### Tabla para configuración de entrada.

<div style="text-align:center;">
    <img src="https://github.com/scortua/MPEI-LAS-AMIGAS/assets/140832465/6c296bfa-ccc0-43f7-b857-99be30bfcb73" alt="Captura de pantalla 2024-04-05 173517" style="max-width: 500px;"/>
</div>

######  Tabla para configuración de salida.

##### MOTOR_PASO

Aqui entra el uso de las interrupciones.

```c
	 IFS0bits.INT0IF = 0; // clarear interrupción
    IEC0bits.INT0IE = 1; // activar interrupción
	
void __attribute__((interrupt, auto_psv)) _INT0Interrupt(void) {
    // código
	IFS0bits.INT0IF = 0; // bandera para desactivar interrupción
    }
}
```

##### CALCULATOR

A partir de un teclado de matriz se hace una acción matematica:

  > { '1', '2', '3', 'A' },
    { '4', '5', '6', 'B' },
    { '7', '8', '9', 'C' },
    { '*', '0', '#', 'D' }

```c
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
```
##### CONVERSORES ADC

Se implementa y configura el conversor análogo digital para adquirir una variación de voltaje.

>Configure el módulo ADC:
1. Seleccione los pines del puerto como entradas analógicas (AD1PCFGH<15:0> ο AD1PCFGL<15:0>).
2. Seleccione la fuente de referencia de voltaje para que coincida rango esperado en entradas analógicas (AD1CON2<15:13>)
3. Seleccione el reloj de conversión analógica para hacer coincidir la velocidad de datos deseada con el procesador reloj (AD1CON3<7:0>)
4. Determinar cuántos canales S/H se utilizan (AD1CON2<9:8> y AD1PCFGH<15:0> ó AD1PCFGL<15:0>)
5. Seleccionar la muestra/conversión adecuada secuencia (AD1CON1<7:5> y AD1CON3<12:8>)
6. Seleccionar cómo son los resultados de conversión presentado en el búfer (AD1CON1<9:8>)
7. Encienda el módulo ADC (AD1CON1<15>)

Se configura el ADC con una función para generalizar con cada registro que indica el datasheet.
```c
void ADC_configuration() {
    AD1CON1 = 0x0000;
    AD1CON2 = 0X0000;
    AD1CON3 = 0x8000;
    AD1CSSL = 0;
    AD1CHS0 = 0x0000;
    AD1CON1bits.ADON = 1;
    AD1CHS0bits.CH0SA = 2;
     AD1CHS0bits.CH0SB = 2;
}
```

Se configura la adquisición de la variación de voltaje que obtiene el PIN AN del micro.
```c
void adquirir_AD() {
    AD1CON1bits.SAMP = 1;
    __delay_ms(1);
    AD1CON1bits.SAMP = 0;
    while (!AD1CON1bits.DONE);
    lectura = ADCBUF0; // >> 2; lectura es el voltaje medido
    // lectura = lectura & (0x03FF); es inecesario
    __delay_ms(1);
}
```

##### VISUALIZACIÓN DINÁMICA

Se implementa un ejemplo de visualizador dinámico para el micro, sin necesidad de decodificador con 2 displays 7 segmentos.

##### UART (universal asynchronous receiver / transmitter)

Se muestra en proyectos la forma de configurar la uart para transmitir y recivir datos, para esto se debe usar un driver que permita de serial a uart.

<div style="text-align:center;">
    <img src="https://github.com/scortua/MPEI-LAS-AMIGAS/raw/main/assets/140832465/f0fde879-0b51-4d76-b3a3-a39353b640a6" alt="Captura de pantalla 2024-04-05 191415" style="max-width: 200px;"/>
</div>

```c
void UART_conf() {
    U1MODEbits.STSEL = 0; // 1-Stop bit        
    U1MODEbits.PDSEL = 0; // No Parity, 8-Data bits
    U1MODEbits.ABAUD = 0; // Auto-Baud disabled
    U1MODEbits.BRGH = 0; // Standard-Speed mode

    U1BRG = BRGVAL; // Baud Rate setting for monda

    U1STAbits.UTXISEL1 = 0; // Interrupt after one RX character is received   
    IEC0bits.U1RXIE = 1; // Enable UART RX interrupt

    IPC3bits.U1TXIP = 5; // Set the transmition priority in 5 Lower than Reception 
    IPC2bits.U1RXIP = 6; // Set the reception priority in 6 Higher than Transmition

    U1MODEbits.UARTEN = 1; // Enable UART
    U1STAbits.UTXEN = 1; // Enable Transmition UART    
}
```

Aquí se expecifica de igual manera la forma en que se debe configurar la uart en transmición y en recepción.


