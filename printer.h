//20-12-2019



#ifndef PRINTER_H_
#define PRINTER_H_


#define _XTAL_FREQ 40000000


#include <stdint.h>

#include <xc.h>


// ------------------> printer head <----------------------

#define HEAD_DAT_LAT   LATDbits.LATD0
#define HEAD_DAT_TRIS  TRISDbits.RD0

#define HEAD_CLK_LAT   LATDbits.LATD1
#define HEAD_CLK_TRIS  TRISDbits.RD1

#define HEAD_LAT_LAT   LATDbits.LATD2
#define HEAD_LAT_TRIS  TRISDbits.RD2

#define HEAD_PRINT_LAT   LATDbits.LATD3
#define HEAD_PRINT_TRIS  TRISDbits.RD3


#define HEAD_DAT_L  HEAD_DAT_LAT=0; HEAD_DAT_TRIS=0
#define HEAD_DAT_H  HEAD_DAT_LAT=0; HEAD_DAT_TRIS=1

#define HEAD_CLK_L  HEAD_CLK_LAT=0; HEAD_CLK_TRIS=0
#define HEAD_CLK_H  HEAD_CLK_LAT=0; HEAD_CLK_TRIS=1

#define HEAD_LAT_L  HEAD_LAT_LAT=0; HEAD_LAT_TRIS=0
#define HEAD_LAT_H  HEAD_LAT_LAT=0; HEAD_LAT_TRIS=1

#define HEAD_PRINT_L  HEAD_PRINT_LAT=0; HEAD_PRINT_TRIS=0
#define HEAD_PRINT_H  HEAD_PRINT_LAT=0; HEAD_PRINT_TRIS=1


#define HEAD_PIN_INIT  HEAD_DAT_H;\
                       HEAD_CLK_H;\
                       HEAD_LAT_H;\
                       HEAD_PRINT_H


// ------------------> stepper motor <---------------------

#define IN11_LAT   LATBbits.LATB0
#define IN11_TRIS  TRISBbits.RB0

#define IN21_LAT   LATBbits.LATB1
#define IN21_TRIS  TRISBbits.RB1

#define IN22_LAT   LATBbits.LATB2
#define IN22_TRIS  TRISBbits.RB2

#define IN12_LAT   LATBbits.LATB3
#define IN12_TRIS  TRISBbits.RB3


#define STEPPER_PIN_INIT  IN12_LAT=0;\
                          IN12_TRIS=0;\
                          IN22_LAT=0;\
                          IN22_TRIS=0;\
                          IN21_LAT=0;\
                          IN21_TRIS=0;\
                          IN11_LAT=0;\
                          IN11_TRIS=0


#define STEPPER_PIN_L  IN12_LAT=0;\
                       IN22_LAT=0;\
                       IN21_LAT=0;\
                       IN11_LAT=0


#define IN12_L  IN12_LAT=0
#define IN22_L  IN22_LAT=0
#define IN21_L  IN21_LAT=0
#define IN11_L  IN11_LAT=0

#define IN12_H  IN12_LAT=1
#define IN22_H  IN22_LAT=1
#define IN21_H  IN21_LAT=1
#define IN11_H  IN11_LAT=1


#define STEPPER_CONT1  IN11_L;\
                       IN21_H;\
                       IN22_H;\
                       IN12_H

#define STEPPER_CONT2  IN11_L;\
                       IN21_H;\
                       IN22_H;\
                       IN12_L

#define STEPPER_CONT3  IN11_H;\
                       IN21_H;\
                       IN22_H;\
                       IN12_H

#define STEPPER_CONT4  IN11_H;\
                       IN21_H;\
                       IN22_L;\
                       IN12_H

#define STEPPER_CONT5  IN11_H;\
                       IN21_L;\
                       IN22_H;\
                       IN12_H

#define STEPPER_CONT6  IN11_H;\
                       IN21_L;\
                       IN22_H;\
                       IN12_L

#define STEPPER_CONT7  IN11_L;\
                       IN21_L;\
                       IN22_H;\
                       IN12_H

#define STEPPER_CONT8  IN11_L;\
                       IN21_L;\
                       IN22_L;\
                       IN12_H


#define  PRINTER_PIN_INIT  STEPPER_PIN_INIT;\
                           HEAD_PIN_INIT


// bit manipulation

#define REG_BIT_SET(reg, bit) ((reg) |= (1<<(bit)))
#define REG_BIT_CLR(reg, bit) ((reg) &= ~(1<<(bit)))

#define REG_BIT_IS_SET(reg, bit) ((reg)>>(bit)&1)
#define REG_BIT_IS_CLR(reg, bit) (!((reg)>>(bit)&1))



void printer_step(uint8_t num);
void printer_end(void);
void printer_string18(const char *string);
void printer_string28(const char *string);
void printer_image(uint16_t height, const uint8_t *image);  //width=384(HEAD_SIZE_DOTS)
void printer_ee_image(uint16_t height);
void printer_string_num30(const char *string);
//void printer_line2(uint8_t *data);
void printer_noise(uint16_t);

void printer_flash_image(uint16_t height);
void printer_roll_stop(uint8_t num);

void printer_buffer(uint8_t *buff);

#endif //PRINTER_H_


