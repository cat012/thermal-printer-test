// at24c64.h
//
// AT24C64 8192*8 EEPROM
//
// 25-12-2019
//


#ifndef AT24C64_H_
#define AT24C64_H_


#define _XTAL_FREQ 40000000

#include <stdint.h>

//#include "main.h"

#include "i2c.h"


#define EE_W  0b10100000
#define EE_R  0b10100001

#define EXT_EE_SIZE  8192


extern void delay_1ms(void);

#define EE_DELAY_WRITE  __delay_ms(10);


void ee_write(uint16_t addr, uint8_t data);
uint8_t ee_read(uint16_t addr);
void ee_clear(void);



#endif // AT24C64_H_



