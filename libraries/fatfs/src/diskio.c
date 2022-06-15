
#include "diskio.h" /* FatFs lower layer API */
// #include "sys_sd.h"
#include <stdlib.h>
#include <string.h>
#include <sys_print.h>
// #include "usb_Mass_storage.h"
#include <sys_spi_flash.h>
#include <sdc.h>

#define SECTOR_SIZE 512
// #define SDx SD0
// int SPIx=SPI0;
// int SPI_FLASH_FAT_START_ADD=8*1024*1024;//spi开始地址
// int FATFS_SPI_SIZE = 4 * 1024 * 1024;

/*-------------------------------------------------------------------------------------------*/
/* Inidialize a Drive                                                                        */
/*-------------------------------------------------------------------------------------------*/
DSTATUS disk_initialize(
		BYTE pdrv /* Physical drive nmuber (0..) */
)
{
	// printf("disk_initialize: %d\r\n", pdrv);
	DSTATUS stat = STA_NOINIT;

	switch (pdrv)
	{
	case FS_SD:
		if (sdc_init(SDC0))
		{
			stat = RES_OK;
		}
		else
		{
			stat = STA_NODISK;
		}
		break;

	case FS_NAND: /* NAND Flash */

		break;

	case FS_USB: /* STM32 USB Host 口外接U盘 */

		break;

	case FS_SPI: /* SPI Flash */
		sys_spi_flash_init();
		stat = 0;
		break;

	case FS_NOR: /* NOR Flash */

		break;

	default:
		break;
	}

	return stat;
}

/*-------------------------------------------------------------------------------------------*/
/* Get Disk Status                                                                           */
/*-------------------------------------------------------------------------------------------*/
DSTATUS disk_status(
		BYTE pdrv /* Physical drive nmuber (0..) */
)
{
	// printf("disk_status: %d\r\n", pdrv);
	DSTATUS stat = STA_NOINIT;

	switch (pdrv)
	{
	case FS_SD:
		stat = 0;
		break;

	case FS_NAND:
		stat = 0;
		break;

	case FS_USB:
		stat = 0;
		break;

	case FS_SPI: /* SPI Flash */
		stat = 0;
		break;

	case FS_NOR: /* NOR Flash */
		stat = 0;
		break;

	default:
		stat = 0;
		break;
	}
	return stat;
}

/*-------------------------------------------------------------------------------------------*/
/* Read Sector(s)                                                                            */
/*-------------------------------------------------------------------------------------------*/
DRESULT disk_read(
		BYTE pdrv,		/* Physical drive nmuber (0..) */
		BYTE *buff,		/* Data buffer to store read data */
		DWORD sector, /* Sector address (LBA) */
		UINT count		/* Number of sectors to read (1..128) */
)
{
	// printf("disk_read: %d, sector: %d, count: %d\r\n", pdrv, sector, count);
	DRESULT res = RES_ERROR;
	unsigned char *b;
	switch (pdrv)
	{
	case FS_SD:
		if ((unsigned int)(long)buff & 0x3)
		{
			b = (unsigned char *)malloc(count * SECTOR_SIZE);
			if (b != NULL)
			{
				if (sdc_read_in(SDC0, sector, count, (unsigned char *)b))
					res = RES_OK;
				else
					res = RES_ERROR;
				memcpy(buff, b, count * SECTOR_SIZE);
				free(b);
			}
			else
			{
				printf("SD malloc ERR...\r\n");
			}
		}
		else
		{
			if (sdc_read_in(SDC0, sector, count, (unsigned char *)buff))
				res = RES_OK;
			else
				res = RES_ERROR;
		}
		break;

	case FS_NAND:

		break;

	case FS_USB:

		break;

	case FS_SPI: /* SPI Flash */
		if (sys_spi_flash_read(FATFS_SPI_START_ADDR + (sector * SECTOR_SIZE), buff, count * SECTOR_SIZE) == 0)
			res = RES_OK;
		break;

	case FS_NOR: /* NOR Flash */

		break;

	default:

		break;
	}
	return res;
}

/*-------------------------------------------------------------------------------------------*/
/* Write Sector(s)                                                                           */
/*-------------------------------------------------------------------------------------------*/

DRESULT disk_write(
		BYTE pdrv,				/* Physical drive nmuber (0..) */
		const BYTE *buff, /* Data to be written */
		DWORD sector,			/* Sector address (LBA) */
		UINT count				/* Number of sectors to write (1..128) */
)
{
	// printf("disk_write: %d, sector: %d, count: %d\r\n", pdrv, sector, count);
	DRESULT res = RES_ERROR;
	unsigned char *b;
	switch (pdrv)
	{
	case FS_SD:
		if ((unsigned int)(long)buff & 0x3)
		{
			b = (unsigned char *)malloc(count * SECTOR_SIZE);
			if (b != NULL)
			{
				memcpy(b, buff, count * SECTOR_SIZE);
				if (sdc_write_out(SDC0, sector, count, (unsigned char *)b))
					res = RES_OK;
				else
					res = RES_ERROR;
				free(b);
			}
			else
			{
				printf("SD malloc ERR...\r\n");
			}
		}
		else
		{
			if (sdc_write_out(SDC0, sector, count, (unsigned char *)buff))
				res = RES_OK;
			else
				res = RES_ERROR;
		}
		break;

	case FS_NAND:

		break;

	case FS_USB:

		break;

	case FS_SPI: /* SPI Flash */
		if (sys_spi_flash_erase_then_write(FATFS_SPI_START_ADDR + (sector * SECTOR_SIZE), buff, count * SECTOR_SIZE) == 0)
			res = RES_OK;
		break;

	case FS_NOR: /* NOR Flash */

		break;

	default:
		res = RES_PARERR;
		break;
	}
	return res;
}

/*-------------------------------------------------------------------------------------------*/
/* Miscellaneous Functions                                                                   */
/*-------------------------------------------------------------------------------------------*/

DRESULT disk_ioctl(
		BYTE pdrv, /* Physical drive nmuber (0..) */
		BYTE cmd,	 /* Control code */
		void *buff /* Buffer to send/receive control data */
)
{
	// printf("disk_ioctl: %d\r\n", pdrv);
	DRESULT res;

	switch (pdrv)
	{
	case FS_SD:
	{
		res = RES_ERROR;
		switch (cmd)
		{
		case CTRL_SYNC: /* Make sure that no pending write process */
			res = RES_OK;
			break;

		case GET_SECTOR_COUNT: /* Get number of sectors on the disk (DWORD) */
			*(DWORD *)buff = sd_info.totalSectorN;
			res = RES_OK;
			break;

		case GET_SECTOR_SIZE: /* Get R/W sector size (WORD) */
			*(WORD *)buff = 512;
			res = RES_OK;
			break;

		case GET_BLOCK_SIZE: /* Get erase block size in unit of sector (DWORD) */
			*(DWORD *)buff = 512;
			res = RES_OK;
			break;

		default:
			res = RES_PARERR;
			break;
		}
		return res;
	}
	break;
	case FS_NAND:
		break;
	case FS_USB:
		break;
	case FS_SPI: /* SPI Flash */
	{
		res = RES_ERROR;
		switch (cmd)
		{
		case CTRL_SYNC: /* Make sure that no pending write process */
			res = RES_OK;
			break;

		case GET_SECTOR_COUNT: /* Get number of sectors on the disk (DWORD) */
			*(DWORD *)buff = FATFS_SPI_SIZE / 512;
			res = RES_OK;
			break;

		case GET_SECTOR_SIZE: /* Get R/W sector size (WORD) */
			*(WORD *)buff = 512;
			res = RES_OK;
			break;

		case GET_BLOCK_SIZE: /* Get erase block size in unit of sector (DWORD) */
			*(DWORD *)buff = 512;
			res = RES_OK;
			break;

		default:
			res = RES_PARERR;
			break;
		}
		return res;
	}
	break;

	case FS_NOR: /* NOR Flash */

		break;
	}

	return RES_PARERR;
}

/*
************************************************** ************************************************** *****
* Function name: get_fattime
* Function description: Obtain the system time, which is used to rewrite the creation and modification time of the file.
* Formal parameters: none
* Return value: None
************************************************** ************************************************** *****
*/

DWORD get_fattime(void)
{
	int T_Year = 2018;
	int T_Month = 8;
	int T_Date = 4;
	int T_Hours = 22;
	int T_Minutes = 48;
	int T_Seconds = 59;

	return ((DWORD)(T_Year - 1980) << 25) /* Year  */
				 | ((DWORD)T_Month << 21)				/* Month */
				 | ((DWORD)T_Date << 16)				/* Day_m */
				 | ((DWORD)T_Hours << 11)				/* Hour  */
				 | ((DWORD)T_Minutes << 5)			/* Min   */
				 | ((DWORD)T_Seconds >> 1);			/* Sec   */
}
