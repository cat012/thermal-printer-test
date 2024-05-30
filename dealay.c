
#include "delays.h"





//-----------------------------------------------------------------------------
void delay_ms(volatile uint16_t delay)
    {
    while(delay-->0) __delay_us(1000);
    }


