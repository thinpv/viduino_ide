#ifndef _PINNAMES_H
#define _PINNAMES_H

// #include "PinNamesTypes.h"
#include "PortNames.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  // Not connected
  NC = 0xFFFFFFFF,

  // Pin name definition
  PA_0  = (PortA << 5) + 0x00,
  PA_1  = (PortA << 5) + 0x01,
  PA_2  = (PortA << 5) + 0x02,
  PA_3  = (PortA << 5) + 0x03,

  PC_0  = (PortC << 5) + 0x00,
  PC_1  = (PortC << 5) + 0x01,
  PC_2  = (PortC << 5) + 0x02,
  PC_3  = (PortC << 5) + 0x03,

  PD_0  = (PortD << 5) + 0x00,
  PD_1  = (PortD << 5) + 0x01,
  PD_2  = (PortD << 5) + 0x02,
  PD_3  = (PortD << 5) + 0x03,
  PD_4  = (PortD << 5) + 0x04,
  PD_5  = (PortD << 5) + 0x05,
  PD_6  = (PortD << 5) + 0x06,
  PD_7  = (PortD << 5) + 0x07,
  PD_8  = (PortD << 5) + 0x08,
  PD_9  = (PortD << 5) + 0x09,
  PD_10 = (PortD << 5) + 0x0A,
  PD_11 = (PortD << 5) + 0x0B,
  PD_12 = (PortD << 5) + 0x0C,
  PD_13 = (PortD << 5) + 0x0D,
  PD_14 = (PortD << 5) + 0x0E,
  PD_15 = (PortD << 5) + 0x0F,
  PD_16 = (PortD << 5) + 0x10,
  PD_17 = (PortD << 5) + 0x11,
  PD_18 = (PortD << 5) + 0x12,
  PD_19 = (PortD << 5) + 0x13,
  PD_20 = (PortD << 5) + 0x14,
  PD_21 = (PortD << 5) + 0x15,
  
  PE_0  = (PortE << 5) + 0x00,
  PE_1  = (PortE << 5) + 0x01,
  PE_2  = (PortE << 5) + 0x02,
  PE_3  = (PortE << 5) + 0x03,
  PE_4  = (PortE << 5) + 0x04,
  PE_5  = (PortE << 5) + 0x05,
  PE_6  = (PortE << 5) + 0x06,
  PE_7  = (PortE << 5) + 0x07,
  PE_8  = (PortE << 5) + 0x08,
  PE_9  = (PortE << 5) + 0x09,
  PE_10 = (PortE << 5) + 0x0A,
  PE_11 = (PortE << 5) + 0x0B,
  PE_12 = (PortE << 5) + 0x0C,
  
  PF_0  = (PortF << 5) + 0x00,
  PF_1  = (PortF << 5) + 0x01,
  PF_2  = (PortF << 5) + 0x02,
  PF_3  = (PortF << 5) + 0x03,
  PF_4  = (PortF << 5) + 0x04,
  PF_5  = (PortF << 5) + 0x05,

  // Specific pin name
  LRADC,
  
  // Specific pin name define in the variant
#if __has_include("PinNamesVar.h")
#include "PinNamesVar.h"
#endif
  P_END = NC
} PinName;

#ifdef __cplusplus
}
#endif

#endif
