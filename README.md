
# MICROPROCESADORES E INTERFACES

##### Este repositorio es un almacen que tiene guardado todos los proyectos y trabajos hechos para sistemas embebidos, porgramados en c, en el IDE de Microchip.

<img src="https://github.com/scortua/MPEI-LAS-AMIGAS/assets/140832465/5c3d58c1-fcc4-4eae-a299-d507adad1c6b" alt="Microchip Logo" width="300"/> <img src="https://github.com/scortua/MPEI-LAS-AMIGAS/assets/140832465/c9450c8f-6ebb-4645-8056-1d28fd988173" alt="LogoEscuelaIng" width="300"/>


## DESCRIPCIÓN 
Este repositorio está dedicado a almacenar y compartir los trabajos, proyectos y ejercicios realizados como parte del programa de desarrollo de sistemas embebidos utilizando el microcontrolador dsPIC33FJ128MC802 de Microchip. Los archivos presentes aquí representan el resultado del esfuerzo en equipo de estudiantes que han trabajado en diversas pruebas y estudios para desarrollar programas específicamente diseñados para este microcontrolador.

El sistema embebido al que se refiere este repositorio se centra en el uso del microcontrolador dsPIC33FJ128MC802 de Microchip. Este microcontrolador es una unidad de procesamiento altamente integrada diseñada para aplicaciones embebidas que requieren procesamiento de señales digitales en tiempo real, como controladores de motores, dispositivos de comunicación y sistemas de control en general. El programa de desarrollo incluye actividades de diseño, programación y prueba de software específicamente adaptado para este microcontrolador, permitiendo a los estudiantes explorar y aplicar conceptos relacionados con el desarrollo de sistemas embebidos en un entorno educativo o profesional.

## TRABAJO

>El repositorio está organizado en las siguientes secciones:

>1. **Tareas de MPEI**: Ejercicios y problemas asignados como tarea en clase relacionados con Métodos Probabilísticos para la Ingeniería Informática.
>2. **Proyectos de Microprocesadores e Interfaces**: Proyectos relacionados con el desarrollo de sistemas embebidos utilizando el microcontrolador dsPIC33FJ128MC802 y sus interfaces.
>3. **Ejemplos de Código**: Ejemplos de código para entender el funcionamiento del microcontrolador y sus periféricos.
>4. **Documentación**: Documentación técnica, manuales y notas de clase relacionadas con MPEI y el desarrollo de sistemas embebidos.
>5. **Recursos**: Enlaces útiles, bibliografía recomendada y material adicional para profundizar en los temas tratados.

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
    <img src="https://github.com/scortua/MPEI-LAS-AMIGAS/assets/140832465/98bf405c-71cc-445c-aef9-73449652c5e0" alt="tabla conf in" style="max-width: 500px;"/>
</div>

###### Tabla para configuración de entrada.

<div style="text-align:center;">
    <img src="https://github.com/scortua/MPEI-LAS-AMIGAS/assets/140832465/6c296bfa-ccc0-43f7-b857-99be30bfcb73" alt="tabla conf out" style="max-width: 500px;"/>
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
  > { '4', '5', '6', 'B' },
  > { '7', '8', '9', 'C' },
  > { '*', '0', '#', 'D' }

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
Este microcontrolador tiene 6 entradas analógicas (AN0-AN5).

![Captura de pantalla 2024-04-09 230901](https://github.com/scortua/MPEI-LAS-AMIGAS/assets/140832465/7daa516f-7b4c-4508-b462-3f04c6aa86a7)

Ahora, El SAR o sucessive aproximation register, es el responsable de identificar el valor de la señal de entrada analogica.
La señal analógica de entrada (Vin) se muestrea y se mantiene a un valor constante. Esto se hace mediante un circuito de muestreo y retención (S/H).
Un convertidor digital-analógico (DAC) interno genera una señal analógica que se compara con la señal de entrada muestreada.
Un comparador determina si la señal del DAC es mayor o menor que la señal de entrada.
El resultado de la comparación se utiliza para controlar un registro de aproximaciones sucesivas (SAR). El registro SAR se desplaza un bit a la derecha en cada ciclo de comparación.
El proceso de comparación y desplazamiento continúa hasta que el valor en el registro SAR sea igual o mayor que la señal de entrada.
El valor final en el registro SAR es la representación digital de la señal analógica de entrada.

El conversor análogo-digital tiene como formatos de salida: con/sin signo fraccional y entero.

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
void ADC_conf (){
    // Se establecen los registros de control del conversor ADC
    // AD1CON1: Registro de control 1 del ADC
    AD1CON1 = 0X0000;	// Se limpian todos los bits del registro para configurarlos según sea necesario
    // AD1CON2: Registro de control 2 del ADC
    AD1CON2 = 0;    	// Se limpian todos los bits del registro
    // AD1CON3: Registro de control 3 del ADC
    AD1CON3 = 0X0000;   // Se limpian todos los bits del registro
    // AD1CSSL: Registro de selección de entrada analógica escaneada
    AD1CSSL = 0;    	// Se establecen todos los bits en 0 para omitir el escaneo de las entradas analógicas
    // AD1CHS0: Registro de selección de canal de entrada analógica
    AD1CHS0 = 0X0000;   // Se limpian todos los bits del registro
    // Se activa el módulo ADC para que pueda operar
    AD1CON1bits.ADON = 1;
    // Se configura el módulo ADC para utilizar el reloj interno RC para la conversión
    AD1CON3bits.ADRC = 1;
    // Se selecciona el canal de entrada positivo AN2 para el canal 0 del ADC
    AD1CHS0bits.CH0SA = 2;
    // Se selecciona el canal de entrada positivo AN2 para el canal 0 del ADC (solo en caso de conversor de 10 bits)
    AD1CHS0bits.CH0SB = 2;
}

```

Se configura la adquisición de la variación de voltaje que obtiene el PIN AN del micro.
```c
void adquirir_AD() {
    // Se inicia la conversión ADC colocando el bit SAMP en 1
    AD1CON1bits.SAMP = 1;
    // Se espera un corto período de tiempo para permitir que el muestreo ocurra
    // Esto permite que la señal de entrada alcance su estado estable antes de iniciar la conversión
    __delay_ms(1);
    // Se detiene el muestreo colocando el bit SAMP en 0, lo que inicia la conversión ADC
    AD1CON1bits.SAMP = 0;
    // Se espera hasta que la conversión ADC esté completa, indicado por el bit DONE en 1
    while (!AD1CON1bits.DONE);
    // Se lee el resultado de la conversión ADC desde el registro ADCBUF0
    // En este caso, se está leyendo el valor de la conversión directamente, sin aplicar desplazamiento o máscara
    // El valor leído representará el voltaje medido en el canal ADC configurado previamente
    lectura = ADCBUF0;
    // Se introduce un pequeño retardo antes de salir de la función
    __delay_ms(1);
}
```
La linea **while (!AD1CON1bits.DONE);**, especifica que se esta esperando a que termine de hacer el proceso de conversión para seguir con el siguiente proceso.
Tambien se destaca que se puede usar voltajes externos como referencia cambiando los valores del registro VCFG del control 2 del ADC.

![Captura de pantalla 2024-04-09 230830](https://github.com/scortua/MPEI-LAS-AMIGAS/assets/140832465/79b0835d-a41c-4327-baa8-d7d5c70b994e)

Además, se permite hacer un muestreo de datos por medio de 4 canales. Esto quiere decir que se puede hacer la conversión simultanea para 4 entradas analógica, dando la señal de salida en los registros respectivos de AD1BUFX de 12 o 10 bits dependiendo la configuración.

##### VISUALIZACIÓN DINÁMICA

Se implementa un ejemplo de visualizador dinámico para el micro, sin necesidad de decodificador con 2 displays 7 segmentos.

##### UART (universal asynchronous receiver / transmitter)

La UART funciona en un modo asíncrono, lo que significa que no hay un reloj de sincronización compartido entre el transmisor y el receptor. En su lugar, los datos se envían en serie, bit por bit, junto con una señal de inicio y posiblemente una señal de parada para delimitar cada byte de datos. La velocidad de transmisión de datos, conocida como baud rate, se configura para asegurar que el receptor pueda interpretar correctamente los bits recibidos.

La UART consta de dos partes principales: el transmisor y el receptor. El transmisor toma datos paralelos desde el microcontrolador y los convierte en una secuencia serial de bits que se envían a través de un solo cable. El receptor, por otro lado, toma la secuencia de bits serial recibida a través del cable y la convierte de nuevo en datos paralelos que pueden ser utilizados por el microcontrolador.

![Captura de pantalla 2024-04-09 230731](https://github.com/scortua/MPEI-LAS-AMIGAS/assets/140832465/36fb7707-b398-4d2d-a26b-f480f4ef1f2b)

Se utiliza un conversor usb-serial (ft232). Y por ende se especifica el uso de baudios(bps) que describe la velocidad de transmición de datos.

![Captura de pantalla 2024-04-09 222518](https://github.com/scortua/MPEI-LAS-AMIGAS/assets/140832465/e3722ef8-6ee8-46f0-866e-527576555a4d)

>configuración del modulo UART
1. Inicializar el registro UxBRG un valor de baudio apropiado.
2. Colocar tamaño del dato UxMODEbits.PDSEL, colocar cantidad bits de stop UxMODEbits.STSEL.
3. Colocar de ser haci la interrupción IEC0bits.U1XTXIE 0 para clarearla y 1 para iniciarla.
4. Especificar la prioridad de la interrupción IPC0.U1TXIP.
5. activar uart UxMODEbits.UARTEN.
6. activar la trnasmicion UxMODEbits.UTXEN

```c
void UART_conf() {
    // Se configuran los registros del módulo UART
    // U1MODE: Registro de modo del módulo UART
    U1MODEbits.STSEL = 0; // Selecciona 1 bit de parada
    U1MODEbits.PDSEL = 0; // Configura sin paridad y 8 bits de datos
    U1MODEbits.ABAUD = 0; // Deshabilita el modo de detección automática de baud rate
    U1MODEbits.BRGH = 0; // Establece el modo de velocidad estándar
    // U1BRG: Registro de generador de baud rate del módulo UART
    U1BRG = BRGVAL; // Establece el valor del generador de baud rate para la velocidad deseada
    // U1STAbits: Registro de estado del módulo UART
    U1STAbits.UTXISEL1 = 0; // Interrupción después de recibir un carácter RX
    // IEC0bits: Registro de habilitación de interrupciones del módulo UART
    IEC0bits.U1RXIE = 1; // Habilita la interrupción de recepción UART
    // IPC3bits: Registro de prioridad de interrupción de transmisión del módulo UART
    IPC3bits.U1TXIP = 5; // Establece la prioridad de interrupción de transmisión (menor que la de recepción)
    // IPC2bits: Registro de prioridad de interrupción de recepción del módulo UART
    IPC2bits.U1RXIP = 6; // Establece la prioridad de interrupción de recepción (mayor que la de transmisión)
    // Se activa el módulo UART y se habilita la transmisión
    U1MODEbits.UARTEN = 1;
    U1STAbits.UTXEN = 1;
}
```

Aquí se expecifica de igual manera la forma en que se debe configurar la uart en transmición y en recepción.

##### TIMERS

Es un contador y/o temporizador por flancos de reloj o de tiempo de instrucción.
Existen 4 tipos (temporizador, comparador, contador sincrono y asincrono).

![Captura de pantalla 2024-04-09 225309](https://github.com/scortua/MPEI-LAS-AMIGAS/assets/140832465/d541c3d6-0a47-4f52-b394-f0a5f4c1f59c)

>Configuración del Timer.
1. Decidimos que timer usar // 5 timers // 1 2/3 4/5
2. Decidimos si es de 16 o 32 bits
3. Se decide el modo de operación
4. Se coloca el prescaler del contador 1: H = 1.8.64.256
5. Definir el límite de bits
6. Definir qué hará el timer

```c
void conf_timer() {
    // T1CONbits: Registro de control del Timer 1
    // TCKPS: Bits de prescaler del Timer 1
    T1CONbits.TCKPS = 3; // Configura el preescalador del Timer 1 a 1:256
    // TGATE: Bit de control de sincronización del Timer 1
    T1CONbits.TGATE = 0; // Deshabilita la sincronización del Timer 1
    // TCS: Bit de selección de fuente de reloj del Timer 1
    T1CONbits.TCS = 0; // Selecciona el reloj interno Fosc/2 como fuente de reloj para el Timer 1
    // PR1: Registro de periodo del Timer 1
    PR1 = 0xFFFF; // Establece el valor máximo de cuenta para el Timer 1 (16 bits)
    // TON: Bit de encendido/apagado del Timer 1
    T1CONbits.TON = 1; // Habilita el Timer 1
}
```

![Captura de pantalla 2024-04-09 230058](https://github.com/scortua/MPEI-LAS-AMIGAS/assets/140832465/f7213a96-808e-4a99-a1f2-e15e4d843f13)

![Captura de pantalla 2024-04-09 230214](https://github.com/scortua/MPEI-LAS-AMIGAS/assets/140832465/d657c67d-48f1-43d7-a4bc-a312a5bb0301)


>Entonces si fuera un reloj, si es de 16 bits:
> FCY/2 * 2^16 * prescaler = Tiempo total en segundos.
