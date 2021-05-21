#include <sdc.h>
#include <sd_mmc.h>
#include <ccu.h>
#include <io.h>
#include <inttypes.h>
#include <stdio.h>

enum
{
	SD_GCTL = 0x00,
	SD_CKCR = 0x04,
	SD_TMOR = 0x08,
	SD_BWDR = 0x0c,
	SD_BKSR = 0x10,
	SD_BYCR = 0x14,
	SD_CMDR = 0x18,
	SD_CAGR = 0x1c,
	SD_RESP0 = 0x20,
	SD_RESP1 = 0x24,
	SD_RESP2 = 0x28,
	SD_RESP3 = 0x2c,
	SD_IMKR = 0x30,
	SD_MISR = 0x34,
	SD_RISR = 0x38,
	SD_STAR = 0x3c,
	SD_FWLR = 0x40,
	SD_FUNS = 0x44,
	SD_A12A = 0x58,
	SD_NTSR = 0x5c,
	SD_SDBG = 0x60,
	SD_HWRST = 0x78,
	SD_DMAC = 0x80,
	SD_DLBA = 0x84,
	SD_IDST = 0x88,
	SD_IDIE = 0x8c,
	SD_THLDC = 0x100,
	SD_DSBD = 0x10c,
	SD_RES_CRC = 0x110,
	SD_DATA7_CRC = 0x114,
	SD_DATA6_CRC = 0x118,
	SD_DATA5_CRC = 0x11c,
	SD_DATA4_CRC = 0x120,
	SD_DATA3_CRC = 0x124,
	SD_DATA2_CRC = 0x128,
	SD_DATA1_CRC = 0x12c,
	SD_DATA0_CRC = 0x130,
	SD_CRC_STA = 0x134,
	SD_FIFO = 0x200,
};

/*
 * Global control register bits
 */
#define SDXC_SOFT_RESET (1 << 0)
#define SDXC_FIFO_RESET (1 << 1)
#define SDXC_DMA_RESET (1 << 2)
#define SDXC_INTERRUPT_ENABLE_BIT (1 << 4)
#define SDXC_DMA_ENABLE_BIT (1 << 5)
#define SDXC_DEBOUNCE_ENABLE_BIT (1 << 8)
#define SDXC_POSEDGE_LATCH_DATA (1 << 9)
#define SDXC_DDR_MODE (1 << 10)
#define SDXC_MEMORY_ACCESS_DONE (1 << 29)
#define SDXC_ACCESS_DONE_DIRECT (1 << 30)
#define SDXC_ACCESS_BY_AHB (1 << 31)
#define SDXC_ACCESS_BY_DMA (0 << 31)
#define SDXC_HARDWARE_RESET (SDXC_SOFT_RESET | SDXC_FIFO_RESET | SDXC_DMA_RESET)

/*
 * Clock control bits
 */
#define SDXC_CARD_CLOCK_ON (1 << 16)
#define SDXC_LOW_POWER_ON (1 << 17)

/*
 * Bus width
 */
#define SDXC_WIDTH1 (0)
#define SDXC_WIDTH4 (1)
#define SDXC_WIDTH8 (2)

/*
 * Smc command bits
 */
#define SDXC_RESP_EXPIRE (1 << 6)
#define SDXC_LONG_RESPONSE (1 << 7)
#define SDXC_CHECK_RESPONSE_CRC (1 << 8)
#define SDXC_DATA_EXPIRE (1 << 9)
#define SDXC_WRITE (1 << 10)
#define SDXC_SEQUENCE_MODE (1 << 11)
#define SDXC_SEND_AUTO_STOP (1 << 12)
#define SDXC_WAIT_PRE_OVER (1 << 13)
#define SDXC_STOP_ABORT_CMD (1 << 14)
#define SDXC_SEND_INIT_SEQUENCE (1 << 15)
#define SDXC_UPCLK_ONLY (1 << 21)
#define SDXC_READ_CEATA_DEV (1 << 22)
#define SDXC_CCS_EXPIRE (1 << 23)
#define SDXC_ENABLE_BIT_BOOT (1 << 24)
#define SDXC_ALT_BOOT_OPTIONS (1 << 25)
#define SDXC_BOOT_ACK_EXPIRE (1 << 26)
#define SDXC_BOOT_ABORT (1 << 27)
#define SDXC_VOLTAGE_SWITCH (1 << 28)
#define SDXC_USE_HOLD_REGISTER (1 << 29)
#define SDXC_START (1 << 31)

/*
 * Interrupt bits
 */
#define SDXC_RESP_ERROR (1 << 1)
#define SDXC_COMMAND_DONE (1 << 2)
#define SDXC_DATA_OVER (1 << 3)
#define SDXC_TX_DATA_REQUEST (1 << 4)
#define SDXC_RX_DATA_REQUEST (1 << 5)
#define SDXC_RESP_CRC_ERROR (1 << 6)
#define SDXC_DATA_CRC_ERROR (1 << 7)
#define SDXC_RESP_TIMEOUT (1 << 8)
#define SDXC_DATA_TIMEOUT (1 << 9)
#define SDXC_VOLTAGE_CHANGE_DONE (1 << 10)
#define SDXC_FIFO_RUN_ERROR (1 << 11)
#define SDXC_HARD_WARE_LOCKED (1 << 12)
#define SDXC_START_BIT_ERROR (1 << 13)
#define SDXC_AUTO_COMMAND_DONE (1 << 14)
#define SDXC_END_BIT_ERROR (1 << 15)
#define SDXC_SDIO_INTERRUPT (1 << 16)
#define SDXC_CARD_INSERT (1 << 30)
#define SDXC_CARD_REMOVE (1 << 31)
#define SDXC_INTERRUPT_ERROR_BIT (SDXC_RESP_ERROR | SDXC_RESP_CRC_ERROR | SDXC_DATA_CRC_ERROR | SDXC_RESP_TIMEOUT | SDXC_DATA_TIMEOUT | SDXC_FIFO_RUN_ERROR | SDXC_HARD_WARE_LOCKED | SDXC_START_BIT_ERROR | SDXC_END_BIT_ERROR)
#define SDXC_INTERRUPT_DONE_BIT (SDXC_AUTO_COMMAND_DONE | SDXC_DATA_OVER | SDXC_COMMAND_DONE | SDXC_VOLTAGE_CHANGE_DONE)

/*
 * Status
 */
#define SDXC_RXWL_FLAG (1 << 0)
#define SDXC_TXWL_FLAG (1 << 1)
#define SDXC_FIFO_EMPTY (1 << 2)
#define SDXC_FIFO_FULL (1 << 3)
#define SDXC_CARD_PRESENT (1 << 8)
#define SDXC_CARD_DATA_BUSY (1 << 9)
#define SDXC_DATA_FSM_BUSY (1 << 10)
#define SDXC_DMA_REQUEST (1 << 31)
#define SDXC_FIFO_SIZE (16)

/*
 * Function select
 */
#define SDXC_CEATA_ON (0xceaa << 16)
#define SDXC_SEND_IRQ_RESPONSE (1 << 0)
#define SDXC_SDIO_READ_WAIT (1 << 1)
#define SDXC_ABORT_READ_DATA (1 << 2)
#define SDXC_SEND_CCSD (1 << 8)
#define SDXC_SEND_AUTO_STOPCCSD (1 << 9)
#define SDXC_CEATA_DEV_IRQ_ENABLE (1 << 10)

#define CARD_TYPE_UNKNOWN 0
#define CARD_TYPE_SD_HIGH 1
#define CARD_TYPE_SD_LOW 2
#define CARD_TYPE_MMC 3

#define Card_Block_size 512
#define SD_BLOCK_SIZE Card_Block_size

disk_data_t sd_info;
uint32_t CARD_uRCA = 0;
uint8_t card_type = CARD_TYPE_UNKNOWN;
unsigned int g_max_valid_sector;
__attribute__((aligned(4096))) unsigned char _card_ucSDHCBuffer[64]; //Card settings cache data

static void sdc_delay_us(uint32_t us)
{
	// delay_us(us);
	volatile unsigned int temp;
	while (us--)
	{
		temp = 0x2f;
		while (temp--)
		{
			temp = temp;
		};
	}
}

static void sdc_delay(uint32_t ms)
{
	// delay(ms);
	sdc_delay_us(1000);
}

static bool_t sdc_transfer_command(SDC_Type *sdc, mmc_cmd_st *cmd, mmc_data_st *dat)
{
	u32_t cmdval = SDXC_START;
	u32_t status = 0;
	int timeout = 0;
	// printf("sdc_transfer_command id: %d, sizeof(u32_t): %d\r\n", cmd->cmdidx, sizeof(u32_t));

	if (cmd->cmdidx == MMC_STOP_TRANSMISSION)
	{
		timeout = 10000;
		do
		{
			status = sdc->SD_STAR_REG;
			if (!timeout--)
			{
				sdc->SD_GCTL_REG = SDXC_HARDWARE_RESET;
				sdc->SD_RISR_REG = 0xffffffff;
				printf("fail 1 id: %d, status: 0x%08X\r\n", cmd->cmdidx, status);
				return FALSE;
			}
		} while (status & SDXC_CARD_DATA_BUSY);
		return TRUE;
	}

	if (cmd->cmdidx == MMC_GO_IDLE_STATE)
		cmdval |= SDXC_SEND_INIT_SEQUENCE;
	if (cmd->resptype & MMC_RSP_PRESENT)
	{
		cmdval |= SDXC_RESP_EXPIRE;
		if (cmd->resptype & MMC_RSP_136)
			cmdval |= SDXC_LONG_RESPONSE;
		if (cmd->resptype & MMC_RSP_CRC)
			cmdval |= SDXC_CHECK_RESPONSE_CRC;
	}

	if (dat)
		cmdval |= SDXC_DATA_EXPIRE | SDXC_WAIT_PRE_OVER;

	if (dat && (dat->flag & MMC_DATA_WRITE))
		cmdval |= SDXC_WRITE;

	if (cmd->cmdidx == MMC_WRITE_MULTIPLE_BLOCK || cmd->cmdidx == MMC_READ_MULTIPLE_BLOCK)
		cmdval |= SDXC_SEND_AUTO_STOP;

	sdc->SD_CAGR_REG = cmd->cmdarg;

	if (dat)
		sdc->SD_GCTL_REG |= BIT(31);
	sdc->SD_CMDR_REG = cmdval | cmd->cmdidx;

	timeout = 1000000;
	do
	{
		status = sdc->SD_RISR_REG;
		if (!timeout-- || (status & SDXC_INTERRUPT_ERROR_BIT))
		{
			sdc->SD_GCTL_REG = SDXC_HARDWARE_RESET;
			sdc->SD_RISR_REG = 0xffffffff;
			printf("fail 2 id: %d, status: 0x%08X\r\n", cmd->cmdidx, status);
			return FALSE;
		}
		sdc_delay_us(5);
	} while (!(status & SDXC_COMMAND_DONE));

	if (cmd->resptype & MMC_RSP_BUSY)
	{
		timeout = 1000000;
		do
		{
			status = sdc->SD_STAR_REG;
			if (!timeout--)
			{
				sdc->SD_GCTL_REG = SDXC_HARDWARE_RESET;
				sdc->SD_RISR_REG = 0xffffffff;
				printf("fail 3 id: %d, status: 0x%08X\r\n", cmd->cmdidx, status);
				return FALSE;
			}
			sdc_delay_us(5);
		} while (status & (1 << 9));
	}

	if (cmd->resptype & MMC_RSP_136)
	{
		cmd->response[0] = sdc->SD_RESP3_REG;
		cmd->response[1] = sdc->SD_RESP2_REG;
		cmd->response[2] = sdc->SD_RESP1_REG;
		cmd->response[3] = sdc->SD_RESP0_REG;
	}
	else
	{
		cmd->response[0] = sdc->SD_RESP0_REG;
	}
	sdc->SD_RISR_REG = 0xffffffff;
	return TRUE;
}

static bool_t sdc_transfer_data_by_cpu(SDC_Type *sdc, mmc_data_st *dat)
{
	u64_t count = dat->blkcnt * dat->blksz;
	u32_t *tmp = dat->buf;
	u32_t status, err, done;

	status = sdc->SD_STAR_REG;
	err = sdc->SD_RISR_REG & SDXC_INTERRUPT_ERROR_BIT;

	if (dat->flag & MMC_DATA_READ)
	{
		while ((!err) && (count >= sizeof(u32_t)))
		{
			if (!(status & SDXC_FIFO_EMPTY))
			{
				*(tmp) = sdc->SD_FIFO_REG;
				tmp++;
				count -= sizeof(u32_t);
			}
			status = sdc->SD_STAR_REG;
			err = sdc->SD_RISR_REG & SDXC_INTERRUPT_ERROR_BIT;
		}
	}
	else if (dat->flag & MMC_DATA_WRITE)
	{
		while ((!err) && (count >= sizeof(u32_t)))
		{
			if (!(status & SDXC_FIFO_FULL))
			{
				sdc->SD_FIFO_REG = *(tmp);
				tmp++;
				count -= sizeof(u32_t);
			}
			status = sdc->SD_STAR_REG;
			err = sdc->SD_RISR_REG & SDXC_INTERRUPT_ERROR_BIT;
		}
	}

	do
	{
		status = sdc->SD_RISR_REG;
		err = status & SDXC_INTERRUPT_ERROR_BIT;
		if (dat->blkcnt > 1)
			done = status & SDXC_AUTO_COMMAND_DONE;
		else
			done = status & SDXC_DATA_OVER;
	} while (!done && !err);

	if (err & SDXC_INTERRUPT_ERROR_BIT)
		return FALSE;
	sdc->SD_GCTL_REG |= SDXC_FIFO_RESET;
	sdc->SD_RISR_REG = 0xffffffff;

	if (count)
		return FALSE;
	return TRUE;
}

static bool_t sdc_read_bytes(SDC_Type *sdc, u32_t *buf, u32_t blkcount, u32_t blksize)
{
	u64_t count = blkcount * blksize;
	u32_t *tmp = buf;
	u32_t status, err, done;

	status = sdc->SD_STAR_REG;
	err = sdc->SD_RISR_REG & SDXC_INTERRUPT_ERROR_BIT;
	while ((!err) && (count >= sizeof(u32_t)))
	{
		if (!(status & SDXC_FIFO_EMPTY))
		{
			*(tmp) = sdc->SD_FIFO_REG;
			tmp++;
			count -= sizeof(u32_t);
		}
		status = sdc->SD_STAR_REG;
		err = sdc->SD_RISR_REG & SDXC_INTERRUPT_ERROR_BIT;
	}

	do
	{
		status = sdc->SD_RISR_REG;
		err = status & SDXC_INTERRUPT_ERROR_BIT;
		if (blkcount > 1)
			done = status & SDXC_AUTO_COMMAND_DONE;
		else
			done = status & SDXC_DATA_OVER;
	} while (!done && !err);

	if (err & SDXC_INTERRUPT_ERROR_BIT)
		return FALSE;
	sdc->SD_RISR_REG = 0xffffffff;

	if (count)
		return FALSE;
	return TRUE;
}

static bool_t sdc_write_bytes(SDC_Type *sdc, u32_t *buf, u32_t blkcount, u32_t blksize)
{
	u64_t count = blkcount * blksize;
	u32_t *tmp = buf;
	u32_t status, err, done;

	status = sdc->SD_STAR_REG;
	err = sdc->SD_RISR_REG & SDXC_INTERRUPT_ERROR_BIT;
	while (!err && count)
	{
		if (!(status & SDXC_FIFO_FULL))
		{
			sdc->SD_FIFO_REG = *(tmp);
			tmp++;
			count -= sizeof(u32_t);
		}
		status = sdc->SD_STAR_REG;
		err = sdc->SD_RISR_REG & SDXC_INTERRUPT_ERROR_BIT;
	}

	do
	{
		status = sdc->SD_RISR_REG;
		err = status & SDXC_INTERRUPT_ERROR_BIT;
		if (blkcount > 1)
			done = status & SDXC_AUTO_COMMAND_DONE;
		else
			done = status & SDXC_DATA_OVER;
	} while (!done && !err);

	if (err & SDXC_INTERRUPT_ERROR_BIT)
		return FALSE;
	sdc->SD_GCTL_REG = sdc->SD_RISR_REG | SDXC_FIFO_RESET;
	sdc->SD_RISR_REG = 0xffffffff;

	if (count)
		return FALSE;
	return TRUE;
}

static bool_t sdc_transfer_data(SDC_Type *sdc, mmc_cmd_st *cmd, mmc_data_st *dat)
{
	u32_t dlen = (u32_t)(dat->blkcnt * dat->blksz);
	bool_t ret = FALSE;

	sdc->SD_BKSR_REG = dat->blksz;
	sdc->SD_BYCR_REG = dlen;
	if ((dat->flag & MMC_DATA_READ) || (dat->flag & MMC_DATA_WRITE))
	{
		if (!sdc_transfer_command(sdc, cmd, dat))
			return FALSE;
		ret = sdc_transfer_data_by_cpu(sdc, dat);
	}
	return ret;
}

static bool_t sdc_detect_card(SDC_Type *sdc)
{
	// if((cd >= 0) && gpio_get_value(cd))
	// 	return FALSE;
	return TRUE;
}

static bool_t sdc_reset_hw(SDC_Type *sdc)
{
	sdc->SD_GCTL_REG = SDXC_HARDWARE_RESET;
	return TRUE;
}

static void sdc_reset_sw(SDC_Type *sdc)
{
	S_Bit(sdc->SD_GCTL_REG, 0);
	S_Bit(sdc->SD_GCTL_REG, 1);
	S_Bit(sdc->SD_GCTL_REG, 2);
	sdc_delay(1);
}

static bool_t sdc_set_voltage(SDC_Type *sdc, u32_t voltage)
{
	return TRUE;
}

static bool_t sdc_set_bus_width(SDC_Type *sdc, u32_t width)
{
	switch (width)
	{
	case MMC_BUS_WIDTH_1:
		sdc->SD_BWDR_REG = SDXC_WIDTH1;
		break;
	case MMC_BUS_WIDTH_4:
		sdc->SD_BWDR_REG = SDXC_WIDTH4;
		break;
	case MMC_BUS_WIDTH_8:
		sdc->SD_BWDR_REG = SDXC_WIDTH8;
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

static bool_t sdc_update_clock(SDC_Type *sdc)
{
	u32_t cmd = BIT(31) | BIT(21) | BIT(13);
	u32_t timeout = 2000000;

	sdc->SD_CMDR_REG = cmd;
	while ((sdc->SD_CMDR_REG & BIT(31)) && timeout--)
	{
		sdc_delay_us(1);
	}
	if (!timeout)
	{
		printf("mmc update clk failed");
		return FALSE;
	}
	sdc->SD_RISR_REG = sdc->SD_RISR_REG;
	return TRUE;
}

static bool_t sdc_set_clock(SDC_Type *sdc, u32_t clock)
{
	// u32_t ratio = udiv32(ccu_get_rate_periph(24000000) + 2 * clock - 1, (2 * clock));

	// if((ratio & 0xff) != ratio)
	// 	return FALSE;
	// write32((uint8_t*)sdc + SD_CKCR, read32((uint8_t*)sdc + SD_CKCR) & ~(1 << 16));
	// write32((uint8_t*)sdc + SD_CKCR, ratio);
	// if(!sdc_update_clock(sdc))
	// 	return FALSE;
	// write32((uint8_t*)sdc + SD_CKCR, read32((uint8_t*)sdc + SD_CKCR) | (3 << 16));
	// if(!sdc_update_clock(sdc))
	// 	return FALSE;
	return TRUE;
}

static bool_t sdc_transfer(SDC_Type *sdc, mmc_cmd_st *cmd, mmc_data_st *dat)
{
	if (!dat)
		return sdc_transfer_command(sdc, cmd, dat);
	return sdc_transfer_data(sdc, cmd, dat);
}

bool_t sdc_clock_enable(SDC_Type *sdc)
{
	S_Bit(sdc->SD_CKCR_REG, 16);
	return sdc_update_clock(sdc);
}

bool_t sdc_clock_disable(SDC_Type *sdc)
{
	C_Bit(sdc->SD_CKCR_REG, 16);
	return sdc_update_clock(sdc);
}

int sdc_set_mod_clk(SDC_Type *sdc, uint32_t hz)
{
	printf("sdc_set_mod_clk hz: %u\r\n", hz);
	uint32_t pll, pll_hz, div, n, oclk_dly, sclk_dly;

	if (hz <= 24000000)
	{
		pll = 0x00 << 24;
		pll_hz = 24000000;
	}
	else
	{
		pll = 0x01 << 24;
		pll_hz = ccu_get_rate_periph(24000000);
	}

	div = pll_hz / hz;
	if (pll_hz % hz)
		div++;

	n = 0;
	while (div > 16)
	{
		n++;
		div = (div + 1) / 2;
	}

	if (n > 3)
	{
		printf("ERR: sdc_set_mod_clk hz: %u\r\n", hz);
		return FALSE;
	}

	/* determine delays */
	if (hz <= 400000)
	{
		oclk_dly = 0;
		sclk_dly = 0;
	}
	else if (hz <= 25000000)
	{
		oclk_dly = 0;
		sclk_dly = 5;
	}
	else if (hz <= 50000000)
	{
		oclk_dly = 3;
		sclk_dly = 4;
	}
	else
	{
		/* hz > 50000000 */
		oclk_dly = 1;
		sclk_dly = 4;
	}

	if (sdc == SDC0)
	{
		CCU->SDMMC0_CLK_REG = pll | (sclk_dly << 20) | (n << 16) | (oclk_dly << 8) | div;
		S_Bit(CCU->SDMMC0_CLK_REG, 31);
	}
	else if (sdc == SDC1)
	{
		CCU->SDMMC1_CLK_REG = pll | (sclk_dly << 20) | (n << 16) | (oclk_dly << 8) | div;
		S_Bit(CCU->SDMMC1_CLK_REG, 31);
	}

#if SD_DBG_PRINTF
	printf("sdc set [mod-clk req %u]-[parent %u]-[n %u]-[m %u]-[rate %u]\r\n",
				 hz, pll_hz, 1u << n, div,
				 pll_hz / (1u << n) / div);
#endif
	return TRUE;
}

static void sdc_config_clock(SDC_Type *sdc, uint32_t hz)
{
	if (!sdc_clock_disable(sdc))
	{
		printf("err: mmc_clock_disable\r\n");
		return FALSE;
	}

	/* Set new clock */
	if (!sdc_set_mod_clk(sdc, hz))
	{
		printf("err: sdc_set_mod_clk\r\n");
		return FALSE;
	}
	/* Clear the internal frequency divider to 0 */
	sdc->SD_CKCR_REG &= ~0xff;

	if (!sdc_clock_enable(sdc))
	{
		printf("err: sdc_clock_enable\r\n");
		return FALSE;
	}
	return TRUE;
}

void sdc_gpio_init(SDC_Type *sdc)
{
	int i = 0;
	if (sdc == SDC0)
		for (i = 0; i < 6; i++)
		{
			gpio_set_cfg(GPIOF, i, GPIO_FUNC_010);
			gpio_set_pull(GPIOF, i, GPIO_PULL_NONE);
		}
	else if (sdc == SDC1)
		for (i = 0; i < 3; i++)
		{
			gpio_set_cfg(GPIOC, i, GPIO_FUNC_011);
			gpio_set_pull(GPIOC, i, GPIO_PULL_NONE);
		}
}

bool sdc_transfer_cmd(SDC_Type *sdc, mmc_cmd_st *cmd, mmc_data_st *dat, uint32_t cmdidx, uint32_t cmdarg, uint32_t resptype)
{
	cmd->cmdidx = cmdidx;
	cmd->cmdarg = cmdarg;
	cmd->resptype = resptype;
	return sdc_transfer(sdc, cmd, dat);
}

void sdc_get_sd_info(SDC_Type *sdc, mmc_cmd_st *cmd, disk_data_t *sd_info)
{
	unsigned int i;
	unsigned int R_LEN, C_Size, MULT, size;
	unsigned int Buffer[4];
	unsigned char *ptr;

	sdc_transfer_cmd(sdc, cmd, NULL, 9, CARD_uRCA, MMC_RSP_PRESENT | MMC_RSP_CRC | MMC_RSP_136);
	Buffer[0] = cmd->response[0];
	Buffer[1] = cmd->response[1];
	Buffer[2] = cmd->response[2];
	Buffer[3] = cmd->response[3];

	if ((Buffer[0] & 0xc0000000) && (card_type != CARD_TYPE_MMC))
	{
		C_Size = ((Buffer[1] & 0x0000003f) << 16) | ((Buffer[2] & 0xffff0000) >> 16);
		size = (C_Size + 1) * 512;

		sd_info->diskSize = size;
		sd_info->totalSectorN = size << 1;
	}
	else
	{
		R_LEN = (Buffer[1] & 0x000f0000) >> 16;
		C_Size = ((Buffer[1] & 0x000003ff) << 2) | ((Buffer[2] & 0xc0000000) >> 30);
		MULT = (Buffer[2] & 0x00038000) >> 15;
		size = (C_Size + 1) * (1 << (MULT + 2)) * (1 << R_LEN);

		sd_info->diskSize = size / 1024;
		sd_info->totalSectorN = size / 512;
	}
	g_max_valid_sector = sd_info->totalSectorN;
	sd_info->sectorSize = 512;

	sdc_transfer_cmd(sdc, cmd, NULL, 10, CARD_uRCA, MMC_RSP_PRESENT | MMC_RSP_CRC | MMC_RSP_136);
	Buffer[0] = cmd->response[0];
	Buffer[1] = cmd->response[1];
	Buffer[2] = cmd->response[2];
	Buffer[3] = cmd->response[3];

	sd_info->vendor[0] = (Buffer[0] & 0xff000000) >> 24;
	ptr = (unsigned char *)Buffer;
	ptr = ptr + 4;
	for (i = 0; i < 5; i++)
		sd_info->product[i] = *ptr++;
	ptr = ptr + 10;
	for (i = 0; i < 4; i++)
		sd_info->serial[i] = *ptr++;

	printf("SD_info print...\r\n");
	printf("SD_info->diskSize=[%d]\r\n", sd_info->diskSize);
	printf("SD_info->totalSectorN=[%d]\r\n", sd_info->totalSectorN);
	printf("SD_info->sectorSize=[%d]\r\n", sd_info->sectorSize);
}

int sdc_switch_to_high_speed(SDC_Type *sdc, struct mmc_cmd *cmd)
{
	mmc_data_st data;
	int volatile status = FALSE;
	unsigned short current_comsumption,
			//			fun1_info, switch_status,
			busy_status0, busy_status1;

	//CMD6
	data.blkcnt = 1;
	data.blksz = 64;
	data.flag = MMC_DATA_READ;
	data.buf = _card_ucSDHCBuffer;
	if ((status = sdc_transfer_cmd(sdc, cmd, &data, 6, 0x00ffff01, MMC_RSP_PRESENT | MMC_RSP_CRC)) != TRUE)
		return status;

	current_comsumption = _card_ucSDHCBuffer[0] << 8 | _card_ucSDHCBuffer[1];
	if (!current_comsumption)
		return FALSE;

	//    fun1_info =  _card_ucSDHCBuffer[12]<<8 | _card_ucSDHCBuffer[13];
	//    switch_status = _card_ucSDHCBuffer[16] & 0xf;// Equal to 1 to support high-speed mode
	busy_status0 = _card_ucSDHCBuffer[28] << 8 | _card_ucSDHCBuffer[29];

	if (!busy_status0) // function ready
	{
		//CMD6
		data.blkcnt = 1;
		data.blksz = 64;
		data.flag = MMC_DATA_READ;
		data.buf = _card_ucSDHCBuffer;
		if ((status = sdc_transfer_cmd(sdc, cmd, &data, 6, 0x80ffff01, MMC_RSP_PRESENT | MMC_RSP_CRC)) != TRUE)
			return status;

		current_comsumption = _card_ucSDHCBuffer[0] << 8 | _card_ucSDHCBuffer[1];
		if (!current_comsumption)
			return FALSE;

		busy_status1 = _card_ucSDHCBuffer[28] << 8 | _card_ucSDHCBuffer[29];

		if (!busy_status1)
			printf("switch into high speed mode !!!\r\n");

		return TRUE;
	}
	else
		return FALSE;
}

int sdc_select_sdcard(SDC_Type *sdc, struct mmc_cmd *cmd, char CARD_WIDTH)
{
	mmc_data_st data;
	int volatile status = FALSE, i;
	printf("SelectCard...\r\n");
	// CMD7, put the SD card into the Transfer state
	if ((status = sdc_transfer_cmd(sdc, cmd, NULL, 7, CARD_uRCA, MMC_RSP_PRESENT | MMC_RSP_CRC)) != TRUE)
		return status;

	// Set bit width
	if (card_type == CARD_TYPE_SD_HIGH)
	{
		printf("CARD_TYPE_SD_HIGH \r\n");
		//Read SCR register
		//SD data line default 1 bit width transmission
		if ((status = sdc_transfer_cmd(sdc, cmd, NULL, 55, CARD_uRCA, MMC_RSP_PRESENT | MMC_RSP_CRC)) != TRUE)
			return status;
		data.blkcnt = 1;
		data.blksz = 8;
		data.flag = MMC_DATA_READ;
		data.buf = _card_ucSDHCBuffer;
		if ((status = sdc_transfer_cmd(sdc, cmd, &data, 51, 0, MMC_RSP_PRESENT | MMC_RSP_CRC)) != TRUE)
			return status;
#if SD_DBG_PRINTF
		printf("buf=0x");
		for (i = 0; i < 8; i++)
			printf("%02x", _card_ucSDHCBuffer[i]);
		printf(" \r\n");
#endif
		//
		if ((_card_ucSDHCBuffer[0] & 0xf) == 0x2) //Judging the SD version 56-59 bits
		{
			//Set card to high speed mode
			status = sdc_switch_to_high_speed(sdc, cmd);
			if (status == TRUE)
			{
				sdc_config_clock(sdc, 48000000);
			}
		}
		if (CARD_WIDTH == MMC_BUS_WIDTH_4)
		{
			printf("MMC_BUS_WIDTH_4 \r\n");
			//Set the CARD bit width parameter=0x2 is 4 bits, 00 is 1 bit
			if ((status = sdc_transfer_cmd(sdc, cmd, NULL, 55, CARD_uRCA, MMC_RSP_PRESENT | MMC_RSP_CRC)) != TRUE)
				return status;
			if ((status = sdc_transfer_cmd(sdc, cmd, NULL, 6, 0x02, MMC_RSP_PRESENT | MMC_RSP_CRC)) != TRUE)
				return status;
			sdc_set_bus_width(sdc, MMC_BUS_WIDTH_4);
		}
	}
	else if (card_type == CARD_TYPE_SD_LOW)
	{
		printf("CARD_TYPE_SD_LOW \r\n");
		sdc_config_clock(sdc, 12000000);
		if (CARD_WIDTH == MMC_BUS_WIDTH_4)
		{
			printf("MMC_BUS_WIDTH_4 \r\n");
			//Set the bit width parameter=0x2 is 4 bits, 00 is 1 bit
			if ((status = sdc_transfer_cmd(sdc, cmd, NULL, 55, CARD_uRCA, MMC_RSP_PRESENT | MMC_RSP_CRC)) != TRUE)
				return status;
			if ((status = sdc_transfer_cmd(sdc, cmd, NULL, 6, 0x02, MMC_RSP_PRESENT | MMC_RSP_CRC)) != TRUE)
				return status;
			sdc_set_bus_width(sdc, MMC_BUS_WIDTH_4);
		}
	}
	else if (card_type == CARD_TYPE_MMC)
	{
	}
	printf("SD_BLOCK_SIZE 512 \r\n");
	//Set block length
	if ((status = sdc_transfer_cmd(sdc, cmd, NULL, 16, SD_BLOCK_SIZE, MMC_RSP_PRESENT | MMC_RSP_CRC | MMC_RSP_BUSY)) != TRUE)
	{
	};																				 //return status;
	sdc_transfer_cmd(sdc, cmd, NULL, 7, 0, 0); //Uncheck all
	return TRUE;
}

/*-----------------------------------------------------------------------------
Read sector
sector: sector address
count: number of blocks
buff: read pointer
 *---------------------------------------------------------------------------*/
int sdc_read_in(SDC_Type *sdc, unsigned int sector, unsigned int count, unsigned char *buff)
{
	mmc_cmd_st cmd;
	mmc_data_st data;
	unsigned int _sector;
	unsigned int cmd_inx;
	int status = FALSE;

	if ((sector + count - 1) > g_max_valid_sector)
		return FALSE;

	if (buff == 0)
	{
		printf("ERROR: fmiSD_Read_in(): uBufcnt cannot be 0!!\r\n");
		return -745435;
	}
	if ((u32)(long)buff & 0x3)
	{
		printf("SD_Read Addr Align Err[%08x] ...\r\n", buff);
		return -474385;
	}
	/* 
	High-capacity cards and low-capacity cards Less than or equal to 2G, low-speed, 
	high-capacity cards, write sector addresses, low-capacity cards, write direct addresses
	*/
	if (card_type == CARD_TYPE_SD_HIGH)
		_sector = sector;
	else
		_sector = sector * SD_BLOCK_SIZE;

	if (count == 1)
		cmd_inx = 17;
	else
		cmd_inx = 18;
	/*Send CMD7-selected*/
	if ((status = sdc_transfer_cmd(sdc, &cmd, NULL, 7, CARD_uRCA, MMC_RSP_PRESENT | MMC_RSP_CRC | MMC_RSP_BUSY)) != TRUE)
		return status;
	/*Read data*/

	data.blkcnt = count;
	data.blksz = SD_BLOCK_SIZE;
	data.flag = MMC_DATA_READ;
	data.buf = buff;
	if ((status = sdc_transfer_cmd(sdc, &cmd, &data, cmd_inx, _sector, MMC_RSP_PRESENT | MMC_RSP_CRC | MMC_RSP_BUSY)) != TRUE)
	{
	};
	/*Send CMD7-uncheck*/
	sdc_transfer_cmd(sdc, &cmd, NULL, 7, 0, 0);
	return TRUE;
}
/*-----------------------------------------------------------------------------
Write sector
sector: sector address
count: number of blocks
buff: read pointer
 *---------------------------------------------------------------------------*/
int sdc_write_out(SDC_Type *sdc, unsigned int sector, unsigned int count, unsigned char *buff)
{
	mmc_cmd_st cmd;
	mmc_data_st data;
	unsigned int _sector;
	unsigned int cmd_inx;
	int status = FALSE;

	if ((sector + count - 1) > g_max_valid_sector)
		return FALSE;

	if (buff == 0)
	{
		printf("ERROR: fmiSD_Read_in(): uBufcnt cannot be 0!!\r\n");
		return -745435;
	}
	if ((u32)(long)buff & 0x3)
	{
		printf("Addr Align Err ...\r\n");
		return FALSE;
	}
	/* 
	High-capacity cards and low-capacity cards Less than or equal to 2G, low-speed, 
	high-capacity cards, write sector addresses, low-capacity cards, write direct addresses
	*/
	if (card_type == CARD_TYPE_SD_HIGH)
		_sector = sector;
	else
		_sector = sector * SD_BLOCK_SIZE;

	if (count == 1)
		cmd_inx = 24;
	else
		cmd_inx = 25;

	if ((status = sdc_transfer_cmd(sdc, &cmd, NULL, 7, CARD_uRCA, MMC_RSP_PRESENT | MMC_RSP_CRC | MMC_RSP_BUSY)) != TRUE)
		return status;

	data.blkcnt = count;
	data.blksz = SD_BLOCK_SIZE;
	data.flag = MMC_DATA_WRITE;
	data.buf = buff;
	if ((status = sdc_transfer_cmd(sdc, &cmd, &data, cmd_inx, _sector, MMC_RSP_PRESENT | MMC_RSP_CRC | MMC_RSP_BUSY)) != TRUE)
		return status;

	sdc_transfer_cmd(sdc, &cmd, NULL, 7, 0, 0);
	return TRUE;
}

int sdc_init(SDC_Type *sdc)
{
	bool result;
	sdc_gpio_init(sdc);
	if (sdc == SDC0)
		ccu_reset(RESET_SD0, true);
	else if (sdc == SDC1)
		ccu_reset(RESET_SD1, true);
	sdc_delay(10);

	sdc_set_mod_clk(sdc, 400000);
	sdc_reset_hw(sdc);
	sdc_reset_sw(sdc);

	C_Bit(sdc->SD_GCTL_REG, 8);
	S_Bit(sdc->SD_CKCR_REG, 17);
	sdc_set_bus_width(sdc, MMC_BUS_WIDTH_1);
	sdc->SD_TMOR_REG = 0xffffffff;

	sdc_clock_enable(sdc);
	sdc_delay(1);

	mmc_cmd_st cmd = {0};
	result = sdc_transfer_cmd(sdc, &cmd, NULL, 0, 0, 0);
	sdc_delay(10);
	result = sdc_transfer_cmd(sdc, &cmd, NULL, 8, 0x155, MMC_RSP_PRESENT | MMC_RSP_CRC);
	sdc_delay(20);
	if (result)
	{
		printf("SD2.0...\r\n");
		/* CMD55, pre-special command command, this command needs to be sent before sending ACMD commands */
		sdc_transfer_cmd(sdc, &cmd, NULL, 55, 0, MMC_RSP_PRESENT | MMC_RSP_CRC);
		/* CMD41, get SD voltage value */
		sdc_transfer_cmd(sdc, &cmd, NULL, 41, 0x40ff8000, MMC_RSP_PRESENT | MMC_RSP_CRC | MMC_RSP_136);
		// The voltage value was not written successfully-rewrite the voltage value
		while (!(cmd.response[0] & 0x80000000))
		{
			/* CMD55, pre-special command command, this command needs to be sent before sending ACMD commands */
			sdc_transfer_cmd(sdc, &cmd, NULL, 55, 0, MMC_RSP_PRESENT | MMC_RSP_CRC);
			/* CMD41, get SD voltage value */
			result = sdc_transfer_cmd(sdc, &cmd, NULL, 41, 0x40ff8000, MMC_RSP_PRESENT | MMC_RSP_CRC | MMC_RSP_136);
			if (result == FALSE)
				break;
		}
		if (cmd.response[0] & 0x40000000)
		{
			card_type = CARD_TYPE_SD_HIGH;
			printf("TYPE_SD_HIGH...\r\n");
		}
		else
		{
			card_type = CARD_TYPE_SD_LOW;
			printf("TYPE_SD_LOW...\r\n");
		}
	}
	else
	{
		printf("SD1.1...\r\n");
		sdc_transfer_cmd(sdc, &cmd, NULL, 0, 0, 0); //发送复位命令 CMD0
		sdc_delay(10);

		result = sdc_transfer_cmd(sdc, &cmd, NULL, 55, 0, MMC_RSP_PRESENT | MMC_RSP_CRC);
		if (result == FALSE) // MMC memory
		{
			sdc_transfer_cmd(sdc, &cmd, NULL, 0, 0, 0); //发送复位命令 CMD0
			sdc_delay(1);
			if (sdc_transfer_cmd(sdc, &cmd, NULL, 1, 0x80ff8000, MMC_RSP_PRESENT | MMC_RSP_CRC) != FALSE) // MMC memory
			{
				while (!(cmd.response[3] & 0x80000000)) // check if card is ready
				{
					result = sdc_transfer_cmd(sdc, &cmd, NULL, 1, 0x80ff8000, MMC_RSP_PRESENT | MMC_RSP_CRC); // high voltage
					if (result == FALSE)
						break;
				}
				card_type = CARD_TYPE_MMC;
				printf("TYPE_MMC...\r\n");
			}
			else
			{
				card_type = CARD_TYPE_UNKNOWN;
				printf("ERR_DEVICE...\r\n");
				return FALSE;
			}
		}
		else if (result == 0) // SD Memory
		{
			sdc_transfer_cmd(sdc, &cmd, NULL, 41, 0x00ff8000, MMC_RSP_PRESENT | MMC_RSP_CRC | MMC_RSP_136);
			while (!(cmd.response[3] & 0x80000000)) // check if card is ready
			{
				sdc_transfer_cmd(sdc, &cmd, NULL, 55, 0x00, MMC_RSP_PRESENT | MMC_RSP_CRC);
				result = sdc_transfer_cmd(sdc, &cmd, NULL, 41, 0x00ff8000, MMC_RSP_PRESENT | MMC_RSP_CRC | MMC_RSP_136);
				if (result == FALSE)
					break;
			}
			card_type = CARD_TYPE_SD_LOW;
			printf("TYPE_SD_LOW...\r\n");
		}
		else
		{
			card_type = CARD_TYPE_UNKNOWN;
			printf("Init Error !!\r\n");
			return FALSE;
		}
	}

	if (card_type != CARD_TYPE_UNKNOWN)
	{
		sdc_transfer_cmd(sdc, &cmd, NULL, 2, 0x0, MMC_RSP_PRESENT | MMC_RSP_CRC | MMC_RSP_136);
		// CIDBuffer[0] = cmd.response[0];
		// CIDBuffer[1] = cmd.response[1];
		// CIDBuffer[2] = cmd.response[2];
		// CIDBuffer[3] = cmd.response[3];
		if (card_type == CARD_TYPE_MMC)
		{
			result = sdc_transfer_cmd(sdc, &cmd, NULL, 3, 0x10000, MMC_RSP_PRESENT | MMC_RSP_CRC | MMC_RSP_136);
			if (result == FALSE)
				return FALSE; // set RCA
			CARD_uRCA = 0x10000;
		}
		else
		{
			result = sdc_transfer_cmd(sdc, &cmd, NULL, 3, 0x00, MMC_RSP_PRESENT | MMC_RSP_CRC | MMC_RSP_136);
			if (result == FALSE)
				return FALSE; // get RCA
			CARD_uRCA = cmd.response[0];
		}
	}
	//Return file information
	sdc_get_sd_info(sdc, &cmd, &sd_info);
	/*Select card, set various parameters*/
	sdc_select_sdcard(sdc, &cmd, MMC_BUS_WIDTH_4);

	return TRUE;
}