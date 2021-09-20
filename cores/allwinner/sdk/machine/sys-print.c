/*
 * sys-uart.c
 *
 * Copyright(c) 2007-2018 Jianjun Jiang <8192542@qq.com>
 * Official site: http://xboot.org
 * Mobile phone: +86-18665388956
 * QQ: 8192542
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <types.h>
#include <stdint.h>
#include <io.h>
#include <sys-print.h>
#include <gpio.h>
#include <uart.h>

// #define UART_PRINT_PORT UART1
// #define UART_PRINT_BAUD 115200

void sys_print_init(void)
{
	// gpio_f1c100s_set_cfg(GPIOE, 0, 5);
	// gpio_f1c100s_set_cfg(GPIOE, 1, 5);
	// uart_f1c100s_set(UART0, 115200, 8, 0, 1);

	gpio_set_cfg(GPIOA, 2, 5);
	gpio_set_cfg(GPIOA, 3, 5);
	uart_set(UART1, 115200, 8, 0, 1);
}

void sys_print_c(char c)
{
	uart_write_c(UART1, c, 100);
}
