#include "usb_keyboard.h"
#include "usb_phy.h"
#include "usb_dev.h"
#include "usb_desc.h"
#include <string.h>

#define CUSTOMHID_SIZ_REPORT_DESC  61  //35//33
		
	
#define	CONFIG_HID_DESCRIPTOR_LEN	(sizeof(USB_ConfigDescriptor) + \
				 sizeof(USB_InterfaceDescriptor) + \
				 sizeof(USB_HID_Descriptor) + \
				 sizeof(USB_EndPointDescriptor) * 2)

__attribute__ ((aligned(4))) USB_DeviceDescriptor HIDdevDesc =
{
	sizeof(USB_DeviceDescriptor),
	DEVICE_DESCRIPTOR,	//1
	0x0200,     //Version 2.0
	CLASS_HID,
	0x00,
	0x00,
	64,	/* Ep0 FIFO size */
	0x8888,
	0x1111,
	0x0200,
	0x01, 	//iManufacturer;
	0x02,   //iProduct;
	0x03,
	0x01
};

__attribute__ ((aligned(4))) static unsigned short str_ret[] = {
		0x0326,
		'H','2','7','5','0',' ',' ','U','s','b',' ','D','e','v','i','c','e','s'
};

__attribute__ ((aligned(4))) static unsigned short str_lang[] = {
	0x0304,
	0x0409
};

__attribute__ ((aligned(4))) static unsigned short str_isernum[] = {
		0x033e,
		'H','e','r','o','J','e',' ','S','c','a','n','n','e','r','s',' ','D','r','i','v','e','s',' ','-',' ','H','2','7','5','0'
};

__attribute__ ((aligned(4))) static unsigned short str_config[] = {
		0x031e,
		'C','D','C',' ','A','C','M',' ','c','o','n','f','i','g'
};

__attribute__ ((aligned(4))) static unsigned short str_interface1[] = {
		0x0342,
		'C','D','C',' ','A','b','s','t','r','a','c','t',' ','C','o','n','t','r','o','l',' ','M','o','d','e','l',' ','(','A','C','M',')'
};

__attribute__ ((aligned(4))) static unsigned short str_interface2[] = {
		0x031a,
		'C','D','C',' ','A','C','M',' ','D','a','t','a'
};

__attribute__ ((aligned(4))) struct {
	USB_ConfigDescriptor    configuration_descriptor;
	USB_InterfaceDescriptor hid_control_interface_descritor;
	USB_HID_Descriptor hid_header_descriptor;
	USB_EndPointDescriptor  hid_endpoint_descriptor[2];
} __attribute__ ((packed)) hid_confDesc = {
	{
		sizeof(USB_ConfigDescriptor),
		CONFIGURATION_DESCRIPTOR,
		CONFIG_HID_DESCRIPTOR_LEN,  /*  Total length of the Configuration descriptor */
		0x01,                   /*  NumInterfaces */
		0x01,                   /*  Configuration Value */
		0x00,                   /* Configuration Description String Index */
		0xC0,	// Self Powered, no remote wakeup
		0x32	// Maximum power consumption 500 mA
	},
	{
		sizeof(USB_InterfaceDescriptor),
		INTERFACE_DESCRIPTOR,
		0x00,   /* bInterfaceNumber */
		0x00,   /* bAlternateSetting */
		0x02,	/* ep number */
		0x03,   /* Interface Class */
		0x00,      /* Interface Subclass*/
		0x00,      /* Interface Protocol*/
		0x00    /* Interface Description String Index*/
	},
	{
		sizeof(USB_HID_Descriptor),
		0x21,
		0x0111,
		0x00,
		0x01,
		0x22,
		CUSTOMHID_SIZ_REPORT_DESC,
	},
	{
		{
			sizeof(USB_EndPointDescriptor),
			ENDPOINT_DESCRIPTOR,
			(1 << 7) | 1,// endpoint 3 IN
			3, /* interrupt */
			DATA_PACKET_SIZE, /* IN EP FIFO size */
			1
		},
		{
			sizeof(USB_EndPointDescriptor),
			ENDPOINT_DESCRIPTOR,
			(0 << 7) | 1,// endpoint 3 OUT
			3, /* interrupt */
			DATA_PACKET_SIZE, /* OUT EP FIFO size */
			1
		}
	},
};

// const u8 CustomHID_ReportDescriptor[CUSTOMHID_SIZ_REPORT_DESC] =
// {
// 	0x06, 0x06, 0xFF,	/* USAGE_PAGE (Vendor Defined)*/
//  	0x09, 0x01, 		/* USAGE (0x01)*/
//  	0xA1, 0x01,			/* COLLECTION (Application)*/
//  	0x15, 0x00,			/*     LOGICAL_MINIMUM (0)*/
//  	0x26, 0xFF, 0x00,	/*     LOGICAL_MAXIMUM (255)*/
//  	0x75, 0x08,			/*     REPORT_SIZE (8)*/
//  	0x96, 
// 	DATA_PACKET_SIZE & 0x00FF,           /*     REPORT_COUNT*/
// 	(DATA_PACKET_SIZE & 0xFF00) >> 8, 	
// 	0x09, 0x01,
//  	0x81, 0x02,			/*     INPUT (Data,Var,Abs)*/
//  	0x96,  	
// 	DATA_PACKET_SIZE & 0x00FF,           /*     REPORT_COUNT*/
// 	(DATA_PACKET_SIZE & 0xFF00) >> 8, 	 	 	
// 	0x09, 0x01,
//  	0x91, 0x02,			/*   OUTPUT (Data,Var,Abs)*/
//  	0x95, 0x08,			/*     REPORT_COUNT (8) */
//  	0x09, 0x01,
// 	0xB1, 0x02,			/*     FEATURE */
//  	0xC0				/* END_COLLECTION*/
	
// }; /* CustomHID_ReportDescriptor */

const u8 CustomHID_ReportDescriptor[CUSTOMHID_SIZ_REPORT_DESC] =
{
	0x05, 0x01, // USAGE_PAGE (Generic Desktop)
	0x09, 0x06, // USAGE (Keyboard)
	0xa1, 0x01, // COLLECTION (Application)
	0x05, 0x07, //     USAGE_PAGE (Keyboard/Keypad)
	0x19, 0xe0, //     USAGE_MINIMUM (Keyboard LeftControl)
	0x29, 0xe7, //     USAGE_MAXIMUM (Keyboard Right GUI)
	0x15, 0x00, //     LOGICAL_MINIMUM (0)
	0x25, 0x01, //     LOGICAL_MAXIMUM (1)
	0x95, 0x08, //     REPORT_COUNT (8)
	0x75, 0x01, //     REPORT_SIZE (1)
	0x81, 0x02, //     INPUT (Data,Var,Abs)
	0x95, 0x01, //     REPORT_COUNT (1)
	0x75, 0x08, //     REPORT_SIZE (8)
	0x81, 0x03, //     INPUT (Cnst,Var,Abs)
	0x95, 0x06, //   REPORT_COUNT (6)
	0x75, 0x08, //   REPORT_SIZE (8)
	0x25, 0xFF, //   LOGICAL_MAXIMUM (255)
	0x19, 0x00, //   USAGE_MINIMUM (Reserved (no event indicated))
	0x29, 0x65, //   USAGE_MAXIMUM (Keyboard Application)
	0x81, 0x00, //     INPUT (Data,Ary,Abs)
	0x25, 0x01, //     LOGICAL_MAXIMUM (1)
	0x95, 0x02, //   REPORT_COUNT (2)
	0x75, 0x01, //   REPORT_SIZE (1)
	0x05, 0x08, //   USAGE_PAGE (LEDs)
	0x19, 0x01, //   USAGE_MINIMUM (Num Lock)
	0x29, 0x02, //   USAGE_MAXIMUM (Caps Lock)
	0x91, 0x02, //   OUTPUT (Data,Var,Abs)
	0x95, 0x01, //   REPORT_COUNT (1)
	0x75, 0x06, //   REPORT_SIZE (6)
	0x91, 0x03, //   OUTPUT (Cnst,Var,Abs)
	0xc0        // END_COLLECTION	
}; /* CustomHID_ReportDescriptor */

/*——————————————————————————————————————————————————————————————
返回设备device描述符
*/
static void usb_get_device_descriptor(USB_DeviceRequest *req_data)
{
	unsigned int len = req_data->wLength;
	usbprint("usb_get_device_descriptor\r\n");
	
	if (len < sizeof(USB_DeviceDescriptor))
	{
		usb_device_write_data_ep_pack(0, &HIDdevDesc, len);
	}
	else
	{
		usb_device_write_data_ep_pack(0, &HIDdevDesc, sizeof(USB_DeviceDescriptor));
	}
}
/*——————————————————————————————————————————————————————————————
返回配置cfg描述符
*/
void usb_hid_get_cfg_descriptor(USB_DeviceRequest *req_data)
{
	usbprint("usb_hid_get_cfg_descriptor\r\n");
	switch(req_data->wLength)
	{
		case 9:
			usb_device_write_data_ep_pack(0,  (unsigned char *)&hid_confDesc, 9);
			break;
		case 8:
			usb_device_write_data_ep_pack(0,  (unsigned char *)&hid_confDesc, 8);
			break;
		default:
			usb_device_write_data_ep_pack(0,  (unsigned char *)&hid_confDesc, sizeof(hid_confDesc));
			break;
	}
}
/*——————————————————————————————————————————————————————————————
返回设备字符串
*/
static inline void udp_get_dev_descriptor_string(USB_DeviceRequest *dreq)
{
	int len = dreq->wLength;
	int index = dreq->wValue & 0xff;
	usbprint("-------------udp_get_dev_descriptor_string:%d\r\n",index);
	switch ( index)
	{
		case 0:       //land ids
			if ( len > sizeof(str_lang) )
				usb_device_write_data_ep_pack(0,(unsigned char *)str_lang,sizeof(str_lang));
			else
				usb_device_write_data_ep_pack(0,(unsigned char *)str_lang,len);
			return;
			break;
		case 1:       //iserialnumber
			if (len >= sizeof(str_isernum))
				len = sizeof(str_isernum);
			usb_device_write_data_ep_pack(0,(unsigned char *)str_isernum,len);
			break;

		case 2:       //iproduct
			if (len >= 38)
				len = 38;
			str_ret[0] = (0x0300 | len);
			usb_device_write_data_ep_pack(0,(unsigned char *)str_ret,len);
			break;
		case 3:       //iserialnumber
			if (len >= sizeof(str_isernum))
				len = sizeof(str_isernum);
			usb_device_write_data_ep_pack(0,(unsigned char *)str_isernum,len);
			break;
		case 4:
			if (len >= sizeof(str_config))
				len = sizeof(str_config);
			usb_device_write_data_ep_pack(0,(unsigned char *)str_config,len);
			break;
		case 5:
			if (len >= sizeof(str_interface1))
				len = sizeof(str_interface1);
			usb_device_write_data_ep_pack(0,(unsigned char *)str_interface1,len);
			break;
		case 6:
			if (len >= sizeof(str_interface2))
				len = sizeof(str_interface2);
			usb_device_write_data_ep_pack(0,(unsigned char *)str_interface2,len);
			break;
		case 0xee:    //microsoft OS!
			if (len >= sizeof(str_isernum))
				len = sizeof(str_isernum);
			str_isernum[0] = (0x0300 | len);
			usb_device_write_data_ep_pack(0,(unsigned char *)str_isernum,len);
			break;
		default:
			usb_device_write_data_ep_pack(0,0,0);
			break;
	}
}

/*——————————————————————————————————————————————————————————————
返回HID REPORT_DESCRIPTOR描述符
*/
void usb_get_hid_descport(USB_DeviceRequest *req_data)
{
	usb_device_write_data_ep_pack(0,  (unsigned char *)&CustomHID_ReportDescriptor, CUSTOMHID_SIZ_REPORT_DESC);
}

void usb_hid_descriptor_request(enum USB_DESCRIPTOR_TYPE type, USB_DeviceRequest* req_data)
{
	usbprint("usb_hid_descriptor_request : 0x%04x!\r\n",type);
	// unsigned char  wValue = req_data->wValue >> 8;
	switch(type)
	{
		case DEVICE_DESCRIPTOR ://Device DISCRIPTOR
					usb_get_device_descriptor(req_data);break;
		case CONFIGURATION_DESCRIPTOR ://Config DISCRIPTOR
					usb_hid_get_cfg_descriptor(req_data);break;
		case STRING_DESCRIPTOR ://String DISCRIPTOR
					udp_get_dev_descriptor_string(req_data);break;
//		case HID_DESCRIPTOR_TYPE ://Hid DISCRIPTOR
//					usb_get_hid_descriptor(req_data);break;
		case REPORT_DESCRIPTOR ://Hid DISCRIPTOR
					usb_get_hid_descport(req_data);	
		break;
		default:
					usbprint("usb get descriptor : 0x%04x not suppost!\r\n",type);
					usb_device_read_data_status_ep0(1);
					break;
	}
}

/*——————————————————————————————————————————————————————————————
usb_hid_in_ep_callback
*/
int usb_hid_in_ep_callback(void)
{
	//printf("usb_hid_in_ep_callback\r\n");
	return 0;
}
/*——————————————————————————————————————————————————————————————
usb_hid_out_ep_callback
*/
void usb_hid_out_ep_callback(unsigned char *pdat,int len)
{
	printf("hid-Len=%d\r\n",len);
	usb_device_write_data(1,pdat,len);//回送	
}