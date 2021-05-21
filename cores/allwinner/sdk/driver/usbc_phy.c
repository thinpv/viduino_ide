/*
 * drivers/usb/sunxi_usb/usbc/usbc_phy.c
 * (C) Copyright 2010-2015
 * Allwinner Technology Co., Ltd. <www.allwinnertech.com>
 * daniel, 2009.10.21
 *
 * usb common ops.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 */

#include "sunxi_usb_typedef.h"
#include "sunxi_usb_bsp.h"

#define USBC_PHY_PRINTF printf
/*
 * define USB PHY controller reg bit
 */

//Common Control Bits for Both PHYs
#define USBC_PHY_PLL_BW 0x03
#define USBC_PHY_RES45_CAL_EN 0x0c

//Private Control Bits for Each PHY
#define USBC_PHY_TX_AMPLITUDE_TUNE 0x20
#define USBC_PHY_TX_SLEWRATE_TUNE 0x22
#define USBC_PHY_VBUSVALID_TH_SEL 0x25
#define USBC_PHY_PULLUP_RES_SEL 0x27
#define USBC_PHY_OTG_FUNC_EN 0x28
#define USBC_PHY_VBUS_DET_EN 0x29
#define USBC_PHY_DISCON_TH_SEL 0x2a

// void usb_phy_open_clock(void)
// {
// 	volatile int i;

// 	USBC_PHY_PRINTF("open_usb_clock\r\n");

// 	USBC_REG_set_bit_l(USBPHY_CLK_GAT_BIT, USBPHY_CLK_REG);
// 	USBC_REG_set_bit_l(USBPHY_CLK_RST_BIT, USBPHY_CLK_REG);

// 	USBC_REG_set_bit_l(BUS_CLK_USB_BIT,BUS_CLK_GATE0_REG);
// 	USBC_REG_set_bit_l(BUS_RST_USB_BIT,BUS_CLK_RST_REG);
//     //USBC_Writel(0x0043031a,USBC_REG_PMU_IRQ(USBC0_BASE));
// 	i = 10000;
// 	while(i--);
// }

static void usb_phy_write(int addr, int data, int len)
{
	int j = 0, usbc_bit = 0;
	void *dest = (void *)USBC_REG_CSR(USBC0_BASE);

	//#ifdef CONFIG_MACH_SUN8I_A33
	//	/* CSR needs to be explicitly initialized to 0 on A33 */
	//	writel(0, dest);
	//#endif

	usbc_bit = 1 << (0 * 2);
	for (j = 0; j < len; j++)
	{
		/* set the bit address to be written */
		USBC_ClrBit_Mask_l(dest, 0xff << 8);
		USBC_SetBit_Mask_l(dest, (addr + j) << 8);

		USBC_ClrBit_Mask_l(dest, usbc_bit);
		/* set data bit */
		if (data & 0x1)
			USBC_SetBit_Mask_l(dest, 1 << 7);
		else
			USBC_ClrBit_Mask_l(dest, 1 << 7);

		USBC_SetBit_Mask_l(dest, usbc_bit);

		USBC_ClrBit_Mask_l(dest, usbc_bit);

		data >>= 1;
	}
}
// void USBC_PhyConfig(void)
// {
// 	/* The following comments are machine
// 	 * translated from Chinese, you have been warned!
// 	 */

// 	/* Regulation 45 ohms */
// 	//if (phy->id == 0)
// 	usb_phy_write( 0x0c, 0x01, 1);

// 	/* adjust PHY's magnitude and rate */
// 	usb_phy_write(0x20, 0x14, 5);

// 	/* threshold adjustment disconnect */
// #if defined CONFIG_MACH_SUN5I || defined CONFIG_MACH_SUN7I
// 	usb_phy_write(0x2a, 2, 2);
// #else
// 	usb_phy_write( 0x2a, 3, 2);
// #endif

// }

// int USBC_Dev_Read_EP0_CSR(void)
// {
// 	return USBC_Readw(USBC_REG_CSR0(USBC0_BASE));
// }

void sun4i_usb_phy_set_squelch_detect(int enabled)
{
	usb_phy_write(0x3c, (enabled == 1) ? 0 : 2, 2);
}

void USBC_CoreRegDump(void)
{
	USBC_PHY_PRINTF("\r\n------------usb deugdump-----------\r\n");
	USBC_PHY_PRINTF("USBC_REG_ISCR:0x%08x\r\n", USBC_Readl(USBC_REG_ISCR(USBC0_BASE)));
	USBC_PHY_PRINTF("USBC_REG_PHYCTL:0x%08x\r\n", USBC_Readl(USBC_REG_PHYCTL(USBC0_BASE)));
	USBC_PHY_PRINTF("USBC_REG_PHYBIST:0x%08x\r\n", USBC_Readl(USBC_REG_PHYBIST(USBC0_BASE)));
	USBC_PHY_PRINTF("USBC_REG_PHYTUNE:0x%08x\r\n", USBC_Readl(USBC_REG_PHYTUNE(USBC0_BASE)));
	USBC_PHY_PRINTF("USBC_REG_PMU_IRQ:0x%08x\r\n", USBC_Readl(USBC_REG_PMU_IRQ(USBC0_BASE)));
	USBC_PHY_PRINTF("USBPHY_CLK_REG:0x%08x\r\n", USBC_Readl(USBPHY_CLK_REG));
	USBC_PHY_PRINTF("BUS_CLK_GATE0_REG:0x%08x\r\n", USBC_Readl(BUS_CLK_GATE0_REG));
	USBC_PHY_PRINTF("BUS_CLK_RST_REG:0x%08x\r\n", USBC_Readl(BUS_CLK_RST_REG));
	USBC_PHY_PRINTF("USBC_REG_CSR:0x%08x\r\n", USBC_Readl(USBC_REG_CSR(USBC0_BASE)));
	USBC_PHY_PRINTF("USBC_REG_VEND0:0x%02x\r\n", USBC_Readb(USBC_REG_VEND0(USBC0_BASE)));
	USBC_PHY_PRINTF("USBC_REG_FADDR:0x%02x\r\n", USBC_Readb(USBC_REG_FADDR(USBC0_BASE)));
	USBC_PHY_PRINTF("USBC_REG_PCTL:0x%02x\r\n", USBC_Readb(USBC_REG_PCTL(USBC0_BASE)));
	USBC_PHY_PRINTF("USBC_REG_INTUSBE:0x%04x\r\n", USBC_Readw(USBC_REG_INTUSBE(USBC0_BASE)));
	USBC_PHY_PRINTF("USBC_REG_INTRxE:0x%04x\r\n", USBC_Readw(USBC_REG_INTRxE(USBC0_BASE)));
	USBC_PHY_PRINTF("USBC_REG_INTTxE:0x%04x\r\n", USBC_Readw(USBC_REG_INTTxE(USBC0_BASE)));
	USBC_PHY_PRINTF("\r\n----------------------------------------------\r\n");
}
