#include "usb.h"
// #include "sys_interrupt.h"
#include "usb_phy.h"
#include "usb_dev.h"
#include "usb_desc.h"
#include <string.h>
#include <irq.h>

usb_in_ep_callback_func usb_in_ep_callback = NULL;
usb_out_ep_callback_func usb_out_ep_callback = NULL;

void set_usb_in_ep_callback_func(usb_in_ep_callback_func callback)
{
	usb_in_ep_callback = callback;
}

void set_usb_out_ep_callback_func(usb_out_ep_callback_func callback)
{
	usb_out_ep_callback = callback;
}

unsigned char current_usb_type = USB_TYPE_DISCONNECT;
unsigned char current_speed = USB_SPEED_UNKNOWN;
unsigned int usb_connect = 0;
int usb_ep0_state = EP0_IDLE;
unsigned char usb_addr = 0;
unsigned char usb_xfer_type = NONE_REQ;
//static unsigned int  is_controller_alive = 0;
enum sunxi_udc_cmd_e
{
	SW_UDC_P_ENABLE = 1,	/* Pull-up enable        */
	SW_UDC_P_DISABLE = 2, /* Pull-up disable       */
	SW_UDC_P_RESET = 3,		/* UDC reset, in case of */
};
static int ep_max_len[3] = {64, DATA_PACKET_SIZE, DATA_PACKET_SIZE};
__attribute__((aligned(4))) unsigned char usb_tx_buf[DATA_PACKET_SIZE];
int usb_tx_buf_len = 0;
int usb_tx_pos = 0;
__attribute__((aligned(4))) unsigned char usb_rx_buf[DATA_PACKET_SIZE];
int usb_rx_buf_len = 0;
int usb_rx_pos = 0;
unsigned char *ep0txbuff;
/*——————————————————————————————————————————————————————————————
EP0 WRITE
*/
int usb_device_write_data_ep_pack(int ep, unsigned char *databuf, int len)
{
	memcpy(usb_tx_buf, databuf, len);
	usb_tx_buf_len = len;
	ep0txbuff = databuf;
	usbprint("CTRL IN LEN : %d\r\n", usb_tx_buf_len);
	if (len > ep_max_len[ep])
	{
		USBC_WritePacket(USBC_SelectFIFO(ep), ep_max_len[ep], databuf);
		USBC_Dev_WriteDataStatus(ep == 0 ? USBC_EP_TYPE_EP0 : USBC_EP_TYPE_TX, 0);
		usb_tx_pos = 64;
	}
	else
	{
		USBC_WritePacket(USBC_SelectFIFO(ep), len, databuf);
		USBC_Dev_WriteDataStatus(ep == 0 ? USBC_EP_TYPE_EP0 : USBC_EP_TYPE_TX, 1);
		usb_tx_pos = 0;
	}
	return 0;
}
/*——————————————————————————————————————————————————————————————
USB读数据包
*/
int usb_device_read_data_ep_pack(int ep, unsigned char *databuf, int len)
{
	USBC_ReadPacket(USBC_SelectFIFO(ep), len, databuf);
	return 0;
}
/*——————————————————————————————————————————————————————————————
USB发送空包
*/
void usb_device_send_nullpack_ep0(void)
{
	USBC_Dev_WriteDataStatus(USBC_EP_TYPE_EP0, 1);
}
/*——————————————————————————————————————————————————————————————
读EP0数据状态
*/
void usb_device_read_data_status_ep0(unsigned char is_complete)
{
	USBC_Dev_ReadDataStatus(USBC_EP_TYPE_EP0, is_complete);
}
/*——————————————————————————————————————————————————————————————
USB写数据1
*/
int usb_device_write_data_1(int ep, unsigned char *databuf, int len)
{
	int length = len;
	volatile int Timeout = 10000000;
	int pack_len = ep_max_len[ep];
	int data_pos = 0;
	if (usb_connect)
	{
		usbprint("usb_device_write_data len:%d\r\n", len);
		void *fifo = USBC_SelectFIFO(ep);
		USBC_SelectActiveEp(ep);
		while (length > pack_len)
		{
			while ((USBC_Dev_IsWriteDataReady(USBC_EP_TYPE_TX)) && (--Timeout))
				; //等待清除写标志
			USBC_WritePacket(fifo, pack_len, databuf + data_pos);
			USBC_Dev_WriteDataStatus(USBC_EP_TYPE_TX, 1);
			data_pos += pack_len;
			length -= pack_len;
			if (Timeout == 0)
			{
				usbprint("usb_device_write_data Time out!\r\n");
				return -1;
			}
			Timeout = 10000000;
		}
		while ((USBC_Dev_IsWriteDataReady(USBC_EP_TYPE_TX)) && (Timeout--))
			; //等待清除写标志
		USBC_WritePacket(fifo, length, databuf + data_pos);
		USBC_Dev_WriteDataStatus(USBC_EP_TYPE_TX, 1);
		return 0;
	}
	else
	{
		usbprint("usb_device_write_data faild! usb not connect!!\r\n");
		return -1;
	}
}
/*——————————————————————————————————————————————————————————————
USB写数据0
*/
int usb_device_write_data(int ep, unsigned char *databuf, int len)
{
	int length = len;
	int write_len = 0;
	volatile int Timeout = 10000000;
	int pack_len = ep_max_len[ep];
	int data_pos = 0;
	int old_ep = 0;
	if (usb_connect)
	{
		usbprint("usb_device_write_data len:%d\r\n", len);
		old_ep = USBC_GetActiveEp();
		void *fifo = USBC_SelectFIFO(ep);
		USBC_SelectActiveEp(ep);
		do
		{
			while ((USBC_Dev_IsWriteDataReady(USBC_EP_TYPE_TX)) && (--Timeout))
				; //等待清除写标志
			write_len = length > pack_len ? pack_len : length;
			USBC_WritePacket(fifo, write_len, databuf + data_pos);
			USBC_Dev_WriteDataStatus(USBC_EP_TYPE_TX, 1);
			data_pos += write_len;
			length -= write_len;
			if (Timeout == 0)
			{
				USBC_SelectActiveEp(old_ep);
				usbprint("usb_device_write_data Time out!\r\n");
				return -1;
			}
			Timeout = 10000000;
		} while (length > 0); // do..while 允许发0长度的packet
		USBC_SelectActiveEp(old_ep);
		return 0;
	}
	else
	{
		usbprint("usb_device_write_data faild! usb not connect!!\r\n");
		return -1;
	}
}
/*——————————————————————————————————————————————————————————————
清除硬件setup
*/
void usb_device_clear_setup_end(void)
{
	USBC_Dev_Ctrl_ClearSetupEnd();
}
/*——————————————————————————————————————————————————————————————
usb设置地址
*/
void usb_device_set_address(unsigned char addr)
{
	USBC_Dev_SetAddress(addr);
	usb_addr = addr;
}
/*——————————————————————————————————————————————————————————————
设置ep0 state
*/
void usb_device_set_ep0_state(enum ep0_state state)
{
	usb_ep0_state = state;
}
/*——————————————————————————————————————————————————————————————
USB硬件初始化
*/
int usb_dev_bsp_init(void)
{
	/* open usb lock */
	usb_phy_open_clock(); //need to open otg and phy clk

	USBC_PhyConfig();
	USBC_ConfigFIFO_Base();
	USBC_EnableDpDmPullUp();
	USBC_EnableIdPullUp();
	USBC_ForceId(USBC_ID_TYPE_DEVICE);
	USBC_ForceVbusValid(USBC_VBUS_TYPE_HIGH);

	USBC_SelectBus(USBC_IO_TYPE_PIO, 0, 0);

	return 0;
}
/*——————————————————————————————————————————————————————————————
清全剖中断
*/
static void clear_all_irq(void)
{
	USBC_INT_ClearEpPendingAll(USBC_EP_TYPE_TX);
	USBC_INT_ClearEpPendingAll(USBC_EP_TYPE_RX);
	USBC_INT_ClearMiscPendingAll();
}
/*——————————————————————————————————————————————————————————————
IRQ中断蒙板，返回具体中断
mask the useless irq, save disconect, reset, resume, suspend 
*/
static u32 filtrate_irq_misc(u32 irq_misc)
{
	u32 irq = irq_misc;

	irq &= ~(USBC_INTUSB_VBUS_ERROR | USBC_INTUSB_SESSION_REQ | USBC_INTUSB_CONNECT | USBC_INTUSB_SOF);
	USBC_INT_ClearMiscPending(USBC_INTUSB_VBUS_ERROR);
	USBC_INT_ClearMiscPending(USBC_INTUSB_SESSION_REQ);
	USBC_INT_ClearMiscPending(USBC_INTUSB_CONNECT);
	USBC_INT_ClearMiscPending(USBC_INTUSB_SOF);

	return irq;
}
/*——————————————————————————————————————————————————————————————
usbd开始
*/
static void usbd_start_work(void)
{
	usbprint("usbd_start_work\r\n");
	USBC_Dev_ConectSwitch(USBC_DEVICE_SWITCH_ON);
}
/*——————————————————————————————————————————————————————————————
usbd停止 
*/
static void usbd_stop_work(void)
{
	usbprint("usbd_stop_work\r\n");
	USBC_Dev_ConectSwitch(USBC_DEVICE_SWITCH_OFF); // default is pulldown
}
/*——————————————————————————————————————————————————————————————
配置UDC 命令
*/
static void cfg_udc_command(enum sunxi_udc_cmd_e cmd)
{
	switch (cmd)
	{
	case SW_UDC_P_ENABLE:
	{
		usbd_start_work();
	}
	break;
	case SW_UDC_P_DISABLE:
	{
		usbd_stop_work();
	}
	break;
	case SW_UDC_P_RESET:
		usbprint("ERR: reset is not support\r\n");
		break;
	default:
		usbprint("ERR: unkown cmd(%d)\r\n", cmd);
		break;
	}

	return;
}

/*——————————————————————————————————————————————————————————————
关闭UDC
*/
static void sunxi_udc_disable(void)
{
	usbprint("sunxi_udc_disable\r\n");
	/* Disable all interrupts */
	USBC_INT_DisableUsbMiscAll();
	USBC_INT_DisableEpAll(USBC_EP_TYPE_RX);
	USBC_INT_DisableEpAll(USBC_EP_TYPE_TX);
	/* Clear the interrupt registers */
	clear_all_irq();
	cfg_udc_command(SW_UDC_P_DISABLE);
	return;
}

extern unsigned char UVCHighSpeedConfigDscr[306];
/*——————————————————————————————————————————————————————————————
EP0控制端点中断处理
*/
void usb_handle_ep0(void)
{
	printf("usb ep 0 interupt!!\r\n");
	/* We make the assumption that sunxi_udc_UDC_IN_CSR1_REG equal to
		 * sunxi_udc_UDC_EP0_CSR_REG when index is zero */
	int ep0csr;
	int len;
	unsigned char databuf[8];
	USBC_SelectActiveEp(0);
	/* clear stall status */
	if (USBC_Dev_IsEpStall(USBC_EP_TYPE_EP0))
	{
		usbprint("ERR: ep0 stall\r\n");
		USBC_Dev_EpClearStall(USBC_EP_TYPE_EP0);
		usb_ep0_state = EP0_IDLE;
		return;
	}
	/* clear setup end */
	if (USBC_Dev_Ctrl_IsSetupEnd())
	{
		usbprint("handle_ep0: ep0 setup end\r\n");
		//sunxi_udc_nuke(dev, ep, 0);
		USBC_Dev_Ctrl_ClearSetupEnd();
		usb_ep0_state = EP0_IDLE;
	}
	usbprint("usb_ep0_state:%d\r\n", usb_ep0_state);
	usbprint("ep0csr:0x%02x\r\n", USBC_Dev_Read_EP0_CSR());
	if (USBC_Dev_IsReadDataReady(USBC_EP_TYPE_EP0)) //if that is setup data ,turn to idle state
	{
		usb_ep0_state = EP0_IDLE;
	}
	switch (usb_ep0_state)
	{
	case EP0_IDLE:
	{
		if (!USBC_Dev_IsReadDataReady(USBC_EP_TYPE_EP0))
		{
			usbprint("ERR: data is ready, can not read data.\r\n");
			return;
		}
		len = USBC_ReadLenFromFifo(USBC_EP_TYPE_EP0);
		usbprint("ep0 len :%d.\r\n", len);
		if (len != 8)
		{
			int timeout = 16;
			while ((timeout--) && (len != 8))
			{
				len = USBC_ReadLenFromFifo(USBC_EP_TYPE_EP0);
			}
			usbprint("ep0 len wait :%d.\r\n", len);
		}
		if (len == 8)
		{
			usb_device_read_data_ep_pack(0, databuf, len);
			usb_device_read_data_status_ep0(0);

			if (databuf[0] & 0x80)
			{
				usb_ep0_state = EP0_IN_DATA_PHASE;
			}
			else
			{
				usb_ep0_state = EP0_OUT_DATA_PHASE;
			}

			if ((current_usb_type == USB_TYPE_USB_HID) || (current_usb_type == USB_TYPE_USB_COM) || (current_usb_type == USB_TYPE_USB_UVC) || (current_usb_type == USB_TYPE_USB_MASS))
			{
				usb_setup_handle(databuf, len);
			}
			else
			{
				usbprint("ERR: usb class %d not suppost!!!\r\n", current_usb_type);
			}
		}
		else
		{
			usbprint("ERR: data len = %d, not setup data.\r\n", len);
			USBC_Dev_ReadDataStatus(USBC_EP_TYPE_EP0, 0);
			USBC_Dev_EpSendStall(USBC_EP_TYPE_EP0);
			return;
		}
	}
	break;
	case EP0_IN_DATA_PHASE:
		if (!usb_tx_pos)
		{
			//sunxi_udc_write_fifo(ep, req);
			usbprint("in data has sended!\r\n");
			usb_ep0_state = EP0_IDLE;
		}
		else
		{
			usbprint("in data is not write ok!\r\n");
			usbprint("usb_tx_pos : %d\r\n", usb_tx_pos);
			usbprint("usb_tx_buf_len : %d\r\n", usb_tx_buf_len);
			if (usb_tx_buf_len - usb_tx_pos > 64)
			{
				USBC_WritePacket(USBC_SelectFIFO(0), ep_max_len[0], ep0txbuff + usb_tx_pos);
				USBC_Dev_WriteDataStatus(USBC_EP_TYPE_EP0, 0);
				usb_tx_pos += 64;
			}
			else
			{
				usbprint("write end\r\n");
				USBC_WritePacket(USBC_SelectFIFO(0), usb_tx_buf_len - usb_tx_pos, ep0txbuff + usb_tx_pos);
				USBC_Dev_WriteDataStatus(USBC_EP_TYPE_EP0, 1);
				usb_tx_pos = 0;
				usb_ep0_state = EP0_IDLE;
			}
		}
		break;
	case EP0_OUT_DATA_PHASE:
		if (!USBC_Dev_IsReadDataReady(USBC_EP_TYPE_EP0))
		{
			usbprint("ep0_out_intr: data is ready, can not read data.\r\n");
			return;
		}
		len = USBC_ReadLenFromFifo(USBC_EP_TYPE_EP0);
		usbprint("ep0_out_intr len :%d.\r\n", len);
		usb_device_read_data_ep_pack(0, databuf, len);
		usb_device_read_data_status_ep0(0);
		usb_ep0_state = EP0_IDLE;
		usbprint("d[0] :0x%02x    data[1] :0x%02x    data[2] :0x%02x    data[3] :0x%02x    data[4] :0x%02x    data[5] :0x%02x    data[6] :0x%02x    data[7] :0x%02x.\r\n",
						 databuf[0], databuf[1], databuf[2], databuf[3], databuf[4], databuf[5],
						 databuf[6], databuf[7]);
		break;
	case EP0_END_XFER:
		if (usb_xfer_type == SET_ADDR) //no use
		{
		}
		else if (usb_xfer_type == SET_CONFIG)
		{
		}
		usb_xfer_type = NONE_REQ;
		usb_ep0_state = EP0_IDLE;
		break;
	default:
		usb_ep0_state = EP0_IDLE;
		break;
	}
	return;
}
/*——————————————————————————————————————————————————————————————
IN回调函数
*/
void usb_handle_epn_in(int ep)
{
	u32 old_ep_idx;
	u32 idx = ep;
	usbprint("USB in ep%d irq\r\n", ep);

	old_ep_idx = USBC_GetActiveEp();
	USBC_SelectActiveEp(idx);

	if (USBC_Dev_IsEpStall(USBC_EP_TYPE_TX))
	{
		usbprint("ERR: tx ep(%d) is stall\r\n", idx);
		USBC_Dev_EpClearStall(USBC_EP_TYPE_TX);
	}
	if (!USBC_Dev_IsWriteDataReady(USBC_EP_TYPE_TX))
	{
		usbprint("tx ep(%d) data ready!\r\n", idx);
		if (usb_in_ep_callback)
		{
			usb_in_ep_callback();
		}
		else
		{
			usbprint("ERR: usb_in_ep_callback NULL\r\n");
		}
	}
	USBC_SelectActiveEp(old_ep_idx);
}
/*——————————————————————————————————————————————————————————————
OUT回调函数
*/
void usb_handle_epn_out(int ep)
{
	u32 old_ep_idx;
	u32 idx = ep;
	u32 len;
	usbprint("USB out ep%d irq\r\n", ep);
	old_ep_idx = USBC_GetActiveEp();
	USBC_SelectActiveEp(idx);
	if (USBC_Dev_IsEpStall(USBC_EP_TYPE_RX))
	{
		usbprint("ERR: rx ep(%d) is stall\r\n", idx);
		USBC_Dev_EpClearStall(USBC_EP_TYPE_RX);
	}
	if (USBC_Dev_IsReadDataReady(USBC_EP_TYPE_RX))
	{
		len = USBC_ReadLenFromFifo(USBC_EP_TYPE_RX);
		usbprint("rx ep(%d) data ready Len:%d!\r\n", idx, len);
		usb_device_read_data_ep_pack(idx, usb_rx_buf, len);
		usb_rx_buf_len = len;
		if (usb_out_ep_callback)
		{
			usb_out_ep_callback(usb_rx_buf, usb_rx_buf_len);
		}
		else
		{
			usbprint("ERR: usb_out_ep_callback NULL\r\n");
		}
		USBC_Dev_ReadDataStatus(USBC_EP_TYPE_RX, 1);
	}
	USBC_SelectActiveEp(old_ep_idx);
}
/*——————————————————————————————————————————————————————————————
中断调用函数
*/
void usb_irq_handle(int arg)
{
	int usb_irq = 0;
	int tx_irq = 0;
	int rx_irq = 0;
	int i = 0;
	int dma_irq = 0;
	u32 old_ep_idx = 0;
	/* Save index */
	old_ep_idx = USBC_GetActiveEp();

	/* Read status registers */
	usb_irq = USBC_INT_MiscPending();
	tx_irq = USBC_INT_EpPending(USBC_EP_TYPE_TX);
	rx_irq = USBC_INT_EpPending(USBC_EP_TYPE_RX);

	usb_irq = filtrate_irq_misc(usb_irq);
	usbprint("_______________________________\r\n");
	usbprint("\r\nirq: usb_irq=%02x, tx_irq=%02x, rx_irq=%02x, dma_irq:%x\r\n", usb_irq, tx_irq, rx_irq, dma_irq);

	/*
		 * Now, handle interrupts. There's two types :
		 * - Reset, Resume, Suspend coming -> usb_int_reg
		 * - EP -> ep_int_reg
		 */

	/* RESET */
	if (usb_irq & USBC_INTUSB_RESET)
	{
		usbprint("IRQ: reset\r\n");
		USBC_INT_ClearMiscPending(USBC_INTUSB_RESET);
		clear_all_irq();
		usb_connect = 1;
		usb_ep0_state = EP0_IDLE;
		USBC_SelectActiveEp(0);
		USBC_Dev_SetAddress_default();
		return;
	}

	/* RESUME */
	if (usb_irq & USBC_INTUSB_RESUME)
	{
		usbprint("IRQ: resume\r\n");
		/* clear interrupt */
		USBC_INT_ClearMiscPending(USBC_INTUSB_RESUME);
		usb_connect = 1;
	}

	/* SUSPEND */
	if (usb_irq & USBC_INTUSB_SUSPEND)
	{
		usbprint("IRQ: suspend\r\n");

		/* clear interrupt */
		USBC_INT_ClearMiscPending(USBC_INTUSB_SUSPEND);
		usb_connect = 0;
		usb_ep0_state = EP0_IDLE;
	}

	/* DISCONNECT */
	if (usb_irq & USBC_INTUSB_DISCONNECT)
	{
		usbprint("IRQ: disconnect\r\n");

		USBC_INT_ClearMiscPending(USBC_INTUSB_DISCONNECT);
		usb_ep0_state = EP0_IDLE;
		usb_connect = 0;
	}

	/* EP */
	/* control traffic */
	/* check on ep0csr != 0 is not a good idea as clearing in_pkt_ready
		 * generate an interrupt
		 */
	if (tx_irq & USBC_INTTx_FLAG_EP0)
	{
		usbprint("USB ep0 irq\r\n");
		/* Clear the interrupt bit by setting it to 1 */
		USBC_INT_ClearEpPending(USBC_EP_TYPE_TX, 0);
		if (current_speed == USB_SPEED_UNKNOWN)
		{
			if (USBC_Dev_QueryTransferMode() == USBC_TS_MODE_HS)
			{
				current_speed = USB_SPEED_HIGH;
				ep_max_len[1] = 512;
				ep_max_len[2] = 512;
				usbprint("\r\n+++++++++++++++++++++++++++++++++++++\r\n");
				usbprint(" usb enter high speed.\r\n");
				usbprint("\r\n+++++++++++++++++++++++++++++++++++++\r\n");
			}
			else
			{
				current_speed = USB_SPEED_FULL;
				ep_max_len[1] = 64;
				ep_max_len[2] = 64;
				usbprint("\r\n+++++++++++++++++++++++++++++++++++++\r\n");
				usbprint(" usb enter full speed.\r\n");
				usbprint("\r\n+++++++++++++++++++++++++++++++++++++\r\n");
			}
		}
		usb_handle_ep0();
	}

	/* firstly to get data */

	/* rx endpoint data transfers */
	for (i = 1; i <= SW_UDC_EPNUMS; i++)
	{
		u32 tmp = 1 << i;
		if (rx_irq & tmp)
		{
			/* Clear the interrupt bit by setting it to 1 */
			USBC_INT_ClearEpPending(USBC_EP_TYPE_RX, i);
			usb_handle_epn_out(i);
		}
	}

	/* tx endpoint data transfers */
	for (i = 1; i <= SW_UDC_EPNUMS; i++)
	{
		u32 tmp = 1 << i;
		if (tx_irq & tmp)
		{
			/* Clear the interrupt bit by setting it to 1 */
			USBC_INT_ClearEpPending(USBC_EP_TYPE_TX, i);
			usb_handle_epn_in(i);
		}
	}

	/* Restore old index */
	USBC_SelectActiveEp(old_ep_idx);
}

/*——————————————————————————————————————————————————————————————
配置IN端点
*/
static u32 fifo_offset = 0; // global
void usb_config_ep_in(int epidx, int maxpack, int type)
{
	u32 old_ep_idx = 0;
	if (epidx)
	{
		usbprint("config int ep %d:%d , %d\r\n", epidx, maxpack, type);
		/* Save index */
		old_ep_idx = USBC_GetActiveEp();
		USBC_SelectActiveEp(epidx);
		u32 fifo_size = maxpack * 2; // double buffer
		USBC_Dev_ConfigEp(type, USBC_EP_TYPE_TX, 1, maxpack);
		USBC_ConfigFifo(USBC_EP_TYPE_TX, 1, fifo_size, fifo_offset);
		fifo_offset += fifo_size;
		USBC_INT_EnableEp(USBC_EP_TYPE_TX, epidx);
		USBC_SelectActiveEp(old_ep_idx);
		ep_max_len[epidx] = maxpack;
	}
}
/*——————————————————————————————————————————————————————————————
配置OUT端点
*/
void usb_config_ep_out(int epidx, int maxpack, int type)
{
	u32 old_ep_idx = 0;
	if (epidx)
	{
		usbprint("config out ep %d:%d , %d\r\n", epidx, maxpack, type);
		/* Save index */
		old_ep_idx = USBC_GetActiveEp();
		USBC_SelectActiveEp(epidx);
		u32 fifo_size = maxpack * 2; // double buffer
		USBC_Dev_ConfigEp(type, USBC_EP_TYPE_RX, 1, maxpack);
		USBC_ConfigFifo(USBC_EP_TYPE_RX, 1, fifo_size, fifo_offset);
		fifo_offset += fifo_size;
		USBC_INT_EnableEp(USBC_EP_TYPE_RX, epidx);
		USBC_SelectActiveEp(old_ep_idx);
		ep_max_len[epidx] = maxpack;
	}
}
/*——————————————————————————————————————————————————————————————
usb_device_clear_ep_halt
*/
void usb_device_clear_ep_halt(int epaddr)
{
	int old_ep_idx;
	int ep = epaddr & 0x7f;

	/* Save index */
	old_ep_idx = USBC_GetActiveEp();
	USBC_SelectActiveEp(ep);
	if (epaddr & 0x80)
	{
		USBC_Clear_Feature_HALT(USBC_EP_TYPE_TX);
	}
	else
	{
		USBC_Clear_Feature_HALT(USBC_EP_TYPE_RX);
	}
	USBC_SelectActiveEp(old_ep_idx);
}
/*——————————————————————————————————————————————————————————————
UDC使能
type com hid
*/
static void sunxi_udc_enable()
{
	usbprint("sunxi_udc_enable called\r\n");

	current_speed = USB_SPEED_UNKNOWN;

	usbprint("CONFIG_USB_GADGET_DUALSPEED: USBC_TS_MODE_HS\r\n");

	USBC_Dev_ConfigTransferMode(USBC_TS_TYPE_BULK, USBC_TS_MODE_HS);

	/* Enable reset and suspend interrupt interrupts */
	USBC_INT_EnableUsbMiscUint(USBC_BP_INTUSB_SUSPEND);
	USBC_INT_EnableUsbMiscUint(USBC_BP_INTUSB_RESUME);
	USBC_INT_EnableUsbMiscUint(USBC_BP_INTUSB_RESET);
	USBC_INT_EnableUsbMiscUint(USBC_BP_INTUSB_DISCONNECT);
	/* Enable ep0 interrupt */
	USBC_INT_EnableEp(USBC_EP_TYPE_TX, 0);

	cfg_udc_command(SW_UDC_P_ENABLE);

	return;
}
/*——————————————————————————————————————————————————————————————
USB初始化
type com hid
*/
int usb_device_init(unsigned char type)
{
	int retval = 0;
	printf("usb init...\r\n");
	if (current_usb_type != type)
	{
		current_usb_type = type;
		retval = usb_dev_bsp_init();
		if (retval != 0)
		{
			usbprint("ERR: sunxi_udc_bsp_init failed\r\n");
			return -1;
		}
		sunxi_udc_disable();
		/*中断实始化*/
		irq_register(IRQ_LEVEL_1, F1C100S_IRQ_USBOTG, usb_irq_handle, 3);
		// IRQ_Init(IRQ_LEVEL_1,IRQ_USG_OTG,usb_irq_handle ,3);
		// sysSetLocalInterrupt(ENABLE_IRQ);//
		/*UDC使能*/
		sunxi_udc_enable();
		printf("usb init end...\r\n");
		return retval;
	}
	printf("usb_type err...\r\n");
	return -1;
}
