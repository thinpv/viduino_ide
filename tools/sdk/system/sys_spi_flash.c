#include <stdint.h>
#include <types.h>
#include <string.h>
#include <io.h>
#include <stddef.h>
#include <printf.h>
#include "sys_spi_flash.h"

#define W25Q80BV_CMD_WRITE_ENABLE	0x06
#define W25Q80BV_CMD_WRITE_DISABLE	0x04
#define W25Q80BV_CMD_PAGE_PROG	0x02
#define W25Q80BV_CMD_READ_DATA	0x03
#define W25Q80BV_CMD_READ_STAT1	0x05
#define W25Q80BV_MASK_STAT_BUSY	(1<<0)
#define W25Q80BV_CMD_ERASE_4K	0x20
#define W25Q80BV_CMD_ERASE_32K	0x52
#define W25Q80BV_CMD_ERASE_64K	0xD8
#define W25Q80BV_CMD_CHIP_ERASE	0xC7 /* alternative 0x60 */
#define W25Q80BV_CMD_READ_MAN_DEV_ID	0x90
#define W25Q80BV_CMD_READ_UNIQUE_ID	0x4B
#define W25Q80BV_PAGE_SIZE	256
#define W25Q80BV_CAPACITY	(1L * 1024L * 1024L)

#define SPIFLASH_PAGE_SIZE	W25Q80BV_PAGE_SIZE

#define SPI_FLASH	SPI0

void sys_spi_flash_init(void)
{
	gpio_set_cfg(GPIOC, 0, 2);
	gpio_set_cfg(GPIOC, 1, 2);
	gpio_set_cfg(GPIOC, 2, 2);
	gpio_set_cfg(GPIOC, 3, 2);
	spi_init(SPI_FLASH);
}

void sys_spi_flash_exit(void)
{
	spi_exit(SPI_FLASH);
}

static uint8_t sys_spi_write_then_read(uint8_t* txbuf, uint32_t txlen, uint8_t* rxbuf, uint32_t rxlen)
{
	if(spi_transfer(SPI_FLASH, txbuf, NULL, txlen) != txlen)
		return -1;
	if(spi_transfer(SPI_FLASH, NULL, rxbuf, rxlen) != rxlen)
		return -1;
	return 0;
}

uint8_t sys_spi_flash_read(uint32_t addr, uint8_t* buf, uint32_t count)
{
	uint8_t tx[4];
	uint8_t result = 0;
	tx[0] = W25Q80BV_CMD_READ_DATA;
	tx[1] = (uint8_t)(addr >> 16);
	tx[2] = (uint8_t)(addr >> 8);
	tx[3] = (uint8_t)(addr >> 0);
	spi_select(SPI_FLASH);
	result = sys_spi_write_then_read(tx, 4, buf, count);
	spi_deselect(SPI_FLASH);
	return result;
}

uint8_t sys_spi_flash_enable_write()
{
	uint8_t write_enable = W25Q80BV_CMD_WRITE_ENABLE;
	spi_select(SPI_FLASH);
	int len_transfer = spi_transfer(SPI_FLASH, &write_enable, NULL, 1);
	if(len_transfer != 1) {
		printf("error sys_spi_flash_enable_write: %d\r\n", len_transfer);
		return 1;
	}
	spi_deselect(SPI_FLASH);
	return 0;
}

uint8_t sys_spi_flash_disable_write()
{
	uint8_t write_disable = W25Q80BV_CMD_WRITE_DISABLE;
	spi_select(SPI_FLASH);
	int len_transfer = spi_transfer(SPI_FLASH, &write_disable, NULL, 1);
	if(len_transfer != 1) {
		printf("error sys_spi_flash_enable_write: %d\r\n", len_transfer);
		return 1;
	}
	spi_deselect(SPI_FLASH);
	return 0;
}

uint8_t sys_spi_flash_is_busy()
{
	uint8_t read_status = W25Q80BV_CMD_READ_STAT1;
	uint8_t status = 1;
	spi_select(SPI_FLASH);
	// while(status & 1) {
		sys_spi_write_then_read(&read_status, 1, &status, 1);
	// }
	spi_deselect(SPI_FLASH);
	return status & 1;
}

static uint8_t sys_spi_flash_write_256(uint32_t addr, uint8_t* buf, uint16_t len)
{
	// printf("sys_spi_flash_write_256 addr: 0x%08X, len: %d\r\n", addr, len);
	uint8_t tx[4];
	tx[0] = W25Q80BV_CMD_PAGE_PROG;
	tx[1] = (uint8_t)(addr >> 16);
	tx[2] = (uint8_t)(addr >> 8);
	tx[3] = (uint8_t)(addr >> 0);
	sys_spi_flash_enable_write();

	spi_select(SPI_FLASH);
	int len_transfer = spi_transfer(SPI_FLASH, tx, NULL, 4);
	if(len_transfer != 4) {
		printf("error sys_spi_flash_write 1: %d\r\n", len_transfer);
		return 1;
	}
	len_transfer = spi_transfer(SPI_FLASH, buf, NULL, len);
	if(len_transfer != len) {
		printf("error sys_spi_flash_write 2: %d\r\n", len_transfer);
		return 1;
	}
	spi_deselect(SPI_FLASH);
	sys_spi_flash_disable_write();
	while(sys_spi_flash_is_busy());
	return 0;
}

uint8_t sys_spi_flash_write(uint32_t addr, uint8_t* buf, uint32_t len)
{
	// printf("sys_spi_flash_write addr: 0x%08X, len: %d\r\n", addr, len);
	uint16_t number_of_block_256 = len / 256;
	uint32_t data_retain = len;
	uint16_t data_in_block;
	uint16_t error = 0;
	if((len & 255) > 0)
		++number_of_block_256;
	for(uint16_t i=0; i<number_of_block_256; i++) {
		if(data_retain >= 256) {
			data_in_block = 256;
			data_retain -= 256;
		} else {
			data_in_block = data_retain;
		}
		error += sys_spi_flash_write_256(addr + 256*i, buf + 256*i, data_in_block);
	}
	return (error > 0);
}

u8 SPI_FLASH_BUF[4096];
uint8_t sys_spi_flash_erase_then_write(uint32_t addr, uint8_t *buf, uint32_t len)
{
	u32 secpos;
	u16 secoff;
	u16 secremain;
	u16 i;

	secpos = addr / 4096;			 //Sector address 0~511 for w25x16 ，4K a sector
	secoff = addr % 4096;			 //Offset within sector
	secremain = 4096 - secoff; //Sector remain size

	//Not more than 4096 bytes, get the remaining space when less than one sector
	if (len <= secremain)
		secremain = len;

	while (1)
	{
		//SPI_Flash_Read(SPI_FLASH_BUF,secpos*4096,4096);//读出整个扇区的内容
		sys_spi_flash_read(secpos * 4096, SPI_FLASH_BUF, 4096);
		for (i = 0; i < secremain; i++) //校验数据
		{
			if (SPI_FLASH_BUF[secoff + i] != 0XFF)
				break; //判断是否需要擦除
		}

		if (i < secremain) //需要擦除
		{
			// SPI_Flash_Erase_Sector(SPI,secpos*4096);//擦除这个扇区 W25Q128要x4096
			sys_spi_flash_erase_4k(secpos * 4096);
			for (i = 0; i < secremain; i++) //复制
			{
				SPI_FLASH_BUF[i + secoff] = buf[i]; //把数据复制到一个扇区的剩余空间，
			}
			sys_spi_flash_write(secpos * 4096, SPI_FLASH_BUF, 4096); //写入整个扇区 ，以便前面擦除的的数据能还原
		}
		else
			sys_spi_flash_write(addr, buf, secremain); //写已经擦除了的,直接写入扇区剩余区间.

		if (len == secremain)
			break; //End of writing
		else		 //写入未结束
		{
			secpos++;		//扇区地址增1
			secoff = 0; //第二次写入时，偏移位置为0

			buf += secremain;	 //指针偏移
			addr += secremain; //写地址偏移
			len -= secremain;	 //字节数递减
			if (len > 4096)
				secremain = 4096; //下一个扇区还是写不完
			else
				secremain = len; //下一个扇区可以写完了
		}
	}
	return 0;
}

uint8_t sys_spi_flash_erase_4k(uint32_t addr)
{
	// printf("sys_spi_flash_erase_4k addr: 0x%08X\r\n", addr);
	uint8_t tx[4];
	tx[0] = W25Q80BV_CMD_ERASE_4K;
	tx[1] = (uint8_t)(addr >> 16);
	tx[2] = (uint8_t)(addr >> 8);
	tx[3] = (uint8_t)(addr >> 0);
	sys_spi_flash_enable_write();
	spi_select(SPI_FLASH);
	int len_transfer = spi_transfer(SPI_FLASH, tx, NULL, 4);
	if(len_transfer != 4) {
		printf("error sys_spi_flash_write 1: %d\r\n", len_transfer);
		return 1;
	}
	spi_deselect(SPI_FLASH);
	sys_spi_flash_disable_write();
	while(sys_spi_flash_is_busy());
	return 0;
}

uint8_t sys_spi_flash_erase_32k(uint32_t addr)
{
	// printf("sys_spi_flash_erase_32k addr: 0x%08X\r\n", addr);
	uint8_t tx[4];
	tx[0] = W25Q80BV_CMD_ERASE_32K;
	tx[1] = (uint8_t)(addr >> 16);
	tx[2] = (uint8_t)(addr >> 8);
	tx[3] = (uint8_t)(addr >> 0);
	sys_spi_flash_enable_write();
	spi_select(SPI_FLASH);
	int len_transfer = spi_transfer(SPI_FLASH, tx, NULL, 4);
	if(len_transfer != 4) {
		printf("error sys_spi_flash_write 1: %d\r\n", len_transfer);
		return 1;
	}
	spi_deselect(SPI_FLASH);
	sys_spi_flash_disable_write();
	while(sys_spi_flash_is_busy());
	return 0;
}

uint8_t sys_spi_flash_erase_64k(uint32_t addr)
{
	// printf("sys_spi_flash_erase_64k addr: 0x%08X\r\n", addr);
	uint8_t tx[4];
	tx[0] = W25Q80BV_CMD_ERASE_64K;
	tx[1] = (uint8_t)(addr >> 16);
	tx[2] = (uint8_t)(addr >> 8);
	tx[3] = (uint8_t)(addr >> 0);
	sys_spi_flash_enable_write();
	spi_select(SPI_FLASH);
	int len_transfer = spi_transfer(SPI_FLASH, tx, NULL, 4);
	if(len_transfer != 4) {
		printf("error sys_spi_flash_write 1: %d\r\n", len_transfer);
		return 1;
	}
	spi_deselect(SPI_FLASH);
	sys_spi_flash_disable_write();
	while(sys_spi_flash_is_busy());
	return 0;
}

uint32_t sys_spi_flash_read_data_size()
{
	uint32_t size = 0;
	sys_spi_flash_read(FLASH_APP_LEN_START_ADDRESS, (uint8_t*)&size, 4);
	return size;
}