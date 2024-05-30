
#include "flash.h"


//-----------------------------------------------------------------------------
void flash_init(void)
    {
    INIT_SPI_PINS;
    }


//-----------------------------------------------------------------------------
void spi_flash_write(uint8_t data)
    {
    uint8_t mask = 0b10000000;

    for(uint8_t k=0; k<8; k++)
        {
        if(data & mask) { SET_SI_PIN; }
        else { CLR_SI_PIN; }
        SET_SCK_PIN;
        __delay_us(5);
        CLR_SCK_PIN;
        __delay_us(5);
        mask>>=1;
        }

    CLR_SI_PIN;
    }


//-----------------------------------------------------------------------------
uint8_t spi_flash_read(void)
    {
    uint8_t data=0;
    uint8_t mask = 0b10000000;

    for(uint8_t k=0; k<8; k++)
        {
        if(READ_SO_PIN) { data|=mask; }
        SET_SCK_PIN;
        __delay_us(5);
        CLR_SCK_PIN;
        __delay_us(5);
        mask>>=1;
        }

    return data;
    }


//-----------------------------------------------------------------------------
uint8_t spi_flash_read_status(void)
    {
    uint8_t data=0;

    CLR_CS_PIN;
    spi_flash_write(INSTR_READ_STATUS);
    data = spi_flash_read();
    SET_CS_PIN;

    return data;
    }


//-----------------------------------------------------------------------------
void spi_flash_write_enable(void)
    {
    CLR_CS_PIN;
    spi_flash_write(INSTR_WRITE_ENABLE);
    SET_CS_PIN;
    }


//-----------------------------------------------------------------------------
void spi_flash_write_disable(void)
    {
    CLR_CS_PIN;
    spi_flash_write(INSTR_WRITE_DISABLE);
    SET_CS_PIN;
    }


uint8_t spi_flash_data_read(uint32_t address)
    {
    uint8_t data=0;

    CLR_CS_PIN;
    spi_flash_write(INSTR_DATA_READ);
    spi_flash_write(address>>16);
    spi_flash_write(address>>8);
    spi_flash_write(address);
    data = spi_flash_read();
    SET_CS_PIN;

    return data;
    }


void spi_flash_chip_erase(void)
    {
    CLR_CS_PIN;
    spi_flash_write(INSTR_CHIP_ERASE);
    SET_CS_PIN;
    }


void spi_flash_sector_erase(uint32_t address)
    {
    CLR_CS_PIN;
    spi_flash_write(address>>16);
    spi_flash_write(address>>8);
    spi_flash_write(address);
    spi_flash_write(INSTR_SECTOR_ERASE);
    SET_CS_PIN;
    }


void spi_flash_block_erase(uint32_t address)
    {
    CLR_CS_PIN;
    spi_flash_write(address>>16);
    spi_flash_write(address>>8);
    spi_flash_write(address);
    spi_flash_write(INSTR_BLOCK_ERASE);
    SET_CS_PIN;
    }


void spi_flash_page_program(uint32_t address, uint8_t *data)
    {
    CLR_CS_PIN;
    spi_flash_write(INSTR_PAGE_PROGRAM);

    spi_flash_write(address>>16);
    spi_flash_write(address>>8);
    spi_flash_write(address);

    for(uint16_t c=0; c<SPI_FLASH_PAGE_SIZE; c++)
        {
        spi_flash_write(data[c]);
        }

    SET_CS_PIN;
    }
