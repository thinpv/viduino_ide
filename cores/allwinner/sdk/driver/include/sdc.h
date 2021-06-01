#ifndef __SDC_H__
#define __SDC_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <types.h>
#include <F1C100S.h>

	struct sdhci_cmd_t
	{
		u16_t cmdidx;
		u32_t cmdarg;
		u32_t resptype;
		u32_t response[4];
	};

	typedef struct sdhci_cmd_t mmc_cmd_st;

	struct sdhci_data_t
	{
		u8_t *buf;
		u32_t flag;
		u32_t blksz;
		u32_t blkcnt;
	};

	typedef struct sdhci_data_t mmc_data_st;

#define STOR_STRING_LEN 32
	typedef struct disk_data
	{
		struct disk_data *next; /* next device */

		/* information about the device -- always good */
		unsigned int totalSectorN;
		unsigned int diskSize; /* disk size in Kbytes */
		int sectorSize;
		char vendor[STOR_STRING_LEN];
		char product[STOR_STRING_LEN];
		char serial[STOR_STRING_LEN];
	} disk_data_t;
	extern disk_data_t sd_info;

	int sdc_init(SDC_Type *sdc);
	int sdc_read_in(SDC_Type *sdc, unsigned int sector, unsigned int count, unsigned char *buff);
	int sdc_write_out(SDC_Type *sdc, unsigned int sector, unsigned int count, unsigned char *buff);

#ifdef __cplusplus
}
#endif

#endif /* __SDC_H__ */
