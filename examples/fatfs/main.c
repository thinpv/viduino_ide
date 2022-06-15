#include <sdc.h>
#define DATA_SIZE 2048
#include "diskio.h"
#include "ff.h"
#include <sys_print.h>

u8 work[FF_MAX_SS];
int i;
UINT br;
u8 buff[100] = {0};

void setup()
{
	sys_print_init();

// sdc_init(SDC0);
	FATFS sdfs;
	DIR dp1;
	FIL fp1;
	if (f_mount(&sdfs, "1:", 1) == FR_OK)
	{
		printf("sdcard file system open successfully!!!\r\n");
		f_setlabel("1:F1C100S-SD");

		if (f_opendir(&dp1, "1:/test") == FR_NO_PATH)
		{
			f_closedir(&dp1);
			printf("No file path, create..\r\n");
			if (f_mkdir("1:/test") == 0)
			{
				printf("Path created successfully...\r\n");
			}
		}
		else
		{
			printf("Path exists...\r\n");
		}

		i = f_open(&fp1, "1:/test/2018.txt", FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
		if (i == 5)
			printf("Folder path does not exist..\r\n");
		else if (i == 4)
		{
			printf("No file,..\r\n");
			i = f_open(&fp1, "1:/test/2018.txt", FA_CREATE_NEW | FA_WRITE | FA_READ);
		}
		else if (i == 0)
		{
			printf("File opened successfully..\r\n");
			f_lseek(&fp1, 0);
			if (f_read(&fp1, (uint8_t *)buff, 20, &br) == 0)
			{
				buff[20] = '\0';
				printf("buff: %s\r\n", buff);
			}
			printf("------------------------------\r\n");
			f_close(&fp1);
		}
		else
		{
			printf("ERR: f_open: %d\r\n", i);
		}
	}
	else
	{
		printf("ERR:not file system Files!!!\r\n");
		printf("Formatting...\r\n");
		f_mkfs("1:", FM_FAT, 0, work, sizeof work); //need to format
		printf("Format successful\r\n");
	}
	f_setlabel("1:F1C100S-SD");
}

void loop()
{
  
}