

#include <xc.h>
#include <stdint.h>

void init_uart(void);
inline void timer_isr(void);
uint8_t _getchar(void);
void _putchar(char ch);
void turn_rx_on(void);
void turn_rx_off(void);
void flush_input_buffer(void);

