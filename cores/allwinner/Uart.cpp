#include "Uart.h"
#include <gpio.h>

Uart Serial(&UART_Desc[0]);
Uart Serial1(&UART_Desc[1]);
Uart Serial2(&UART_Desc[2]);

Uart::Uart(UARTPinDescription* uartDesc) : uartDesc(uartDesc)
{

}

void Uart::begin(unsigned long baudrate) 
{
  gpio_set_cfg(uartDesc->pintype[0].P, uartDesc->pintype[0].num, uartDesc->pintype[0].type);
  gpio_set_cfg(uartDesc->pintype[1].P, uartDesc->pintype[1].num, uartDesc->pintype[1].type);
  uart_set(uartDesc->U, baudrate, 8, 0, 1);
}

void Uart::begin(unsigned long baudrate, uint16_t config)
{
  begin(baudrate);
}

void Uart::end(void) {
}

int Uart::available(void) {
  return uart_available(uartDesc->U);
}

int Uart::peek( void ) {
  return 0;
}

int Uart::read(void) {
  uint8_t c;
  uart_read(uartDesc->U, &c, 1, 100);
  return (int)c;
}

void Uart::flush(void) {
  /* No need to implement because we use hardware UART and it is with hardware FIFO*/
}

size_t Uart::write(const uint8_t ch) {
  // if(!gpio_tx || !gpio_rx || !uart_device)
  //   return 0;
	uart_write_c(uartDesc->U, ch, 100);
  return 1;
}

size_t Uart::write(const uint8_t *buffer, size_t size) {
  // if(!gpio_tx || !gpio_rx || !uart_device)
  //   return 0;
	uart_write(uartDesc->U, buffer, size, 1000);
  return size;
}
