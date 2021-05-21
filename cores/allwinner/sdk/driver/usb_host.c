/*
 * usb_host.c
 *
 *  Created on: Apr 25, 2020
 *      Author: root
 */
#include "usb.h"
// #include "irq_misc.h"
#include "usb_host.h"
#include "sizes.h"
#include "sys-delay.h"
#include <irq.h>
#include <sunxi_usb_bsp.h>

#define USB_HOST_PRINTF printf
#define USB_DIR_OUT 0		/* to device */
#define USB_DIR_IN 0x80 /* to host */

void sunxi_hcd_generic_disable(void);
int sunxi_usb_enable_hcd0(struct sunxi_hcd *sunxi_hcd);

static struct sunxi_hcd_context_registers sunxi_hcd_context;
static struct sunxi_hcd_hw_ep sunxi_hcd_packt[] = {
		{
				.hw_ep_num = 1,
				.style = FIFO_TX,
				.maxpacket = 512,
				.fifo = 0,
		},
		{
				.hw_ep_num = 1,
				.style = FIFO_RX,
				.maxpacket = 512,
				.fifo = 0,
		},
		{
				.hw_ep_num = 2,
				.style = FIFO_TX,
				.maxpacket = 512,
				.fifo = 0,
		},
		{
				.hw_ep_num = 2,
				.style = FIFO_RX,
				.maxpacket = 512,
				.fifo = 0,
		},
		{
				.hw_ep_num = 3,
				.style = FIFO_TX,
				.maxpacket = 512,
				.fifo = 0,
		},
		{
				.hw_ep_num = 3,
				.style = FIFO_RX,
				.maxpacket = 512,
				.fifo = 0,
		},
		{
				.hw_ep_num = 4,
				.style = FIFO_TX,
				.maxpacket = 512,
				.fifo = 0,
		},
		{
				.hw_ep_num = 4,
				.style = FIFO_RX,
				.maxpacket = 512,
				.fifo = 0,
		},
		{
				.hw_ep_num = 5,
				.style = FIFO_TX,
				.maxpacket = 512,
				.fifo = 0,
		},
		{
				.hw_ep_num = 5,
				.style = FIFO_RX,
				.maxpacket = 512,
				.fifo = 0,
		},
};
static struct sunxi_hcd_hw_ep sunxi_hcd_packt0 = {

		.hw_ep_num = 0, .style = FIFO_RXTX, .maxpacket = 64, .fifo = 0};

#if 1
static void sunxi_hcd_h_ep0_flush_fifo(void)
{
	u16 csr = 0;
	int retries = 5;
	/* scrub any data left in the fifo */
	do
	{
		csr = USBC_Readw(USBC_REG_CSR0(USBC0_BASE));
		if (!(csr & ((1 << USBC_BP_CSR0_H_TxPkRdy) | (1 << USBC_BP_CSR0_H_RxPkRdy))))
		{
			break;
		}

		USBC_REG_set_bit_w(USBC_BP_CSR0_H_FlushFIFO, USBC_REG_CSR0(USBC0_BASE));

		csr = USBC_Readw(USBC_REG_CSR0(USBC0_BASE));
		delay_us(10);
	} while (--retries);

	if (retries == 0)
	{
		//DMSG_PANIC("ERR: Could not flush host TX%d fifo: csr: %04x\r\n", csr);
	}

	/* and reset for the next transfer */
	USBC_Writew(0x00, USBC_REG_CSR0(USBC0_BASE));

	return;
}
/*
 * Clear TX fifo. Needed to avoid BABBLE errors.
 * @ep: physic ep info
 *
 */
static void sunxi_hcd_h_tx_flush_fifo(void)
{

	u16 csr = 0;
	u16 lastcsr = 0;
	int retries = 1000;

	/* if has checked that fifo not empty, then flush fifo */
	csr = USBC_Readw(USBC_REG_TXCSR(USBC0_BASE));
	while (csr & (1 << USBC_BP_TXCSR_H_FIFO_NOT_EMPTY))
	{
		if (csr != lastcsr)
		{
			//DMSG_PANIC("WRN: Host TX FIFONOTEMPTY csr: %02x\r\n", csr);
		}

		lastcsr = csr;

		csr |= (1 << USBC_BP_TXCSR_H_FLUSH_FIFO);
		USBC_Writew(csr, USBC_REG_TXCSR(USBC0_BASE));

		csr = USBC_Readw(USBC_REG_TXCSR(USBC0_BASE));

		delay_ms(1);
	}

	return;
}
/*
 * @hw_ep: physic ep info
 * @csr:   rx status reg value
 *
 */
static u16 sunxi_hcd_h_rx_flush_fifo(void)
{

	u16 csr;
	/* we don't want fifo to fill itself again;
	 * ignore dma (various models),
	 * leave toggle alone (may not have been saved yet)
	 */
	csr |= (1 << USBC_BP_RXCSR_H_FLUSH_FIFO) | (1 << USBC_BP_RXCSR_H_RX_PKT_READY);
	csr &= ~(1 << USBC_BP_RXCSR_H_REQ_PACKET);
	csr &= ~(1 << USBC_BP_RXCSR_H_AUTO_REQ);
	csr &= ~(1 << USBC_BP_RXCSR_H_AUTO_CLEAR);

	/* write 2x to allow double buffering */
	USBC_Writew(csr, USBC_REG_RXCSR(USBC0_BASE));
	USBC_Writew(csr, USBC_REG_RXCSR(USBC0_BASE));

	/* flush writebuffer */
	return USBC_Readw(USBC_REG_RXCSR(USBC0_BASE));
}
#endif
/*
* Interrupt Service Routine to record USB "global" interrupts.
* Since these do not happen often and signify things of
* paramount importance, it seems OK to check them individually;
* the order of the tests is specified in the manual
*
* @param sunxi_hcd instance pointer
* @param int_usb register contents
* @param devctl
* @param power
*/
#define STAGE0_MASK ((1 << USBC_BP_INTUSB_RESUME) | (1 << USBC_BP_INTUSB_SESSION_REQ) | (1 << USBC_BP_INTUSB_VBUS_ERROR) | (1 << USBC_BP_INTUSB_CONNECT) | (1 << USBC_BP_INTUSB_RESET) | (1 << USBC_BP_INTUSB_SOF))
struct sunxi_hcd host;
int processs_flg;

static void sunxi_hcd_board_set_vbus(int is_on)
{

	if (is_on)
	{
		USBC_Host_StartSession();
		USBC_ForceVbusValid(USBC_VBUS_TYPE_HIGH);
	}
	else
	{
		USBC_Host_EndSession();
		USBC_ForceVbusValid(USBC_VBUS_TYPE_DISABLE);
	}
}
void sunxi_hcd_irq_work(struct sunxi_hcd *sunxi_hcd)
{
	int reg_val = 0;
	struct sunxi_hcd *q;

	q = sunxi_hcd;

	if (sunxi_hcd->vbus_error_flag)
	{
		sunxi_hcd->vbus_error_flag = 0;
		/* power down */
		reg_val = USBC_Readb(USBC_REG_DEVCTL(USBC0_BASE));
		reg_val &= ~(1 << USBC_BP_DEVCTL_SESSION);
		USBC_Writeb(reg_val, USBC_REG_DEVCTL(USBC0_BASE));

		USBC_SelectBus(USBC_IO_TYPE_PIO, 0, 0);
		USBC_INT_EnableUsbMiscUint(1 << USBC_BP_INTUSB_VBUS_ERROR);
	}

	if (sunxi_hcd->session_req_flag)
	{
		sunxi_hcd->session_req_flag = 0;
		/* power down */
		reg_val = USBC_Readb(USBC_REG_DEVCTL(USBC0_BASE));
		reg_val &= ~(1 << USBC_BP_DEVCTL_SESSION);
		USBC_Writeb(reg_val, USBC_REG_DEVCTL(USBC0_BASE));

		USBC_ForceVbusValid(USBC_VBUS_TYPE_LOW);
		USBC_SelectBus(USBC_IO_TYPE_PIO, 0, 0);
		/* delay */
		delay_ms(100);

		/* power on */
		reg_val = USBC_Readb(USBC_REG_DEVCTL(USBC0_BASE));
		reg_val |= (1 << USBC_BP_DEVCTL_SESSION);
		USBC_Writeb(reg_val, USBC_REG_DEVCTL(USBC0_BASE));

		USBC_ForceVbusValid(USBC_VBUS_TYPE_HIGH);

		USBC_SelectBus(USBC_IO_TYPE_PIO, 0, 1);
		USBC_INT_EnableUsbMiscUint(1 << USBC_BP_INTUSBE_EN_SESSION_REQ);
	}

	if (sunxi_hcd->reset_flag)
	{
		sunxi_hcd->reset_flag = 0;
		/* power down */
		reg_val = USBC_Readb(USBC_REG_DEVCTL(USBC0_BASE));
		reg_val &= ~(1 << USBC_BP_DEVCTL_SESSION);
		USBC_Writeb(reg_val, USBC_REG_DEVCTL(USBC0_BASE));

		USBC_ForceVbusValid(USBC_VBUS_TYPE_LOW);
		USBC_SelectBus(USBC_IO_TYPE_PIO, 0, 0);
		/* delay */
		delay_ms(100);

		/* power on */
		reg_val = USBC_Readb(USBC_REG_DEVCTL(USBC0_BASE));
		reg_val |= (1 << USBC_BP_DEVCTL_SESSION);
		USBC_Writeb(reg_val, USBC_REG_DEVCTL(USBC0_BASE));

		USBC_ForceVbusValid(USBC_VBUS_TYPE_HIGH);

		//sunxi_hcd_set_vbus(sunxi_hcd, 1);
		USBC_SelectBus(USBC_IO_TYPE_PIO, 0, 1);
		/* disconnect */
		sunxi_hcd->ep0_stage = SUNXI_HCD_EP0_START;
		USBC_INT_EnableUsbMiscUint((1 << USBC_BP_INTUSB_RESET));

		sunxi_hcd->is_connected = 0;
	}

	return;
}
/********************************************************************************************/
int fifo_setup(int epnum, int style, uint32_t is_double_fifo, uint32_t fifo_size, uint32_t fifo_addr)
{
	int ep_fifo_size = 0;
	u32 c_off = fifo_addr >> 3;
	int size_temp1 = 0;
	int size_temp2 = 0;
	u8 c_size = 0;

	size_temp1 = fifo_size;
	for (c_size = 0; size_temp1 != 0; c_size++)
	{
		size_temp2 = size_temp1 >> 1;
		size_temp1 = size_temp2;
	}

	//printf("ffs : %d\r\n",c_size);
	c_size -= 4;

	/* configure the FIFO */
	USBC_SelectActiveEp(epnum);
	/* REVISIT error check:  be sure ep0 can both rx and tx ... */

	printf("MUSB_INDEX=%02x\r\n", epnum);
	printf("MUSB_TXFIFOSZ=%02x\r\n", c_size);
	printf("MUSB_TXFIFOADD=%04x\r\n", c_off);
	switch (style)
	{
	case FIFO_TX:
	{

		USBC_Writeb(c_size, USBC_REG_TXFIFOSZ(USBC0_BASE));
		USBC_Writew(c_off, USBC_REG_TXFIFOAD(USBC0_BASE));
	}
	break;
	case FIFO_RX:
	{
		USBC_Writeb(c_size, USBC_REG_RXFIFOSZ(USBC0_BASE));
		USBC_Writew(c_off, USBC_REG_RXFIFOAD(USBC0_BASE));
	}
	break;
	case FIFO_RXTX:
	{
		USBC_Writeb(c_size, USBC_REG_TXFIFOSZ(USBC0_BASE));
		USBC_Writew(c_off, USBC_REG_TXFIFOAD(USBC0_BASE));

		USBC_Writeb(c_size, USBC_REG_RXFIFOSZ(USBC0_BASE));
		USBC_Writew(c_off, USBC_REG_RXFIFOAD(USBC0_BASE));
	}
	break;
	default:

		break;
	}
	ep_fifo_size = fifo_addr + fifo_size;
	return ep_fifo_size;
}
static int ep_config_from_table(void)
{

	unsigned i = 0;
	unsigned n = 10;
	int offset = 0;

	//USBC_SelectActiveEp(0);
	offset = fifo_setup(sunxi_hcd_packt0.hw_ep_num, sunxi_hcd_packt0.style, 0, sunxi_hcd_packt0.maxpacket, 0);

	for (i = 0; i < n; i++)
	{
		offset = fifo_setup(sunxi_hcd_packt[i].hw_ep_num, sunxi_hcd_packt[i].style, 0, sunxi_hcd_packt[i].maxpacket, offset);
	}

	return 0;
}
/*
 * ep_config_from_hw - when sunxi_hcd_C_DYNFIFO_DEF is false
 */
static int ep_config_from_hw(void)
{

	return 0;
}
enum
{
	SW_HCD_CONTROLLER_MHDRC,
	SW_HCD_CONTROLLER_HDRC,
};
/****************************************************************/
void sunxi_hcd_generic_disable(void)
{

	/* disable interrupts */
	USBC_Writeb(0x00, USBC_REG_INTUSBE(USBC0_BASE));
	USBC_Writew(0x00, USBC_REG_INTTxE(USBC0_BASE));
	USBC_Writew(0x00, USBC_REG_INTRxE(USBC0_BASE));

	/* off */
	USBC_Writew(0x00, USBC_REG_DEVCTL(USBC0_BASE));

	/*  flush pending interrupts */
	USBC_Writeb(0xff, USBC_REG_INTUSB(USBC0_BASE));
	USBC_Writew(0x3f, USBC_REG_INTTx(USBC0_BASE));
	USBC_Writew(0x3f, USBC_REG_INTRx(USBC0_BASE));
}
int sunxi_usb_disable_hcd0(struct sunxi_hcd *sunxi_hcd)
{
	struct sunxi_hcd *q;
	q = sunxi_hcd;

	USBC_Host_SuspendPort();
	sunxi_hcd_generic_disable();

	USBC_SelectBus(USBC_IO_TYPE_PIO, 0, 0);
	USBC_ForceVbusValid(USBC_VBUS_TYPE_HIGH);
	return 0;
}
int sunxi_usb_enable_hcd0(struct sunxi_hcd *sunxi_hcd)
{
	u8 power = 0;
	struct sunxi_hcd *q;
	u8 devctl;

	printf("dectl:%02x", USBC_Readb(USBC_REG_DEVCTL(USBC0_BASE)));

	USBC_Writew(0x3f, USBC0_BASE + USBC_REG_o_INTTxE);
	USBC_Writew((0x3f & 0xfe), USBC0_BASE + USBC_REG_o_INTRxE);
	USBC_Writeb(0xf7, USBC0_BASE + USBC_REG_o_INTUSBE);

	//musb_writeb(regs, MUSB_TESTMODE, 0);
	/* select PIO mode */
	USBC_SelectBus(USBC_IO_TYPE_PIO, 0, 0);

	//power
	/*
	 * treating UNKNOWN as unspecified maximum speed, in which case
	 * we will default to high-speed.
	 */
	USBC_Writeb(0xa0, USBC0_BASE + USBC_REG_o_PCTL);

	printf("dectl:%02x", USBC_Readb(USBC_REG_DEVCTL(USBC0_BASE)));
	USBC_Writeb(0x81, USBC_REG_DEVCTL(USBC0_BASE));
	printf("*************USB start*****************\r\n");
	printf("enable end\r\n");

	power = USBC_Readb(USBC0_BASE + USBC_REG_o_PCTL);
	printf("power : %02x\r\n", power);
	USBC_Writeb(0xf0, USBC0_BASE + USBC_REG_o_PCTL);

	printf("dectl:%02x", USBC_Readb(USBC_REG_DEVCTL(USBC0_BASE)));
	USBC_Writeb(0x99, USBC_REG_DEVCTL(USBC0_BASE));
	//sunxi_hcd_board_set_vbus(1);

	return 0;
}
static void musb_default_write_fifo(int ep, int fifo, unsigned char *src, int len)
{
	uint32_t old_ep_index = 0;
	printf("\r\nwrite\r\n");
	old_ep_index = USBC_GetActiveEp();
	USBC_SelectActiveEp(ep);

	//thinpv
	volatile int Timeout = 10000000;

	while ((USBC_Dev_IsWriteDataReady(USBC_EP_TYPE_TX)) && (--Timeout))
		; // wait for last write to clear
	if (!Timeout)
	{
		printf("*");
		return 0;
	}

	if ((0x01 & (unsigned long)src) == 0)
	{
		u16 index = 0;

		/* best case is 32bit-aligned source address */
		if ((0x02 & (unsigned long)src) == 0)
		{
			if (len >= 4)
			{
				writesl(fifo, src + index, len >> 2);
				index += len & ~0x03;
			}
			if (len & 0x02)
			{
				__raw_writew(*(u16 *)&src[index], fifo + 0);
				index += 2;
			}
		}
		else
		{
			if (len >= 2)
			{
				writesw(fifo, src + index, len >> 1);
				index += len & ~0x01;
			}
		}
		if (len & 0x01)
		{
			__raw_writeb(src[index], fifo + 0);
		}
	}
	else
	{
		/* byte aligned */
		writesb(fifo, src, len);
	}
	USBC_SelectActiveEp(old_ep_index);
}
/*
 * Unload an endpoint's FIFO
 */
static void musb_default_read_fifo(int ep, int fifo, unsigned char *dst, int len)
{
	uint32_t old_ep_index = 0;
	old_ep_index = USBC_GetActiveEp();
	USBC_SelectActiveEp(ep);

	printf("read\r\n");
	/* we can't assume unaligned writes work */
	if ((0x01 & (unsigned long)dst) == 0)
	{
		u16 index = 0;

		/* best case is 32bit-aligned destination address */
		if ((0x02 & (unsigned long)dst) == 0)
		{
			if (len >= 4)
			{
				readsl(fifo, dst, len >> 2);
				index = len & ~0x03;
			}
			if (len & 0x02)
			{
				*(u16 *)&dst[index] = __raw_readw(fifo + 0); //musb_readw(fifo, 0);
				index += 2;
			}
		}
		else
		{
			if (len >= 2)
			{
				readsw(fifo, dst, len >> 1);
				index = len & ~0x01;
			}
		}

		if (len & 0x01)
			dst[index] = __raw_readb(fifo + 0);
	}
	else
	{
		/* byte aligned */
		readsb(fifo, dst, len);
	}
}
static USB_DeviceRequest usb_host_setup_request;
int host_setup_handle(struct sunxi_hcd *sunxi_hcd)
{
	int flg = 0;
#if 0
	if (!usb_host_setup_request->wLength)
	{
		printf("start no-DATA");
	} else if (usb_host_setup_request->bRequestType & USB_DIR_IN)
	{
		printf( "start IN-DATA");
		sunxi_hcd->ep0_stage = SUNXI_HCD_EP0_IN;
	} else
	{
		printf( "start OUT-DATA");
		sunxi_hcd->ep0_stage = SUNXI_HCD_EP0_OUT;
		flg = 1;
	}
#endif
	return flg;
}

//thinpv
int my_step = 0;
static void handle_schedule()
{
	printf("handle_schedule my_step: %d\r\n", my_step);
	if (my_step == 0)
	{
		// 	printf("set_addr\r\n");
		// 	unsigned char buff[] = {0x00, 0x05, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00};
		// 	musb_default_write_fifo(0, USBC_SelectFIFO(0), buff, 8);
		// 	USBC_Writew(0x000a, USBC_REG_CSR0(USBC0_BASE));
		// 	USBC_Writew(USBC_BP_CSR0_H_TxPkRdy, USBC_REG_CSR0(USBC0_BASE));
		// 	USBC_Writew(0x003f, USBC0_BASE + USBC_REG_o_INTTxE);
		my_step++;
		// }
		// else if (my_step == 1)
		// {
		printf("usb_get_device_descriptor\r\n");
		unsigned char buff[] = {0x80, 0x06, 0x00, 0x01, 0x00, 0x00, 0x12, 0x00};
		musb_default_write_fifo(0, USBC_SelectFIFO(0), buff, 8);
		USBC_Writew(0x000a, USBC_REG_CSR0(USBC0_BASE));
		USBC_Writew(USBC_BP_CSR0_H_TxPkRdy, USBC_REG_CSR0(USBC0_BASE));
		USBC_Writew(0x003f, USBC0_BASE + USBC_REG_o_INTTxE);
		my_step++;
	}
	else if (my_step == 2)
	{
		printf("usb_hid_get_cfg_descriptor\r\n");
		unsigned char buff[] = {0x80, 0x06, 0x00, 0x02, 0x00, 0x00, 0x09, 0x00};
		musb_default_write_fifo(0, USBC_SelectFIFO(0), buff, 8);
		USBC_Writew(0x000a, USBC_REG_CSR0(USBC0_BASE));
		USBC_Writew(USBC_BP_CSR0_H_TxPkRdy, USBC_REG_CSR0(USBC0_BASE));
		USBC_Writew(0x003f, USBC0_BASE + USBC_REG_o_INTTxE);
		my_step++;
	}
	else if (my_step == 3)
	{
		printf("usb_hid_get_cfg_descriptor\r\n");
		unsigned char buff[] = {0x80, 0x06, 0x00, 0x02, 0x00, 0x00, 0x29, 0x00};
		musb_default_write_fifo(0, USBC_SelectFIFO(0), buff, 8);
		USBC_Writew(0x000a, USBC_REG_CSR0(USBC0_BASE));
		USBC_Writew(USBC_BP_CSR0_H_TxPkRdy, USBC_REG_CSR0(USBC0_BASE));
		USBC_Writew(0x003f, USBC0_BASE + USBC_REG_o_INTTxE);
		my_step++;
	}
	else if (my_step == 4)
	{
		printf("set_configuration\r\n");
		unsigned char buff[] = {0x00, 0x09, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00};
		musb_default_write_fifo(0, USBC_SelectFIFO(0), buff, 8);
		USBC_Writew(0x000a, USBC_REG_CSR0(USBC0_BASE));
		USBC_Writew(USBC_BP_CSR0_H_TxPkRdy, USBC_REG_CSR0(USBC0_BASE));
		USBC_Writew(0x003f, USBC0_BASE + USBC_REG_o_INTTxE);
		my_step++;
	}
}

//extern int usb_device_read_data_ep_pack(int ep,unsigned char * databuf,int len);
static int sunxi_hcd_h_ep0_continue(struct sunxi_hcd *sunxi_hcd, u16 len)
{
	u8 count = 0;
	u8 *fifo_dest = NULL;
	u16 fifo_count = 0;
	int flg = 0;
	u8 buff[128] = {0};
	switch (sunxi_hcd->ep0_stage)
	{
	case SUNXI_HCD_EP0_IN:
		printf("SUNXI_HCD_EP0_IN\r\n");
		//musb_default_write_fifo(0,buff,len);
		//usb_device_read_data_ep_pack(0,buff,len);

		musb_default_read_fifo(0, USBC_SelectFIFO(0), buff, len);
		printf("\r\n**********recive data*********\r\n");
		for (count = 0; count < len; count++)
		{
			printf("%02x", buff[count]);
		}
		printf("\r\n**********recive end*********\r\n");

		if (1)
		{
			/* always terminate on short read; it's
			 * rarely reported as an error.
			 */
			USBC_Host_ReadDataStatus(USBC_EP_TYPE_EP0, 1); //thinpv
		}
		else
		{
			flg = 1;
			//USBC_Host_ReadDataStatus(USBC_EP_TYPE_EP0, 0);
		}

		break;
	case SUNXI_HCD_EP0_START:
		printf("SUNXI_HCD_EP0_START\r\n");
		sunxi_hcd->ep0_stage = SUNXI_HCD_EP0_IN;
		if (host_setup_handle(sunxi_hcd))
		{
		}
		else
		{
		}
		flg = 1;
		break;
		/* FALLTHROUGH */
	case SUNXI_HCD_EP0_OUT:
		printf("SUNXI_HCD_EP0_OUT\r\n");
		//musb_write_fifo(0, fifo_count, fifo_dest);
		flg = 1;
		break;
	default:
		printf("bogus ep0 stage %d\r\n", sunxi_hcd->ep0_stage);
		break;
	}

	return flg;
}
static irqreturn_t sunxi_hcd_h_ep0_irq(struct sunxi_hcd *sunxi_hcd)
{
	int ret = 0;
	u16 csr, len;
	int complete = 0;
	int status = 0;
	irqreturn_t retval = IRQ_NONE;

	sunxi_hcd_ep_select(USBC0_BASE, 0);

	csr = USBC_Readw(USBC_REG_CSR0(USBC0_BASE));
	printf("csr:%04x\r\n", csr);
	if (csr & (1 << USBC_BP_CSR0_H_RxPkRdy))
	{
		len = USBC_Readw(USBC_REG_COUNT0(USBC0_BASE));
	}
	else
	{
		len = 0;
	}
	printf("len:%d\r\n", len);
	/* if we just did status stage, we are done */
	if (SUNXI_HCD_EP0_STATUS == sunxi_hcd->ep0_stage)
	{
		retval = IRQ_HANDLED;
		complete = 1;
	}

	/* prepare status */
	if (csr & (1 << USBC_BP_CSR0_H_RxStall))
	{
		USBC_Host_ClearEpStall(USBC_EP_TYPE_EP0);
		printf("STALLING ENDPOINT csr0 %04x\r\n", csr);
		status = -1;
	}
	else if (csr & (1 << USBC_BP_CSR0_H_Error))
	{
		USBC_Host_ClearEpError(USBC_EP_TYPE_EP0);
		printf("no response,csr0 %04x\r\n", csr);

		status = -1;
	}
	else if (csr & (1 << USBC_BP_CSR0_H_NAK_Timeout))
	{
		printf("control NAK timeout\r\n");

		USBC_Writew(0x00, USBC_REG_CSR0(USBC0_BASE));

		USBC_Host_DisablePing();
		USBC_Writeb(0x00, USBC_REG_NAKLIMIT0(USBC0_BASE));
		retval = IRQ_HANDLED;
	}

	if (status)
	{
		printf("123\r\n");
		retval = IRQ_HANDLED;

		complete = 1;

		/* use the proper sequence to abort the transfer */
		if (csr & 0x0020)
		{
			csr &= ~0x0020;
			USBC_Writew(csr, USBC_REG_CSR0(USBC0_BASE));
			csr &= ~0x0080;
			USBC_Writew(csr, USBC_REG_CSR0(USBC0_BASE));
		}
		else
		{
			sunxi_hcd_h_ep0_flush_fifo();
		}

		USBC_Writew(0x00, USBC_REG_NAKLIMIT0(USBC0_BASE));
		/* clear it */
		USBC_Writew(0x00, USBC_REG_CSR0(USBC0_BASE));
		USBC_Host_DisablePing();
	}

	if (complete == 0)
	{
		/* call common logic and prepare response */

		ret = sunxi_hcd_h_ep0_continue(sunxi_hcd, len);
		printf("value %d\r\n", ret);
		if (ret == 1)
		{
			/* more packets required */
			printf("more packets required\r\n");
			csr = (SUNXI_HCD_EP0_IN == sunxi_hcd->ep0_stage)
								? (1 << USBC_BP_CSR0_H_ReqPkt)
								: (1 << USBC_BP_CSR0_H_TxPkRdy);
		}
		else
		{
			/* data transfer complete; perform status phase */
			printf("data transfer complete; perform status phase\r\n");
			if (0)
			{
				csr = (1 << USBC_BP_CSR0_H_StatusPkt) | (1 << USBC_BP_CSR0_H_ReqPkt);
			}
			else
			{
				csr = (1 << USBC_BP_CSR0_H_StatusPkt) | (1 << USBC_BP_CSR0_H_TxPkRdy);
			}
			csr |= (1 << USBC_BP_CSR0_H_DisPing);
			/* flag status stage */
			sunxi_hcd->ep0_stage = SUNXI_HCD_EP0_STATUS;
		}
		printf("csr:%04x\r\n", csr);
		USBC_Writew(csr, USBC_REG_CSR0(USBC0_BASE));
		USBC_Host_DisablePing(); //thinpv

		retval = IRQ_HANDLED;
	}
	else
	{
		sunxi_hcd->ep0_stage = SUNXI_HCD_EP0_START;
		USBC_Writew(0x00, USBC_REG_CSR0(USBC0_BASE));
		USBC_Host_DisablePing();

		// handle_schedule();//thinpv
	}

done:
	return retval;
	return IRQ_HANDLED;
}

void host_process(void)
{
	u8 devctl = 0;
	u8 power = 0;

	delay_ms(1000);
	devctl = USBC_Readb(USBC_REG_DEVCTL(USBC0_BASE));
	power = USBC_Readb(USBC_REG_PCTL(USBC0_BASE));

	printf("devctl : %02x;power : %02x\r\n", devctl, power);
	USBC_Writeb(0xe8, USBC_REG_PCTL(USBC0_BASE));

	delay_ms(30);
	power = USBC_Readb(USBC_REG_PCTL(USBC0_BASE));

	sun4i_usb_phy_set_squelch_detect(0);
	printf("power : %02x\r\n", power);
	USBC_Writeb(0xf0, USBC_REG_PCTL(USBC0_BASE));
	sun4i_usb_phy_set_squelch_detect(1);
	power = USBC_Readb(USBC_REG_PCTL(USBC0_BASE));
	printf("power: %02x\r\n", power);

	//send
	USBC_SelectActiveEp(0);
	printf("ep control reg access : %04x", USBC_Readw(USBC_REG_CSR0(USBC0_BASE)));
	USBC_Writew(0x003e, USBC0_BASE + USBC_REG_o_INTTxE);
	printf("ep control reg access : %04x", USBC_Readw(USBC_REG_CSR0(USBC0_BASE)));
	USBC_Writew(0x0000, USBC_REG_CSR0(USBC0_BASE));

	USBC_Writew(0x00, USBC_REG_FADDR(USBC0_BASE));
	USBC_Writew(0x00, USBC_REG_HADDR0(USBC0_BASE));
	USBC_Writew(0x00, USBC_REG_HPORT0(USBC0_BASE));

	USBC_Writeb(0x00, USBC_REG_NAKLIMIT0(USBC0_BASE));
	USBC_Writeb(0x40, USBC_REG_EP0TYPE(USBC0_BASE));

#if 1
	//write
	unsigned char buff[] = {0x80, 0x06, 0x00, 0x01, 0x00, 0x00, 0x40, 0x00};
	musb_default_write_fifo(0, USBC_SelectFIFO(0), buff, 8);

	//usb_device_write_data_ep_pack(0,buff,8);
	USBC_Writew(0x000a, USBC_REG_CSR0(USBC0_BASE));
	USBC_Writew(USBC_BP_CSR0_H_TxPkRdy, USBC_REG_CSR0(USBC0_BASE));
	USBC_Writew(0x003f, USBC0_BASE + USBC_REG_o_INTTxE);
	//USBC_Host_DisablePing();
	//printf("devctl : %04x",USBC_REG_DEVCTL(USBC0_BASE));
#endif
}
static void musb_handle_intr_resume(struct sunxi_hcd *sunxi_hcd, u8 devctl, u8 power)
{
	if (devctl & (1 << USBC_BP_DEVCTL_HOST_MODE))
	{
		if (power & (1 << USBC_BP_POWER_H_SUSPEND))
		{
			/* spurious */
			//usb_irq &= ~(1 << USBC_BP_INTUSBE_EN_SUSPEND);

			USB_HOST_PRINTF("sunxi_hcd_stage0_irq, Spurious SUSPENDM\r\n");
		}

		power &= ~(1 << USBC_BP_POWER_H_SUSPEND);
		power |= (1 << USBC_BP_POWER_H_RESUME);
		USBC_Writeb(power, USBC_REG_PCTL(USBC0_BASE));
	}
}
static irqreturn_t musb_handle_intr_sessreq(struct sunxi_hcd *sunxi_hcd, u8 devctl, u8 power)
{
	if ((devctl & 0x18) == 0x18 && (devctl & 80))
	{
		USB_HOST_PRINTF("SessReq while on B state");
		return IRQ_HANDLED;
	}
	/* IRQ arrives from ID pin sense or (later, if VBUS power
	 * is removed) SRP.  responses are time critical:
	 *  - turn on VBUS (with silicon-specific mechanism)
	 *  - go through A_WAIT_VRISE
	 *  - ... to A_WAIT_BCON.
	 * a_wait_vrise_tmout triggers VBUS_ERROR transitions
	 */

	USBC_Writeb(0x01, USBC_REG_DEVCTL(USBC0_BASE));
	sunxi_hcd->session_req_flag = 1;
	sunxi_hcd->ep0_stage = SUNXI_HCD_EP0_START;
	return IRQ_NONE;
}
static void musb_handle_intr_vbuserr(struct sunxi_hcd *sunxi_hcd, u8 devctl, u8 power)
{
}
static void musb_handle_intr_suspend(struct sunxi_hcd *sunxi_hcd, u8 devctl, u8 power)
{
}
static void musb_handle_intr_connect(struct sunxi_hcd *sunxi_hcd, u8 devctl, u8 power)
{
	{
		int old_ep_index = 0;
		old_ep_index = USBC_GetActiveEp();
		USBC_SelectActiveEp(0);
		USBC_Host_DisablePing();
		USBC_SelectActiveEp(old_ep_index);
		//printf("active ep%d",old_ep_index);
	}
	USBC_INT_ClearMiscPending(1 << USBC_BP_INTUSB_CONNECT);

	//USBC_INT_ClearMiscPending(1 << USBC_BP_INTUSB_CONNECT);
	USBC_Writew(0x003f, USBC0_BASE + USBC_REG_o_INTTxE);
	USBC_Writew(0x003e, USBC0_BASE + USBC_REG_o_INTRxE);
	USBC_Writeb(0xf7, USBC0_BASE + USBC_REG_o_INTUSBE);

	sunxi_hcd->is_active = 1;
	sunxi_hcd->is_connected = 1;
	//set_bit(HCD_FLAG_SAW_IRQ, &hcd->flags);

	sunxi_hcd->ep0_stage = SUNXI_HCD_EP0_START;

	sunxi_hcd->port1_status &= ~(USB_PORT_STAT_LOW_SPEED | USB_PORT_STAT_HIGH_SPEED | USB_PORT_STAT_ENABLE);
	sunxi_hcd->port1_status |= USB_PORT_STAT_CONNECTION | (USB_PORT_STAT_C_CONNECTION << 16);

	/* high vs full speed is just a guess until after reset */
	if (devctl & (1 << USBC_BP_DEVCTL_LS_DEV))
	{
		sunxi_hcd->port1_status |= USB_PORT_STAT_LOW_SPEED;
	}
	processs_flg = 1;
	host_process();
}
static void musb_handle_intr_disconnect(struct sunxi_hcd *sunxi_hcd, u8 devctl, u8 power)
{
}
static void musb_handle_intr_reset(struct sunxi_hcd *sunxi_hcd, u8 devctl, u8 power)
{
	sunxi_hcd->reset_flag = 1;

	USBC_Host_SetFunctionAddress_Deafult(USBC_EP_TYPE_TX, 0);
	{
		u32 i = 1;
		for (i = 1; i <= 5; i++)
		{
			USBC_Host_SetFunctionAddress_Deafult(USBC_EP_TYPE_TX, i);
			USBC_Host_SetFunctionAddress_Deafult(USBC_EP_TYPE_RX, i);
		}
	}

	/* clear all the irq status of the plugout devie, now has no hub, so can clear all irqs */
	USBC_INT_ClearMiscPendingAll();
	USBC_INT_ClearEpPendingAll(USBC_EP_TYPE_TX);
	USBC_INT_ClearEpPendingAll(USBC_EP_TYPE_RX);
}
static irqreturn_t sunxi_hcd_stage0_irq(struct sunxi_hcd *sunxi_hcd, u8 devctl, u8 power)
{
	struct sunxi_hcd *q;
	irqreturn_t handled = IRQ_NONE;
	int usb_irq = 0;
	u8 reg_val;

	q = sunxi_hcd;
	usb_irq = q->int_usb;
#if 1
	/* in host mode, the peripheral may issue remote wakeup.
	 * in peripheral mode, the host may resume the link.
	 * spurious RESUME irqs happen too, paired with SUSPEND.
	 */

	if (usb_irq & (1 << USBC_BP_INTUSB_SOF))
	{
		USB_HOST_PRINTF("\r\n\r\n------------IRQ SOF-------------\r\n\r\n");

		//thinpv
		USBC_INT_ClearMiscPending(1 << USBC_BP_INTUSB_SOF);
		handled = IRQ_HANDLED;

		//thinpv
		USBC_INT_DisableUsbMiscUint(1 << USBC_BP_INTUSB_SOF);
	}

	if (usb_irq & (1 << USBC_BP_INTUSB_RESUME))
	{
		USBC_INT_ClearMiscPending(1 << USBC_BP_INTUSB_RESUME);
		USB_HOST_PRINTF("****************MUSB_INTR_RESUME**************\r\n");
		musb_handle_intr_resume(q, devctl, power);
		handled = IRQ_HANDLED;
		USB_HOST_PRINTF("***************end***************************\r\n");
	}

	/* see manual for the order of the tests */
	if (usb_irq & (1 << USBC_BP_INTUSB_SESSION_REQ))
	{
		USB_HOST_PRINTF("****************MUSB_INTR_SESSREQ**************\r\n");
		USBC_INT_ClearMiscPending(1 << USBC_BP_INTUSB_SESSION_REQ);

		if (musb_handle_intr_sessreq(q, devctl, power))
			return IRQ_HANDLED;
		handled = IRQ_HANDLED;
		USB_HOST_PRINTF("***************end***************************\r\n");
	}

	if (usb_irq & (1 << USBC_BP_INTUSB_VBUS_ERROR))
	{
		USB_HOST_PRINTF("****************MUSB_INTR_VBUSERROR**************\r\n");
		USBC_INT_ClearMiscPending(1 << USBC_BP_INTUSB_VBUS_ERROR);
		musb_handle_intr_vbuserr(q, devctl, power);
		handled = IRQ_HANDLED;
		USB_HOST_PRINTF("***************end***************************\r\n");
	}

	if (usb_irq & (1 << USBC_BP_INTUSB_SUSPEND))
	{
		USBC_INT_ClearMiscPending(1 << USBC_BP_INTUSB_SUSPEND);

		USB_HOST_PRINTF("****************MUSB_INTR_SUSPEND**************\r\n");
		musb_handle_intr_suspend(q, devctl, power);
		handled = IRQ_HANDLED;
		USB_HOST_PRINTF("***************end***************************\r\n");
	}

	if (usb_irq & (1 << USBC_BP_INTUSB_CONNECT))
	{
		USB_HOST_PRINTF("****************MUSB_INTR_CONNECT**************\r\n");
		USBC_INT_ClearMiscPending(1 << USBC_BP_INTUSB_CONNECT);

		musb_handle_intr_connect(q, devctl, power);
		handled = IRQ_HANDLED;
		USB_HOST_PRINTF("***************end***************************\r\n");
	}

	if (usb_irq & (1 << USBC_BP_INTUSB_DISCONNECT))
	{

		USBC_INT_ClearMiscPending(1 << USBC_BP_INTUSB_DISCONNECT);

		USB_HOST_PRINTF("****************MUSB_INTR_DISCONNECT**************\r\n");
		musb_handle_intr_disconnect(q, devctl, power);
		handled = IRQ_HANDLED;
		USB_HOST_PRINTF("***************end***************************\r\n");
	}

	if (usb_irq & (1 << USBC_BP_INTUSB_RESET))
	{
		USB_HOST_PRINTF("****************MUSB_INTR_RESET**************\r\n");

		USBC_INT_ClearMiscPending(1 << USBC_BP_INTUSB_RESET);
		handled = IRQ_HANDLED;
		USBC_INT_DisableUsbMiscUint(1 << USBC_BP_INTUSB_RESET);

		musb_handle_intr_reset(q, devctl, power);
		handled = IRQ_HANDLED;
		USB_HOST_PRINTF("***************end***************************\r\n");
	}

#else

#endif

	sunxi_hcd_irq_work(q);

	return handled;
}
/*
 *    handle all the irqs defined by the sunxi_hcd core. for now we expect:  other
 * irq sources //(phy, dma, etc) will be handled first, sunxi_hcd->int_* values
 * will be assigned, and the irq will already have been acked.
 *
 * called in irq context with spinlock held, irqs blocked
 *
 */
static irqreturn_t sunxi_hcd_interrupt(struct sunxi_hcd *sunxi_hcd)
{
	struct sunxi_hcd *q;
	irqreturn_t retval = IRQ_NONE;
	u8 devctl = 0;
	u8 power = 0;
	int ep_num = 0;
	u32 reg = 0;

	int usb_irq_temp = 0;
	int tx_irq_temp = 0;
	int rx_irq_temp = 0;

	q = sunxi_hcd;
	usb_irq_temp = q->int_usb;
	tx_irq_temp = q->int_tx;
	rx_irq_temp = q->int_rx;

	devctl = USBC_Readb(USBC_REG_DEVCTL(USBC0_BASE));
	power = USBC_Readb(USBC_REG_PCTL(USBC0_BASE));

	//printf("devctl : %02x;power : %02x\r\n",devctl,power);
	/* the core can interrupt us for multiple reasons; docs have
	 * a generic interrupt flowchart to follow
	 */
	if (usb_irq_temp)
	{
		//printf("usb_irq_temp %02x\r\n",usb_irq_temp);
		retval |= sunxi_hcd_stage0_irq(q, devctl, power);
	}

	/* "stage 1" is handling endpoint irqs */

	/* handle endpoint 0 first */
	if (tx_irq_temp & 1)
	{
		USBC_INT_ClearEpPending(USBC_EP_TYPE_TX, 0);

		USB_HOST_PRINTF("XXXXXXXsunxi_hcd_h_ep0_irq\r\n");

		retval |= sunxi_hcd_h_ep0_irq(sunxi_hcd);
	}

	/* RX on endpoints 1-15 */
	reg = rx_irq_temp >> 1;
	ep_num = 1;
	while (reg)
	{
		if (reg & 1)
		{
			USBC_INT_ClearEpPending(USBC_EP_TYPE_RX, ep_num);

			/* sunxi_hcd_ep_select(sunxi_hcd->mregs, ep_num); */
			/* REVISIT just retval = ep->rx_irq(...) */
			retval = IRQ_HANDLED;
			if (devctl & (1 << USBC_BP_DEVCTL_HOST_MODE))
			{
				USB_HOST_PRINTF("sunxi_hcd_host_rx\r\n");
				//sunxi_hcd_host_rx(sunxi_hcd, ep_num);
			}
		}

		reg >>= 1;
		ep_num++;
	}

	/* TX on endpoints 1-15 */
	reg = tx_irq_temp >> 1;
	ep_num = 1;
	while (reg)
	{
		if (reg & 1)
		{
			USBC_INT_ClearEpPending(USBC_EP_TYPE_TX, ep_num);

			/* sunxi_hcd_ep_select(sunxi_hcd->mregs, ep_num); */
			/* REVISIT just retval |= ep->tx_irq(...) */
			retval = IRQ_HANDLED;
			if (devctl & (1 << USBC_BP_DEVCTL_HOST_MODE))
			{

				USB_HOST_PRINTF("sunxi_hcd_host_tx\r\n");
				//sunxi_hcd_host_tx(sunxi_hcd, ep_num);
			}
		}

		reg >>= 1;
		ep_num++;
	}

	return retval;
}
irqreturn_t generic_interrupt(int arg)
{

	struct sunxi_hcd *q;
	int ret = 0;

	q = &host;
	/* Read status registers */
	q->int_usb = USBC_Readb(USBC_REG_INTUSB(USBC0_BASE));
	q->int_tx = USBC_Readw(USBC_REG_INTTx(USBC0_BASE));
	q->int_rx = USBC_Readw(USBC_REG_INTRx(USBC0_BASE));

	USB_HOST_PRINTF("\r\ngeneric_interrupt: usb_irq=%02x, tx_irq=%02x, rx_irq=%02x\r\n", q->int_usb, q->int_tx, q->int_rx);

	if (q->int_usb || q->int_tx || q->int_rx)
	{
		sunxi_hcd_interrupt(q);
	}
	printf("interrupt enable\r\n");
	return IRQ_HANDLED;
}
void host_reg_config(void)
{

	/* disable interrupts */
	USBC_Writeb(0x00, USBC_REG_INTUSBE(USBC0_BASE));
	USBC_Writew(0x00, USBC_REG_INTTxE(USBC0_BASE));
	USBC_Writew(0x00, USBC_REG_INTRxE(USBC0_BASE));

	printf("usb: %04x", USBC_Readb(USBC_REG_INTUSB(USBC0_BASE)));
	printf("tx:%04x", USBC_Readw(USBC_REG_INTTx(USBC0_BASE)));
	printf("rx:%04x\r\n", USBC_Readw(USBC_REG_INTRx(USBC0_BASE)));
	USBC_Writeb(0x00, USBC_REG_DEVCTL(USBC0_BASE));

	ep_config_from_table();
}
void host_init_controller(void)
{
	int reg = 0;
	int status = 0;

	host_reg_config();
	USB_HOST_PRINTF("\r\nhost_init_controller\r\n");
	// reg = request_irq(IRQ_USBOTG,generic_interrupt,0);
	irq_register(IRQ_LEVEL_1, F1C100S_IRQ_USBOTG, generic_interrupt, 3);

	sunxi_usb_enable_hcd0(&host);
}
static void host_DP_DM(void)
{
}
void host_IO_init(void)
{
	uint32_t reg_value = 0;
	USB_HOST_PRINTF("\r\nhost_IO_init\r\n");

	host_DP_DM();

	//1. open usb lock
	usb_phy_open_clock();
	//2
	USBC_PhyConfig();
	//3
	USBC_ConfigFIFO_Base();
	USBC_EnableDpDmPullUp();
	USBC_EnableIdPullUp();

	USBC_ForceId(USBC_ID_TYPE_HOST);
	USBC_ForceVbusValid(USBC_VBUS_TYPE_HIGH);
}
void test_usb(void)
{

	USB_HOST_PRINTF("\r\nusb_host_init\r\n");
	host_IO_init();
	host_init_controller();
	delay_ms(1000);

	while (1)
	{
		if (processs_flg)
		{
			processs_flg = 0;
		}
		delay_ms(1);
	}
}
