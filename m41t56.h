// m41t56.h
//
// RTC M41T56
//
// cat 24-dec-2019
//


#ifndef M41T56_H_
#define M41T56_H_



#include <stdint.h>



//M41T56 I2C address
#define M41T56_ADDR_WRITE 0b11010000
#define M41T56_ADDR_READ  0b11010001


//M41T56 Register map
#define SECONDS_REG  0
#define MINUTES_REG  1
#define HOURS_REG    2
#define DAY_REG      3
#define DATE_REG     4
#define MONTH_REG    5
#define YEAR_REG     6
#define CONTROL_REG  7


#define M41T56_RAM_OFFSET  0x08
#define M41T56_RAM_MAX     56

#define M41T56_BUFF_SIZE   8



void rtc_init(void);
void rtc_read(uint8_t *data);

void m41t56_write_reg(uint8_t addr, uint8_t val);
uint8_t m41t56_read_reg(uint8_t addr);

void rtc_set_sec(uint8_t val);
void rtc_set_min(uint8_t val);
void rtc_set_hrs(uint8_t val);
void rtc_set_day(uint8_t val);
void rtc_set_dat(uint8_t val);
void rtc_set_mon(uint8_t val);
void rtc_set_year(uint8_t val);



#endif //M41T56_H_


