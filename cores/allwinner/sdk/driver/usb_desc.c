#include "usb.h"
#include "usb_dev.h"
#include "usb_desc.h"
#include <string.h>
#include "usb_phy.h"
// #include <sys_delay.h>
#include <timer.h>
/*
 * HID class requests
 */
#define HID_REQ_GET_REPORT 0x01
#define HID_REQ_GET_IDLE 0x02
#define HID_REQ_GET_PROTOCOL 0x03
#define HID_REQ_SET_REPORT 0x09
#define HID_REQ_SET_IDLE 0x0A
#define HID_REQ_SET_PROTOCOL 0x0B

usb_descriptor_request_func usb_descriptor_request = NULL;

void set_usb_descriptor_request_func(usb_descriptor_request_func callback)
{
	usb_descriptor_request = callback;
}

USB_DeviceRequest usb_setup_request;

// __attribute__ ((aligned(4))) USB_DeviceDescriptor MASSdevDesc =
// {
// 	sizeof(USB_DeviceDescriptor),
// 	DEVICE_DESCRIPTOR,	//1
// 	0x0200,     //Version 2.0
// 	CLASS_DEVICE,
// 	0x00,
// 	0x00,
// 	0x40,	/* Ep0 FIFO size */
// 	0x0483,
// 	0x5720,
// 	0x0200,
// 	0x01, 	//iManufacturer;
// 	0x02,   //iProduct;
// 	0x00,
// 	0x01
// };

// __attribute__ ((aligned(4))) USB_DeviceDescriptor CDCdevDesc =
// {
// 	sizeof(USB_DeviceDescriptor),
// 	DEVICE_DESCRIPTOR,	//1
// 	0x0200,     //Version 2.0
// 	CLASS_COMM_AND_CDC_CONTROL,
// 	0x00,
// 	0x00,
// 	64,	/* Ep0 FIFO size */
// 	0x0483,
// 	0x5740,
// 	0x0200,
// 	0x01, 	//iManufacturer;
// 	0x02,   //iProduct;
// 	0x00,
// 	0x01
// };

// __attribute__ ((aligned(4))) USB_DeviceDescriptor HIDdevDesc =
// {
// 	sizeof(USB_DeviceDescriptor),
// 	DEVICE_DESCRIPTOR,	//1
// 	0x0200,     //Version 2.0
// 	CLASS_HID,
// 	0x00,
// 	0x00,
// 	64,	/* Ep0 FIFO size */
// 	0x8888,
// 	0x1111,
// 	0x0200,
// 	0x01, 	//iManufacturer;
// 	0x02,   //iProduct;
// 	0x03,
// 	0x01
// };

// __attribute__ ((aligned(4))) USB_DeviceDescriptor UVCdevDesc =
// {
// 	sizeof(USB_DeviceDescriptor),
// 	DEVICE_DESCRIPTOR,	//1
// 	0x0200,     //Version 2.0
// 	CLASS_MISCELLANEOUS,
// 	0x02,
// 	0x01,
// 	64,	/* Ep0 FIFO size */
// 	0x0416,
// 	0x9393,
// 	0x0200,
// 	0x01, 	//iManufacturer;
// 	0x02,   //iProduct;
// 	0x00,
// 	0x01
// };

/*——————————————————————————————————————————————————————————————
设置地址
*/
static void set_address(unsigned char addr)
{
	// sys_delay_ms(1);
	usb_device_set_address(addr);
	usb_device_read_data_status_ep0(1);

	usbprint("set dev address: %d\r\n", addr);
	usb_device_set_ep0_state(EP0_IDLE);
}
/*——————————————————————————————————————————————————————————————
设置配置
*/
static void set_configuration(void)
{
	if (current_usb_type == USB_TYPE_USB_MASS)
	{
		usb_config_ep_out(1, DATA_PACKET_SIZE, USBC_TS_TYPE_BULK);
		usb_config_ep_in(1, DATA_PACKET_SIZE, USBC_TS_TYPE_BULK);
	}
	else
	{
		usb_config_ep_out(1, DATA_PACKET_SIZE, USBC_TS_TYPE_INT);
		usb_config_ep_in(1, DATA_PACKET_SIZE, USBC_TS_TYPE_INT);
	}

	usb_device_read_data_status_ep0(1);

	usb_device_clear_setup_end();
	usb_device_send_nullpack_ep0();
	usb_device_set_ep0_state(EP0_IDLE);
}
/*——————————————————————————————————————————————————————————————
 标准步
*/
void standard_setup_request(USB_DeviceRequest *req_data)
{
	unsigned char bRequest = req_data->bRequest;
	if (bRequest == USB_REQ_GET_DESCRIPTOR)
	{
		usbprint("getDescriptor\r\n");
		if (usb_descriptor_request)
		{
			unsigned char wValue = req_data->wValue >> 8;
			usb_descriptor_request(wValue, req_data);
		}
		else
		{
			usbprint("getDescriptor NULL\r\n");
		}
	}
	else if (bRequest == USB_REQ_SET_CONFIGURATION)
	{
		usbprint("set_configuration\r\n");
		set_configuration();
	}
	else if (bRequest == USB_REQ_GET_CONFIGURATION)
	{
		usbprint("get_configuration\r\n");
		//not support
		usbprint("Error!! unsupprot USB_REQ_GET_CONFIGURATION command");
		usb_device_read_data_status_ep0(1);
	}
	else if (bRequest == USB_REQ_SET_INTERFACE)
	{
		usbprint("set_interface\r\n");
		//not support
		usbprint("Error!! unsupprot USB_REQ_SET_INTERFACE command");
		usb_device_read_data_status_ep0(1);
	}
	else if (bRequest == USB_REQ_GET_INTERFACE)
	{
		//not support
		usbprint("Error!! unsupprot USB_REQ_GET_INTERFACE command");
		usb_device_read_data_status_ep0(1);
	}
	else if (bRequest == USB_REQ_SET_ADDRESS)
	{
		usbprint("set_addr\r\n");
		set_address(req_data->wValue & 0x7f);
	}
	else if (bRequest == USB_REQ_SET_DESCRIPTOR)
	{
		usbprint("set_Descriptor\r\n");
		//not support
		usbprint("Error!! unsupprot USB_REQ_SET_DESCRIPTOR command");
		usb_device_read_data_status_ep0(1);
	}
	else if (bRequest == USB_REQ_SYNCH_FRAME)
	{
		usbprint("sync frame\r\n");
		//not support
		usbprint("Error!! unsupprot USB_REQ_SYNCH_FRAME command");
		usb_device_read_data_status_ep0(1);
	}
	else if (bRequest == USB_REQ_CLEAR_FEATURE)
	{
		int epaddr = req_data->wIndex;
		usbprint("clear feature\r\n");
		usb_device_clear_ep_halt(epaddr);
		usb_device_read_data_status_ep0(1);
	}
	else
	{
		usbprint("other  req \r\n");
		usbprint("Error!! received controller command:%02X.\r\n", bRequest);
		usb_device_read_data_status_ep0(1);
		//other command process by controller
	}
}
/*——————————————————————————————————————————————————————————————
 类步
*/

__attribute__((aligned(4))) USB_CDC_LineCoding port_line_coding = {
		.dwDTERate = 115200,
		.bCharFormat = 0,
		.bParityType = 0,
		.bDataBits = 8};
unsigned int port_line_coding_flag;
void set_line_codingstatic_ext(unsigned char *pdata)
{
	port_line_coding_flag = 0;
	port_line_coding.dwDTERate = *(volatile unsigned int *)(pdata);
	port_line_coding.bCharFormat = *(volatile unsigned char *)(pdata + 4);
	port_line_coding.bParityType = *(volatile unsigned char *)(pdata + 5);
	port_line_coding.bDataBits = *(volatile unsigned char *)(pdata + 6);
}

void class_setup_request(USB_DeviceRequest *req_data)
{
	u32 dMax_Lun = 0;
	u8 buffl[8] = {0};
	unsigned char bRequest = req_data->bRequest;
	if (bRequest == HID_REQ_SET_REPORT) //HID Set report
	{
		usbprint("HID_REQ_SET_REPORT, host set device_report success!!!\r\n");
	}
	else if (bRequest == HID_REQ_SET_IDLE)
	{
		usbprint("HID_REQ_SET_IDLE, host set device_report success!!!\r\n");
	}
	else if (bRequest == 0x20) //set_line_coding
	{
		usbprint("set_line_coding\r\n");
		port_line_coding_flag = 1;
		/*------------------------------------------------------*/
		delay_us(200); //COM去打印后电脑提示占用，加延时最少120us
									 /*------------------------------------------------------*/
		int len = USBC_ReadLenFromFifo(1);
		usbprint("set_line_coding:readlen:%d\r\n", len);
		if (len == 7)
		{
			unsigned char tmp[7];
			usb_device_read_data_ep_pack(0, tmp, 7);
			set_line_codingstatic_ext(tmp);
			usb_device_read_data_status_ep0(1);
			usb_device_set_ep0_state(EP0_IDLE);
			port_line_coding_flag = 0;
		}
	}
	else if (bRequest == 0x21) //get_line_coding
	{
		int lent = sizeof(USB_CDC_LineCoding);
		usb_device_write_data_ep_pack(0, (u8 *)(&port_line_coding), lent);
		usbprint("get_line_coding\r\n");
	}
	else if (bRequest == 0x22) //set_control_line_state
	{
		usbprint("set_control_line_state \r\n");
		usb_device_clear_setup_end();
		usb_device_set_ep0_state(EP0_IDLE);
	}
	else if (bRequest == 0xFE) //msc
	{
		// usbprint("MASS GET_MAX_LUN: %d \r\n",*(( u8 *)Get_Max_Lun()));
		usbprint("MASS GET_MAX_LUN: \r\n");
		usb_device_write_data_ep_pack(0, (u8 *)(&dMax_Lun), req_data->wLength);
	}
	else
	{
		usbprint("Warnning!!! received unsupport command:%08x ,do nothing\r\n", bRequest);
	}
}
/*——————————————————————————————————————————————————————————————
usb_hid_setup_handle
*/
void usb_setup_handle(unsigned char *dat, int len)
{
	volatile int Timeout = 10000000;
	if (len == 8)
	{
		memcpy(&usb_setup_request, dat, len); //usb is big endian
		usbprint("bRequestType:0x%02x\r\n", usb_setup_request.bRequestType);
		usbprint("bRequest:0x%02x\r\n", usb_setup_request.bRequest);
		usbprint("wValue:0x%04x\r\n", usb_setup_request.wValue);
		usbprint("wIndex:0x%04x\r\n", usb_setup_request.wIndex);
		usbprint("wLength:0x%04x\r\n", usb_setup_request.wLength);

		if ((usb_setup_request.bRequestType & USB_TYPE_MASK) == USB_TYPE_STANDARD)
		{
			standard_setup_request(&usb_setup_request);
		}
		else if ((usb_setup_request.bRequestType & USB_TYPE_MASK) == USB_TYPE_CLASS)
		{
			class_setup_request(&usb_setup_request);
		}
		else if ((usb_setup_request.bRequestType & USB_TYPE_MASK) == USB_TYPE_VENDOR)
		{
		}
	}
	else
	{
		usbprint("not setup data.\r\n");
	}
}
