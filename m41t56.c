// m41t56.c
//
// 24-dec-2019
//


#include "m41t56.h"
#include "i2c.h"


//-----------------------------------------------------------------------------
static void rtc_conv_data(uint8_t *d, uint8_t m)  //d-data //m-mask
    {
    *d=(((((*d & m)>>4)<<1)+(((*d & m)>>4)<<3))+(*d & 0x0F));
    }

/*
//-------------------------------------------------------------------------------------------------
static void m41t56_read(uint16_t size, uint8_t *data)
    {
    i2c1_start();
    i2c1_send_byte(M41T56_ADDR_WRITE);
    i2c1_send_byte(0x00);

    i2c1_start();
    i2c1_send_byte(M41T56_ADDR_READ);

    for(uint8_t i=0; i<(size-1); i++) data[i] = i2c1_read_byte(I2C_ACK);

    data[(size-1)] = i2c1_read_byte(I2C_NACK);

    i2c1_stop();
    }
*/

//-----------------------------------------------------------------------------
void rtc_read(uint8_t *data)
    {
    i2c1_start();
    i2c1_send_byte(M41T56_ADDR_WRITE);

    i2c1_send_byte(0x00);

    i2c1_start();

    i2c1_send_byte(M41T56_ADDR_READ);

    for(uint8_t i=0; i<(M41T56_BUFF_SIZE-1); i++) data[i] = i2c1_read_byte(I2C_ACK);

    data[7] = i2c1_read_byte(I2C_NACK);

    i2c1_stop();

   // m41t56_read(M41T56_BUFF_SIZE, data);

    rtc_conv_data(&data[0], 0b01110000);
    rtc_conv_data(&data[1], 0b01110000);
    rtc_conv_data(&data[2], 0b00110000);
    data[3]=data[3]&0b00000111;
    rtc_conv_data(&data[4], 0b00110000);
    rtc_conv_data(&data[5], 0b00010000);
    rtc_conv_data(&data[6], 0b11110000);
    //7
    }


//-----------------------------------------------------------------------------
void m41t56_write_reg(uint8_t addr, uint8_t val)
    {
    i2c1_start();
    i2c1_send_byte(M41T56_ADDR_WRITE);

    i2c1_send_byte(addr);
    i2c1_send_byte(val);

    i2c1_stop();
    }


//-----------------------------------------------------------------------------
uint8_t m41t56_read_reg(uint8_t addr)
    {
    i2c1_start();
    i2c1_send_byte(M41T56_ADDR_WRITE);

    i2c1_send_byte(addr);

    i2c1_start();
    i2c1_send_byte(M41T56_ADDR_READ);

    uint8_t data = i2c1_read_byte(I2C_NACK);

    i2c1_stop();

    return data;
    }


//-----------------------------------------------------------------------------
void rtc_set_sec(uint8_t val)
    {
    m41t56_write_reg(0x00, (((val/10)<<4)+(val%10)) & 0b01111111);
    }


//-----------------------------------------------------------------------------
void rtc_set_min(uint8_t val)
    {
    m41t56_write_reg(0x01, (((val/10)<<4)+(val%10)) & 0b01111111);
    }


//-----------------------------------------------------------------------------
void rtc_set_hrs(uint8_t val)
    {
    m41t56_write_reg(0x02, (((val/10)<<4)+(val%10)) & 0b10111111);
    }


//-----------------------------------------------------------------------------
void rtc_set_day(uint8_t val)
    {
    m41t56_write_reg(0x03, (val & 0b00000111));
    }


//-----------------------------------------------------------------------------
void rtc_set_dat(uint8_t val)
    {
    m41t56_write_reg(0x04, (((val/10)<<4)+(val%10)) & 0b00111111);
    }


//-----------------------------------------------------------------------------
void rtc_set_mon(uint8_t val)
    {
    m41t56_write_reg(0x05, (((val/10)<<4)+(val%10)) & 0b00011111);
    }


//-----------------------------------------------------------------------------
void rtc_set_year(uint8_t val)
    {
    m41t56_write_reg(0x06, ((val/10)<<4)+(val%10));
    }

//-----------------------------------------------------------------------------
void rtc_init(void)
    {
    i2c1_init();

    uint8_t temp=m41t56_read_reg(0x00);

    if(temp&0b10000000) m41t56_write_reg(0x00, temp & 0b01111111); //rtc start

    m41t56_write_reg(0x07, 0b10000000);  //OUT=1 FT=0 S=0 Calibration=0000
    }
