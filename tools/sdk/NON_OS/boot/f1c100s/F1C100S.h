#ifndef __F1C100S_H_
#define __F1C100S_H_

#include <stddef.h>
#include <types.h>

#define DMA_BASE 0x01C02000
#define SPI_BASE 0x01C05000
#define TVE_BASE 0x01C0A000
#define TVD_BASE 0x01C0B000
#define TCON_BASE 0x01C0C000
#define VE_BASE 0x01C0E000
#define MPEG_BASE 0x01C0E100
#define SDC_BASE 0x01C0F000
#define OTG_BASE 0x01C13000
#define CCU_BASE 0x01C20000
#define INTC_BASE 0x01C20400
#define PIO_BASE 0x01C20800
#define TIMER_BASE 0x01C20C00
#define PWM_BASE 0x01C21000
#define OWA_BASE 0x01C21400
#define RSB_BASE 0x01C21800
#define DA_BASE 0x01C22000
#define CIR_BASE 0x01C22C00
#define KEYADC_BASE 0x01C23400
#define AUDIOC_BASE 0x01C23C00
#define TP_BASE 0x01C24800
#define UART_BASE 0x01C25000
#define TWI_BASE 0x01C27000
#define CSI_BASE 0x01CB0000
#define DEFE_BASE 0x01E00000
#define DEBE_BASE 0x01E60000
#define DI_BASE 0x01E70000

typedef struct
{
	vuint32_t PLL_CPU_CTRL_REG;				/* 0x000 */
	vuint32_t rsv1;										/* 0x004 */
	vuint32_t PLL_AUDIO_CTRL_REG;			/* 0x008 */
	vuint32_t rsv2;										/* 0x00c */
	vuint32_t PLL_VIDEO_CTRL_REG;			/* 0x010 */
	vuint32_t rsv3;										/* 0x014 */
	vuint32_t PLL_VE_CTRL_REG;				/* 0x018 */
	vuint32_t rsv4;										/* 0x01c */
	vuint32_t PLL_DDR_CTRL_REG;				/* 0x020 */
	vuint32_t rsv5;										/* 0x024 */
	vuint32_t PLL_PERIPH_CTRL_REG;		/* 0X028 */
	vuint32_t rsv6[9];								/* 0X02C */
	vuint32_t CPU_CLK_SRC_REG;				/* 0x050 */
	vuint32_t AHB_APB_HCLKC_CFG_REG;	/* 0x054 */
	vuint32_t rsv7[2];								/* 0x058 */
	vuint32_t BUS_CLK_GATING_REG0;		/* 0x060 */
	vuint32_t BUS_CLK_GATING_REG1;		/* 0x064 */
	vuint32_t BUS_CLK_GATING_REG2;		/* 0x068 */
	vuint32_t rsv8[7];								/* 0x06C */
	vuint32_t SDMMC0_CLK_REG;					/* 0x088 */
	vuint32_t SDMMC1_CLK_REG;					/* 0x08C */
	vuint32_t rsv9[8];								/* 0x090 */
	vuint32_t DAUDIO_CLK_REG;					/* 0x0B0 */
	vuint32_t OWA_CLK_REG;						/* 0x0B4 */
	vuint32_t CIR_CLK_REG;						/* 0x0B8 */
	vuint32_t rsv10[4];								/* 0x0BC */
	vuint32_t USBPHY_CLK_REG;					/* 0x0CC */
	vuint32_t rsv11[12];							/* 0x0D0 */
	vuint32_t DRAM_GATING_REG;				/* 0x100 */
	vuint32_t BE_CLK_REG;							/* 0x104 */
	vuint32_t rsv12;									/* 0x108 */
	vuint32_t FE_CLK_REG;							/* 0x10C */
	vuint32_t rsv13[2];								/* 0x110 */
	vuint32_t TCON_CLK_REG;						/* 0x118 */
	vuint32_t DI_CLK_REG;							/* 0x11C */
	vuint32_t TVE_CLK_REG;						/* 0x120 */
	vuint32_t TVD_CLK_REG;						/* 0x124 */
	vuint32_t rsv14[3];								/* 0x128 */
	vuint32_t CSI_CLK_REG;						/* 0x134 */
	vuint32_t rsv15;									/* 0x138 */
	vuint32_t VE_CLK_REG;							/* 0x13C */
	vuint32_t AUDIO_CODEC_CLK_REG;		/* 0x140 */
	vuint32_t AVS_CLK_REG;						/* 0x144 */
	vuint32_t rsv16[46];							/* 0x148 */
	vuint32_t PLL_STABLE_TIME_REG0;		/* 0x200 */
	vuint32_t PLL_STABLE_TIME_REG1;		/* 0x204 */
	vuint32_t rsv17[6];								/* 0x208 */
	vuint32_t PLL_CPU_BIAS_REG;				/* 0x220 */
	vuint32_t PLL_AUDIO_BIAS_REG;			/* 0x224 */
	vuint32_t PLL_VIDEO_BIAS_REG;			/* 0x228 */
	vuint32_t PLL_VE_BIAS_REG;				/* 0x22C */
	vuint32_t PLL_DDR_BIAS_REG;				/* 0x230 */
	vuint32_t PLL_PERIPH_BIAS_REG;		/* 0x234 */
	vuint32_t rsv18[6];								/* 0x238 */
	vuint32_t PLL_CPU_TUN_REG;				/* 0x250 */
	vuint32_t rsv19[3];								/* 0x254 */
	vuint32_t PLL_DDR_TUN_REG;				/* 0x260 */
	vuint32_t rsv20[8];								/* 0x264 */
	vuint32_t PLL_AUDIO_PAT_CTRL_REG; /* 0x284 */
	vuint32_t PLL_VIDEO_PAT_CTRL_REG; /* 0x288 */
	vuint32_t rsv21;									/* 0x28C */
	vuint32_t PLL_DDR_PAT_CTRL_REG;		/* 0x290 */
	vuint32_t rsv22[11];							/* 0x294 */
	vuint32_t BUS_SOFT_RST_REG0;			/* 0x2C0 */
	vuint32_t BUS_SOFT_RST_REG1;			/* 0x2C4 */
	vuint32_t rsv23[2];								/* 0x2C8 */
	vuint32_t BUS_SOFT_RST_REG2;			/* 0x2D0 */
} CCU_Type;

typedef struct
{
	vuint32_t CFG0; //Configure Register 0
	vuint32_t CFG1; //Configure Register 1
	vuint32_t CFG2; //Configure Register 2
	vuint32_t CFG3; //Configure Register 3
	vuint32_t DATA; //Data Register
	vuint32_t DRV0; //Multi-Driving Register 0
	vuint32_t DRV1; //Multi-Driving Register 1
	vuint32_t PUL0; //Pull Register 0
	vuint32_t PUL1; //Pull Register 1
} GPIO_Type;

typedef struct
{
	vuint32_t PIO_INT_CFG0; //PIO Interrupt Configure Register 0
	vuint32_t PIO_INT_CFG1; //PIO Interrupt Configure Register 1
	vuint32_t PIO_INT_CFG2; //PIO Interrupt Configure Register 2
	vuint32_t PIO_INT_CFG3; //PIO Interrupt Configure Register 3
	vuint32_t PIO_INT_CTRL; //PIO Interrupt Control Register
	vuint32_t PIO_INT_STA;	//PIO Interrupt Status Register
	vuint32_t PIO_INT_DEB;	//PIO Interrupt Debounce Register
} GPIO_INT_Type;

typedef struct
{
	vuint32_t MR; //Multi-Driving Register
	vuint32_t PR; //Pull Register
} GPIO_DDR_Type;

typedef struct
{
	vuint32_t rsv1;
	vuint32_t SPI_GCR_REG; //Global Control Register
	vuint32_t SPI_TCR_REG; //Transfer Control Register
	vuint32_t rsv2;
	vuint32_t SPI_IER_REG; //Interrupt Control Register
	vuint32_t SPI_ISR_REG; //Interrupt Status Registe
	vuint32_t SPI_FCR_REG; //FIFO Control Register
	vuint32_t SPI_FSR_REG; //FIFO Status Register
	vuint32_t SPI_WCR_REG; //Wait Clock Counter Register
	vuint32_t SPI_CCR_REG; //Clock Rate Control Register
	vuint32_t rsv3[2];
	vuint32_t SPI_MBC_REG; //Burst Counter Register
	vuint32_t SPI_MTC_REG; //Transmit Counter Register
	vuint32_t SPI_BCC_REG; //Burst Control Counter Register
	vuint32_t rsv4[113];
	vuint8_t SPI_TXD_REG; //TX Data Register
	vuint8_t rsv5[3];
	vuint32_t rsv6[63];
	vuint8_t SPI_RXD_REG; //RX Data Register
} SPI_Type;

typedef struct
{
	vuint32_t TVE_000; /* 0x000 */
	vuint32_t TVE_004; /* 0x004 */
	vuint32_t TVE_008; /* 0x008 */
	vuint32_t TVE_00C; /* 0x00c */
	vuint32_t TVE_010; /* 0x010 */
	vuint32_t TVE_014; /* 0x014 */
	vuint32_t TVE_018; /* 0x018 */
	vuint32_t TVE_01C; /* 0x01c */
	vuint32_t TVE_020; /* 0x020 */
	vuint32_t TVE_024; /* 0x024 */
	vuint32_t TVE_028; /* 0x028 */
	vuint32_t TVE_02C; /* 0x02c */
	vuint32_t TVE_030; /* 0X030 */
	vuint32_t TVE_034; /* 0x034 */
	vuint32_t TVE_038; /* 0x038 */
	vuint32_t TVE_03C; /* 0x03c */
	vuint32_t TVE_040; /* 0x040 */
	vuint32_t TVE_044; /* 0x044 */
	vuint32_t TVE_048; /* 0x048 */
	vuint32_t TVE_04C; /* 0x04c */
	vuint32_t TVE_050; /* 0x050 */
	vuint32_t TVE_054; /* 0x054 */
	vuint32_t TVE_058; /* 0x058 */
	vuint32_t TVE_05C; /* 0x05c */
	vuint32_t TVE_060; /* 0x060 */
	vuint32_t TVE_064; /* 0x064 */
	vuint32_t TVE_068; /* 0x068 */
	vuint32_t TVE_06C; /* 0x06c */
	vuint32_t TVE_070; /* 0x070 */
	vuint32_t TVE_074; /* 0x074 */
	vuint32_t TVE_078; /* 0x078 */
	vuint32_t TVE_07C; /* 0x07c */
	vuint32_t TVE_080; /* 0x080 */
	vuint32_t TVE_084; /* 0x084 */
	vuint32_t TVE_088; /* 0x088 */
	vuint32_t TVE_08C; /* 0x08c */
	vuint32_t TVE_090; /* 0x090 */
	vuint32_t TVE_094; /* 0x094 */
	vuint32_t TVE_098; /* 0x098 */
	vuint32_t TVE_09C; /* 0x09c */
	vuint32_t TVE_0A0; /* 0x0A0 */
	vuint32_t TVE_0A4; /* 0x0A4 */
	vuint32_t TVE_0A8; /* 0x0A8 */
	vuint32_t TVE_0AC; /* 0x0Ac */
	vuint32_t TVE_0B0; /* 0x0B0 */
	vuint32_t TVE_0B4; /* 0x0B4 */
	vuint32_t TVE_0B8; /* 0x0B8 */
	vuint32_t TVE_0BC; /* 0x0Bc */
	vuint32_t TVE_0C0; /* 0x0C0 */
	vuint32_t TVE_0C4; /* 0x0C4 */
	vuint32_t TVE_0C8; /* 0x0C8 */
	vuint32_t TVE_0CC; /* 0x0Cc */
	vuint32_t TVE_0D0; /* 0x0d0 */
	vuint32_t TVE_0D4; /* 0x0d4 */
	vuint32_t TVE_0D8; /* 0x0d8 */
	vuint32_t TVE_0DC; /* 0x0dc */
	vuint32_t TVE_0E0; /* 0x0e0 */
	vuint32_t TVE_0E4; /* 0x0e4 */
	vuint32_t TVE_0E8; /* 0x0e8 */
	vuint32_t TVE_0EC; /* 0x0ec */
	vuint32_t TVE_0F0; /* 0x0f0 */
	vuint32_t TVE_0F4; /* 0x0f4 */
	vuint32_t TVE_0F8; /* 0x0f8 */
	vuint32_t TVE_0FC; /* 0x0fc */
	vuint32_t TVE_100; /* 0x100 */
	vuint32_t TVE_104; /* 0x104 */
	vuint32_t TVE_108; /* 0x108 */
	vuint32_t TVE_10C; /* 0x10c */
	vuint32_t TVE_110; /* 0x110 */
	vuint32_t TVE_114; /* 0x114 */
	vuint32_t TVE_118; /* 0x118 */
	vuint32_t TVE_11C; /* 0x11c */
	vuint32_t TVE_120; /* 0x120 */
	vuint32_t TVE_124; /* 0x124 */
	vuint32_t TVE_128; /* 0x128 */
	vuint32_t TVE_12C; /* 0x12c */
	vuint32_t TVE_130; /* 0x130 */
	vuint32_t TVE_134; /* 0x134 */
	vuint32_t TVE_138; /* 0x138 */
	vuint32_t TVE_13C; /* 0x13C */
	vuint32_t res0[0x70];
	vuint32_t TVE_300; /* 0x300 */
	vuint32_t TVE_304; /* 0x304 */
	vuint32_t TVE_308; /* 0x308 */
	vuint32_t res1[0x1D];
	vuint32_t TVE_380; /* 0x380 */
	vuint32_t res2[0x7];
	vuint32_t TVE_3A0; /* 0x3a0 */
} TVE_Type;

typedef struct
{
	vuint32_t UART_RBR_THR_DLL_REG; /* Receiver Buffer Register
                                         * Transmit Holding Register
                                         * Divisor Latch Low Register
                                         */
	vuint32_t UART_DLH_IER_REG;			/* Divisor Latch High Register
                                         * Interrupt Enable Register
                                         */
	vuint32_t UART_IIR_FCR_REG;			/* Interrupt Identity Register
                                         * FIFO Control Register
                                         */
	vuint32_t UART_LCR_REG;					//Line Control Register
	vuint32_t UART_MCR_REG;					//Modem Control Register
	vuint32_t UART_LSR_REG;					//Line Status Register
	vuint32_t UART_MSR_REG;					//Modem Status Register
	vuint32_t UART_SCH_REG;					//Scratch Register
	vuint32_t rsv1[23];
	vuint32_t UART_USR_REG; //Status Register
	vuint32_t UART_TFL_REG; //Transmit FIFO Level Register
	vuint32_t UART_RFL_REG; //Receive FIFO Level Register
	vuint32_t UART_HSK_REG; //DMA Handshake Config Register
	vuint32_t rsv2[6];
	vuint32_t UART_HALT_REG; //Halt TX Register
	vuint32_t rsv3[2];
	vuint32_t UART_DBG_DLL_REG; //Debug DLL Register
	vuint32_t UART_DBG_DLH_REG; //Debug DLH Register
} UART_Type;

typedef struct
{
	vuint32_t TIME_CTRL_REG;			 //0x10
	vuint32_t TIME_INTV_VALUE_REG; //0x14
	vuint32_t TIME_CUR_VALUE_REG;	 //0x18
	vuint32_t rsv[1];							 //
} Timer_Type;

typedef struct
{
	vuint32_t TMR_IRQ_EN_REG;		//0x00
	vuint32_t TMR_IRQ_STA_REG;	//0x04
	vuint32_t rsv1[2];					//
	Timer_Type TIME0;						//0x10
	Timer_Type TIME1;						//0x20
	Timer_Type TIME2;						//0x30
	vuint32_t rsv4[16];					//
	vuint32_t AVS_CNT_CTL_REG;	//0x80
	vuint32_t AVS_CNT0_REG;			//0x84
	vuint32_t AVS_CNT1_REG;			//0x88
	vuint32_t AVS_CNT_DIV_REG;	//0x8C
	vuint32_t rsv5[4];					//
	vuint32_t WDOG_IRQ_EN_REG;	//0xA0
	vuint32_t WDOG_IRQ_STA_REG; //0xA4
	vuint32_t rsv6[2];					//
	vuint32_t WDOG_CTRL_REG;		//0xB0
	vuint32_t WDOG_CFG_REG;			//0xB4
	vuint32_t WDOG_MODE_REG;		//0xB8
} TimerStruct_Type;

typedef struct
{
	vuint32_t INTC_VECTOR_REG;		//0x00
	vuint32_t INTC_BASE_ADDR_REG; //0x04
	vuint32_t rsv1[1];						//
	vuint32_t NMI_INT_CTRL_REG;		//0x0C
	vuint32_t INTC_PEND_REG0;			//0x10
	vuint32_t INTC_PEND_REG1;			//0x14
	vuint32_t rsv2[2];						//
	vuint32_t INTC_EN_REG0;				//0x20
	vuint32_t INTC_EN_REG1;				//0x24
	vuint32_t rsv3[2];						//
	vuint32_t INTC_MASK_REG0;			//0x30
	vuint32_t INTC_MASK_REG1;			//0x34
	vuint32_t rsv4[2];						//
	vuint32_t INTC_RESP_REG0;			//0x40
	vuint32_t INTC_RESP_REG1;			//44
	vuint32_t rsv5[2];						//
	vuint32_t INTC_FF_REG0;				//0x50
	vuint32_t INTC_FF_REG1;				//0x54
	vuint32_t rsv6[2];						//
	vuint32_t INTC_PRIO_REG0;			//0x60
	vuint32_t INTC_PRIO_REG1;			//0x64
	vuint32_t INTC_PRIO_REG2;			//0x68
	vuint32_t INTC_PRIO_REG3;			//0x6C
} INTC_Type;

typedef struct
{
	vuint32_t TWI_ADDR_REG;	 //0x00
	vuint32_t TWI_XADDR_REG; //0x04
	vuint32_t TWI_DATA_REG;	 //0x08
	vuint32_t TWI_CNTR_REG;	 //0x0C
	vuint32_t TWI_STAT_REG;	 //0x10
	vuint32_t TWI_CCR_REG;	 //0x14
	vuint32_t TWI_SRST_REG;	 //0x18
	vuint32_t TWI_EFR_REG;	 //0x1C
	vuint32_t TWI_LCR_REG;	 //0x20
} I2C_Type;

typedef struct
{
	vuint32_t CSI_EN_REG;					//0x00
	vuint32_t CSI_CFG_REG;				//0x04
	vuint32_t CSI_CAP_REG;				//0x08
	vuint32_t CSI_SCALE_REG;			//0x0C
	vuint32_t CSI_FIFO0_BUFA_REG; //0x10
	vuint32_t CSI_FIFO0_BUFB_REG; //0x14
	vuint32_t CSI_FIFO1_BUFA_REG; //0x18
	vuint32_t CSI_FIFO1_BUFB_REG; //0x1C
	vuint32_t CSI_FIFO2_BUFA_REG; //0x20
	vuint32_t CSI_FIFO2_BUFB_REG; //0x24
	vuint32_t CSI_BUF_CTL_REG;		//0x28
	vuint32_t CSI_BUF_STA_REG;		//0x2C
	vuint32_t CSI_INT_EN_REG;			//0x30
	vuint32_t CSI_INT_STA_REG;		//0x34
	vuint32_t rsv1[2];						//
	vuint32_t CSI_HSIZE_REG;			//0x40
	vuint32_t CSI_VSIZE_REG;			//0x44
	vuint32_t CSI_BUF_LEN_REG;		//0x48
} CSI_Type;

typedef struct
{
	vuint32_t SD_GCTL_REG;		//0x00
	vuint32_t SD_CKCR_REG;		//0x04
	vuint32_t SD_TMOR_REG;		//0x08
	vuint32_t SD_BWDR_REG;		//0x0C
	vuint32_t SD_BKSR_REG;		//0x10
	vuint32_t SD_BYCR_REG;		//0x14
	vuint32_t SD_CMDR_REG;		//0x18
	vuint32_t SD_CAGR_REG;		//0x1C
	vuint32_t SD_RESP0_REG;		//0x20
	vuint32_t SD_RESP1_REG;		//0x24
	vuint32_t SD_RESP2_REG;		//0x28
	vuint32_t SD_RESP3_REG;		//0x2C
	vuint32_t SD_IMKR_REG;		//0x30
	vuint32_t SD_MISR_REG;		//0x34
	vuint32_t SD_RISR_REG;		//0x38
	vuint32_t SD_STAR_REG;		//0x3C
	vuint32_t SD_FWLR_REG;		//0x40
	vuint32_t SD_FUNS_REG;		//0x44
	vuint32_t SD_CBCR_REG;		//0x48
	vuint32_t SD_BBCR_REG;		//0x4C
	vuint32_t SD_DBGC_REG;		//0x50
	vuint32_t rsv1[1];				//
	vuint32_t SD_A12A_REG;		//0x58
	vuint32_t rsv2[7];				//
	vuint32_t SD_HWRST_REG;		//0x78
	vuint32_t rsv3[1];				//
	vuint32_t SD_DMAC_REG;		//0x80
	vuint32_t SD_DLBA_REG;		//0x84
	vuint32_t SD_IDST_REG;		//0x88
	vuint32_t SD_IDIE_REG;		//0x8C
	vuint32_t SD_CHDA_REG;		//0x90
	vuint32_t SD_CBDA_REG;		//0x94
	vuint32_t rsv4[26];				//
	vuint32_t CARD_THLDC_REG; //0x100
	vuint32_t rsv5[2];				//
	vuint32_t EMMC_DSBD_REG;	//0x10C
	vuint32_t rsv6[60];				//
	vuint32_t SD_FIFO_REG;		//0x200
} SDC_Type;

typedef struct
{
	vuint32_t AC_DAC_DPC_REG;		 //0x00
	vuint32_t AC_DAC_FIFOC_REG;	 //0x04
	vuint32_t AC_DAC_FIFOS_REG;	 //0x08
	vuint32_t AC_DAC_TXDATA_REG; //0x0C
	vuint32_t AC_ADC_FIFOC_REG;	 //0x10
	vuint32_t AC_ADC_FIFOS_REG;	 //0x14
	vuint32_t AC_ADC_RXDATA_REG; //0x18
	vuint32_t rsv1[1];
	vuint32_t DAC_MIXER_CTRL_REG;			 //0x20
	vuint32_t ADC_MIXER_CTRL_REG;			 //0x24
	vuint32_t ADDA_TUNE_REG;					 //0x28
	vuint32_t BIAS_DA16_CAL_CTRL_REG0; //0x2C
	vuint32_t rsv2[1];
	vuint32_t BIAS_DA16_CAL_CTRL_REG1; //0x34
	vuint32_t rsv3[2];
	vuint32_t AC_DAC_CNT_REG; //0x40
	vuint32_t AC_ADC_CNT_REG; //0x44
	vuint32_t AC_DAC_DG_REG;	//0x48
	vuint32_t AC_ADC_DG_REG;	//0x4C
	vuint32_t rsv4[8];
	vuint32_t AC_ADC_DAP_CTR_REG;	 //0x70
	vuint32_t AC_ADC_DAP_LCTR_REG; //0x74
	vuint32_t AC_ADC_DAP_RCTR_REG; //0x78
	vuint32_t AC_ADC_DAP_PARA_REG; //0x7C
	vuint32_t AC_ADC_DAP_LAC_REG;	 //0x80
	vuint32_t AC_ADC_DAP_LDAT_REG; //0x84
	vuint32_t AC_ADC_DAP_RAC_REG;	 //0x88
	vuint32_t AC_ADC_DAP_RDAT_REG; //0x8C
	vuint32_t ADC_DAP_HPFC_REG;		 //0x90
	vuint32_t ADC_DAP_LINAC_REG;	 //0x94
	vuint32_t ADC_DAP_RINAC_REG;	 //0x98
	vuint32_t ADC_DAP_ORT_REG;		 //0x9C
} Audio_Type;

typedef struct
{
	vuint32_t NDMA_CFG_REG;			 //0x00
	vuint32_t NDMA_SRC_ADR_REG;	 //0x04
	vuint32_t NDMA_DES_ADR_REG;	 //0x08
	vuint32_t NDMA_BYTE_CNT_REG; //0x0C
	vuint32_t rsv[4];
} NDMA_Type;

typedef struct
{
	vuint32_t DDMA_CFG_REG;			 //0x00
	vuint32_t DDMA_SRC_ADR_REG;	 //0x04
	vuint32_t DDMA_DES_ADR_REG;	 //0x08
	vuint32_t DDMA_BYTE_CNT_REG; //0x0C
	vuint32_t rsv[2];
	vuint32_t DDMA_PAR_REG;	 //0x18
	vuint32_t DDMA_GEN_DATA; //0x1C
} DDMA_Type;

typedef struct
{
	vuint32_t DMA_INT_CTRL_REG; //0x00
	vuint32_t DMA_INT_STA_REG;	//0x04
	vuint32_t DMA_PTY_CFG_REG;	//0x08
	vuint32_t rsv[61];
	NDMA_Type nDMA[4];
	DDMA_Type dDMA[4];
} DMA_Type;

typedef struct
{
	vuint32_t KEYADC_CTRL_REG; //0x00
	vuint32_t KEYADC_INTC_REG; //0x04
	vuint32_t KEYADC_INTS_REG; //0x08
	vuint32_t KEYADC_DATA_REG; //0x0C
} KEYADC_Type;

typedef struct
{
	vuint32_t rsv;
} IRQn_Type;

typedef struct
{
	vuint8_t res0[0x800];								/* 0x000 */
	vuint32_t DEBE_MODE_CTRL_REG;				/* 0x800 */
	vuint32_t DEBE_COLOR_CTRL_REG;			/* 0x804 */
	vuint32_t disp_size;								/* 0x808 */
	vuint32_t res1[1];									/* 0x80c */
	vuint32_t DEBE_LAY0_SIZE_REG;				/* 0x810 */
	vuint32_t DEBE_LAY1_SIZE_REG;				/* 0x814 */
	vuint32_t DEBE_LAY2_SIZE_REG;				/* 0x818 */
	vuint32_t DEBE_LAY3_SIZE_REG;				/* 0x81c */
	vuint32_t DEBE_LAY0_CODNT_REG;			/* 0x820 */
	vuint32_t DEBE_LAY1_CODNT_REG;			/* 0x824 */
	vuint32_t DEBE_LAY2_CODNT_REG;			/* 0x828 */
	vuint32_t DEBE_LAY3_CODNT_REG;			/* 0x82c */
	vuint32_t res2[4];									/* 0x830 */
	vuint32_t DEBE_LAY0_LINEWIDTH_REG;	/* 0x840 */
	vuint32_t DEBE_LAY1_LINEWIDTH_REG;	/* 0x844 */
	vuint32_t DEBE_LAY2_LINEWIDTH_REG;	/* 0x848 */
	vuint32_t DEBE_LAY3_LINEWIDTH_REG;	/* 0x84c */
	vuint32_t DEBE_LAY0_FB_ADDR_REG;		/* 0x850 */
	vuint32_t DEBE_LAY1_FB_ADDR_REG;		/* 0x854 */
	vuint32_t DEBE_LAY2_FB_ADDR_REG;		/* 0x858 */
	vuint32_t DEBE_LAY3_FB_ADDR_REG;		/* 0x85c */
	vuint32_t layer0_addr_high4b;				/* 0x860 */
	vuint32_t layer1_addr_high4b;				/* 0x864 */
	vuint32_t layer2_addr_high4b;				/* 0x868 */
	vuint32_t layer3_addr_high4b;				/* 0x86c */
	vuint32_t DEBE_REGBUFF_CTRL_REG;		/* 0x870 */
	vuint32_t res3[3];									/* 0x874 */
	vuint32_t DEBE_CK_MAX_REG;					/* 0x880 */
	vuint32_t DEBE_CK_MIN_REG;					/* 0x884 */
	vuint32_t DEBE_CK_CFG_REG;					/* 0x888 */
	vuint32_t res4[1];									/* 0x88c */
	vuint32_t DEBE_LAY0_ATT_CTRL_REG0;	/* 0x890 */
	vuint32_t DEBE_LAY1_ATT_CTRL_REG0;	/* 0x894 */
	vuint32_t DEBE_LAY2_ATT_CTRL_REG0;	/* 0x898 */
	vuint32_t DEBE_LAY3_ATT_CTRL_REG0;	/* 0x89c */
	vuint32_t DEBE_LAY0_ATT_CTRL_REG1;	/* 0x8a0 */
	vuint32_t DEBE_LAY1_ATT_CTRL_REG1;	/* 0x8a4 */
	vuint32_t DEBE_LAY2_ATT_CTRL_REG1;	/* 0x8a8 */
	vuint32_t DEBE_LAY3_ATT_CTRL_REG1;	/* 0x8ac */
	vuint32_t res5[10];									/* 0x8b0 */
	vuint32_t DEBE_HWC_CTRL_REG;				/* 0x8D8 */
	vuint32_t res6[1];									/* 0x8DC */
	vuint32_t DEBE_HWCFB_CTRL_REG;			/* 0x8E0 */
	vuint32_t res7[3];									/* 0x8E4 */
	vuint32_t DEBE_WB_CTRL_REG;					/* 0x8F0 */
	vuint32_t DEBE_WB_ADDR_REG;					/* 0x8F4 */
	vuint32_t DEBE_WB_LW_REG;						/* 0x8F8 */
	vuint32_t res8[9];									/* 0x8FC */
	vuint32_t DEBE_IYUV_CH_CTRL_REG;		/* 0x920 */
	vuint32_t res9[3];									/* 0x924 */
	vuint32_t DEBE_CH0_YUV_FB_ADDR_REG; /* 0x930 */
	vuint32_t DEBE_CH1_YUV_FB_ADDR_REG; /* 0x934 */
	vuint32_t DEBE_CH2_YUV_FB_ADDR_REG; /* 0x938 */
	vuint32_t res10[1];									/* 0x93C */
	vuint32_t DEBE_CH0_YUV_BLW_REG;			/* 0x940 */
	vuint32_t DEBE_CH1_YUV_BLW_REG;			/* 0x944 */
	vuint32_t DEBE_CH2_YUV_BLW_REG;			/* 0x948 */
	vuint32_t res11[1];									/* 0x94C */
	vuint32_t DEBE_COEF00_REG;					/* 0x950 */
	vuint32_t DEBE_COEF01_REG;					/* 0x954 */
	vuint32_t DEBE_COEF02_REG;					/* 0x958 */
	vuint32_t DEBE_COEF03_REG;					/* 0x95c */
	vuint32_t DEBE_COEF10_REG;					/* 0x960 */
	vuint32_t DEBE_COEF11_REG;					/* 0x964 */
	vuint32_t DEBE_COEF12_REG;					/* 0x968 */
	vuint32_t DEBE_COEF13_REG;					/* 0x96c */
	vuint32_t DEBE_COEF20_REG;					/* 0x970 */
	vuint32_t DEBE_COEF21_REG;					/* 0x974 */
	vuint32_t DEBE_COEF22_REG;					/* 0x978 */
	vuint32_t DEBE_COEF23_REG;					/* 0x97c */
} DEBE_Type;

typedef struct
{
	vuint32_t TCON_CTRL_REG;					 /* 0x00 */
	vuint32_t TCON_INT_REG0;					 /* 0x04 */
	vuint32_t TCON_INT_REG1;					 /* 0x08 */
	vuint32_t res0[1];								 /* 0x0c */
	vuint32_t TCON_FRM_CTRL_REG;			 /* 0x10 */
	vuint32_t TCON_FRM_SEED0_R_REG;		 /* 0x14 */
	vuint32_t TCON_FRM_SEED0_G_REG;		 /* 0x18 */
	vuint32_t TCON_FRM_SEED0_B_REG;		 /* 0x1c */
	vuint32_t TCON_FRM_SEED1_R_REG;		 /* 0x20 */
	vuint32_t TCON_FRM_SEED1_G_REG;		 /* 0x24 */
	vuint32_t TCON_FRM_SEED1_B_REG;		 /* 0x28 */
	vuint32_t TCON_FRM_TBL_REG0;			 /* 0x2c */
	vuint32_t TCON_FRM_TBL_REG1;			 /* 0x30 */
	vuint32_t TCON_FRM_TBL_REG2;			 /* 0x34 */
	vuint32_t TCON_FRM_TBL_REG3;			 /* 0x38 */
	vuint32_t res1[1];								 /* 0x3c */
	vuint32_t TCON0_CTRL_REG;					 /* 0x40 */
	vuint32_t TCON_CLK_CTRL_REG;			 /* 0x44 */
	vuint32_t TCON0_BASIC_TIMING_REG0; /* 0x48 */
	vuint32_t TCON0_BASIC_TIMING_REG1; /* 0x4c */
	vuint32_t TCON0_BASIC_TIMING_REG2; /* 0x50 */
	vuint32_t TCON0_BASIC_TIMING_REG3; /* 0x54 */
	vuint32_t TCON0_HV_TIMING_REG;		 /* 0x58 */
	vuint32_t res2[1];								 /* 0x5c */
	vuint32_t TCON0_CPU_IF_REG;				 /* 0x60 */
	vuint32_t TCON0_CPU_WR_REG;				 /* 0x64 */
	vuint32_t TCON0_CPU_RD_REG;				 /* 0x68 */
	vuint32_t TCON0_CPU_RD_NX_REG;		 /* 0x6c */
	vuint32_t res3[6];								 /* 0x70 */
	vuint32_t TCON0_IO_CTRL_REG0;			 /* 0x88 */
	vuint32_t TCON0_IO_CTRL_REG1;			 /* 0x8c */
	vuint32_t TCON1_CTRL_REG;					 /* 0x90 */
	vuint32_t TCON1_BASIC_REG0;				 /* 0x94 */
	vuint32_t TCON1_BASIC_REG1;				 /* 0x98 */
	vuint32_t TCON1_BASIC_REG2;				 /* 0x9c */
	vuint32_t TCON1_BASIC_REG3;				 /* 0xa0 */
	vuint32_t TCON1_BASIC_REG4;				 /* 0xa4 */
	vuint32_t TCON1_BASIC_REG5;				 /* 0xa8 */
	vuint32_t res4[17];								 /* 0xac */
	vuint32_t TCON1_IO_CTRL_REG0;			 /* 0xf0 */
	vuint32_t TCON1_IO_CTRL_REG1;			 /* 0xf4 */
	vuint32_t res5[1];								 /* 0xf8 */
	vuint32_t TCON_DEBUG_INFO_REG;		 /* 0xfc */
} TCON_Type;

typedef struct {
  vuint32_t CTRL;           // 0x00 Sub-Engine & RAM Type Select
  vuint32_t RST;            // 0x04 Sub-Engines Reset
  vuint32_t RES[58];
  vuint32_t VERSION;        // 0xF0 IP Version register
} VE_Type;

typedef struct {
  vuint32_t PHDR;           // 0x00 MPEG12 Picture Header register
  vuint32_t VOPHDR;         // 0x04 MPEG Video Object Plane Header
  vuint32_t SIZE;           // 0x08 Frame size in MPEG macroblocks (16x16)
  vuint32_t FRAME_SIZE;     // 0x0C Frame size in pixels
  vuint32_t MBA;            // 0x10 MPEG Macro Block Address register
  vuint32_t CTRL;           // 0x14 MPEG Control Register
  vuint32_t TRIG;           // 0x18 MPEG Decoding Trigger
  vuint32_t STATUS;         // 0x1C MACC MPEG Status register
  vuint32_t TRBTRD_FIELD;   // 0x20 Temporal References (TRB(B-VOP)&TRD)
  vuint32_t TRBTRD_FRAME;   // 0x24
  vuint32_t VLD_ADDR;       // 0x28 MPEG Variable Length Decoding Address
  vuint32_t VLD_OFFSET;     // 0x2C MPEG Variable Length Decoding Offset
  vuint32_t VLD_LEN;        // 0x30 MPEG Variable Length Decoding Length
  vuint32_t VLD_END;        // 0x34 MPEG video source buffer end
  vuint32_t MBH_ADDR;       // 0x38 MBH buffer address
  vuint32_t DCAC_ADDR;      // 0x3C DCAC Buffer address
  vuint32_t BLK_OFFSET;     // 0x40 MPEG Block address???
  vuint32_t NCF_ADDR;       // 0x44 NFC buffer address
  vuint32_t REC_LUMA;       // 0x48 MPEG Luma reconstruct buffer
  vuint32_t REC_CHROMA;     // 0x4C MPEG Chroma reconstruct buffer
  vuint32_t FWD_LUMA;       // 0x50 MPEG Luma forward buffer
  vuint32_t FWD_CHROMA;     // 0x54 MPEG Chroma forward buffer
  vuint32_t BACK_LUMA;      // 0x58 MPEG Luma Back buffer
  vuint32_t BACK_CHROMA;    // 0x5C MPEG Chroma Back buffer
  vuint32_t RES0[8];
  vuint32_t IQ_MIN_INPUT;   // 0x80 MPEG Inverse Quantization min input level
  vuint32_t QP_INPUT;       // 0x84 MPEG Inverse Quantization input level
  vuint32_t RES1[12];
  vuint32_t JPEG_SIZE;      // 0xB8 JPEG Size
  vuint32_t JPEG_MCU;       // 0xBC JPEG Minimum Coded Unit
  vuint32_t JPEG_RES_INT;   // 0xC0 JPEG Restart Interval
  vuint32_t RES2[2];
  vuint32_t ROT_LUMA;       // 0xCC MPEG Rotate-Scale Luma buffer
  vuint32_t ROT_CHROMA;     // 0xD0 MPEG Rotate-Scale Chroma buffer
  vuint32_t ROT_SCALE_CTRL; // 0xD4 Control Rotate/Scale Buffe
  vuint32_t JPEG_MCU_START; // 0xD8 JPEG Macro Cell Unit Start
  vuint32_t JPEG_MCU_END;   // 0xDC JPEG Macro Cell Unit End
  vuint32_t RAM_WRITE_PTR;  // 0xE0 Auto incremental pointer for rd/wr VE SRAM
  vuint32_t RAM_WRITE_DATA; // 0xE4 FIFO Like Data register for wr/rd VE SRAM
} MPEG_Type;

typedef struct
{
	vuint32_t TP_CTRL_REG0;					/* 0x000 */
	vuint32_t TP_CTRL_REG1;					/* 0x004 */
	vuint32_t TP_CTRL_REG2;					/* 0x008 */
	vuint32_t TP_CTRL_REG3;					/* 0x00c */
	vuint32_t TP_INT_FIFO_CTRL_REG; /* 0x010 */
	vuint32_t TP_INT_FIFO_STAT_REG; /* 0x014 */
	vuint32_t res1;									/* 0x018 */
	vuint32_t TP_COM_DATA_REG;			/* 0x01c */
	vuint32_t res2;									/* 0x020 */
	vuint32_t TP_DATA_REG;					/* 0x024 */
} TP_Type;

#define CCU ((CCU_Type *)(CCU_BASE + 0x0))

#define GPIOA ((GPIO_Type *)(PIO_BASE + 0 * 0x24))
#define GPIOB ((GPIO_Type *)(PIO_BASE + 1 * 0x24))
#define GPIOC ((GPIO_Type *)(PIO_BASE + 2 * 0x24))
#define GPIOD ((GPIO_Type *)(PIO_BASE + 3 * 0x24))
#define GPIOE ((GPIO_Type *)(PIO_BASE + 4 * 0x24))
#define GPIOF ((GPIO_Type *)(PIO_BASE + 5 * 0x24))

// #define GPIOA_INT   ((GPIO_INT_Type *)(PIO_BASE + 0x200 + 0*0x20))
// #define GPIOB_INT   ((GPIO_INT_Type *)(PIO_BASE + 0x200 + 1*0x20))
// #define GPIOC_INT   ((GPIO_INT_Type *)(PIO_BASE + 0x200 + 2*0x20))
#define GPIOD_INT ((GPIO_INT_Type *)(PIO_BASE + 0x200 + 0 * 0x20))
#define GPIOE_INT ((GPIO_INT_Type *)(PIO_BASE + 0x200 + 1 * 0x20))
#define GPIOF_INT ((GPIO_INT_Type *)(PIO_BASE + 0x200 + 2 * 0x20))

#define GPIO_DDR ((GPIO_DDR_Type *)(PIO_BASE + 0x2C0))

#define SPI0 ((SPI_Type *)(SPI_BASE + 0x0))
#define SPI1 ((SPI_Type *)(SPI_BASE + 0x1000))

#define TVE ((TVE_Type *)TVE_BASE)

#define UART0 ((UART_Type *)(UART_BASE + 0x0))
#define UART1 ((UART_Type *)(UART_BASE + 0x400))
#define UART2 ((UART_Type *)(UART_BASE + 0x800))

#define TIMER ((TimerStruct_Type *)TIMER_BASE)
#define TIMER0 ((Timer_Type *)(TIMER_BASE + 0x10))
#define TIMER1 ((Timer_Type *)(TIMER_BASE + 0x20))
#define TIMER2 ((Timer_Type *)(TIMER_BASE + 0x30))

#define INTC ((INTC_Type *)INTC_BASE)

#define I2C0 ((I2C_Type *)(TWI_BASE + 0x0))
#define I2C1 ((I2C_Type *)(TWI_BASE + 0x400))
#define I2C2 ((I2C_Type *)(TWI_BASE + 0x800))

#define CSI ((CSI_Type *)CSI_BASE)

#define SDC0 ((SDC_Type *)SDC_BASE)
#define SDC1 ((SDC_Type *)(SDC_BASE + 0x1000))

#define AUDIO ((Audio_Type *)AUDIOC_BASE)

#define DMA ((DMA_Type *)DMA_BASE)

#define KEYADC ((KEYADC_Type *)KEYADC_BASE)

#define DEBE ((DEBE_Type *)DEBE_BASE)

#define TCON ((TCON_Type *)TCON_BASE)

#define VE ((VE_Type *)VE_BASE)

#define MPEG ((MPEG_Type*)MPEG_BASE)

#define TP ((TP_Type *)TP_BASE)

#include <gpio.h>
#include <uart.h>
#include <irq.h>
#include <irq_gpio.h>

#endif // !__F1C100S_H_
