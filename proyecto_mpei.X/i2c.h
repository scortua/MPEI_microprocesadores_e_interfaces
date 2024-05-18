#include <xc.h>
#define FCY 7372800

#define TRIS_SCL TRISBbits.TRISB8
#define TRIS_SDA TRISBbits.TRISB9

#define FSCL 400000
#define FCLK FCY/2
#define delay 0.00000011
#define BRGI2C ((1/FSCL - delay)*FCLK)-1

#define I2C_MASTER_MODE
//#define I2C_SLAVE_MODE

#ifdef I2C_MASTER_MODE
void I2C_Init_Master(void);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_Restart(void);
void I2C_Ack(void);
void I2C_Nack(void);
unsigned char I2C_Rx(void);
short I2C_Tx(char data);
#endif

#ifdef I2C_SLAVE_MODE
void I2C_Init_Slave(unsigned char add_slave);
unsigned char I2C_Read_Slave(void);
void I2C_Write_Slave(char dato_i2c);
void I2C_Error_Data(void);
short I2C_Write_Mode(void);
short I2C_Read_Mode(void);
short I2C_Error_Read(void);
#endif