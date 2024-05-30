// main.h
// PIC18F4520
// 23-11-2019


#ifndef MAIN_H
#define MAIN_H


#define _XTAL_FREQ 40000000


// CONFIG1H
#pragma config OSC = HSPLL
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown Out Reset Voltage bits (Minimum setting)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 1024     // Watchdog Timer Postscale Select bits (1:1024)

// CONFIG3H
#pragma config CCP2MX = PORTBE   // CCP2 MUX bit
#pragma config PBADEN = ON      // PORTB A/D Enable bit (PORTB<4:0> pins are configured as analog input channels on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = OFF      // MCLR Pin Enable bit (RE3 input pin enabled; MCLR disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) not protected from table reads executed in other blocks)


#include <xc.h>


#define SW_UART_BAUD_RATE  4800
#define SW_UART_BAUD_CORR  500  //0xxx 200 ok  500isisok 1000okerr 1100err


#define BUZZER_PIN   LATBbits.LATB4
#define BUZZER_TRIS  TRISBbits.RB4

#define BUZZER_PIN_INIT BUZZER_PIN=0; BUZZER_TRIS=0



#define  TMR0_PRESCALER_256  0b00000111
#define  TMR0_PRESCALER_128  0b00000110
#define  TMR0_PRESCALER_64   0b00000101
#define  TMR0_PRESCALER_32   0b00000100
#define  TMR0_PRESCALER_16   0b00000011
#define  TMR0_PRESCALER_8    0b00000010
#define  TMR0_PRESCALER_4    0b00000001
#define  TMR0_PRESCALER_2    0b00000000

#define TMR0_PRESCALER  TMR0_PRESCALER_2

#define TMR0_OVF_FREQ     100U
#define TMR0_OVF_PRELOAD  (65536U-(((_XTAL_FREQ/4U)/2U)/TMR0_OVF_FREQ))

#define TMR0_PERIOD_MS  (1000U/TMR0_OVF_FREQ)

#define EVENT_PERIOD(x)  ((x)/TMR0_PERIOD_MS)


#define TMR1_PRESCALER_8  0b00110000
#define TMR1_PRESCALER_4  0b00100000
#define TMR1_PRESCALER_2  0b00010000
#define TMR1_PRESCALER_1  0b00000000

#define TMR1_PRESCALER  TMR1_PRESCALER_1

#define TMR1_OVF_FREQ     ((SW_UART_BAUD_RATE*3)+SW_UART_BAUD_CORR)
#define TMR1_OVF_PRELOAD  (65536U-(((_XTAL_FREQ/4U)/1U)/TMR1_OVF_FREQ))



#endif /* MAIN_H */


