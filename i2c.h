// Software I2C
//
// i2c.h
// Microchip XC8 Compiler v1.33
//
// 24-12-2019


#ifndef I2C_H_
#define I2C_H_


#include <xc.h>

#include <stdint.h>


#define _XTAL_FREQ 40000000


#define I2C0_DELAY  __delay_us(1)
#define I2C1_DELAY  __delay_us(1)


/////////////////////////////////////////////////////////////////////

// GPIO for i2c #0

#define I2C0_SDA_PIN   PORTEbits.RE2
#define I2C0_SDA_TRIS  TRISEbits.RE2

#define I2C0_SCK_PIN   PORTEbits.RE0
#define I2C0_SCK_TRIS  TRISEbits.RE0


// GPIO for i2c #1

#define I2C1_SDA_PIN   PORTDbits.RD7
#define I2C1_SDA_TRIS  TRISDbits.RD7

#define I2C1_SCK_PIN   PORTAbits.RA5
#define I2C1_SCK_TRIS  TRISAbits.RA5

/////////////////////////////////////////////////////////////////////


#define I2C0_SDA_CLR  I2C0_SDA_PIN=0
#define I2C0_SDA_SET  I2C0_SDA_PIN=1
#define I2C0_SDA_OUT  I2C0_SDA_TRIS=0
#define I2C0_SDA_INP  I2C0_SDA_TRIS=1

#define I2C0_SCK_CLR  I2C0_SCK_PIN=0
#define I2C0_SCK_SET  I2C0_SCK_PIN=1
#define I2C0_SCK_OUT  I2C0_SCK_TRIS=0
#define I2C0_SCK_INP  I2C0_SCK_TRIS=1

#define I2C0_SDA_PULL  I2C0_SDA_INP
#define I2C0_SDA_NULL  I2C0_SDA_CLR; I2C0_SDA_OUT

#define I2C0_SDA_IS_PULL  I2C0_SDA_PIN==1

#define I2C0_SCK_PULL  I2C0_SCK_INP
#define I2C0_SCK_NULL  I2C0_SCK_CLR; I2C0_SCK_OUT


#define I2C1_SDA_CLR  I2C1_SDA_PIN=0
#define I2C1_SDA_SET  I2C1_SDA_PIN=1
#define I2C1_SDA_OUT  I2C1_SDA_TRIS=0
#define I2C1_SDA_INP  I2C1_SDA_TRIS=1

#define I2C1_SCK_CLR  I2C1_SCK_PIN=0
#define I2C1_SCK_SET  I2C1_SCK_PIN=1
#define I2C1_SCK_OUT  I2C1_SCK_TRIS=0
#define I2C1_SCK_INP  I2C1_SCK_TRIS=1

#define I2C1_SDA_PULL  I2C1_SDA_INP
#define I2C1_SDA_NULL  I2C1_SDA_CLR; I2C1_SDA_OUT

#define I2C1_SDA_IS_PULL  I2C1_SDA_PIN==1

#define I2C1_SCK_PULL  I2C1_SCK_INP
#define I2C1_SCK_NULL  I2C1_SCK_CLR; I2C1_SCK_OUT


#define I2C_ACK   0
#define I2C_NACK  1



void i2c0_init(void);
void i2c0_start(void);
void i2c0_stop(void);
void i2c0_send_byte(uint8_t data);
uint8_t i2c0_read_byte(uint8_t ack);

void i2c1_init(void);
void i2c1_start(void);
void i2c1_stop(void);
void i2c1_send_byte(uint8_t data);
uint8_t i2c1_read_byte(uint8_t ack);



#endif  //end of I2C_H_


