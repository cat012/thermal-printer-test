
#ifndef HW_UART_H
#define	HW_UART_H

#include <xc.h>
#include <stdint.h>

#define HW_TXUART_TRIS  TRISCbits.RC6
#define HW_TXUART_LAT   LATCbits.LATC6
#define HW_TXUART_PIN   PORTCbits.RC6

#define INIT_HW_TXUART_PIN  HW_TXUART_LAT=0; HW_TXUART_TRIS=1

#define HW_RXUART_TRIS  TRISCbits.RC7
#define HW_RXUART_LAT   LATCbits.LATC7
#define HW_RXUART_PIN   PORTCbits.RC7

#define INIT_HW_RXUART_PIN  HW_RXUART_LAT=0; HW_RXUART_TRIS=1


void hw_uart_init(void);
void hw_uart_rx_enable(void);
void hw_uart_rx_disable(void);
uint8_t hw_uart_read_byte(void);
uint16_t hw_uart_read_two_byte(void);
void hw_uart_get_data(uint8_t *buf, uint8_t size);

#endif	/* HW_UART_H */

