

/* Includes ------------------------------------------------------------------*/

#include "usb_memory.h"
#include "usb_scsi.h"
#include "usb_bot.h"
#include "usb.h"
#include "usb_Mass_storage.h"  
#include "string.h"

#include <spi.h>
#include <sdc.h>

#include <diskio.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile uint32_t Block_Read_count = 0;
volatile uint32_t Block_offset;
volatile uint32_t Counter = 0;
uint32_t MemoryType;

#define SDx SD0

uint32_t  Idx;
//不使用malloc的时候
//u32 Data_Buffer[BULK_MAX_PACKET_SIZE *2]; //不使用malloc

//使用malloc的时候
__attribute__ ((aligned(4))) u32 Data_Buffer[BULK_MAX_PACKET_SIZE];	//外部必须用malloc申请 BULK_MAX_PACKET_SIZE*2*4 这么多字节的内存。

uint8_t TransferState = TXFR_IDLE;

////////////////////////////自己定义的一个标记USB状态的寄存器///////////////////
//bit0:表示电脑正在向SD卡写入数据
//bit1:表示电脑正从SD卡读出数据
//bit2:SD卡写数据错误标志位
//bit3:SD卡读数据错误标志位
//bit4:1,表示电脑有轮询操作(表明连接还保持着)
u8 USB_STATUS_REG=0;
////////////////////////////////////////////////////////////////////////////////
 
/* Extern variables ----------------------------------------------------------*/
extern u8 *Bulk_Data_Buff;//在usb_bot.c里面申明了
//extern uint8_t Bulk_Data_Buff[BULK_MAX_PACKET_SIZE];  /* data buffer*/
extern uint16_t Data_Len;
extern uint8_t Bot_State;
extern Bulk_Only_CBW CBW;
extern Bulk_Only_CSW CSW; 

__attribute__ ((aligned(4))) u8 Virtual_Sd_Memory[MSC_DDR_Size]; //DRAM虚拟为U盘

//LED读写指示
void Led_RW_ON(void )
{

}
void Led_RW_OFF(void )
{

}




/* Private function prototypes -----------------------------------------------*/
/* Extern function prototypes ------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : Read_Memory
* Description    : Handle the Read operation from the microSD card.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
__attribute__ ((aligned(4))) u8 WR_Data_Buffer[128*512];
void Read_SD_Memory(uint8_t lun, uint32_t Memory_Offset, uint32_t Transfer_Length)
{
	static u32  Length;
	u8 STA; 
	if (TransferState == TXFR_IDLE )
	{
		Length = Transfer_Length;
		Block_Read_count=0;
		TransferState = TXFR_ONGOING;
	} 
	if (TransferState == TXFR_ONGOING )
	{
			if (!Block_Read_count)
			{
				//一次性读出
	 			// STA=SD_Read_in(SDx,Memory_Offset,Transfer_Length,(unsigned char *)WR_Data_Buffer);
	 			STA=sdc_read_in(SDC0,Memory_Offset,Transfer_Length,(unsigned char *)WR_Data_Buffer);		
				if(STA == FALSE)USB_STATUS_REG|=0X08;//SD卡读错误!
				//传输一次
				USB_SIL_Write(EP1_IN, (uint8_t *)WR_Data_Buffer, BULK_MAX_PACKET_SIZE);
				Length -= 1;
				Block_Read_count++;
				Block_offset = BULK_MAX_PACKET_SIZE;
			}
			else//传输剩余的 
			{
				USB_SIL_Write(EP1_IN, (uint8_t *)WR_Data_Buffer + Block_offset, BULK_MAX_PACKET_SIZE);
				Length -= 1;
				Block_Read_count++;
				Block_offset += BULK_MAX_PACKET_SIZE;
			} 	
			CSW.dDataResidue -= BULK_MAX_PACKET_SIZE;
			Led_RW_ON();
	}
	if (Length == 0)
	{
		Block_Read_count = 0;
		Block_offset = 0;
		Bot_State = BOT_DATA_IN_LAST;
		TransferState = TXFR_IDLE;
		Led_RW_OFF();
	}
}


void Read_DDR_Memory(uint8_t lun, uint32_t Memory_Offset, uint32_t Transfer_Length)
{
	static u32  Offset, Length;
	u8 STA; 
	if (TransferState == TXFR_IDLE )
	{
		Offset = Memory_Offset * Mass_Block_Size[lun];
		Length = Transfer_Length * Mass_Block_Size[lun];
		TransferState = TXFR_ONGOING;
	} 
	if (TransferState == TXFR_ONGOING )
	{

		memcpy(Data_Buffer,&Virtual_Sd_Memory[Offset],Mass_Block_Size[lun]);
		STA=0;			
		if(STA)USB_STATUS_REG|=0X08;//SD卡读错误!			
		USB_SIL_Write(EP1_IN, (uint8_t *)Data_Buffer, BULK_MAX_PACKET_SIZE);

		Offset += BULK_MAX_PACKET_SIZE;
		Length -= BULK_MAX_PACKET_SIZE;

		CSW.dDataResidue -= BULK_MAX_PACKET_SIZE;
		Led_RW_ON();
	}
	if (Length == 0)
	{
		Block_Read_count = 0;
		Block_offset = 0;
		Offset = 0;
		Bot_State = BOT_DATA_IN_LAST;
		TransferState = TXFR_IDLE;
		Led_RW_OFF();
	}
}
void Read_SPIFLASH_Memory(uint8_t lun, uint32_t Memory_Offset, uint32_t Transfer_Length)
{
	static u32  Length;
	u8 STA; 
	if (TransferState == TXFR_IDLE )
	{
		Length = Transfer_Length;
		Block_Read_count=0;
		TransferState = TXFR_ONGOING;
	} 
	if (TransferState == TXFR_ONGOING )
	{
			if (!Block_Read_count)
			{
				//一次性读出
				sys_spi_flash_read(FATFS_SPI_START_ADDR+(Memory_Offset* Mass_Block_Size[lun]),(unsigned char *)WR_Data_Buffer,(Transfer_Length* Mass_Block_Size[lun]));
	 			STA=0;		
				if(STA)USB_STATUS_REG|=0X08;//SD卡读错误!
				//传输一次
				USB_SIL_Write(EP1_IN, (uint8_t *)WR_Data_Buffer, BULK_MAX_PACKET_SIZE);
				Length -= 1;
				Block_Read_count++;
				Block_offset = BULK_MAX_PACKET_SIZE;
			}
			else//传输剩余的 
			{
				USB_SIL_Write(EP1_IN, (uint8_t *)WR_Data_Buffer + Block_offset, BULK_MAX_PACKET_SIZE);
				Length -= 1;
				Block_Read_count++;
				Block_offset += BULK_MAX_PACKET_SIZE;
			} 	
			CSW.dDataResidue -= BULK_MAX_PACKET_SIZE;
			Led_RW_ON();
	}
	if (Length == 0)
	{
		Block_Read_count = 0;
		Block_offset = 0;
		Bot_State = BOT_DATA_IN_LAST;
		TransferState = TXFR_IDLE;
		Led_RW_OFF();
	}
}

/*******************************************************************************
* Function Name  : Write_Memory
* Description    : Handle the Write operation to the microSD card.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Write_SD_Memory (uint8_t lun, uint32_t Memory_Offset, uint32_t Transfer_Length)
{ 
	static u32 W_Offset, W_Length, scount;
	u8 STA;   
	
	if (TransferState == TXFR_IDLE )
	{
		W_Offset = Memory_Offset;
		W_Length = Transfer_Length;
		scount=0;
		TransferState = TXFR_ONGOING;
	} 
	//传输Transfer_Length次数据
	if(TransferState == TXFR_ONGOING)
	{
		memcpy(((uint8_t *)WR_Data_Buffer + (scount*BULK_MAX_PACKET_SIZE)),Bulk_Data_Buff,BULK_MAX_PACKET_SIZE);
		scount++; 
		CSW.dDataResidue -= Data_Len;  
		Led_RW_ON();
	}
	//传输完成，存储
	if ((scount>=W_Length) || (Bot_State == BOT_CSW_Send))
	{
		// STA=SD_Write_out(SDx,W_Offset,W_Length,(unsigned char *)WR_Data_Buffer);
		STA=sdc_write_out(SDC0,W_Offset,W_Length,(unsigned char *)WR_Data_Buffer);
		if(STA == FALSE)USB_STATUS_REG|=0X04;//SD卡写错误!		
    scount=0;
		Set_CSW (CSW_CMD_PASSED, SEND_CSW_ENABLE);
		TransferState = TXFR_IDLE;
		Led_RW_OFF();
	}
}

void Write_DDR_Memory (uint8_t lun, uint32_t Memory_Offset, uint32_t Transfer_Length)
{ 
	static u32 W_Offset, W_Length;
	u8 STA;   

	uint32_t temp =  Counter + 64;

	if (TransferState == TXFR_IDLE )
	{
		W_Offset = Memory_Offset * Mass_Block_Size[lun];
		W_Length = Transfer_Length * Mass_Block_Size[lun];
		TransferState = TXFR_ONGOING;
	} 
	if (TransferState == TXFR_ONGOING )
	{
		W_Offset += Data_Len;
		W_Length -= Data_Len; 
		//不同读写类型
		memcpy(&Virtual_Sd_Memory[W_Offset - Mass_Block_Size[lun]],Bulk_Data_Buff,Mass_Block_Size[lun]);
		STA=0;
		if(STA)USB_STATUS_REG|=0X04;//SD卡写错误!	

		CSW.dDataResidue -= Data_Len;
		Led_RW_ON();
	}

	if ((W_Length == 0) || (Bot_State == BOT_CSW_Send))
	{
		Counter = 0;
		Set_CSW (CSW_CMD_PASSED, SEND_CSW_ENABLE);
		TransferState = TXFR_IDLE;
		Led_RW_OFF();
	}
}

void Write_SPIFLASH_Memory (uint8_t lun, uint32_t Memory_Offset, uint32_t Transfer_Length)
{ 
	static u32 W_Offset, W_Length, scount;
	u8 STA;   
	
	if (TransferState == TXFR_IDLE )
	{
		W_Offset = Memory_Offset;
		W_Length = Transfer_Length;
		scount=0;
		TransferState = TXFR_ONGOING;
	} 
	//传输Transfer_Length次数据
	if(TransferState == TXFR_ONGOING)
	{
		memcpy(((uint8_t *)WR_Data_Buffer + (scount*BULK_MAX_PACKET_SIZE)),Bulk_Data_Buff,BULK_MAX_PACKET_SIZE);
		scount++; 
		CSW.dDataResidue -= Data_Len;  
		Led_RW_ON();
	}
	//传输完成，存储
	if ((scount>=W_Length) || (Bot_State == BOT_CSW_Send))
	{
		// SPI_Flash_Write(SPIx,(unsigned char *)WR_Data_Buffer,FATFS_SPI_START_ADDR+(W_Offset* Mass_Block_Size[lun]),W_Length* Mass_Block_Size[lun]);
		sys_spi_flash_erase_then_write(FATFS_SPI_START_ADDR+(W_Offset* Mass_Block_Size[lun]),(unsigned char *)WR_Data_Buffer,W_Length* Mass_Block_Size[lun]);
		STA=0;
		if(STA)USB_STATUS_REG|=0X04;//SD卡写错误!		
    scount=0;
		Set_CSW (CSW_CMD_PASSED, SEND_CSW_ENABLE);
		TransferState = TXFR_IDLE;
		Led_RW_OFF();
	}
}


/*不同存储器读*/
void Read_Memory(uint8_t lun, uint32_t Memory_Offset, uint32_t Transfer_Length)
{
	if(MemoryType==MemoryTypeSD)
		Read_SD_Memory(lun,Memory_Offset,Transfer_Length);
	else if(MemoryType==MemoryTypeDDR)	
		Read_DDR_Memory(lun,Memory_Offset,Transfer_Length);
	else if(MemoryType==MemoryTypeSPIFLASH)	
		Read_SPIFLASH_Memory(lun,Memory_Offset,Transfer_Length);	
}
/*不同存储器写*/
void Write_Memory (uint8_t lun, uint32_t Memory_Offset, uint32_t Transfer_Length)
{
	if(MemoryType==MemoryTypeSD)
		Write_SD_Memory(lun,Memory_Offset,Transfer_Length);
	else if(MemoryType==MemoryTypeDDR)	
		Write_DDR_Memory(lun,Memory_Offset,Transfer_Length);
	else if(MemoryType==MemoryTypeSPIFLASH)	
		Write_SPIFLASH_Memory(lun,Memory_Offset,Transfer_Length);
}

/*存储器初始化*/
void Memory_init(void)
{
	Mass_Block_Size[0] =512;//块大小	
	if(MemoryType==MemoryTypeSD)
		Mass_Memory_Size[0]=(uint64_t)sd_info.totalSectorN*(uint64_t)Mass_Block_Size[0];//内存大小
	else if(MemoryType==MemoryTypeDDR)
		Mass_Memory_Size[0]=MSC_DDR_Size;
	else if(MemoryType==MemoryTypeSPIFLASH)
		Mass_Memory_Size[0]=FATFS_SPI_SIZE;
	
	Mass_Block_Count[0]=(u32)((uint64_t)Mass_Memory_Size[0]/(uint64_t)Mass_Block_Size[0]);//块计数	
	
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

