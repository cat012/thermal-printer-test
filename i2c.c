// i2c.c
//
// 25-12-2019


#include "i2c.h"


//-----------------------------------------------------------------------------
void i2c0_init(void)
    {
    I2C0_SDA_PULL;
    I2C0_SCK_PULL;
    }


//-----------------------------------------------------------------------------
void i2c0_start(void)
    {
    I2C0_SDA_PULL;
    I2C0_DELAY;
    I2C0_SCK_PULL;
    I2C0_DELAY;

    I2C0_SDA_NULL;
    I2C0_DELAY;
    I2C0_SCK_NULL;
    I2C0_DELAY;
    }


//-----------------------------------------------------------------------------
void i2c0_stop(void)
    {
    I2C0_SCK_NULL;
    I2C0_DELAY;
    I2C0_SDA_NULL;
    I2C0_DELAY;

    I2C0_SCK_PULL;
    I2C0_DELAY;
    I2C0_SDA_PULL;
    I2C0_DELAY;
    }


//-----------------------------------------------------------------------------
void i2c0_send_byte(uint8_t data)
    {
    if(data & 0b10000000) { I2C0_SDA_PULL; } else { I2C0_SDA_NULL; }
    I2C0_DELAY;
    I2C0_SCK_PULL;
    I2C0_DELAY;
    I2C0_SCK_NULL;

    if(data & 0b01000000) { I2C0_SDA_PULL; } else { I2C0_SDA_NULL; }
    I2C0_DELAY;
    I2C0_SCK_PULL;
    I2C0_DELAY;
    I2C0_SCK_NULL;

    if(data & 0b00100000) { I2C0_SDA_PULL; } else { I2C0_SDA_NULL; }
    I2C0_DELAY;
    I2C0_SCK_PULL;
    I2C0_DELAY;
    I2C0_SCK_NULL;

    if(data & 0b00010000) { I2C0_SDA_PULL; } else { I2C0_SDA_NULL; }
    I2C0_DELAY;
    I2C0_SCK_PULL;
    I2C0_DELAY;
    I2C0_SCK_NULL;

    if(data & 0b00001000) { I2C0_SDA_PULL; } else { I2C0_SDA_NULL; }
    I2C0_DELAY;
    I2C0_SCK_PULL;
    I2C0_DELAY;
    I2C0_SCK_NULL;

    if(data & 0b00000100) { I2C0_SDA_PULL; } else { I2C0_SDA_NULL; }
    I2C0_DELAY;
    I2C0_SCK_PULL;
    I2C0_DELAY;
    I2C0_SCK_NULL;

    if(data & 0b00000010) { I2C0_SDA_PULL; } else { I2C0_SDA_NULL; }
    I2C0_DELAY;
    I2C0_SCK_PULL;
    I2C0_DELAY;
    I2C0_SCK_NULL;

    if(data & 0b00000001) { I2C0_SDA_PULL; } else { I2C0_SDA_NULL; }
    I2C0_DELAY;
    I2C0_SCK_PULL;
    I2C0_DELAY;
    I2C0_SCK_NULL;

    I2C0_SDA_PULL;
    I2C0_DELAY;
    I2C0_SCK_PULL;
    I2C0_DELAY;
    I2C0_SCK_NULL;
    }


//-----------------------------------------------------------------------------
uint8_t i2c0_read_byte(uint8_t ack)
    {
    uint8_t data=0;

    I2C0_SDA_PULL;

    for(uint8_t i=0; i<8; i++)
        {
        data<<=1;
        I2C0_SCK_PULL;
        I2C0_DELAY;
        if(I2C0_SDA_IS_PULL) data |= 0x01;
        I2C0_SCK_NULL;
        I2C0_DELAY;
        }

    if(ack==I2C_ACK) { I2C0_SDA_NULL; }
    else { I2C0_SDA_PULL; }

    I2C0_DELAY;
    I2C0_SCK_PULL;
    I2C0_DELAY;
    I2C0_SCK_NULL;
    I2C0_DELAY;
    I2C0_SDA_PULL;

    return data;
    }


///////////////////////////////////////////////////////////////////////////////


//-----------------------------------------------------------------------------
void i2c1_init(void)
    {
    I2C1_SDA_PULL;
    I2C1_SCK_PULL;
    }


//-----------------------------------------------------------------------------
void i2c1_start(void)
    {
    I2C1_SDA_PULL;
    I2C1_DELAY;
    I2C1_SCK_PULL;
    I2C1_DELAY;

    I2C1_SDA_NULL;
    I2C1_DELAY;
    I2C1_SCK_NULL;
    I2C1_DELAY;
    }


//-----------------------------------------------------------------------------
void i2c1_stop(void)
    {
    I2C1_SCK_NULL;
    I2C1_DELAY;
    I2C1_SDA_NULL;
    I2C1_DELAY;

    I2C1_SCK_PULL;
    I2C1_DELAY;
    I2C1_SDA_PULL;
    I2C1_DELAY;
    }


//-----------------------------------------------------------------------------
void i2c1_send_byte(uint8_t data)
    {
    for(uint8_t mask=0x80; mask; mask>>=1)
        {
        if(data & mask) { I2C1_SDA_PULL; }
        else { I2C1_SDA_NULL; }

        I2C1_DELAY;
        I2C1_SCK_PULL;
        I2C1_DELAY;
        I2C1_SCK_NULL;
        }

    I2C1_SDA_PULL;
    I2C1_DELAY;
    I2C1_SCK_PULL;
    I2C1_DELAY;
    I2C1_SCK_NULL;
    }


//-----------------------------------------------------------------------------
uint8_t i2c1_read_byte(uint8_t ack)
    {
    uint8_t data=0;

    I2C1_SDA_PULL;

    for(uint8_t i=0; i<8; i++)
        {
        data<<=1;
        I2C1_SCK_PULL;
        I2C1_DELAY;
        if(I2C1_SDA_IS_PULL) data |= 0x01;
        I2C1_SCK_NULL;
        I2C1_DELAY;
        }

    if(ack==I2C_ACK) { I2C1_SDA_NULL; }
    else { I2C1_SDA_PULL; }

    I2C1_DELAY;
    I2C1_SCK_PULL;
    I2C1_DELAY;
    I2C1_SCK_NULL;
    I2C1_DELAY;
    I2C1_SDA_PULL;

    return data;
    }


