#ifndef _PORTNAMES_H
#define _PORTNAMES_H

#ifdef __cplusplus
extern "C" {
#endif

// extern GPIO_TypeDef *GPIOPort[];

typedef enum {
  FirstPort = 0x00,
  PortA = FirstPort,
  PortB,
  PortC,
  PortD,
  PortE,
  PortF,
  PortEND,
  LastPort = PortEND - 1
} PortName;

#define MAX_NB_PORT (LastPort-FirstPort+1)

// /* Return GPIO base address */
// #define get_GPIO_Port(p) ((p < MAX_NB_PORT) ? GPIOPort[p] : (GPIO_TypeDef *)NULL)
// /* Enable GPIO clock and return GPIO base address */
// GPIO_TypeDef *set_GPIO_Port_Clock(uint32_t port_idx);

#ifdef __cplusplus
}
#endif
#endif
