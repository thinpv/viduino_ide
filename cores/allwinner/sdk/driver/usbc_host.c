/*
 * drivers/usb/sunxi_usb/usbc/usbc_host.c
 * (C) Copyright 2010-2015
 * Allwinner Technology Co., Ltd. <www.allwinnertech.com>
 * daniel, 2009.09.21
 *
 * usb register ops.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 */

#include "sunxi_usb_typedef.h"
#include "sunxi_usb_bsp.h"

/* select usb speed type, high/full/low */

/* not config speed type */
static void USBC_Host_TsMode_Default(void)
{
	USBC_REG_clear_bit_b(USBC_BP_POWER_H_HIGH_SPEED_EN, USBC_REG_PCTL(USBC0_BASE));
}

/* config high speed */
static void USBC_Host_TsMode_Hs(void)
{
	USBC_REG_set_bit_b(USBC_BP_POWER_H_HIGH_SPEED_EN, USBC_REG_PCTL(USBC0_BASE));
}

/* config full speed */
static void USBC_Host_TsMode_Fs(void)
{
	USBC_REG_clear_bit_b(USBC_BP_POWER_H_HIGH_SPEED_EN, USBC_REG_PCTL(USBC0_BASE));
}

/* config low speed */
static void USBC_Host_TsMode_Ls(void)
{
	USBC_REG_clear_bit_b(USBC_BP_POWER_H_HIGH_SPEED_EN, USBC_REG_PCTL(USBC0_BASE));
}

static void USBC_Host_ep0_EnablePing(void)
{
	USBC_REG_clear_bit_w(USBC_BP_CSR0_H_DisPing, USBC_REG_CSR0(USBC0_BASE));
}

static void USBC_Host_ep0_DisablePing(void)
{
	u16 csr0 = 0;
	csr0 = USBC_Readw(USBC_REG_CSR0(USBC0_BASE));
	csr0 |= (1 << USBC_BP_CSR0_H_DisPing);
	USBC_Writew(csr0, USBC_REG_CSR0(USBC0_BASE));
}

static __u32 USBC_Host_ep0_IsNakTimeOut(void)
{
	return USBC_REG_test_bit_w(USBC_BP_CSR0_H_NAK_Timeout, USBC_REG_CSR0(USBC0_BASE));
}

static void USBC_Host_ep0_ClearNakTimeOut(void)
{
	USBC_REG_clear_bit_w(USBC_BP_CSR0_H_NAK_Timeout, USBC_REG_CSR0(USBC0_BASE));
}

static __u32 USBC_Host_ep0_IsError(void)
{
	return USBC_REG_test_bit_w(USBC_BP_CSR0_H_Error, USBC_REG_CSR0(USBC0_BASE));
}

static void USBC_Host_ep0_ClearError(void)
{
	USBC_REG_clear_bit_w(USBC_BP_CSR0_H_Error, USBC_REG_CSR0(USBC0_BASE));
}

static void USBC_Host_ep0_EpType(__u32 ts_mode)
{
	__u32 reg_val = 0;

	/* config transfer speed */
	switch (ts_mode)
	{
	case USBC_TS_MODE_HS:
		reg_val |= 0x01 << USBC_BP_RXTYPE_SPEED;
		break;

	case USBC_TS_MODE_FS:
		reg_val |= 0x02 << USBC_BP_RXTYPE_SPEED;
		break;

	case USBC_TS_MODE_LS:
		reg_val |= 0x03 << USBC_BP_RXTYPE_SPEED;
		break;

	default:
		reg_val = 0;
	}

	USBC_Writeb(reg_val, USBC_REG_EP0TYPE(USBC0_BASE));
}

static void USBC_Host_ep0_FlushFifo(void)
{
	USBC_Writew(1 << USBC_BP_CSR0_H_FlushFIFO, USBC_REG_CSR0(USBC0_BASE));
	USBC_Host_ep0_DisablePing();
}

static void USBC_Host_ep0_ConfigEp_Default(void)
{
	//--<1>--config ep0 csr
	USBC_Writew(1 << USBC_BP_CSR0_H_FlushFIFO, USBC_REG_CSR0(USBC0_BASE));
	USBC_Host_ep0_DisablePing();

	//--<2>--config polling interval
	USBC_Writeb(0x00, USBC_REG_TXINTERVAL(USBC0_BASE));

	/* config ep transfer type */
	USBC_Writeb(0x00, USBC_REG_EP0TYPE(USBC0_BASE));
}

static void USBC_Host_ep0_ConfigEp(__u32 ts_mode, __u32 interval)
{
	//--<1>--config ep0 csr
	USBC_Writew(1 << USBC_BP_CSR0_H_FlushFIFO, USBC_REG_CSR0(USBC0_BASE));
	USBC_Host_ep0_DisablePing();

	//--<2>--config polling interval
	USBC_Writeb(interval, USBC_REG_NAKLIMIT0(USBC0_BASE));

	/* config ep0 transfer type */
	USBC_Host_ep0_EpType(ts_mode);
}

static __u32 USBC_Host_ep0_IsReadDataReady(void)
{
	return USBC_REG_test_bit_w(USBC_BP_CSR0_H_RxPkRdy, USBC_REG_CSR0(USBC0_BASE));
}

static __u32 USBC_Host_ep0_IsWriteDataReady(void)
{
	return USBC_REG_test_bit_w(USBC_BP_CSR0_H_TxPkRdy, USBC_REG_CSR0(USBC0_BASE));
}

static void USBC_Host_ep0_ReadDataHalf(void)
{
	USBC_REG_clear_bit_w(USBC_BP_CSR0_H_RxPkRdy, USBC_REG_CSR0(USBC0_BASE));
}

static void USBC_Host_ep0_ReadDataComplete(void)
{
	USBC_REG_clear_bit_w(USBC_BP_CSR0_H_RxPkRdy, USBC_REG_CSR0(USBC0_BASE));
}

static void USBC_Host_ep0_WriteDataHalf(void)
{
	USBC_REG_set_bit_w(USBC_BP_CSR0_H_TxPkRdy, USBC_REG_CSR0(USBC0_BASE));
}

static void USBC_Host_ep0_WriteDataComplete(void)
{
	USBC_REG_set_bit_w(USBC_BP_CSR0_H_TxPkRdy, USBC_REG_CSR0(USBC0_BASE));
}

static __u32 USBC_Host_ep0_IsStall(void)
{
	return USBC_REG_test_bit_w(USBC_BP_CSR0_H_RxStall, USBC_REG_CSR0(USBC0_BASE));
}

static void USBC_Host_ep0_ClearStall(void)
{
	USBC_REG_clear_bit_w(USBC_BP_CSR0_H_RxStall, USBC_REG_CSR0(USBC0_BASE));
}

static void USBC_Host_ep0_ClearCSR(void)
{
	USBC_Writew(0x00, USBC_REG_CSR0(USBC0_BASE));
	USBC_Host_ep0_DisablePing();
}

static __u32 USBC_Host_ep0_IsReqPktSet(void)
{
	return USBC_REG_test_bit_w(USBC_BP_CSR0_H_ReqPkt, USBC_REG_CSR0(USBC0_BASE));
}

static void USBC_Host_ep0_StartInToken(void)
{
	USBC_REG_set_bit_w(USBC_BP_CSR0_H_ReqPkt, USBC_REG_CSR0(USBC0_BASE));
}

static void USBC_Host_ep0_StopInToken(void)
{
	USBC_REG_clear_bit_w(USBC_BP_CSR0_H_ReqPkt, USBC_REG_CSR0(USBC0_BASE));
}

static void USBC_Host_ep0_StatusAfterIn(void)
{
	__u32 reg_val = 0;

	reg_val = USBC_Readw(USBC_REG_CSR0(USBC0_BASE));
	reg_val |= 1 << USBC_BP_CSR0_H_TxPkRdy;
	reg_val |= 1 << USBC_BP_CSR0_H_StatusPkt;
	USBC_Writew(reg_val, USBC_REG_CSR0(USBC0_BASE));
}

static void USBC_Host_ep0_StatusAfterOut(void)
{
	__u32 reg_val = 0;

	reg_val = USBC_Readw(USBC_REG_CSR0(USBC0_BASE));
	reg_val |= 1 << USBC_BP_CSR0_H_ReqPkt;
	reg_val |= 1 << USBC_BP_CSR0_H_StatusPkt;
	USBC_Writew(reg_val, USBC_REG_CSR0(USBC0_BASE));
}

static void USBC_Host_ep0_SendSetupPkt(void)
{
	__u32 reg_val = 0;

	reg_val = USBC_Readw(USBC_REG_CSR0(USBC0_BASE));
	reg_val |= 1 << USBC_BP_CSR0_H_SetupPkt;
	reg_val |= 1 << USBC_BP_CSR0_H_TxPkRdy;
	USBC_Writew(reg_val, USBC_REG_CSR0(USBC0_BASE));
}

static void USBC_Host_Tx_EpType(__u32 ep_index, __u32 ts_mode, __u32 ts_type)
{
	__u32 reg_val = 0;

	/* config transfer speed */
	switch (ts_mode)
	{
	case USBC_TS_MODE_HS:
		reg_val |= 0x01 << USBC_BP_TXTYPE_SPEED;
		break;

	case USBC_TS_MODE_FS:
		reg_val |= 0x02 << USBC_BP_TXTYPE_SPEED;
		break;

	case USBC_TS_MODE_LS:
		reg_val |= 0x03 << USBC_BP_TXTYPE_SPEED;
		break;

	default:
		reg_val = 0;
	}

	//--<1>--config protocol
	switch (ts_type)
	{
	case USBC_TS_TYPE_ISO:
		reg_val |= 0x1 << USBC_BP_TXTYPE_PROROCOL;
		break;

	case USBC_TS_TYPE_BULK:
		reg_val |= 0x2 << USBC_BP_TXTYPE_PROROCOL;
		break;

	case USBC_TS_TYPE_INT:
		reg_val |= 0x3 << USBC_BP_TXTYPE_PROROCOL;
		break;

	default:
		break;
	}

	//--<2>--config target ep number
	reg_val |= ep_index;

	USBC_Writeb(reg_val, USBC_REG_TXTYPE(USBC0_BASE));
}

static void USBC_Host_Tx_FlushFifo(void)
{
	USBC_Writew((1 << USBC_BP_TXCSR_H_CLEAR_DATA_TOGGLE) | (1 << USBC_BP_TXCSR_H_FLUSH_FIFO),
							USBC_REG_TXCSR(USBC0_BASE));
}

static void USBC_Host_Tx_ConfigEp_Default(void)
{
	//--<1>--config tx_csr, flush fifo, clear all to 0
	USBC_Writew((1 << USBC_BP_TXCSR_H_CLEAR_DATA_TOGGLE) | (1 << USBC_BP_TXCSR_H_FLUSH_FIFO),
							USBC_REG_TXCSR(USBC0_BASE));

	USBC_Writew(0x00, USBC_REG_TXCSR(USBC0_BASE));

	//--<2>--config tx ep max packet
	USBC_Writew(0x00, USBC_REG_TXMAXP(USBC0_BASE));

	//--<3>--config ep transfer type
	USBC_Writeb(0x00, USBC_REG_TXTYPE(USBC0_BASE));

	//--<4>--config polling interval
	USBC_Writeb(0x00, USBC_REG_TXINTERVAL(USBC0_BASE));
}

static void USBC_Host_Tx_ConfigEp(__u32 ep_index, __u32 ts_mode, __u32 ts_type, __u32 is_double_fifo, __u32 ep_MaxPkt, __u32 interval)
{
	__u16 reg_val = 0;
	__u16 temp = 0;

	//--<1>--config tx_csr
	USBC_Writew((1 << USBC_BP_TXCSR_H_MODE) | (1 << USBC_BP_TXCSR_H_CLEAR_DATA_TOGGLE) | (1 << USBC_BP_TXCSR_H_FLUSH_FIFO),
							USBC_REG_TXCSR(USBC0_BASE));

	if (is_double_fifo)
	{
		USBC_Writew((1 << USBC_BP_TXCSR_H_MODE) | (1 << USBC_BP_TXCSR_H_CLEAR_DATA_TOGGLE) | (1 << USBC_BP_TXCSR_H_FLUSH_FIFO),
								USBC_REG_TXCSR(USBC0_BASE));
	}

	//--<2>--config tx ep max packet
	reg_val = USBC_Readw(USBC_REG_TXMAXP(USBC0_BASE));
	temp = ep_MaxPkt & ((1 << USBC_BP_TXMAXP_PACKET_COUNT) - 1);
	reg_val |= temp;
	USBC_Writew(reg_val, USBC_REG_TXMAXP(USBC0_BASE));

	//--<3>--config ep transfer type
	USBC_Host_Tx_EpType(ep_index, ts_mode, ts_type);

	//--<4>--config polling interval
	USBC_Writeb(interval, USBC_REG_TXINTERVAL(USBC0_BASE));
}

static void USBC_Host_Tx_ConfigEpDma(void)
{
	__u16 ep_csr = 0;

	//auto_set, tx_mode, dma_tx_en, mode1
	ep_csr = USBC_Readb(USBC_REG_TXCSR(USBC0_BASE) + 1);
	ep_csr |= (1 << USBC_BP_TXCSR_H_AUTOSET) >> 8;
	ep_csr |= (1 << USBC_BP_TXCSR_H_MODE) >> 8;
	ep_csr |= (1 << USBC_BP_TXCSR_H_DMA_REQ_EN) >> 8;
	ep_csr |= (1 << USBC_BP_TXCSR_H_DMA_REQ_MODE) >> 8;
	USBC_Writeb(ep_csr, (USBC_REG_TXCSR(USBC0_BASE) + 1));
}

static void USBC_Host_Tx_ClearEpDma(void)
{
	__u16 ep_csr = 0;

	//auto_set, dma_tx_en, mode1
	ep_csr = USBC_Readb(USBC_REG_TXCSR(USBC0_BASE) + 1);
	ep_csr &= ~((1 << USBC_BP_TXCSR_H_AUTOSET) >> 8);
	ep_csr &= ~((1 << USBC_BP_TXCSR_H_DMA_REQ_EN) >> 8);
	USBC_Writeb(ep_csr, (USBC_REG_TXCSR(USBC0_BASE) + 1));

	//DMA_REQ_EN and DMA_REQ_MODE cannot be cleared in the same cycle
	ep_csr = USBC_Readb(USBC_REG_TXCSR(USBC0_BASE) + 1);
	ep_csr &= ~((1 << USBC_BP_TXCSR_H_DMA_REQ_MODE) >> 8);
	USBC_Writeb(ep_csr, (USBC_REG_TXCSR(USBC0_BASE) + 1));
}

static __u32 USBC_Host_Tx_IsWriteDataReady(void)
{
	return USBC_REG_test_bit_w(USBC_BP_TXCSR_H_TX_READY, USBC_REG_TXCSR(USBC0_BASE)) | USBC_REG_test_bit_w(USBC_BP_TXCSR_H_FIFO_NOT_EMPTY, USBC_REG_TXCSR(USBC0_BASE));
}

static void USBC_Host_Tx_WriteDataHalf(void)
{
	USBC_REG_set_bit_w(USBC_BP_TXCSR_H_TX_READY, USBC_REG_TXCSR(USBC0_BASE));
}

static void USBC_Host_Tx_WriteDataComplete(void)
{
	USBC_REG_set_bit_w(USBC_BP_TXCSR_H_TX_READY, USBC_REG_TXCSR(USBC0_BASE));
}

static __u32 USBC_Host_Tx_IsNakTimeOut(void)
{
	return USBC_REG_test_bit_w(USBC_BP_TXCSR_H_NAK_TIMEOUT, USBC_REG_TXCSR(USBC0_BASE));
}

static void USBC_Host_Tx_ClearNakTimeOut(void)
{
	USBC_REG_clear_bit_w(USBC_BP_TXCSR_H_NAK_TIMEOUT, USBC_REG_TXCSR(USBC0_BASE));
}

static __u32 USBC_Host_Tx_IsError(void)
{
	return USBC_REG_test_bit_w(USBC_BP_TXCSR_H_ERROR, USBC_REG_TXCSR(USBC0_BASE));
}

static void USBC_Host_Tx_ClearError(void)
{
	USBC_REG_clear_bit_w(USBC_BP_TXCSR_H_ERROR, USBC_REG_TXCSR(USBC0_BASE));
}

static __u32 USBC_Host_Tx_IsStall(void)
{
	return USBC_REG_test_bit_w(USBC_BP_TXCSR_H_TX_STALL, USBC_REG_TXCSR(USBC0_BASE));
}

static void USBC_Host_Tx_ClearStall(void)
{
	USBC_REG_clear_bit_w(USBC_BP_TXCSR_H_TX_STALL, USBC_REG_TXCSR(USBC0_BASE));
}

/*
static void __USBC_Host_Tx_ForbidStall(__u32 USBC0_BASE)
{
	USBC_REG_set_bit_w(USBC_BP_TXCSR_H_RX_STALL, USBC_REG_TXCSR(USBC0_BASE));
}
*/

static void USBC_Host_Tx_ClearCSR(void)
{
	USBC_Writew(0x00, USBC_REG_TXCSR(USBC0_BASE));
}

static void USBC_Host_Rx_EpType(__u32 ep_index, __u32 ts_mode, __u32 ts_type)
{
	__u32 reg_val = 0;

	/* config transfer speed */
	switch (ts_mode)
	{
	case USBC_TS_MODE_HS:
		reg_val |= 0x01 << USBC_BP_RXTYPE_SPEED;
		break;

	case USBC_TS_MODE_FS:
		reg_val |= 0x02 << USBC_BP_RXTYPE_SPEED;
		break;

	case USBC_TS_MODE_LS:
		reg_val |= 0x03 << USBC_BP_RXTYPE_SPEED;
		break;

	default:
		reg_val = 0;
	}

	//--<1>--config protocol
	switch (ts_type)
	{
	case USBC_TS_TYPE_ISO:
		reg_val |= 0x1 << USBC_BP_RXTYPE_PROROCOL;
		break;

	case USBC_TS_TYPE_BULK:
		reg_val |= 0x2 << USBC_BP_RXTYPE_PROROCOL;
		break;

	case USBC_TS_TYPE_INT:
		reg_val |= 0x3 << USBC_BP_RXTYPE_PROROCOL;
		break;

	default:
		break;
	}

	//--<2>--config target ep number
	reg_val |= ep_index;

	USBC_Writeb(reg_val, USBC_REG_RXTYPE(USBC0_BASE));
}

static void USBC_Host_Rx_FlushFifo(void)
{
	USBC_Writew((1 << USBC_BP_RXCSR_H_CLEAR_DATA_TOGGLE) | (1 << USBC_BP_RXCSR_H_FLUSH_FIFO),
							USBC_REG_RXCSR(USBC0_BASE));
}

static void USBC_Host_Rx_ConfigEp_Default(void)
{
	//--<1>--config rx_csr, flush fifo
	USBC_Writew((1 << USBC_BP_RXCSR_H_CLEAR_DATA_TOGGLE) | (1 << USBC_BP_RXCSR_H_FLUSH_FIFO),
							USBC_REG_RXCSR(USBC0_BASE));

	USBC_Writew(0x00, USBC_REG_RXCSR(USBC0_BASE));

	//--<2>--config rx ep max packet
	USBC_Writew(0x00, USBC_REG_RXMAXP(USBC0_BASE));

	//--<3>--config ep transfer type
	USBC_Writeb(0x00, USBC_REG_RXTYPE(USBC0_BASE));

	//--<4>--config polling interval
	USBC_Writeb(0x00, USBC_REG_RXINTERVAL(USBC0_BASE));
}

static void USBC_Host_Rx_ConfigEp(__u32 ep_index, __u32 ts_mode, __u32 ts_type, __u32 is_double_fifo, __u32 ep_MaxPkt, __u32 interval)
{
	__u16 reg_val = 0;
	__u16 temp = 0;

	//--<1>--config rx_csr
	USBC_Writew((1 << USBC_BP_RXCSR_H_CLEAR_DATA_TOGGLE) | (1 << USBC_BP_RXCSR_H_FLUSH_FIFO),
							USBC_REG_RXCSR(USBC0_BASE));

	if (is_double_fifo)
	{
		USBC_Writew((1 << USBC_BP_RXCSR_H_CLEAR_DATA_TOGGLE) | (1 << USBC_BP_RXCSR_H_FLUSH_FIFO),
								USBC_REG_RXCSR(USBC0_BASE));
	}

	//--<2>--config tx ep max packet
	reg_val = USBC_Readw(USBC_REG_RXMAXP(USBC0_BASE));
	temp = ep_MaxPkt & ((1 << USBC_BP_RXMAXP_PACKET_COUNT) - 1);
	reg_val |= temp;
	USBC_Writew(reg_val, USBC_REG_RXMAXP(USBC0_BASE));

	//--<3>--config ep transfer type
	USBC_Host_Rx_EpType(ep_index, ts_mode, ts_type);

	//--<4>--config polling interval
	USBC_Writeb(interval, USBC_REG_RXINTERVAL(USBC0_BASE));
}

static void USBC_Host_Rx_ConfigEpDma(void)
{
	__u16 ep_csr = 0;

	//config dma, auto_clear, dma_rx_en, mode1,
	ep_csr = USBC_Readb(USBC_REG_RXCSR(USBC0_BASE) + 1);
	ep_csr |= (1 << USBC_BP_RXCSR_H_AUTO_CLEAR) >> 8;
	ep_csr |= (1 << USBC_BP_RXCSR_H_AUTO_REQ) >> 8;
	//ep_csr &= ~((1 << USBC_BP_RXCSR_H_DMA_REQ_MODE) >> 8);
	ep_csr |= ((1 << USBC_BP_RXCSR_H_DMA_REQ_MODE) >> 8);
	ep_csr |= (1 << USBC_BP_RXCSR_H_DMA_REQ_EN) >> 8;
	USBC_Writeb(ep_csr, (USBC_REG_RXCSR(USBC0_BASE) + 1));
}

static void USBC_Host_Rx_ClearEpDma(void)
{
	__u16 ep_csr = 0;

	//auto_clear, dma_rx_en, mode1,
	ep_csr = USBC_Readb(USBC_REG_RXCSR(USBC0_BASE) + 1);
	ep_csr &= ~((1 << USBC_BP_RXCSR_H_AUTO_CLEAR) >> 8);
	ep_csr &= ~((1 << USBC_BP_RXCSR_H_AUTO_REQ) >> 8);
	ep_csr &= ~((1 << USBC_BP_RXCSR_H_DMA_REQ_MODE) >> 8);
	ep_csr &= ~((1 << USBC_BP_RXCSR_H_DMA_REQ_EN) >> 8);
	USBC_Writeb(ep_csr, (USBC_REG_RXCSR(USBC0_BASE) + 1));
}

static __u32 USBC_Host_Rx_IsReadDataReady(void)
{
	return USBC_REG_test_bit_w(USBC_BP_RXCSR_H_RX_PKT_READY, USBC_REG_RXCSR(USBC0_BASE));
}

static void USBC_Host_Rx_ReadDataHalf(void)
{
	USBC_REG_clear_bit_w(USBC_BP_RXCSR_H_RX_PKT_READY, USBC_REG_RXCSR(USBC0_BASE));
}

static void USBC_Host_Rx_ReadDataComplete(void)
{
	USBC_REG_clear_bit_w(USBC_BP_RXCSR_H_RX_PKT_READY, USBC_REG_RXCSR(USBC0_BASE));
}

static __u32 USBC_Host_Rx_IsNakTimeOut(void)
{
	return USBC_REG_test_bit_w(USBC_BP_RXCSR_H_NAK_TIMEOUT, USBC_REG_RXCSR(USBC0_BASE));
}

static void USBC_Host_Rx_ClearNakTimeOut(void)
{
	USBC_REG_clear_bit_w(USBC_BP_RXCSR_H_NAK_TIMEOUT, USBC_REG_RXCSR(USBC0_BASE));
}

static __u32 USBC_Host_Rx_IsError(void)
{
	return USBC_REG_test_bit_w(USBC_BP_RXCSR_H_ERROR, USBC_REG_RXCSR(USBC0_BASE));
}

static void USBC_Host_Rx_ClearError(void)
{
	USBC_REG_clear_bit_w(USBC_BP_RXCSR_H_ERROR, USBC_REG_RXCSR(USBC0_BASE));
}

static __u32 USBC_Host_Rx_IsStall(void)
{
	return USBC_REG_test_bit_w(USBC_BP_RXCSR_H_RX_STALL, USBC_REG_RXCSR(USBC0_BASE));
}

static void USBC_Host_Rx_ClearStall(void)
{
	USBC_REG_clear_bit_w(USBC_BP_RXCSR_H_RX_STALL, USBC_REG_RXCSR(USBC0_BASE));
}

static void USBC_Host_Rx_ClearCSR(void)
{
	USBC_Writew(0x00, USBC_REG_RXCSR(USBC0_BASE));
}

static __u32 USBC_Host_Rx_IsReqPktSet(void)
{
	return USBC_REG_test_bit_w(USBC_BP_RXCSR_H_REQ_PACKET, USBC_REG_RXCSR(USBC0_BASE));
}

static void USBC_Host_Rx_StartInToken(void)
{
	USBC_REG_set_bit_w(USBC_BP_RXCSR_H_REQ_PACKET, USBC_REG_RXCSR(USBC0_BASE));
}

static void USBC_Host_Rx_StopInToken(void)
{
	USBC_REG_clear_bit_w(USBC_BP_RXCSR_H_REQ_PACKET, USBC_REG_RXCSR(USBC0_BASE));
}

void USBC_Host_SetFunctionAddress_Deafult(__u32 ep_type, __u32 ep_index)
{
	switch (ep_type)
	{
	case USBC_EP_TYPE_TX:
		USBC_Writeb(0x00, USBC_REG_TXFADDRx(USBC0_BASE, ep_index));
		USBC_Writeb(0x00, USBC_REG_TXHADDRx(USBC0_BASE, ep_index));
		USBC_Writeb(0x00, USBC_REG_TXHPORTx(USBC0_BASE, ep_index));
		break;

	case USBC_EP_TYPE_RX:
		USBC_Writeb(0x00, USBC_REG_RXFADDRx(USBC0_BASE, ep_index));
		USBC_Writeb(0x00, USBC_REG_RXHADDRx(USBC0_BASE, ep_index));
		USBC_Writeb(0x00, USBC_REG_RXHPORTx(USBC0_BASE, ep_index));
		break;

	default:
		break;
	}
}

void USBC_Host_SetFunctionAddress(
		__u32 EpType,
		__u32 EpIndex,
		__u32 FunctionAdress,
		__u32 MultiTT,
		__u32 HubAddress,
		__u32 HubPortNumber)
{
	__u8 temp_8 = 0;

	if (MultiTT)
	{
		temp_8 = 1 << USBC_BP_HADDR_MULTI_TT;
	}

	temp_8 |= HubAddress;

	switch (EpType)
	{
	case USBC_EP_TYPE_TX:
		USBC_Writeb(FunctionAdress, USBC_REG_TXFADDRx(USBC0_BASE, EpIndex));
		USBC_Writeb(temp_8, USBC_REG_TXHADDRx(USBC0_BASE, EpIndex));
		USBC_Writeb(HubPortNumber, USBC_REG_TXHPORTx(USBC0_BASE, EpIndex));
		break;

	case USBC_EP_TYPE_RX:
		USBC_Writeb(FunctionAdress, USBC_REG_RXFADDRx(USBC0_BASE, EpIndex));
		USBC_Writeb(temp_8, USBC_REG_RXHADDRx(USBC0_BASE, EpIndex));
		USBC_Writeb(HubPortNumber, USBC_REG_RXHPORTx(USBC0_BASE, EpIndex));
		break;

	default:
		break;
	}
}

void USBC_Host_SetHubAddress_Deafult(__u32 ep_type, __u32 ep_index)
{
	switch (ep_type)
	{
	case USBC_EP_TYPE_TX:
		USBC_Writeb(0x00, USBC_REG_TXHADDRx(USBC0_BASE, ep_index));
		break;

	case USBC_EP_TYPE_RX:
		USBC_Writeb(0x00, USBC_REG_RXHADDRx(USBC0_BASE, ep_index));
		break;

	default:
		break;
	}
}

void USBC_Host_SetHubAddress(__u32 ep_type, __u32 ep_index, __u32 is_mutli_tt, __u8 address)
{
	switch (ep_type)
	{
	case USBC_EP_TYPE_TX:
		USBC_Writeb(((is_mutli_tt << USBC_BP_HADDR_MULTI_TT) | address), USBC_REG_TXHADDRx(USBC0_BASE, ep_index));
		break;

	case USBC_EP_TYPE_RX:
		USBC_Writeb(((is_mutli_tt << USBC_BP_HADDR_MULTI_TT) | address), USBC_REG_RXHADDRx(USBC0_BASE, ep_index));
		break;

	default:
		break;
	}
}

void USBC_Host_SetHPortAddress_Deafult(__u32 ep_type, __u32 ep_index)
{
	switch (ep_type)
	{
	case USBC_EP_TYPE_TX:
		USBC_Writeb(0x00, USBC_REG_TXHPORTx(USBC0_BASE, ep_index));
		break;

	case USBC_EP_TYPE_RX:
		USBC_Writeb(0x00, USBC_REG_RXHPORTx(USBC0_BASE, ep_index));
		break;

	default:
		break;
	}
}

void USBC_Host_SetHPortAddress(__u32 ep_type, __u32 ep_index, __u8 address)
{
	switch (ep_type)
	{
	case USBC_EP_TYPE_TX:
		USBC_Writeb(address, USBC_REG_TXHPORTx(USBC0_BASE, ep_index));
		break;

	case USBC_EP_TYPE_RX:
		USBC_Writeb(address, USBC_REG_RXHPORTx(USBC0_BASE, ep_index));
		break;

	default:
		break;
	}
}

__u32 USBC_Host_QueryTransferMode(void)
{
	if (USBC_REG_test_bit_b(USBC_BP_POWER_H_HIGH_SPEED_FLAG, USBC_REG_PCTL(USBC0_BASE)))
	{
		return USBC_TS_MODE_HS;
	}
	else
	{
		return USBC_TS_MODE_FS;
	}
}

void USBC_Host_ConfigTransferMode(__u32 speed_mode)
{
	// select transfer speed
	switch (speed_mode)
	{
	case USBC_TS_MODE_HS:
		USBC_Host_TsMode_Hs();
		break;

	case USBC_TS_MODE_FS:
		USBC_Host_TsMode_Fs();
		break;

	case USBC_TS_MODE_LS:
		USBC_Host_TsMode_Ls();
		break;

	default: // default�hs
		USBC_Host_TsMode_Default();
	}
}

/* reset the device on usb port, recommended reset gap is 100ms */
void USBC_Host_ResetPort(void)
{
	USBC_REG_set_bit_b(USBC_BP_POWER_H_RESET, USBC_REG_PCTL(USBC0_BASE));
}

/* USBC_Host_ResetPort and USBC_Host_ClearResetPortFlag should combine, but may influent efficiency in bsp delay */
void USBC_Host_ClearResetPortFlag(void)
{
	USBC_REG_clear_bit_b(USBC_BP_POWER_H_RESET, USBC_REG_PCTL(USBC0_BASE));
}

/* resume the device on usb port, recommended resume gap is 10ms */
void USBC_Host_RusumePort(void)
{
	USBC_REG_set_bit_b(USBC_BP_POWER_H_RESUME, USBC_REG_PCTL(USBC0_BASE));
}

/* USBC_Host_RusumePort and USBC_Host_ClearRusumePortFlag should combine, but may influent efficiency in bsp delay */
void USBC_Host_ClearRusumePortFlag(void)
{
	USBC_REG_clear_bit_b(USBC_BP_POWER_H_RESUME, USBC_REG_PCTL(USBC0_BASE));
}

/* usb port suspend */
void USBC_Host_SuspendPort(void)
{
	USBC_REG_set_bit_b(USBC_BP_POWER_H_SUSPEND, USBC_REG_PCTL(USBC0_BASE));
}

__u32 USBC_Host_QueryPowerStatus(void)
{
	return USBC_Readb(USBC_REG_PCTL(USBC0_BASE));
}

void USBC_Host_StartSession(void)
{
	USBC_REG_set_bit_b(USBC_BP_DEVCTL_SESSION, USBC_REG_DEVCTL(USBC0_BASE));
}

void USBC_Host_EndSession(void)
{
	USBC_REG_clear_bit_b(USBC_BP_DEVCTL_SESSION, USBC_REG_DEVCTL(USBC0_BASE));
}

/*
 * peripheral device type
 * @hUSB: handle return by USBC_open_otg, include the key data which USBC need
 *
 */
__u32 USBC_Host_PeripheralType(void)
{
	__u8 reg_val = 0;

	reg_val = USBC_Readb(USBC_REG_DEVCTL(USBC0_BASE));
	if (reg_val & (1 << USBC_BP_DEVCTL_FS_DEV))
	{
		return USBC_DEVICE_FSDEV;
	}
	else if (reg_val & (1 << USBC_BP_DEVCTL_LS_DEV))
	{
		return USBC_DEVICE_LSDEV;
	}
	else
	{
		return USBC_DEVICE_LSDEV;
	}
}

void USBC_Host_FlushFifo(__u32 ep_type)
{
	switch (ep_type)
	{
	case USBC_EP_TYPE_EP0:
		USBC_Host_ep0_FlushFifo();
		break;

	case USBC_EP_TYPE_TX:
		USBC_Host_Tx_FlushFifo();
		break;

	case USBC_EP_TYPE_RX:
		USBC_Host_Rx_FlushFifo();
		break;

	default:
		break;
	}
}

/*
 * release all resource of Ep, except irq.
 * @hUSB:    handle return by USBC_open_otg, include the key data which USBC need
 * @ep_type: ep type
 *
 */
void USBC_Host_ConfigEp_Default(__u32 ep_type)
{
	switch (ep_type)
	{
	case USBC_EP_TYPE_EP0:
		USBC_Host_ep0_ConfigEp_Default();
		break;

	case USBC_EP_TYPE_TX:
		USBC_Host_Tx_ConfigEp_Default();
		break;

	case USBC_EP_TYPE_RX:
		USBC_Host_Rx_ConfigEp_Default();
		break;

	default:
		break;
	}
}

/*
 * config Ep, including double fifo, max packet, etc.
 * @hUSB:    handle return by USBC_open_otg, include the key data which USBC need
 * @ep_type:    ep type
 * @ep_index:   ep index for dest device
 * @ts_type:    transfer type
 * @is_double_fifo: if is double fifo
 * @ep_MaxPkt:  ep max packet
 * @interval:   time gap
 *
 */
void USBC_Host_ConfigEp(__u32 ep_type, __u32 ep_index, __u32 ts_mode, __u32 ts_type, __u32 is_double_fifo, __u32 ep_MaxPkt, __u32 interval)
{
	switch (ep_type)
	{
	case USBC_EP_TYPE_EP0:
		USBC_Host_ep0_ConfigEp(ts_mode, interval);
		break;

	case USBC_EP_TYPE_TX:
		USBC_Host_Tx_ConfigEp(ep_index, ts_mode, ts_type, is_double_fifo, ep_MaxPkt, interval);
		break;

	case USBC_EP_TYPE_RX:
		USBC_Host_Rx_ConfigEp(ep_index, ts_mode, ts_type, is_double_fifo, ep_MaxPkt, interval);
		break;

	default:
		break;
	}
}

/*
 * config Ep dma
 * @hUSB:    handle return by USBC_open_otg, include the key data which USBC need
 * @ep_type: ep type
 *
 */
void USBC_Host_ConfigEpDma(__u32 ep_type)
{
	switch (ep_type)
	{
	case USBC_EP_TYPE_EP0:
		//not support
		break;

	case USBC_EP_TYPE_TX:
		USBC_Host_Tx_ConfigEpDma();
		break;

	case USBC_EP_TYPE_RX:
		USBC_Host_Rx_ConfigEpDma();
		break;

	default:
		break;
	}
}

/*
 * clear Ep dma setting
 * @hUSB:    handle return by USBC_open_otg, include the key data which USBC need
 * @ep_type: ep type
 *
 */
void USBC_Host_ClearEpDma(__u32 ep_type)
{
	switch (ep_type)
	{
	case USBC_EP_TYPE_EP0:
		//not support
		break;

	case USBC_EP_TYPE_TX:
		USBC_Host_Tx_ClearEpDma();
		break;

	case USBC_EP_TYPE_RX:
		USBC_Host_Rx_ClearEpDma();
		break;

	default:
		break;
	}
}

/*
 * check if Ep is NakTimeOut
 * @hUSB:    handle return by USBC_open_otg, include the key data which USBC need
 * @ep_type: ep type
 *
 * return: 0 - NAK not timeout, 1 - NAK timeout
 */
__u32 USBC_Host_IsEpNakTimeOut(__u32 ep_type)
{

	switch (ep_type)
	{
	case USBC_EP_TYPE_EP0:
		return USBC_Host_ep0_IsNakTimeOut();

	case USBC_EP_TYPE_TX:
		return USBC_Host_Tx_IsNakTimeOut();

	case USBC_EP_TYPE_RX:
		return USBC_Host_Rx_IsNakTimeOut();

	default:
		break;
	}

	return 0;
}

/*
 * clear Ep NakTimeOut status
 * @hUSB:    handle return by USBC_open_otg, include the key data which USBC need
 * @ep_type: ep type
 *
 */
void USBC_Host_ClearEpNakTimeOut(__u32 ep_type)
{
	switch (ep_type)
	{
	case USBC_EP_TYPE_EP0:
		USBC_Host_ep0_ClearNakTimeOut();
		break;

	case USBC_EP_TYPE_TX:
		USBC_Host_Tx_ClearNakTimeOut();
		break;

	case USBC_EP_TYPE_RX:
		USBC_Host_Rx_ClearNakTimeOut();
		break;

	default:
		break;
	}
}

/*
 * check if Ep is error
 * @hUSB:    handle return by USBC_open_otg, include the key data which USBC need
 * @ep_type: ep type
 *
 * return: 0 - ep is not error, 1 - ep is error
 */
__u32 USBC_Host_IsEpError(__u32 ep_type)
{
	switch (ep_type)
	{
	case USBC_EP_TYPE_EP0:
		return USBC_Host_ep0_IsError();

	case USBC_EP_TYPE_TX:
		return USBC_Host_Tx_IsError();

	case USBC_EP_TYPE_RX:
		return USBC_Host_Rx_IsError();

	default:
		break;
	}

	return 0;
}

/*
 * clear Ep error status
 * @hUSB:    handle return by USBC_open_otg, include the key data which USBC need
 * @ep_type: ep type
 *
 */
void USBC_Host_ClearEpError(__u32 ep_type)
{
	switch (ep_type)
	{
	case USBC_EP_TYPE_EP0:
		USBC_Host_ep0_ClearError();
		break;

	case USBC_EP_TYPE_TX:
		USBC_Host_Tx_ClearError();
		break;

	case USBC_EP_TYPE_RX:
		USBC_Host_Rx_ClearError();
		break;

	default:
		break;
	}
}

/*
 * check if Ep is stalled
 * @hUSB:    handle return by USBC_open_otg, include the key data which USBC need
 * @ep_type: ep type
 *
 * return: 0 - ep is not stall, 1 - ep is stall
 */
__u32 USBC_Host_IsEpStall(__u32 ep_type)
{
	switch (ep_type)
	{
	case USBC_EP_TYPE_EP0:
		return USBC_Host_ep0_IsStall();

	case USBC_EP_TYPE_TX:
		return USBC_Host_Tx_IsStall();

	case USBC_EP_TYPE_RX:
		return USBC_Host_Rx_IsStall();

	default:
		break;
	}

	return 0;
}

/*
 * clear Ep stall status
 * @hUSB:    handle return by USBC_open_otg, include the key data which USBC need
 * @ep_type: ep type
 *
 */
void USBC_Host_ClearEpStall(__u32 ep_type)
{
	switch (ep_type)
	{
	case USBC_EP_TYPE_EP0:
		USBC_Host_ep0_ClearStall();
		break;

	case USBC_EP_TYPE_TX:
		USBC_Host_Tx_ClearStall();
		break;

	case USBC_EP_TYPE_RX:
		USBC_Host_Rx_ClearStall();
		break;

	default:
		break;
	}
}

void USBC_Host_ClearEpCSR(__u32 ep_type)
{
	switch (ep_type)
	{
	case USBC_EP_TYPE_EP0:
		USBC_Host_ep0_ClearCSR();
		break;

	case USBC_EP_TYPE_TX:
		USBC_Host_Tx_ClearCSR();
		break;

	case USBC_EP_TYPE_RX:
		USBC_Host_Rx_ClearCSR();
		break;

	default:
		break;
	}
}

static __s32 USBC_Host_ReadDataHalf(__u32 ep_type)
{
	switch (ep_type)
	{
	case USBC_EP_TYPE_EP0:
		USBC_Host_ep0_ReadDataHalf();
		break;

	case USBC_EP_TYPE_TX:
		//not support
		return -1;

	case USBC_EP_TYPE_RX:
		USBC_Host_Rx_ReadDataHalf();
		break;

	default:
		return -1;
	}

	return 0;
}

static __s32 USBC_Host_ReadDataComplete(__u32 ep_type)
{
	switch (ep_type)
	{
	case USBC_EP_TYPE_EP0:
		USBC_Host_ep0_ReadDataComplete();
		break;

	case USBC_EP_TYPE_TX:
		//not support
		return -1;

	case USBC_EP_TYPE_RX:
		USBC_Host_Rx_ReadDataComplete();
		break;

	default:
		return -1;
	}

	return 0;
}

static __s32 USBC_Host_WriteDataHalf(__u32 ep_type)
{
	switch (ep_type)
	{
	case USBC_EP_TYPE_EP0:
		USBC_Host_ep0_WriteDataHalf();
		break;

	case USBC_EP_TYPE_TX:
		USBC_Host_Tx_WriteDataHalf();
		break;

	case USBC_EP_TYPE_RX:
		//not support
		return -1;

	default:
		return -1;
	}

	return 0;
}

static __s32 USBC_Host_WriteDataComplete(__u32 ep_type)
{
	switch (ep_type)
	{
	case USBC_EP_TYPE_EP0:
		USBC_Host_ep0_WriteDataComplete();
		break;

	case USBC_EP_TYPE_TX:
		USBC_Host_Tx_WriteDataComplete();
		break;

	case USBC_EP_TYPE_RX:
		//not support
		return -1;

	default:
		return -1;
	}

	return 0;
}

/*
 * check if the data ready for read
 * @hUSB:    handle return by USBC_open_otg, include the key data which USBC need
 * @ep_type: ep type
 *
 */
__u32 USBC_Host_IsReadDataReady(__u32 ep_type)
{

	switch (ep_type)
	{
	case USBC_EP_TYPE_EP0:
		return USBC_Host_ep0_IsReadDataReady();

	case USBC_EP_TYPE_TX:
		//not support
		break;

	case USBC_EP_TYPE_RX:
		return USBC_Host_Rx_IsReadDataReady();

	default:
		break;
	}

	return 0;
}

/*
 * check if fifo empty
 * @hUSB:    handle return by USBC_open_otg, include the key data which USBC need
 * @ep_type: ep type
 *
 */
__u32 USBC_Host_IsWriteDataReady(__u32 ep_type)
{

	switch (ep_type)
	{
	case USBC_EP_TYPE_EP0:
		return USBC_Host_ep0_IsWriteDataReady();

	case USBC_EP_TYPE_TX:
		return USBC_Host_Tx_IsWriteDataReady();

	case USBC_EP_TYPE_RX:
		//not support
		break;

	default:
		break;
	}

	return 0;
}

/*
 * get read status. if read over or not
 * @hUSB:    handle return by USBC_open_otg, include the key data which USBC need
 * @ep_type: ep type
 * @complete: if all data has been written
 *
 * return 0 on success, !0 on fail.
 */
__s32 USBC_Host_ReadDataStatus(__u32 ep_type, __u32 complete)
{

	if (complete)
	{
		USBC_Host_ReadDataComplete(ep_type);
	}
	else
	{
		USBC_Host_ReadDataHalf(ep_type);
	}

	return 0;
}

/*
 * get write status. if write over or not
 * @hUSB:    handle return by USBC_open_otg, include the key data which USBC need
 * @ep_type: ep type
 * @complete: if all data has been written
 *
 * return 0 on success, !0 on fail.
 */
__s32 USBC_Host_WriteDataStatus(__u32 ep_type, __u32 complete)
{
	if (complete)
	{
		return USBC_Host_WriteDataComplete(ep_type);
	}
	else
	{
		return USBC_Host_WriteDataHalf(ep_type);
	}
}

/*
 * check if ReqPkt bit is set
 * @hUSB: handle return by USBC_open_otg, include the key data which USBC need
 * @ep_type: ep type
 *
 */
__u32 USBC_Host_IsReqPktSet(__u32 ep_type)
{
	switch (ep_type)
	{
	case USBC_EP_TYPE_EP0:
		return USBC_Host_ep0_IsReqPktSet();
		//break;

	case USBC_EP_TYPE_TX:
		//not support
		break;

	case USBC_EP_TYPE_RX:
		return USBC_Host_Rx_IsReqPktSet();
		//break;

	default:
		break;
	}

	return 0;
}

/*
 * start sending in token to device
 * @hUSB:    handle return by USBC_open_otg, include the key data which USBC need
 * @ep_type: ep type
 *
 */
void USBC_Host_StartInToken(__u32 ep_type)
{

	switch (ep_type)
	{
	case USBC_EP_TYPE_EP0:
		USBC_Host_ep0_StartInToken();
		break;

	case USBC_EP_TYPE_TX:
		//not support
		break;

	case USBC_EP_TYPE_RX:
		USBC_Host_Rx_StartInToken();
		break;

	default:
		break;
	}
}

/*
 * stop sending in token to device
 * @hUSB:    handle return by USBC_open_otg, include the key data which USBC need
 * @ep_type: ep type
 *
 */
void USBC_Host_StopInToken(__u32 ep_type)
{
	switch (ep_type)
	{
	case USBC_EP_TYPE_EP0:
		USBC_Host_ep0_StopInToken();
		break;

	case USBC_EP_TYPE_TX:
		//not support
		break;

	case USBC_EP_TYPE_RX:
		USBC_Host_Rx_StopInToken();
		break;

	default:
		break;
	}
}
/*
void USBC_Host_ConfigRqPktCount(__hdle hUSB, __u32 ep_index, __u32 RqPktCount)
{
	__usbc_otg_t *usbc_otg = (__usbc_otg_t *)hUSB;

	if (usbc_otg == NULL) {
		return;
	}

	USBC_Writew(RqPktCount, USBC_REG_RPCOUNTx(USBC0_BASE, ep_index));
}
*/

void USBC_Host_ConfigRqPktCount(__u32 ep_index, __u32 RqPktCount)
{
	USBC_Writew(RqPktCount, USBC_REG_RPCOUNTx(USBC0_BASE, ep_index));
}

void USBC_Host_ClearRqPktCount(__u32 ep_index)
{
	USBC_Writew(0x00, USBC_REG_RPCOUNTx(USBC0_BASE, ep_index));
}

void USBC_Host_EnablePing(void)
{

	USBC_Host_ep0_EnablePing();
}

void USBC_Host_DisablePing(void)
{
	USBC_Host_ep0_DisablePing();
}

void USBC_Host_SendCtrlStatus(__u32 is_after_in)
{

	if (is_after_in)
	{
		USBC_Host_ep0_StatusAfterIn();
	}
	else
	{
		USBC_Host_ep0_StatusAfterOut();
	}
}

void USBC_Host_SendSetupPkt(void)
{
	USBC_Host_ep0_SendSetupPkt();
}

/*
EXPORT_SYMBOL(USBC_Host_SetFunctionAddress_Deafult);
EXPORT_SYMBOL(USBC_Host_SetFunctionAddress);
EXPORT_SYMBOL(USBC_Host_SetHubAddress_Deafult);
EXPORT_SYMBOL(USBC_Host_SetHubAddress);
EXPORT_SYMBOL(USBC_Host_SetHPortAddress_Deafult);
EXPORT_SYMBOL(USBC_Host_SetHPortAddress);

EXPORT_SYMBOL(USBC_Host_QueryTransferMode);
EXPORT_SYMBOL(USBC_Host_ConfigTransferMode);

EXPORT_SYMBOL(USBC_Host_ResetPort);
EXPORT_SYMBOL(USBC_Host_ClearResetPortFlag);
EXPORT_SYMBOL(USBC_Host_RusumePort);
EXPORT_SYMBOL(USBC_Host_ClearRusumePortFlag);
EXPORT_SYMBOL(USBC_Host_SuspendPort);
EXPORT_SYMBOL(USBC_Host_QueryPowerStatus);

EXPORT_SYMBOL(USBC_Host_EnablePing);
EXPORT_SYMBOL(USBC_Host_DisablePing);
EXPORT_SYMBOL(USBC_Host_IsReqPktSet);
EXPORT_SYMBOL(USBC_Host_StartInToken);
EXPORT_SYMBOL(USBC_Host_StopInToken);
EXPORT_SYMBOL(USBC_Host_SendCtrlStatus);
EXPORT_SYMBOL(USBC_Host_SendSetupPkt);

EXPORT_SYMBOL(USBC_Host_StartSession);
EXPORT_SYMBOL(USBC_Host_EndSession);
EXPORT_SYMBOL(USBC_Host_ConfigRqPktCount);
EXPORT_SYMBOL(USBC_Host_ClearRqPktCount);

EXPORT_SYMBOL(USBC_Host_PeripheralType);

EXPORT_SYMBOL(USBC_Host_FlushFifo);
EXPORT_SYMBOL(USBC_Host_ConfigEp_Default);
EXPORT_SYMBOL(USBC_Host_ConfigEp);
EXPORT_SYMBOL(USBC_Host_ConfigEpDma);
EXPORT_SYMBOL(USBC_Host_ClearEpDma);

EXPORT_SYMBOL(USBC_Host_IsEpStall);
EXPORT_SYMBOL(USBC_Host_ClearEpStall);
EXPORT_SYMBOL(USBC_Host_IsEpNakTimeOut);
EXPORT_SYMBOL(USBC_Host_ClearEpNakTimeOut);
EXPORT_SYMBOL(USBC_Host_IsEpError);
EXPORT_SYMBOL(USBC_Host_ClearEpError);
EXPORT_SYMBOL(USBC_Host_ClearEpCSR);

EXPORT_SYMBOL(USBC_Host_IsReadDataReady);
EXPORT_SYMBOL(USBC_Host_IsWriteDataReady);
EXPORT_SYMBOL(USBC_Host_ReadDataStatus);
EXPORT_SYMBOL(USBC_Host_WriteDataStatus);
*/
