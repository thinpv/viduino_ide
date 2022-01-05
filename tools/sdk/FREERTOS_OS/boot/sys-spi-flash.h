#ifndef __SYS_SPI_FLASH_H_
#define __SYS_SPI_FLASH_H_

#include <stdint.h>
#include <types.h>
#include <string.h>
#include <io.h>
#include <stddef.h>

#include <spi.h>

#define SIZE_4K                     0x1000
#define SIZE_64K                    0x10000

#define FLASH_APP_LEN_START_ADDRESS (0x00200000 - SIZE_4K)
#define FLASH_APP_START_ADDRESS     0x00200000

void sys_spi_flash_init(void);
void sys_spi_flash_exit(void);
uint8_t sys_spi_flash_read(uint32_t addr, uint8_t* buf, uint32_t count);
uint8_t sys_spi_flash_enable_write();
uint8_t sys_spi_flash_disable_write();
uint8_t sys_spi_flash_is_busy();
uint8_t sys_spi_flash_write(uint32_t addr, uint8_t* buf, uint32_t count);
uint8_t sys_spi_flash_erase_then_write(uint32_t addr, uint8_t *buf, uint32_t len);
uint8_t sys_spi_flash_erase_4k(uint32_t addr);
uint8_t sys_spi_flash_erase_32k(uint32_t addr);
uint8_t sys_spi_flash_erase_64k(uint32_t addr);
uint32_t sys_spi_flash_read_data_size();

#endif
