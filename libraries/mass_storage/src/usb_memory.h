
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __memory_H
#define __memory_H
#include "usb.h" 
#include "usb_desc.h" 

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define TXFR_IDLE     0
#define TXFR_ONGOING  1
////////////////////////////自己定义的一个标记USB状态的寄存器///////////////////
//bit0:表示电脑正在向SD卡写入数据
//bit1:表示电脑正从SD卡读出数据
//bit2:SD卡写数据错误标志位
//bit3:SD卡读数据错误标志位
//bit4:1,表示电脑有轮询操作(表明连接还保持着) 
extern u8 USB_STATUS_REG;

//使用内存管理的时候

extern u8* Bulk_Data_Buff;//在usb_bot里面定义
#define BULK_MAX_PACKET_SIZE  DATA_PACKET_SIZE	//最大包大小
extern u32 Data_Buffer[BULK_MAX_PACKET_SIZE];

extern int SPIx;
// extern int SPI_FLASH_FAT_START_ADD;//spi开始地址
extern int SPI_FLASH_FAT_Size;//spi大小
#define MSC_DDR_Size     	(4*1024*1024)//定义文件系统DDR RAM大小


extern u8 Virtual_Sd_Memory[MSC_DDR_Size]; //DRAM虚拟为U盘

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Write_Memory (uint8_t lun, uint32_t Memory_Offset, uint32_t Transfer_Length);
void Read_Memory (uint8_t lun, uint32_t Memory_Offset, uint32_t Transfer_Length);
void Memory_init(void);

#ifdef __cplusplus
}
#endif

#endif /* __memory_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
