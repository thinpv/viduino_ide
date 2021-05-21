#include "usb.h"
#include "usb_phy.h"
#include "usb_dev.h"
#include "usb_desc.h"
#include <string.h>
#include "usb_scsi.h"
#include "usb_Mass_storage.h"
#include "usb_bot.h"
#include "usb_desc.h"
// #include <types.h>

extern Bulk_Only_CBW CBW;
extern Bulk_Only_CSW CSW;

uint64_t Mass_Memory_Size[MAX_LUN+1];
u32 Mass_Block_Size[MAX_LUN+1]={512};
u32 Mass_Block_Count[MAX_LUN+1];

u8 Max_Lun =MAX_LUN; //定义USB设备的数量，1代表2个，0代表1个


__attribute__ ((aligned(4))) unsigned char MASS_ConfigDescriptor[MASS_SIZ_CONFIG_DESC] =
  {

    0x09,   /* bLength: Configuration Descriptor size */
    0x02,   /* bDescriptorType: Configuration */
    MASS_SIZ_CONFIG_DESC,

    0x00,
    0x01,   /* bNumInterfaces: 1 interface */
    0x01,   /* bConfigurationValue: */
    /*      Configuration value */
    0x00,   /* iConfiguration: */
    /*      Index of string descriptor */
    /*      describing the configuration */
    0xC0,   /* bmAttributes: */
    /*      Self powered */
    0x32,   /* MaxPower 100 mA */

    /******************** Descriptor of Mass Storage interface ********************/
    /* 09 */
    0x09,   /* bLength: Interface Descriptor size */
    0x04,   /* bDescriptorType: */
    /*      Interface descriptor type */
    0x00,   /* bInterfaceNumber: Number of Interface */
    0x00,   /* bAlternateSetting: Alternate setting */
    0x02,   /* bNumEndpoints*/
    0x08,   /* bInterfaceClass: MASS STORAGE Class */
    0x06,   /* bInterfaceSubClass : SCSI transparent*/
    0x50,   /* nInterfaceProtocol */
    4,          /* iInterface: */
    /* 18 */
    0x07,   /*Endpoint descriptor length = 7*/
    0x05,   /*Endpoint descriptor type */
    0x81,   /*Endpoint address (IN, address 1) */
    0x02,   /*Bulk endpoint type */		
		DATA_PACKET_SIZE & 0x00FF,           /*     REPORT_COUNT*/
		(DATA_PACKET_SIZE & 0xFF00) >> 8, 
    0x00,   /*Polling interval in milliseconds */
    /* 25 */
    0x07,   /*Endpoint descriptor length = 7 */
    0x05,   /*Endpoint descriptor type */
    0x01,   /*Endpoint address (OUT, address 2) */
    0x02,   /*Bulk endpoint type */
		DATA_PACKET_SIZE & 0x00FF,           /*     REPORT_COUNT*/
		(DATA_PACKET_SIZE & 0xFF00) >> 8, 
    0x00     /*Polling interval in milliseconds*/
    /*32*/
  };

//写数据
uint32_t USB_SIL_Write(uint8_t bEpAddr, uint8_t* pBufferPointer, uint32_t wBufferSize)
{
  usb_device_write_data(1,pBufferPointer,wBufferSize);//回送	
	return 0;
}	
//读数据
uint32_t USB_SIL_Read(uint8_t bEpAddr, uint8_t* pBufferPointer)
{
  uint32_t DataLength = 0;
  return DataLength;
}
//写状态
void SetEPTxStatus(uint8_t bEpNum, uint16_t wState)
{

}
void SetEPRxStatus(uint8_t bEpNum, uint16_t wState)
{

}
uint16_t GetEPRxStatus(uint8_t bEpNum)
{
  return 0;
}
void SetEPTxCount(uint8_t bEpNum, uint16_t wCount)
{

}
/*******************************************************************************
* Function Name  : Get_Max_Lun
* Description    : Handle the Get Max Lun request.
* Input          : uint16_t Length.
*******************************************************************************/	
u8 *Get_Max_Lun(void)
{
    return((u8*)(&Max_Lun));
}
/*——————————————————————————————————————————————————————————————
返回HID配置cfg描述符
*/
void usb_mass_get_cfg_descriptor(USB_DeviceRequest *req_data)
{
	usbprint("usb_mass_get_cfg_descriptor\r\n");
	switch(req_data->wLength)
	{
		case 9:
			usb_device_write_data_ep_pack(0,  (unsigned char *)&MASS_ConfigDescriptor, 9);
			break;
		case 8:
			usb_device_write_data_ep_pack(0,  (unsigned char *)&MASS_ConfigDescriptor, 8);
			break;
		default:
			usb_device_write_data_ep_pack(0,  (unsigned char *)&MASS_ConfigDescriptor, sizeof(MASS_ConfigDescriptor));
			break;
	}
}

/*——————————————————————————————————————————————————————————————
usb_mass_in_ep_callback
*/
int usb_mass_in_ep_callback(void)
{
	//printf("usb_mass_in_ep_callback\r\n");
  Mass_Storage_In ();

	return 0;
}
/*——————————————————————————————————————————————————————————————
usb_mass_out_ep_callback
*/
void usb_mass_out_ep_callback(unsigned char *pdat,int len)
{
  int i=0;
	Mass_Storage_Out(pdat,len);
	
//	printf("mass-Len=%d\r\n",len);
//	for(i=0;i<len;i++)printf("%02x,",pdat[i]);printf("\r\n");
//	usb_device_write_data(1,pdat,len);//回送	
}

__attribute__ ((aligned(4))) USB_DeviceDescriptor MASSdevDesc =
{
	sizeof(USB_DeviceDescriptor),
	DEVICE_DESCRIPTOR,	//1
	0x0200,     //Version 2.0
	CLASS_DEVICE,
	0x00,
	0x00,
	0x40,	/* Ep0 FIFO size */
	0x0483,
	0x5720,
	0x0200,
	0x01, 	//iManufacturer;
	0x02,   //iProduct;
	0x00,
	0x01
};

static void usb_get_device_descriptor(USB_DeviceRequest *req_data)
{
	unsigned int len = req_data->wLength;
	usbprint("usb_get_device_descriptor\r\n");
	
	if (len < sizeof(USB_DeviceDescriptor))
	{
		usb_device_write_data_ep_pack(0, &MASSdevDesc, len);
	}
	else
	{
		usb_device_write_data_ep_pack(0, &MASSdevDesc, sizeof(USB_DeviceDescriptor));
	}
}

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

void usb_mass_descriptor_request(enum USB_DESCRIPTOR_TYPE type, USB_DeviceRequest* req_data)
{
	usbprint("usb_mass_descriptor_request : 0x%04x!\r\n",type);
	// unsigned char  wValue = req_data->wValue >> 8;
	switch(type)
	{
		case DEVICE_DESCRIPTOR ://Device DISCRIPTOR
					usb_get_device_descriptor(req_data);break;
		case CONFIGURATION_DESCRIPTOR ://Config DISCRIPTOR
					usb_mass_get_cfg_descriptor(req_data);break;
		case STRING_DESCRIPTOR ://String DISCRIPTOR
					udp_get_dev_descriptor_string(req_data);break;
//		case HID_DESCRIPTOR_TYPE ://Hid DISCRIPTOR
//					usb_get_hid_descriptor(req_data);break;
		case REPORT_DESCRIPTOR ://Hid DISCRIPTOR
					// usb_get_hid_descport(req_data);	
		break;
		default:
					usbprint("usb get descriptor : 0x%04x not suppost!\r\n",type);
					usb_device_read_data_status_ep0(1);
					break;
	}
}
