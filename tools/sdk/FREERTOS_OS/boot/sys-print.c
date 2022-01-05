#include <types.h>
#include <stdint.h>
#include <io.h>
#include <sys-print.h>
#include <gpio.h>
#include <uart.h>

// #define DEBUG_ENABLE

void sys_print_init(void)
{
#ifdef DEBUG_ENABLE
	// gpio_f1c100s_set_cfg(GPIOE, 0, 5);
	// gpio_f1c100s_set_cfg(GPIOE, 1, 5);
	// uart_f1c100s_set(UART0, 115200, 8, 0, 1);

	gpio_set_cfg(GPIOA, 2, 5);
	gpio_set_cfg(GPIOA, 3, 5);
	uart_set(UART1, 115200, 8, 0, 1);
#endif
}

void sys_print_c(char c)
{
#ifdef DEBUG_ENABLE
	uart_write_c(UART1, c, 100);
#endif
}
