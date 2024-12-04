// SSD1306 Display Module 128*64 I2C
//
// ssd1306.h
// Microchip XC8 Compiler v1.33
//
// version without display buffer
// 15-03-2024
//


#ifndef SSD1306_H
#define	SSD1306_H


#include <xc.h>
#include <stdlib.h>
#include <stdint.h>
#include "i2c.h"


#define SSD1306_SCR_HEIGHT  64
#define SSD1306_SCR_WIDHT   128
#define SSD1306_SCR_SIZE    (SSD1306_SCR_WIDHT*(SSD1306_SCR_HEIGHT/8U))

#define SSD1306_DEFAULT_BRIGHTNESS  128U

void oled_fill(uint8_t);  //fill screen
void oled_bright(uint8_t);  //brightness
void oled_init(void);  //init i2c and display
void oled_sleep(void);  //display power off
void oled_data(uint8_t);  //write data to screen
void oled_char(uint8_t, uint8_t, uint8_t);  //draw a character
void oled_print(uint8_t, uint8_t, char*);  //print a string


#endif	/* SSD1306_H */