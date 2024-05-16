#include "i2c.h"

#ifdef I2C_MASTER_MODE
void I2C_Init_Master(void)
{
    /*
     se puede mandar directamente que se habiliten los tris de sda y dlk
     TRIS_SCL = 1;
     TRIS_SDA = 1;
     */
    I2C1BRG = BRGI2C;       // baudiaje para 400kHZ
    I2C1CONbits.I2CEN = 1; // se habilita el I2C
}

void I2C_Start(void)
{
    IFS1bits.MI2C1IF = 0;
    I2C1CONbits.SEN = 1;     // bit de inicio or start
    while(IFS1bits.MI2C1IF == 0);
}

void I2C_Stop(void)
{
    IFS1bits.MI2C1IF = 0;
    I2C1CONbits.PEN = 1;    // bit de parada or stop
    while(IFS1bits.MI2C1IF == 0);
}

void I2C_Restart(void)
{
    IFS1bits.MI2C1IF = 0;
    I2C1CONbits.RSEN = 1;
    while(IFS1bits.MI2C1IF == 0);
}

void I2C_Ack(void)
{
    IFS1bits.MI2C1IF = 0;
    I2C1CONbits.ACKEN = 1;   // activacion del bit ACK
    I2C1CONbits.ACKDT = 0;  // ACK
    while(IFS1bits.MI2C1IF == 0);
}

void I2C_Nack(void)
{
    IFS1bits.MI2C1IF = 0;
    I2C1CONbits.ACKEN = 1;  // activacion del bit NACK
    I2C1CONbits.ACKDT = 1;  // NACK
    while(IFS1bits.MI2C1IF == 0);
}

short I2C_Tx(char data)
{
    IFS1bits.MI2C1IF = 0;
    I2C1TRN = data;     // registro de envio de datos de 8 bits
    while(IFS1bits.MI2C1IF == 0);
}

unsigned char I2C_Rx(void)
{
    IFS1bits.MI2C1IF = 0;
    I2CCONbits.RCEN = 1;    // activacion del bit de recepcion del esclavo
    while(IFS1bits.MI2C1IF == 0);
    return I2C1RCV;              // registro de recepcion de datos de 8 bits
}
#endif

#ifdef I2C_SLAVE_MODE
void I2C_Init_Slave(unsigned char add_slave)
{
    TRIS_SCL = 1;
    TRIS_SDA = 1;
    SSPSTAT = 0x80;
    SSPADD = add_slave;
    SSPCON1 = 0x36;
    SSPCON2 = 0x01;
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    PIR1bits.SSPIF = 0;
    PIE1bits.SSPIE = 1;
}

short I2C_Error_Read(void)
{
    SSPCON1bits.CKP = 0;
    return ((SSPCON1bits.SSPOV) || (SSPCON1bits.WCOL)) ? 1 : 0;
}

short I2C_Write_Mode(void)
{
    return(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) ? 1 : 0;
}
short I2C_Read_Mode(void)
{
    return (!SSPSTATbits.D_nA && SSPSTATbits.R_nW) ? 1: 0;
}

void I2C_Error_Data(void)
{
    short z;
    SSPCON1bits.CKP = 0;
    z = SSPBUF;
    SSPCON1bits.SSPOV = 0;
    SSPCON1bits.WCOL = 0;
    SSPCON1bits.CKP = 1;
    SSPCON1bits.SSPM3 = 0;
}

unsigned char I2C_Read_Slave(void)
{
    short z;
    unsigned char dato_i2c;
    z = SSPBUF;
    while(!BF);
    dato_i2c = SSPBUF;
    SSPCON1bits.CKP = 1;
    SSPCON1bits.SSPM3 = 0;
    return dato_i2c;
}

void I2C_Write_Slave(char dato_i2c)
{
    short z;
    z = SSPBUF;
    BF = 0;
    SSPBUF = dato_i2c;
    SSPCON1bits.CKP = 1;
    while(SSPSTATbits.BF == 1);
}
#endif