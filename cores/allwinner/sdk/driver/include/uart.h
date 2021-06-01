#ifndef __UART_H__
#define __UART_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <types.h>
#include <F1C100S.h>

	bool_t uart_set(UART_Type *uart, uint32_t baud, uint32_t data, uint32_t parity, uint32_t stop);
	bool_t uart_get(UART_Type *uart, uint32_t *baud, uint32_t *data, uint32_t *parity, uint32_t *stop);
	bool_t uart_available(UART_Type *uart);
	ssize_t uart_read(UART_Type *uart, uint8_t *buf, size_t count, uint32_t delay);
	ssize_t uart_write(UART_Type *uart, const uint8_t *buf, size_t count, uint32_t delay);
	ssize_t uart_write_c(UART_Type *uart, const uint8_t c, uint32_t delay);

#ifdef __cplusplus
}
#endif

#endif
