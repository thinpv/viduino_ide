#ifndef __DMA_H__
#define __DMA_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <types.h>
#include <F1C100S.h>

	typedef enum
	{
		NDMA_SRC_TYPE_IR_Rx = 0x00,
		NDMA_SRC_TYPE_SPI0_Rx = 0x04,
		NDMA_SRC_TYPE_SPI1_Rx = 0x05,
		NDMA_SRC_TYPE_UART0_Rx = 0x08,
		NDMA_SRC_TYPE_UART1_Rx = 0x09,
		NDMA_SRC_TYPE_UART2_Rx = 0x0A,
		NDMA_SRC_TYPE_AUDIO_CODEC = 0x0C,
		NDMA_SRC_TYPE_TP_ADC = 0x0D,
		NDMA_SRC_TYPE_DAUDIO = 0x0E,
		NDMA_SRC_TYPE_SRAM = 0x10,
		NDMA_SRC_TYPE_SDRAM = 0x11,
		NDMA_SRC_TYPE_USB = 0x14,
		NDMA_SRC_TYPE_USB_EP1 = 0x15,
		NDMA_SRC_TYPE_USB_EP2 = 0x16,
		NDMA_SRC_TYPE_USB_EP3 = 0x17
	} NDMA_src_type_t;

	typedef enum
	{
		NDMA_DES_TYPE_OWA_Tx = 0x01,
		NDMA_DES_TYPE_SPI0_Tx = 0x04,
		NDMA_DES_TYPE_SPI1_Tx = 0x05,
		NDMA_DES_TYPE_UART0_Tx = 0x08,
		NDMA_DES_TYPE_UART1_Tx = 0x09,
		NDMA_DES_TYPE_UART2_Tx = 0x0A,
		NDMA_DES_TYPE_AUDIO_CODEC = 0x0C,
		NDMA_DES_TYPE_DAUDIO = 0x0E,
		NDMA_DES_TYPE_SRAM = 0x10,
		NDMA_DES_TYPE_SDRAM = 0x11,
		NDMA_DES_TYPE_USB = 0x14,
		NDMA_DES_TYPE_USB_EP1 = 0x15,
		NDMA_DES_TYPE_USB_EP2 = 0x16,
		NDMA_DES_TYPE_USB_EP3 = 0x17
	} NDMA_des_type_t;

	typedef enum
	{
		NDMA_ADDR_MODE_LINEAR = 0x00,
		NDMA_ADDR_MODE_IO = 0x01
	} NDMA_addr_mode_t;

	typedef enum
	{
		NDMA_BURST_LENTH_1 = 0x00,
		NDMA_BURST_LENTH_4 = 0x01
	} NDMA_burst_length_t;

	typedef enum
	{
		NDMA_DATA_WIDTH_8 = 0x00,
		NDMA_DATA_WIDTH_16 = 0x01,
		NDMA_DATA_WIDTH_32 = 0x10
	} NDMA_data_width_t;

	typedef struct
	{
		NDMA_src_type_t src_type : 5;
		NDMA_addr_mode_t src_addr_mode : 2;
		NDMA_burst_length_t src_burst_length : 1;
		NDMA_data_width_t src_data_width : 2;
		uint8_t rsv : 5;
		uint8_t remain_byte_read_enable : 1;
		NDMA_des_type_t des_type : 5;
		NDMA_addr_mode_t des_addr_mode : 2;
		NDMA_burst_length_t des_burst_length : 1;
		NDMA_data_width_t des_data_width : 2;
		uint8_t dma_wait_state : 3;
		uint8_t dma_continuous_mode_enable : 1;
		uint8_t dma_busy_status : 1;
		uint8_t dma_loading : 1;
	} NDMA_Conf;

	void dma_init();
	void dma_exit();
	void dma_ndma_enable(uint8_t ndma_index);
	void dma_ndma_disable(uint8_t ndma_index);
	void dma_ndma_set(uint8_t ndma_index, NDMA_Conf ndma_conf, uint32_t *src, uint32_t *des, uint32_t byte_count);
	uint8_t dma_ndma_get_half_tip(uint8_t ndma_index);
	uint8_t dma_ndma_get_full_tip(uint8_t ndma_index);

#ifdef __cplusplus
}
#endif

#endif
