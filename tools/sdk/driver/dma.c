#include <dma.h>
#include <ccu.h>
#include <io.h>

#define DEBUG( ... ) // printf(##__VA_ARGS__)

void dma_init()
{
	ccu_reset(RESET_DMA, true);
}

void dma_exit()
{
	ccu_reset(RESET_DMA, false);
}

void dma_ndma_enable(uint8_t ndma_index)
{
	NDMA_Type *nDMA = &(DMA->nDMA[ndma_index]);
	S_Bit(nDMA->NDMA_CFG_REG, 31);
}

void dma_ndma_disable(uint8_t ndma_index)
{
	NDMA_Type *nDMA = &(DMA->nDMA[ndma_index]);
	C_Bit(nDMA->NDMA_CFG_REG, 31);
}

void dma_ndma_set(uint8_t ndma_index, NDMA_Conf ndma_conf, uint32_t *src, uint32_t *des, uint32_t byte_count)
{
	NDMA_Type *nDMA = &(DMA->nDMA[ndma_index]);

	//In automatic mode, [Auto Clock] must be set to 0, otherwise two clocks will transmit one data in the past
	C_Bit(DMA->DMA_PTY_CFG_REG, 16);
	DMA->DMA_PTY_CFG_REG = 0;

	//Set source address
	nDMA->NDMA_SRC_ADR_REG = src;
	//Set destination address
	nDMA->NDMA_DES_ADR_REG = des;
	//Set up BCNT
	nDMA->NDMA_BYTE_CNT_REG = byte_count;

	nDMA->NDMA_CFG_REG = *(uint32_t *)&ndma_conf;

	//Set DMA load enable
	S_Bit(nDMA->NDMA_CFG_REG, 31);
}

uint8_t dma_ndma_get_half_tip(uint8_t ndma_index)
{
	if (R_Bit(DMA->DMA_INT_STA_REG, 2 * ndma_index))
	{
		S_Bit(DMA->DMA_INT_STA_REG, 2 * ndma_index);
		return 1;
	}
	return 0;
}

uint8_t dma_ndma_get_full_tip(uint8_t ndma_index)
{
	if (R_Bit(DMA->DMA_INT_STA_REG, 2 * ndma_index + 1))
	{
		S_Bit(DMA->DMA_INT_STA_REG, 2 * ndma_index + 1);
		return 1;
	}
	return 0;
}
