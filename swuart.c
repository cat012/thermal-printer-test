// UART.C
//
// Generic software uart written in C, requiring a timer set to 3 times
// the baud rate, and two software read/write pins for the receive and
// transmit functions.
//
// * Received characters are buffered
// * putchar(), getchar(), kbhit() and flush_input_buffer() are available
// * There is a facility for background processing while waiting for input
//
// Colin Gittins, Software Engineer, Halliburton Energy Services
//
// The baud rate can be configured by changing the BAUD_RATE macro as
// follows:
//
// #define BAUD_RATE            19200.0
//
// The function init_uart() must be called before any comms can take place
//
// Interface routines required:
// 1. get_rx_pin_status()
//    Returns 0 or 1 dependent on whether the receive pin is high or low.
// 2. set_tx_pin_high()
//    Sets the transmit pin to the high state.
// 3. set_tx_pin_low()
//    Sets the transmit pin to the low state.
// 4. idle()
//    Background functions to execute while waiting for input.
// 5. timer_set( BAUD_RATE )
//    Sets the timer to 3 times the baud rate.
// 6. set_timer_interrupt( timer_isr )
//    Enables the timer interrupt.
//
// Functions provided:
// 1. void flush_input_buffer( void )
//    Clears the contents of the input buffer.
// 2. char kbhit( void )
//    Tests whether an input character has been received.
// 3. char getchar( void )
//    Reads a character from the input buffer, waiting if necessary.
// 4. void turn_rx_on( void )
//    Turns on the receive function.
// 5. void turn_rx_off( void )
//    Turns off the receive function.
// 6. void putchar( char )
//    Writes a character to the serial port.


//17.05.2013


#include "swuart.h"



#define IN_BUF_SIZE        128

#define uTRUE 1
#define uFALSE 0

static unsigned char        inbuf[IN_BUF_SIZE];
static unsigned char        qin = 0;
static unsigned char        qout = 0;

static char             flag_rx_waiting_for_stop_bit;
static char             flag_rx_off;
static char             rx_mask;
static char             flag_rx_ready;
static char             flag_tx_ready;
static char             timer_rx_ctr;
static char             timer_tx_ctr;
static char             bits_left_in_rx;
static char             bits_left_in_tx;
static char             rx_num_of_bits;
static char             tx_num_of_bits;
static unsigned int     internal_rx_buffer;
static unsigned int     internal_tx_buffer;
static char             user_tx_buffer;


#define TXUART_TRIS  TRISCbits.RC1
#define TXUART_LAT   LATCbits.LATC1
#define TXUART_PIN   PORTCbits.RC1

#define INIT_TXUART_PIN  TXUART_LAT=1; TXUART_TRIS=0

#define RXUART_TRIS  TRISCbits.RC0
#define RXUART_LAT   LATCbits.LATC0
#define RXUART_PIN   PORTCbits.RC0

#define INIT_RXUART_PIN  RXUART_LAT=0; RXUART_TRIS=1

//-----------------------------------------------------------------------------
static inline void set_tx_pin_high(void)
    {
    TXUART_LAT=1;
    }

//-----------------------------------------------------------------------------
static inline void set_tx_pin_low(void)
    {
    TXUART_LAT=0;
    }

//-----------------------------------------------------------------------------
static inline unsigned char get_rx_pin_status(void)
    {
    return RXUART_PIN;
    }

/*
//-----------------------------------------------------------------------------
static inline void idle(void)
    {

    }

//-----------------------------------------------------------------------------
static inline void timer_set(void)
    {

    }

//-----------------------------------------------------------------------------
static inline void set_timer_interrupt(void)
    {

    }
*/


inline void timer_isr(void)
    {
    char            mask, start_bit, flag_in;

// Transmitter Section
    if ( flag_tx_ready )
        {
        if ( --timer_tx_ctr == 0 )
            {
            mask = internal_tx_buffer&1;
            internal_tx_buffer >>= 1;
            if ( mask )
                {
                set_tx_pin_high();
                }
            else
                {
                set_tx_pin_low();
                }
            timer_tx_ctr = 3;
            if ( --bits_left_in_tx<=0 )
                {
                flag_tx_ready = uFALSE;
                }
            }
        }
// Receiver Section
    if ( flag_rx_off==uFALSE )
        {
        if ( flag_rx_waiting_for_stop_bit )
            {
            if ( --timer_rx_ctr ==0 )
                {
                flag_rx_waiting_for_stop_bit = uFALSE;
                flag_rx_ready = uFALSE;
                internal_rx_buffer &= 0xFF;
                //if ( internal_rx_buffer!=13 )
                    //{
                    inbuf[qin] = internal_rx_buffer;
                    if ( ++qin>=IN_BUF_SIZE )
                        {
                        qin = 0;
                        }
                    //}
                }
            }
        else        // rx_test_busy
            {
            if ( flag_rx_ready==uFALSE )
                {
                start_bit = get_rx_pin_status();
// Test for Start Bit
                if ( start_bit==0 )
                    {
                    flag_rx_ready = uTRUE;
                    internal_rx_buffer = 0;
                    timer_rx_ctr = 4;
                    bits_left_in_rx = rx_num_of_bits;
                    rx_mask = 1;
                    }
                }
            else    // rx_busy
                {
                if ( --timer_rx_ctr == 0 )
                    {                // rcv
                    timer_rx_ctr = 3;
                    flag_in = get_rx_pin_status();
                    if ( flag_in )
                        {
                        internal_rx_buffer |= rx_mask;
                        }
                    rx_mask <<= 1;
                    if ( --bits_left_in_rx == 0 )
                        {
                        flag_rx_waiting_for_stop_bit = uTRUE;
                        }
                    }
                }
            }
        }
    }

void init_uart( void )
    {
    flag_tx_ready = uFALSE;
    flag_rx_ready = uFALSE;
    flag_rx_waiting_for_stop_bit = uFALSE;
    flag_rx_off = uFALSE;
    rx_num_of_bits = 8;
    tx_num_of_bits = 10;

    INIT_TXUART_PIN;
    INIT_RXUART_PIN;

    set_tx_pin_low();

    //timer_set();
    //set_timer_interrupt();     // Enable timer interrupt
    }


//-----------------------------------------------------------------------------
uint8_t _getchar(void)
    {
    uint8_t ch;

    //do
        {
        while ( qout==qin )
            {
            //idle();
            }
        ch = inbuf[qout]; // & 0xFF; //xxx
        if ( ++qout>=IN_BUF_SIZE )
            {
            qout = 0;
            }
        }
    //while ( ch=='\r' );
    return( ch );
    }

void _putchar( char ch )
    {
    while ( flag_tx_ready );
    user_tx_buffer = ch;

// invoke_UART_transmit
    timer_tx_ctr = 3;
    bits_left_in_tx = tx_num_of_bits;
    internal_tx_buffer = (user_tx_buffer<<1) | 0x200;
    flag_tx_ready = uTRUE;
    }

void flush_input_buffer( void )
    {
    qin = 0;
    qout = 0;
    }
/*
char kbhit( void )
    {
    return( qin!=qout );
    }
*/
void turn_rx_on( void )
    {
    flag_rx_off = uFALSE;
    }

void turn_rx_off( void )
    {
    flag_rx_off = uTRUE;
    }
