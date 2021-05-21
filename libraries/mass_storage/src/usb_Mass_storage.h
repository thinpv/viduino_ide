#ifndef __USB_MASS_STORAGE_H__
#define __USB_MASS_STORAGE_H__

#include "usb.h"  

#ifdef __cplusplus
extern "C" {
#endif

#define MASS_SIZ_CONFIG_DESC              32


#define MemoryTypeDDR 			0 
#define MemoryTypeSD  			1 
#define MemoryTypeSPIFLASH  2 

#define MAL_OK   0
#define MAL_FAIL 1

#define ENDP1       ((uint8_t)1)
#define ENDP2       ((uint8_t)2)
#define EP_RX_VALID 2
#define EP_TX_VALID 2
#define EP1_IN 2
#define EP2_OUT 2
#define EP_RX_STALL 2
#define EP_TX_STALL 2
extern uint32_t MemoryType;
uint32_t USB_SIL_Write(uint8_t bEpAddr, uint8_t* pBufferPointer, uint32_t wBufferSize);
uint32_t USB_SIL_Read(uint8_t bEpAddr, uint8_t* pBufferPointer);
void SetEPTxStatus(uint8_t bEpNum, uint16_t wState);
void SetEPRxStatus(uint8_t bEpNum, uint16_t wState);
uint16_t GetEPRxStatus(uint8_t bEpNum);
void SetEPTxCount(uint8_t bEpNum, uint16_t wCount);

void usb_mass_descriptor_request(enum USB_DESCRIPTOR_TYPE type, USB_DeviceRequest* req_data);
int usb_mass_in_ep_callback(void);
void usb_mass_out_ep_callback(unsigned char *pdat,int len);

#ifdef __cplusplus
}
#endif

#endif
