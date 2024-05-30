#ifndef FLASH_H
#define FLASH_H

#include <xc.h>
#include <stdint.h>


#define _XTAL_FREQ 40000000



#define SO_PIN   PORTDbits.RD6
#define SO_TRIS  TRISDbits.RD6
#define SO_LAT   LATDbits.LATD6

#define INIT_SO_PIN  SO_LAT=0; SO_TRIS=1
#define READ_SO_PIN  !SO_PIN


#define SI_TRIS  TRISBbits.RB5
#define SI_LAT   LATBbits.LATB5

#define INIT_SI_PIN  SI_LAT=0; SI_TRIS=0
#define SET_SI_PIN  SI_LAT=1
#define CLR_SI_PIN  SI_LAT=0


#define SCK_TRIS  TRISCbits.RC4
#define SCK_LAT   LATCbits.LATC4

#define INIT_SCK_PIN  SCK_LAT=0; SCK_TRIS=0
#define SET_SCK_PIN  SCK_LAT=1
#define CLR_SCK_PIN  SCK_LAT=0


#define CS_TRIS  TRISCbits.RC3
#define CS_LAT   LATCbits.LATC3

#define INIT_CS_PIN  CS_LAT=1; CS_TRIS=0
#define SET_CS_PIN  CS_LAT=1
#define CLR_CS_PIN  CS_LAT=0


#define INIT_SPI_PINS  INIT_CS_PIN; INIT_SO_PIN; INIT_SI_PIN; INIT_SCK_PIN



#define STATUS_BIT_WRITE_IN_PROGRESS  0b0000001

#define INSTR_READ_STATUS  0x05
#define INSTR_ID_READ_JEDEC  0x9f

#define INSTR_WRITE_ENABLE  0x06
#define INSTR_WRITE_DISABLE  0x04
#define INSTR_DATA_READ  0x03

#define INSTR_SECTOR_ERASE  0xd7
#define INSTR_BLOCK_ERASE  0xd8
#define INSTR_CHIP_ERASE  0xc7
#define INSTR_PAGE_PROGRAM  0x02

#define SPI_FLASH_PAGE_SIZE  256



uint8_t flashbuff[SPI_FLASH_PAGE_SIZE*3]={0x00};



void flash_init(void);

void spi_flash_write(uint8_t);
uint8_t spi_flash_read(void);

uint8_t spi_flash_read_status(void);

void spi_flash_write_enable(void);
void spi_flash_write_disable(void);

uint8_t spi_flash_data_read(uint32_t);
void spi_flash_chip_erase(void);

void spi_flash_page_program (uint32_t, uint8_t *);

void spi_flash_sector_erase(uint32_t address);
void spi_flash_block_erase(uint32_t address);


#endif //FLASH_H