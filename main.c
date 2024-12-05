
#include <stdio.h>
#include <stdint.h>
//#include <stdlib.h>

#include "main.h"
#include "printer.h"
#include "ssd1306.h"
#include "m41t56.h"
#include "at24c64.h"
#include "flash.h"
#include "delays.h"
#include "hwuart.h"


extern uint8_t imgtest[];


#define EE_IMG_HEIGHT  170  //48*170=8160 (ee memory = 8192)
#define RX_BUFF_SIZE   48   //48*8(bit)=384(dots)

uint8_t databuff[RX_BUFF_SIZE]=0;

volatile uint8_t steprun=0;

const char msg0[]="------------------------";

const char ptmenumsg[]={"Test"};
const char tsmenumsg[]={"Set Date and Time"};
const char tscursor[7][22]=
{
  {"                   __"},
  {"                __   "},
  {"             __      "},
  {"_                    "},
  {"  __                 "},
  {"     __              "},
  {"        __           "}
};

//xxx
/*
const char mainmenustr[7][22]=
{
  {"UART Load TXT"},
  {"Print Image EEPROM"},
  {"UART Load IMAGE"},
  {"Roll"},
  {"Pint Date Tame"},
  {"Print Test"},
  {""}
};
*/

uint8_t rxrun=0;

volatile uint8_t scrcnt=0;
volatile uint8_t btncnt=0;

uint8_t rtcdata[M41T56_BUFF_SIZE];

//char strbuff[32]; //string buffer

const uint16_t keysadc[5] = { 55, 145, 300, 515, 820 };

//volatile uint32_t debugcnt=0;
//uint32_t t1=0;
//uint32_t t2=0;

uint16_t fimgh=0;

uint16_t rximgheight=0;


//=============================================================================
void interrupt handler(void)
    {
    if(TMR0IF && TMR0IE)  //timer0 overflow
        {
        TMR0IF=0;
        TMR0=TMR0_OVF_PRELOAD;

        if(scrcnt) scrcnt--;
        if(btncnt) btncnt--;
        return;
        }

    if(TMR1IF && TMR1IE)  //timer1 overflow
        {
        TMR1IF=0;
        TMR1=TMR1_OVF_PRELOAD;

        //debugcnt++;
        return;
        }

    if(RCIF && RCIE)
        {
        RCIF=0;
        return;
        }
    }


//-----------------------------------------------------------------------------
uint16_t get_adc(uint8_t channel)
    {
    uint16_t adc=0;

    ADCON0=((channel<<2)&0b00111100);

    ADON=1;

    GODONE=1;
    while(GODONE);

    GODONE=1;
    while(GODONE);
    adc=ADRES;

    ADON=0;

    return adc;
    }


//-----------------------------------------------------------------------------
uint8_t get_key(void)
    {
    uint8_t key = 0;
    uint16_t adc=0;

    adc = get_adc(1);

    for(uint8_t i=0; i<5; i++)
        {
        if(adc<keysadc[i])
            {
            key=i+1;
            break;
            }
        }

    return key;
    }


//-----------------------------------------------------------------------------
uint8_t check_keys(void)
    {
    uint8_t k=0;
    static uint8_t oldkey;

    uint8_t newkey = get_key();
    if(newkey != oldkey)
        {
        delay_ms(50);
        newkey = get_key();
        if(newkey != oldkey)
            {
            oldkey = newkey;
            k=newkey;
            }
        }
    return k;
    }


//-----------------------------------------------------------------------------
static void beep_delay(uint8_t ms)
    {
    for(uint8_t b=0; b<ms; b++)
        {
        BUZZER_PIN=1;
        __delay_us(200);
        BUZZER_PIN=0;
        __delay_us(700);
       }
    }


//-----------------------------------------------------------------------------
void screen_date_time(void)
    {
    sprintf(databuff, "%1u %02u-%02u-%02u", rtcdata[DAY_REG], rtcdata[DATE_REG], rtcdata[MONTH_REG], rtcdata[YEAR_REG]);
    oled_print(0,0,databuff);

    sprintf(databuff, "%02u:%02u:%02u", rtcdata[HOURS_REG], rtcdata[MINUTES_REG], rtcdata[SECONDS_REG]);
    oled_print(78,0,databuff);
    }


//-----------------------------------------------------------------------------
inline void main_screen(void)
    {
    screen_date_time();

    //t1=debugcnt;
    //t2=debugcnt;
    }


//-----------------------------------------------------------------------------
inline void set_time_screen(int8_t * mode)
    {
    oled_print(0,1,(char*)tscursor[*mode]);
    oled_print(0,7,(char*)tsmenumsg);

    screen_date_time();
    }


//-----------------------------------------------------------------------------
inline void ab_screen(void)
    {
    if(rxrun==1) oled_print(0,0,(char*)"TEXT");
    else if(rxrun) oled_print(0,0,(char*)"IMAGE");
    else
        {
        oled_print(0,0,(char*)"     ");
        oled_print(120,0,(char*)"+");

        sprintf(databuff, "%03u %03u",fimgh,rximgheight);
        oled_print(40,0,databuff);

        oled_print(50,2,(char*)"TEXT");
        oled_print(0,4,(char*)"< Roll        Print >");
        oled_print(45,6,(char*)"IMAGE");
        }
    }


//-----------------------------------------------------------------------------
inline void set_time_up(int8_t * mode)
    {
    if(*mode==0) rtc_set_sec(0);
    if(*mode==1) { int8_t t=rtcdata[MINUTES_REG]; if(++t>59) t=0; rtc_set_min(t); }
    if(*mode==2) { int8_t t=rtcdata[HOURS_REG]; if(++t>23) t=0; rtc_set_hrs(t); }
    if(*mode==3) { int8_t t=rtcdata[DAY_REG]; if(++t>7) t=1; rtc_set_day(t); }
    if(*mode==4) { int8_t t=rtcdata[DATE_REG]; if(++t>31) t=1; rtc_set_dat(t); }
    if(*mode==5) { int8_t t=rtcdata[MONTH_REG]; if(++t>12) t=1; rtc_set_mon(t); }
    if(*mode==6) { int8_t t=rtcdata[YEAR_REG]; if(++t>99) t=0; rtc_set_year(t); }
    }

//-----------------------------------------------------------------------------
inline void set_time_down(int8_t * mode)
    {
    if(*mode==0) rtc_set_sec(0);
    if(*mode==1) { int8_t t=rtcdata[MINUTES_REG]; if(--t<0) t=59; rtc_set_min(t); }
    if(*mode==2) { int8_t t=rtcdata[HOURS_REG]; if(--t<0) t=23; rtc_set_hrs(t); }
    if(*mode==3) { int8_t t=rtcdata[DAY_REG]; if(--t<1) t=7; rtc_set_day(t); }
    if(*mode==4) { int8_t t=rtcdata[DATE_REG]; if(--t<1) t=31; rtc_set_dat(t); }
    if(*mode==5) { int8_t t=rtcdata[MONTH_REG]; if(--t<1) t=12; rtc_set_mon(t); }
    if(*mode==6) { int8_t t=rtcdata[YEAR_REG]; if(--t<0) t=99; rtc_set_year(t); }
    }


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
inline void print_date_time(void)
    {
    printer_step(2);

    printer_string18("Время      Дата");

    printer_step(2);

    sprintf(databuff, "%2u:%02u:%02u   %1u %02u-%02u-20%02u",
    rtcdata[HOURS_REG], rtcdata[MINUTES_REG], rtcdata[SECONDS_REG],
    rtcdata[DAY_REG], rtcdata[DATE_REG], rtcdata[MONTH_REG], rtcdata[YEAR_REG]);

    printer_string18(databuff);

    printer_string18(msg0);

    printer_end();
    }


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
inline void printer_test_screen(int8_t * mode)
    {
    oled_print(0,7,(char*)ptmenumsg);
    screen_date_time();

    oled_print(45,2,(char*)"Int EE");
    oled_print(0,4,(char*)"< Noise      EEPROM >");
    oled_print(50,6,(char*)"Roll");
    }


//24 char width 384 (print_string28,18)
//-----------------------------------------------------------------------------
static inline uint8_t uart_getstr(char *buf, uint8_t size)
    {
    uint8_t i=0;
    uint8_t r=0;

    for(; i<size-1; i++)
        {
        buf[i] = hw_uart_read_byte();
        if(buf[i]==13 || buf[i]==27) break;   // RC or escape  //xxx define
        }

    if(buf[i]==27) r=1;
    buf[i] = 0; //add end of string
    return r;
    }


//-----------------------------------------------------------------------------
static inline void get_msg_print(void)
    {
    uint8_t stop=0;

    hw_uart_rx_enable();
    stop=uart_getstr(databuff, 24+1);
    hw_uart_rx_disable();

    if(stop)
        {
        rxrun=0;
        printer_step(10);
        printer_end();
        }
    else
        {
        printer_string28(databuff);
        printer_step(2);

        printer_end();
        }
    }


//-----------------------------------------------------------------------------
static inline void get_img_data_ee(void)
    {
    for(uint8_t k=0; k<EE_IMG_HEIGHT; k++)
        {
        hw_uart_rx_enable();
        hw_uart_get_data(databuff, RX_BUFF_SIZE);
        hw_uart_rx_disable();

        for(uint8_t i=0; i<RX_BUFF_SIZE; i++)
            {
            ee_write(i+((uint16_t)RX_BUFF_SIZE*k),databuff[i]);
            }
        }
    rxrun=0;
    }


#define FLASH_IMG_LINE_SIZE  48
#define TRIPLE_SPI_FLASH_PAGE_SIZE (SPI_FLASH_PAGE_SIZE*3U)
//-----------------------------------------------------------------------------
static inline uint16_t get_img_data_flash(void)
    {
    spi_flash_write_enable();
    spi_flash_chip_erase();
    while(spi_flash_read_status()&STATUS_BIT_WRITE_IN_PROGRESS);

    hw_uart_rx_enable();

    uint16_t height=hw_uart_read_two_byte();

    for(uint8_t h=0; h<(height/16); h++)
        {
        for(uint8_t k=0; k<(TRIPLE_SPI_FLASH_PAGE_SIZE/FLASH_IMG_LINE_SIZE); k++)
            {
            hw_uart_get_data(databuff, RX_BUFF_SIZE);

            for(uint8_t i=0; i<FLASH_IMG_LINE_SIZE; i++)
                {
                flashbuff[(uint16_t)i+((uint16_t)FLASH_IMG_LINE_SIZE*(uint16_t)k)] = databuff[i];
                }
            }

        for(uint8_t i=0; i<3; i++)
            {
            spi_flash_write_enable();
            spi_flash_page_program((i*SPI_FLASH_PAGE_SIZE)+(h*TRIPLE_SPI_FLASH_PAGE_SIZE),flashbuff+(SPI_FLASH_PAGE_SIZE*i));
            while(spi_flash_read_status()&STATUS_BIT_WRITE_IN_PROGRESS);
            }
        }

    hw_uart_rx_disable();

    ee_write(EXT_EE_SIZE-2,height>>8);
    ee_write(EXT_EE_SIZE-1,height);

    rxrun=0;
    return height;
    }


//-----------------------------------------------------------------------------
static inline void get_print_img(void)
    {
    hw_uart_rx_enable();

    rximgheight=hw_uart_read_two_byte();

    for(uint16_t h=0; h<rximgheight; h++)
        {
        hw_uart_get_data(databuff, RX_BUFF_SIZE);
        printer_buffer(databuff);
        }

    hw_uart_rx_disable();
    }


//=============================================================================
void main(void)
    {
    IRCF2=1; IRCF1=1; IRCF0=1; //111=8M 110=4M 101=2M 100=1M 011=500k 010=250k 001=125k 000=31k

    //INTSRC PLLEN - TUN4 TUN3 TUN2 TUN1 TUN0
    OSCTUNE=0b00000010;

    SBOREN=1; //1=enable brown out reset
    SWDTEN=0; //0=disable watchdog timer
    RBPU=1;   //0=enable pull-up

    PEIE=1;
    GIE=1;

    //TMR0ON T08BIT T0CS T0SE PSA T0PS2 T0PS1 T0PS0
    T0CON=0b00000000|TMR0_PRESCALER;
    TMR0=TMR0_OVF_PRELOAD;
    TMR0ON=1;
    TMR0IF=0;
    TMR0IE=1;

    //RD16 T1RUN T1CKPS1 T1CKPS0 T1OSCEN T1SYNC TMR1CS TMR1ON
    T1CON=0b00000000 | TMR1_PRESCALER;
    TMR1=TMR1_OVF_PRELOAD;
    TMR1ON=0;
    TMR1IF=0;
    TMR1IE=0;

    //- TOUTPS3 TOUTPS2 TOUTPS1 TOUTPS0 TMR2ON T2CKPS1 T2CKPS0
    T2CON=0;
    TMR2=0;
    TMR2ON=0;
    TMR2IF=0;
    TMR2IE=0;

    //RD16 T3CCP2 T3CKPS1 T3CKPS0 T3CCP1 T3SYNC TMR3CS TMR3ON
    T3CON=0;
    TMR3=0;
    TMR3ON=0;
    TMR3IF=0;
    TMR3IE=0;

    //- - DCxB1 DCxB0 CCPxM3 CCPxM2 CCPxM1 CCPxM0
    CCP2CON=0b00000000; //CCP2 Capture mode, every falling edge
    CCP1CON=0b00000000;
    CCP2IE=0; //CCPx Interrupt
    CCP1IE=0;

    CMCON=0b000111; //Comparators Off

    ADCON1 = 0b001101; //5-Vref=Vss //4+Vref=Vdd //AN0,AN1
    ADCON0 = 0b000000; //Channel 0  //A/D converter module is disabled
    //ADFM - ACQT2 ACQT1 ACQT0 ADCS2 ADCS1 ADCS0
    ADCON2 = 0b10010101; //TACQ 010=4TAD //TAD 101=FOSC/16 Fosc/16=0.5M=2us  //111=FRC
    ADON=0;

    oled_init();

    PRINTER_PIN_INIT;

    BUZZER_PIN_INIT;

    beep_delay(40);

    rtc_init();

    //flags
    uint8_t print0=0;
    uint8_t print1=0;
    uint8_t print2=0;
    uint8_t print3=0;
    uint8_t print4=0;
    uint8_t print5=0;

    uint8_t scrmod=3;
    int8_t tsmod=0;

    int8_t ptmod=0;
    hw_uart_init();

    flash_init();

    fimgh=(ee_read(EXT_EE_SIZE-2)<<8)|ee_read(EXT_EE_SIZE-1);

    for(;;)
        {
        if(scrcnt==0)
            {
            scrcnt=EVENT_PERIOD(1000);

            rtc_read(rtcdata);

            if(scrmod==0) main_screen();
            if(scrmod==1) set_time_screen(&tsmod);
            if(scrmod==2) printer_test_screen(&ptmod);
            if(scrmod==3) ab_screen();
            }

        if(rxrun)
            {
            if(rxrun==1) get_msg_print();
            if(rxrun==2)
                {
                fimgh=get_img_data_flash();
                //get_img_data_ee();
                beep_delay(30);
                }
            if(rxrun==3)
                {
                get_print_img();
                rxrun=0;
                print0=1;
                }

            //scrcnt=0;
            //beep_delay(10);
            }

        if(print0) //+++++++ roll ++++++++
            {
            print0=0;

            printer_roll_stop(75);

            beep_delay(100);
            scrcnt=0;
            }

        if(print1) //++++++++ date ++++++++
            {
            print1=0;

            print_date_time();

            print0=1; //roll

            scrcnt=0;
            }

        if(print2) //+++++++ test ++++++++
            {
            print2=0;

            printer_step(8);
            printer_image(72, imgtest);
            printer_roll_stop(8);
            beep_delay(70);

            print0=1; //roll

            scrcnt=0;
            }

        if(print4) //+++++++ test ++++++++
            {
            print4=0;

            printer_step(8);
            printer_string_num30((char*)"01234567890");
            printer_step(8);
            printer_noise(100);
            printer_roll_stop(8);
            beep_delay(70);

            scrcnt=0;
            }

        if(print3)
            {
            print3=0;

            printer_step(8);
            printer_flash_image(fimgh);
            printer_roll_stop(8);;
            beep_delay(70);

            //rxrun=3;

            print0=1; //roll

            scrcnt=0;
            }

        if(print5)
            {
            print5=0;

            printer_step(8);
            printer_ee_image(EE_IMG_HEIGHT);
            printer_roll_stop(8);
            beep_delay(70);

            //print0=1; //roll

            scrcnt=0;
            }

        if(btncnt==0)
            {
            btncnt=EVENT_PERIOD(10);

            uint8_t key=check_keys();

            switch(key)
                {
                case 0: break;

                case 1: //----------- center ------------
                        scrcnt=0;
                        if(++scrmod>3) scrmod=0;
                        oled_fill(0);
                        tsmod=0;
                        break;
                case 2: //----------- down -------------
                        scrcnt=0;
                        if(scrmod==1) set_time_down(&tsmod);
                        if(scrmod==2) print0=1;
                        if(scrmod==3) { if(rxrun==0) rxrun=3; }
                        break;
                case 3: //------------ up -------------
                        scrcnt=0;
                        if(scrmod==0) print1=1;
                        if(scrmod==1) set_time_up(&tsmod);
                        if(scrmod==2) print2=1;
                        if(scrmod==3) {if(rxrun==0) rxrun=1; }
                        break;
                case 4: //------------ right ---------------
                        scrcnt=0;
                        if(scrmod==3) print3=1;
                        if(scrmod==2) print5=1;
                        if(scrmod==1) { if(++tsmod>6) tsmod=0; }
                        break;
                case 5: //------------ left -------------
                        scrcnt=0;
                        if(scrmod==3) print0=1;
                        if(scrmod==2) print4=1;
                        if(scrmod==1) { if(--tsmod<0) tsmod=6; }
                        break;

                default: break;
                }
            }
        }
    }


