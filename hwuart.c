//02-12-2024

#include "hwuart.h"


//-----------------------------------------------------------------------------
void hw_uart_init(void)
    {
    SPEN=1;
    INIT_HW_RXUART_PIN;
    INIT_HW_TXUART_PIN;

    //TXSTA: CSRC TX9 TXEN SYNC SENDB BRGH TRMT TX9D
    TX9=0;
    TXEN=0;
    SYNC=0; //EUSART Mode Select bit, 0 = Asynchronous mode
    SENDB=0;
    BRGH=1; //High Baud Rate Select bit

    //RCSTA
    SPEN=1;
    RX9=0;
    CREN=0; //Continuous Receive Enable bit, Asynchronous mode: 1 = Enables receiver 0 = Disables receiver

    //BAUDCON
    RXDTP=0;
    TXCKP=0;
    BRG16=0; //16-Bit Baud Rate Register Enable bit, 0 = 8-bit Baud Rate Generator – SPBRG only (Compatible mode), SPBRGH value ignored
    WUE=0;
    ABDEN=0;

    //FOSC/[64 (n + 1)]  //High Baud Rate FOSC/[16 (n + 1)]
    SPBRGH=0;
    SPBRG=21;

    RCIF=0;
    RCIE=0;
    }


//-----------------------------------------------------------------------------
void hw_uart_rx_enable(void)
    {
    CREN=1;
    RCIF=0;
    }

//-----------------------------------------------------------------------------
void hw_uart_rx_disable(void)
    {
    CREN=0;
    }

//-----------------------------------------------------------------------------
uint8_t hw_uart_read_byte(void)
    {
    while(!RCIF);
    return RCREG;
    }

//-----------------------------------------------------------------------------
uint16_t hw_uart_read_two_byte(void)
    {
    uint16_t low=hw_uart_read_byte();
    uint8_t high=hw_uart_read_byte();

    return((low<<8)|high);
    }

//-----------------------------------------------------------------------------
void hw_uart_get_data(uint8_t *buf, uint8_t size)
    {
    for(uint8_t i=0; i<size; i++)
        {
        buf[i]=hw_uart_read_byte();
        }
    }

