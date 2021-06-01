#ifndef __USB_DESC_H__
#define __USB_DESC_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "usb.h"

//#define DATA_PACKET_SIZE (u16)(1024)
//#define DATA_PACKET_SIZE (u16)(64)
#define DATA_PACKET_SIZE (u16)(512)

/*
 * USB types, the second of three bRequestType fields
 */
#define USB_TYPE_MASK (0x03 << 5)
#define USB_TYPE_STANDARD (0x00 << 5)
#define USB_TYPE_CLASS (0x01 << 5)
#define USB_TYPE_VENDOR (0x02 << 5)
#define USB_TYPE_RESERVED (0x03 << 5)

	typedef void (*usb_descriptor_request_func)(enum USB_DESCRIPTOR_TYPE type, USB_DeviceRequest *req_data);

	void set_usb_descriptor_request_func(usb_descriptor_request_func callback);

	extern USB_DeviceRequest usb_setup_request;

	void standard_setup_request(USB_DeviceRequest *req_data);
	void class_setup_request(USB_DeviceRequest *req_data);
	int usb_device_write_data(int ep, unsigned char *databuf, int len);
	void usb_cdc_get_cfg_descriptor(USB_DeviceRequest *req_data);
	void usb_hid_get_cfg_descriptor(USB_DeviceRequest *req_data);
	void usb_get_hid_descport(USB_DeviceRequest *req_data);
	void usb_setup_handle(unsigned char *dat, int len);
	int usb_hid_in_ep_callback(void);
	void usb_hid_out_ep_callback(unsigned char *pdat, int len);
	void usb_cdc_out_ep_callback(unsigned char *pdat, int len);

	void usb_uvc_get_cfg_descriptor(USB_DeviceRequest *req_data);
	int usb_uvc_in_ep_callback(void);
	void usb_uvc_out_ep_callback(unsigned char *pdat, int len);
	void usb_get_uvc_descport(USB_DeviceRequest *req_data);

	void usb_mass_get_cfg_descriptor(USB_DeviceRequest *req_data);
	void usb_mass_out_ep_callback(unsigned char *pdat, int len);
	int usb_mass_in_ep_callback(void);
	u8 *Get_Max_Lun(void);

#ifdef __cplusplus
}
#endif

#endif
