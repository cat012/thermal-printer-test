// ssd1306.c
//
// 10-13-2022 128x64
// nobuff 15.03.2024


#include "ssd1306.h"


extern const uint8_t font5x7[];


extern void delay_ms(volatile uint16_t val);


static const uint8_t SSD1306_I2C_ADDRESS = 0x78;

static const uint8_t SSD1306_COMM_MODE = 0x00;
static const uint8_t SSD1306_DATA_MODE = 0x40;

static const uint8_t SSD1306_SLEEP_CMD = 0xae;
static const uint8_t SSD1306_BRIGHT_CMD = 0x81;


static const uint8_t initc[] =  //init commands
{
  0xae, //display off sleep mode
  0xd5, //display clock divide
  0x80, //
  0xa8, //set multiplex ratio
  0x3f, //
  0xd3, //display offset
  0x00, //
  0x40, //set display start line
  0x8d, //charge pump setting
  0x14, //
  0x20, //memory addressing mode
  0x00, //horizontal addressing mode

  0xa1, //segment re-map
  0xc8, //COM output scan direction
  0xda, //COM pins hardware configuration
  0x12, //0x12 for 128*64  //0x22 for 128*32

  0x81, //set contrast (brightness)
  SSD1306_DEFAULT_BRIGHTNESS,
  0xd9, //pre-charge period
  0xf1, //
  0xdb, //VCOMH deselect level
  0x40, //
  0xa4, //entire display off
  0xa6, //normal display, 0xa7 inverse display
  0xaf  //display turned on
};


//-------------------------------------------------------------------------------------------------
static void oled_goto(uint8_t x, uint8_t y)
    {
    i2c0_start();
    i2c0_send_byte(SSD1306_I2C_ADDRESS);
    i2c0_send_byte(SSD1306_COMM_MODE);
    i2c0_send_byte(0x21); //set column address
    i2c0_send_byte(x);    //start address
    i2c0_send_byte(127);  //end address

    i2c0_send_byte(0x22); //set page address
    i2c0_send_byte(y);
    i2c0_send_byte(7);
    i2c0_stop();
    }


//-------------------------------------------------------------------------------------------------
void oled_fill(uint8_t data)
    {
    oled_goto(0,0);

    i2c0_start();
    i2c0_send_byte(SSD1306_I2C_ADDRESS);
    i2c0_send_byte(SSD1306_DATA_MODE);
    for(uint16_t k=0; k<SSD1306_SCR_SIZE; k++) i2c0_send_byte(data);
    i2c0_stop();
    }


//-------------------------------------------------------------------------------------------------
static void oled_comm(uint8_t data)  //send a command
    {
    i2c0_start();
    i2c0_send_byte(SSD1306_I2C_ADDRESS);
    i2c0_send_byte(SSD1306_COMM_MODE);
    i2c0_send_byte(data);
    i2c0_stop();
    }


//-------------------------------------------------------------------------------------------------
void oled_bright(uint8_t val)  //brightness
    {
    oled_comm(SSD1306_BRIGHT_CMD);
    oled_comm(val);
    }



//-------------------------------------------------------------------------------------------------
void oled_init(void) //init i2c0 and display
    {
    delay_ms(100);  //delay for display power up
    i2c0_init();
    for(uint8_t i=0; i<(sizeof initc); i++) oled_comm(initc[i]); //send the init commands
    oled_fill(0x00);
    //oled_bright(0);
    }


//-------------------------------------------------------------------------------------------------
void oled_sleep(void)  //display off
    {
    oled_comm(SSD1306_SLEEP_CMD);
    }


//-------------------------------------------------------------------------------------------------
void oled_data(uint8_t data)  //send data
    {
    i2c0_start();
    i2c0_send_byte(SSD1306_I2C_ADDRESS);
    i2c0_send_byte(SSD1306_DATA_MODE);
    i2c0_send_byte(data);
    i2c0_stop();
    }


//-------------------------------------------------------------------------------------------------
void oled_char(uint8_t x, uint8_t y, uint8_t sign)  //print character  x-0..127 y-0..8
    {
    oled_goto(x,y);
    for(uint8_t i=0; i<5; i++) oled_data(font5x7[5*(sign-32)+i]);
    }


//-------------------------------------------------------------------------------------------------
void oled_print(uint8_t x, uint8_t y, char *str)  //print a string  x-0..127 y-0..8
    {
    oled_goto(x,y);
    for(; *str; *str++)
        {
        i2c0_start();
        i2c0_send_byte(SSD1306_I2C_ADDRESS);
        i2c0_send_byte(SSD1306_DATA_MODE);
        i2c0_send_byte(font5x7[5*(*str-32)+0]);  //write font data
        i2c0_send_byte(font5x7[5*(*str-32)+1]);
        i2c0_send_byte(font5x7[5*(*str-32)+2]);
        i2c0_send_byte(font5x7[5*(*str-32)+3]);
        i2c0_send_byte(font5x7[5*(*str-32)+4]);
        i2c0_send_byte(0x00);
        i2c0_stop();
        }
    }


