#ifndef __CCU_H__
#define __CCU_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <types.h>
#include <F1C100S.h>
#include <stdbool.h>

#define RESET_DMA (6)
#define RESET_SD0 (8)
#define RESET_SD1 (9)
#define RESET_SDRAM (14)
#define RESET_SPI0 (20)
#define RESET_SPI1 (21)
#define RESET_USB_OTG (24)

#define RESET_VE (32)
#define RESET_LCD (36)
#define RESET_DEINTERLACE (37)
#define RESET_CSI (40)
#define RESET_TVD (41)
#define RESET_TVE (42)
#define RESET_DEBE (44)
#define RESET_DEFE (46)

#define RESET_AUDIO_CODEC (64)
#define RESET_SPDIF (65)
#define RESET_CIR (66)
#define RESET_RSB (67)
#define RESET_DAUDIO (76)
#define RESET_I2C0 (80)
#define RESET_I2C1 (81)
#define RESET_I2C2 (82)
#define RESET_UART0 (84)
#define RESET_UART1 (85)
#define RESET_UART2 (86)

	void ccu_set_enable_cpu(bool enable);
	void ccu_set_enable_audio(bool enable);
	void ccu_set_enable_video(bool enable);
	void ccu_set_enable_ve(bool enable);
	void ccu_set_enable_ddr(bool enable);
	void ccu_set_enable_periph(bool enable);

	uint64_t ccu_get_rate_cpu(uint64_t prate);
	uint64_t ccu_get_rate_audio(uint64_t prate);
	uint64_t ccu_get_rate_video(uint64_t prate);
	uint64_t ccu_get_rate_ve(uint64_t prate);
	uint64_t ccu_get_rate_ddr(uint64_t prate);
	uint64_t ccu_get_rate_periph(uint64_t prate);

	void ccu_reset(int32_t bit, bool enable);

#ifdef __cplusplus
}
#endif

#endif /* __CCU_H__ */
