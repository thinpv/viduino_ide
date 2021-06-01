#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <io.h>
#include <reg-ccu.h>
#include <uart.h>
#include <timer.h>
#include <ccu.h>

#define FALSE 0
#define TRUE 1

bool_t uart_set(UART_Type *uart, uint32_t baud, uint32_t data, uint32_t parity, uint32_t stop)
{
	uint8_t dreg, preg, sreg;
	uint16_t breg;

	switch (baud)
	{
	case 1200:
		breg = 5208;
		break;
	case 2400:
		breg = 2604;
		break;
	case 4800:
		breg = 1302;
		break;
	case 9600:
		breg = 651;
		break;
	case 19200:
		breg = 326;
		break;
	case 38400:
		breg = 163;
		break;
	case 57600:
		breg = 108;
		break;
	case 115200:
		breg = 54;
		break;
	case 230400:
		breg = 27;
		break;
	case 256000:
		breg = 24;
		break;
	case 460800:
	case 500000:
		breg = 13;
		break;
	case 512000:
		breg = 12;
		break;
	case 921600:
		breg = 7;
		break;
	case 1000000:
		breg = 6;
		break;
	default:
		return FALSE;
	}

	switch (data)
	{
	case 5: /* Data bits = 5 */
		dreg = 0x0;
		break;
	case 6: /* Data bits = 6 */
		dreg = 0x1;
		break;
	case 7: /* Data bits = 7 */
		dreg = 0x2;
		break;
	case 8: /* Data bits = 8 */
		dreg = 0x3;
		break;
	default:
		return FALSE;
	}

	switch (parity)
	{
	case 0: /* Parity none */
		preg = 0x0;
		break;
	case 1: /* Parity odd */
		preg = 0x1;
		break;
	case 2: /* Parity even */
		preg = 0x3;
		break;
	default:
		return FALSE;
	}

	switch (stop)
	{
	case 1: /* Stop bits = 1 */
		sreg = 0;
		break;
	case 2: /* Stop bits = 2 */
		sreg = 1;
		break;
	case 0: /* Stop bits = 1.5 */
	default:
		return FALSE;
	}

	if (uart == UART0)
		ccu_reset(RESET_UART0, true);
	else if (uart == UART1)
		ccu_reset(RESET_UART1, true);
	else if (uart == UART2)
		ccu_reset(RESET_UART2, true);

	uart->UART_DLH_IER_REG = 0;
	uart->UART_IIR_FCR_REG = 0xF7;
	uart->UART_MCR_REG = 0;

	uart->UART_LCR_REG |= (1 << 7);
	uart->UART_RBR_THR_DLL_REG = breg & 0xFF;
	uart->UART_DLH_IER_REG = (breg >> 8) & 0xFF;
	uart->UART_LCR_REG &= ~(1 << 7);

	uart->UART_LCR_REG &= ~(0x3F);
	uart->UART_LCR_REG |= ((dreg << 0) | (sreg << 2) | (preg << 3));

	return TRUE;
}

bool_t uart_get(UART_Type *uart, uint32_t *baud, uint32_t *data, uint32_t *parity, uint32_t *stop)
{
	// if(baud)
	// 	*baud = uart->baud;
	// if(data)
	// 	*data = uart->data;
	// if(parity)
	// 	*parity = uart->parity;
	// if(stop)
	// 	*stop = uart->stop;
	return TRUE;
}

bool_t uart_available(UART_Type *uart)
{
	if (uart->UART_USR_REG & (1 << 3))
		return TRUE;
	return FALSE;
}

ssize_t uart_read(UART_Type *uart, uint8_t *buf, size_t count, uint32_t delay)
{
	ssize_t i;
	uint32_t delay_to = millis() + delay;
	for (i = 0; i < count; i++)
	{
		while ((uart->UART_USR_REG & (1 << 3)) == 0)
		{
			if (millis() >= delay_to)
				return i;
		}
		buf[i] = uart->UART_RBR_THR_DLL_REG & 0xFF;
	}
	return i;
}

ssize_t uart_write(UART_Type *uart, const uint8_t *buf, size_t count, uint32_t delay)
{
	ssize_t i;
	uint32_t delay_to = millis() + delay;
	for (i = 0; i < count; i++)
	{
		while (!(uart->UART_USR_REG & (1 << 1)))
		{
			if (millis() >= delay_to)
				return i;
		}
		uart->UART_RBR_THR_DLL_REG = buf[i];
	}
	return i;
}

ssize_t uart_write_c(UART_Type *uart, const uint8_t c, uint32_t delay)
{
	uint32_t u32delayno = delay * 10000;
	while (!(uart->UART_USR_REG & (1 << 1)))
	{
		--u32delayno;
		if (u32delayno == 0)
			return 0;
	}
	uart->UART_RBR_THR_DLL_REG = c;
	return 1;
}