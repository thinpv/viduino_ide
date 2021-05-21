#ifndef USB_KEYBOARD_H_
#define USB_KEYBOARD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "usb.h"

void usb_hid_descriptor_request(enum USB_DESCRIPTOR_TYPE type, USB_DeviceRequest* req_data);

#ifdef __cplusplus
}
#endif

#endif