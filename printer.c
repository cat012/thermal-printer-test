//20-12-2019
//10.10.2022


#include <stdlib.h>
#include "printer.h"
#include "at24c64.h"
#include "flash.h"



extern volatile uint8_t steprun;

extern const uint8_t num16x30[];

extern const uint8_t font16x28en[95*56];
extern const uint8_t font16x28ru[64*56];

extern const uint8_t font16x18en[95*36];
extern const uint8_t font16x18ru[64*36];


#define HEAD_SIZE_DOTS  384
#define HEAD_BUFF_SIZE  (HEAD_SIZE_DOTS/8)

static uint8_t headbuff[HEAD_BUFF_SIZE]={0};

#define STEPPER_DELAY __delay_us(700)


//-----------------------------------------------------------------------------
static void head_buff_clear(void)
    {
    for(uint8_t k=0; k<HEAD_BUFF_SIZE; k++) headbuff[k]=0x00;
    }


//-----------------------------------------------------------------------------
static inline void head_clk_pulse(void)
    {
    HEAD_CLK_L;
    __delay_us(1);
    HEAD_CLK_H;
    __delay_us(1);
    }


//-----------------------------------------------------------------------------
static void head_write(void)
    {
    for(uint8_t i=0; i<HEAD_BUFF_SIZE; i++)
        {
        if(REG_BIT_IS_SET(headbuff[i],7)) { HEAD_DAT_H; } else { HEAD_DAT_L; }
        head_clk_pulse();
        if(REG_BIT_IS_SET(headbuff[i],6)) { HEAD_DAT_H; } else { HEAD_DAT_L; }
        head_clk_pulse();
        if(REG_BIT_IS_SET(headbuff[i],5)) { HEAD_DAT_H; } else { HEAD_DAT_L; }
        head_clk_pulse();
        if(REG_BIT_IS_SET(headbuff[i],4)) { HEAD_DAT_H; } else { HEAD_DAT_L; }
        head_clk_pulse();
        if(REG_BIT_IS_SET(headbuff[i],3)) { HEAD_DAT_H; } else { HEAD_DAT_L; }
        head_clk_pulse();
        if(REG_BIT_IS_SET(headbuff[i],2)) { HEAD_DAT_H; } else { HEAD_DAT_L; }
        head_clk_pulse();
        if(REG_BIT_IS_SET(headbuff[i],1)) { HEAD_DAT_H; } else { HEAD_DAT_L; }
        head_clk_pulse();
        if(REG_BIT_IS_SET(headbuff[i],0)) { HEAD_DAT_H; } else { HEAD_DAT_L; }
        head_clk_pulse();
        }

    HEAD_DAT_H;

    HEAD_LAT_L;
    __delay_us(1);
    HEAD_LAT_H;
    __delay_us(1);

    HEAD_PRINT_L;
    __delay_ms(3);
    HEAD_PRINT_H;
    //__delay_ms(2);
    }


//-----------------------------------------------------------------------------
static void stepper_cont(void)
    {
    static uint8_t cnt=0;

    if(cnt==0)
        {
        STEPPER_CONT1;
        STEPPER_DELAY;
        STEPPER_CONT2;
        STEPPER_DELAY;
        STEPPER_CONT3;
        STEPPER_DELAY;
        STEPPER_CONT4;
        STEPPER_DELAY;
        }
    else
        {
        STEPPER_CONT5;
        STEPPER_DELAY;
        STEPPER_CONT6;
        STEPPER_DELAY;
        STEPPER_CONT7;
        STEPPER_DELAY;
        STEPPER_CONT8;
        STEPPER_DELAY;
        }

    if(++cnt>1) cnt=0;
    }


//-----------------------------------------------------------------------------
void printer_end(void)
    {
    STEPPER_PIN_L;
    }


//-----------------------------------------------------------------------------
void printer_step(uint8_t num)
    {
    for(uint8_t k=0; k<num; k++)
        {
        stepper_cont();
        stepper_cont();
        }
    }


//-----------------------------------------------------------------------------
void printer_roll_stop(uint8_t num)
    {
    printer_step(num);
    printer_end();
    }


//-----------------------------------------------------------------------------
void printer_ee_image(uint16_t height)
    {
    for(uint16_t k=0; k<height; k++)
        {
        for(uint8_t i=0; i<HEAD_BUFF_SIZE; i++)
            {
            headbuff[i]=0xff-ee_read(i+(HEAD_BUFF_SIZE*k));
            }

        head_write();
        stepper_cont();
        }
    }


//-----------------------------------------------------------------------------
void printer_flash_image(uint16_t height)
    {
    head_buff_clear();
    for(uint16_t k=0; k<height; k++)
        {
        for(uint8_t i=0; i<48; i++)
            {
            headbuff[i]=0xff-spi_flash_data_read(i+(48*k));
            }

        head_write();
        stepper_cont();
        }
    }


//-----------------------------------------------------------------------------
void printer_image(uint16_t height, const uint8_t *image)
    {
    for(uint16_t k=0; k<height; k++)
        {
        for(uint8_t i=0; i<HEAD_BUFF_SIZE; i++)
            {
            headbuff[i]=0xff-image[i+(HEAD_BUFF_SIZE*k)];
            //headbuff[i]=0xff-image[i+(HEAD_BUFF_SIZE*k)]; source - inversed image
            }

        head_write();
        stepper_cont();
        }
    }


//-----------------------------------------------------------------------------
void printer_noise(uint16_t height)
    {
    for(uint16_t k=0; k<height; k++)
        {
        for(uint8_t i=0; i<HEAD_BUFF_SIZE; i++)
            {
            headbuff[i]=rand();
            }

        head_write();
        stepper_cont();
        }
    }

/*
//-----------------------------------------------------------------------------
void printer_line2(uint8_t *data)
    {
    //head_buff_clear();

    for(uint8_t k=0; k<2; k++)
        {
        for(uint8_t i=0; i<HEAD_BUFF_SIZE; i++)
            {
            headbuff[i]=data[i+HEAD_BUFF_SIZE*k];
            }
        head_write();
        stepper_cont();
        }
    }
*/

//-----------------------------------------------------------------------------
void printer_string18(const char *string)
    {
    head_buff_clear();

    for(uint8_t k=0; k<36/2; k++)
        {
        for(uint8_t i=0; i<HEAD_BUFF_SIZE; i++)
            {
            uint8_t tmp=string[i/2];
            if(!tmp) break; //end of string
            else if((tmp>=32) && (tmp<=126)) headbuff[i]=font16x18en[((i%2)+(2*k))+36*(tmp-32)];
            else if(tmp>=192) headbuff[i]=font16x18ru[((i%2)+(2*k))+36*(tmp-192)];
            }
        head_write();
        stepper_cont();
        }
    }


//-----------------------------------------------------------------------------
void printer_string28(const char *string)
    {
    head_buff_clear();

    for(uint8_t k=0; k<56/2; k++)
        {
        for(uint8_t i=0; i<HEAD_BUFF_SIZE; i++)
            {
            uint8_t tmp=string[i/2];
            if(!tmp) break; //end of string
            else if((tmp>=32) && (tmp<=126)) headbuff[i]=font16x28en[((i%2)+(2*k))+56*(tmp-32)];
            else if(tmp>=192) headbuff[i]=font16x28ru[((i%2)+(2*k))+56*(tmp-192)];
            }
        head_write();
        stepper_cont();
        }
    }


//-----------------------------------------------------------------------------
void printer_string_num30(const char *string)
    {
    head_buff_clear();

    for(uint8_t k=0; k<60/2; k++)
        {
        for(uint8_t i=0; i<HEAD_BUFF_SIZE; i++)
            {
            uint8_t tmp=string[i/2];
            if(!tmp) break; //end of string
            else if((tmp>=48) && (tmp<=57)) headbuff[i]=num16x30[((i%2)+(2*k))+60*(tmp-48)];
            }
        head_write();
        stepper_cont();
        }
    }


