/*
 * usb_host.h
 *
 *  Created on: Apr 25, 2020
 *      Author: root
 */

#ifndef DRIVER_INCLUDE_USB_HOST_H_
#define DRIVER_INCLUDE_USB_HOST_H_

#define SW_HCD_C_NUM_EPS USBC_MAX_EP_NUM

enum irqreturn
{
	IRQ_NONE = (0 << 0),
	IRQ_HANDLED = (1 << 0),
	IRQ_WAKE_THREAD = (1 << 1),
};

typedef enum irqreturn irqreturn_t;

enum fifo_style
{
	FIFO_RXTX = 0,
	FIFO_TX,
	FIFO_RX
} __attribute__((packed));

typedef struct sunxi_hcd_hw_ep
{
	u8 hw_ep_num;
	enum fifo_style style;
	u16 maxpacket;
	void *fifo; //packt base
} sunxi_hcd_hw_ep_t;

enum sunxi_hcd_h_ep0_state
{
	SUNXI_HCD_EP0_IDLE,
	SUNXI_HCD_EP0_START,	/* expect ack of setup */
	SUNXI_HCD_EP0_IN,			/* expect IN DATA */
	SUNXI_HCD_EP0_OUT,		/* expect ack of OUT DATA */
	SUNXI_HCD_EP0_STATUS, /* expect ack of STATUS */
} __attribute__((packed));

/*
 * wPortChange bit field
 * See USB 2.0 spec Table 11-22 and USB 2.0 LPM ECN Table-4.10
 * Bits 0 to 5 shown, bits 6 to 15 are reserved
 */
#define USB_PORT_STAT_C_CONNECTION 0x0001
#define USB_PORT_STAT_C_ENABLE 0x0002
#define USB_PORT_STAT_C_SUSPEND 0x0004
#define USB_PORT_STAT_C_OVERCURRENT 0x0008
#define USB_PORT_STAT_C_RESET 0x0010
#define USB_PORT_STAT_C_L1 0x0020

/*
* wPortStatus bit field
* See USB 2.0 spec Table 11-21
*/
#define USB_PORT_STAT_CONNECTION 0x0001
#define USB_PORT_STAT_ENABLE 0x0002
#define USB_PORT_STAT_SUSPEND 0x0004
#define USB_PORT_STAT_OVERCURRENT 0x0008
#define USB_PORT_STAT_RESET 0x0010
#define USB_PORT_STAT_L1 0x0020
/* bits 6 to 7 are reserved */
#define USB_PORT_STAT_POWER 0x0100
#define USB_PORT_STAT_LOW_SPEED 0x0200
#define USB_PORT_STAT_HIGH_SPEED 0x0400
#define USB_PORT_STAT_TEST 0x0800
#define USB_PORT_STAT_INDICATOR 0x1000

#define sunxi_hcd_read_configdata(usbc_base) USBC_Readb(USBC_REG_CONFIGDATA(usbc_base))
#define sunxi_hcd_ep_select(usbc_base, epnum) USBC_Writeb(epnum, USBC_REG_EPIND(usbc_base))

typedef struct sunxi_hcd_eps_bits
{
	const char name[16];
	u8 bits;
} sunxi_hcd_eps_bits_t;

typedef struct sunxi_hcd
{
	/* this hub status bit is reserved by USB 2.0 and not seen by usbcore */
#define SUNXI_HCD_PORT_STAT_RESUME (1 << 31)
	u32 port1_status;				/* virtual hub's port status      */
	unsigned long rh_timer; /* root hub's delay time          */

	enum sunxi_hcd_h_ep0_state ep0_stage;

	/* passed down from chip/board specific irq handlers */
	u8 int_usb; /* USB irq                       */
	u16 int_rx; /* rx irq                        */
	u16 int_tx; /* tx irq                        */

	unsigned char is_host; /* flag. if is host transfer      */
	/* active means connected and not suspended */
	unsigned is_active : 1;
	unsigned is_connected : 1;
	unsigned is_reset : 1;
	unsigned is_suspend : 1;
	unsigned is_multipoint : 1;			/* flag. is multiple transaction ep? */
	unsigned ignore_disconnect : 1; /* during bus resets              */
	unsigned bulk_split : 1;
	unsigned bulk_combine : 1;

	u32 enable;
	u32 init_controller;
	u32 suspend;
	u32 session_req_flag;
	u32 reset_flag;
	u32 vbus_error_flag;
} sunxi_hcd_t;

struct sunxi_hcd_ep_reg
{
	uint32_t USB_CSR0;
	uint32_t USB_TXCSR;
	uint32_t USB_RXCSR;
	uint32_t USB_COUNT0;
	uint32_t USB_RXCOUNT;
	uint32_t USB_ATTR0;
	uint32_t USB_EPATTR;
	uint32_t USB_TXFIFO;
	uint32_t USB_RXFIFO;
	uint32_t USB_FADDR;
	uint32_t USB_TXFADDR;
	uint32_t USB_RXFADDR;
};

struct sunxi_hcd_context_registers
{
	/* FIFO Entry for Endpoints */
	uint32_t USB_EPFIFO0;
	uint32_t USB_EPFIFO1;
	uint32_t USB_EPFIFO2;
	uint32_t USB_EPFIFO3;
	uint32_t USB_EPFIFO4;
	uint32_t USB_EPFIFO5;

	/* Common Register */
	uint32_t USB_GCS;
	uint32_t USB_EPINTF;
	uint32_t USB_EPINTE;
	uint32_t USB_BUSINTF;
	uint32_t USB_BUSINTE;
	uint32_t USB_FNUM;
	uint32_t USB_TESTC;

	/* Endpoint Index Register */
	struct sunxi_hcd_ep_reg ep_reg[SW_HCD_C_NUM_EPS];

	/* Configuration Register */
	uint32_t USB_CONFIGINFO;
	uint32_t USB_LINKTIM;
	uint32_t USB_OTGTIM;

	/* PHY and Interface Control and Status Register */
	uint32_t USB_ISCR;
	uint32_t USB_PHYCTL;
	uint32_t USB_PHYBIST;
};

#endif /* DRIVER_INCLUDE_USB_HOST_H_ */
