#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <arm32.h>
#include <io.h>
#include <reg-ccu.h>
#include <irq_gpio.h>

#define DEBUG( ... ) // printf(##__VA_ARGS__)

static void F1C100S_Interrupt_Shell() {} /* Empty function */

#define GPIOD_MAX 22
#define GPIOE_MAX 13
#define GPIOF_MAX 6

sys_pvFunPtr irqGpioDHandlerTable[] = {
		F1C100S_Interrupt_Shell, /* 0 */
		F1C100S_Interrupt_Shell, /* 1 */
		F1C100S_Interrupt_Shell, /* 2 */
		F1C100S_Interrupt_Shell, /* 3 */
		F1C100S_Interrupt_Shell, /* 4 */
		F1C100S_Interrupt_Shell, /* 5 */
		F1C100S_Interrupt_Shell, /* 6 */
		F1C100S_Interrupt_Shell, /* 7 */
		F1C100S_Interrupt_Shell, /* 8 */
		F1C100S_Interrupt_Shell, /* 9 */
		F1C100S_Interrupt_Shell, /* 10 */
		F1C100S_Interrupt_Shell, /* 11 */
		F1C100S_Interrupt_Shell, /* 12 */
		F1C100S_Interrupt_Shell, /* 13 */
		F1C100S_Interrupt_Shell, /* 14 */
		F1C100S_Interrupt_Shell, /* 15 */
		F1C100S_Interrupt_Shell, /* 16 */
		F1C100S_Interrupt_Shell, /* 17 */
		F1C100S_Interrupt_Shell, /* 18 */
		F1C100S_Interrupt_Shell, /* 19 */
		F1C100S_Interrupt_Shell, /* 20 */
		F1C100S_Interrupt_Shell, /* 21 */
};

sys_pvFunPtr irqGpioEHandlerTable[] = {
		F1C100S_Interrupt_Shell, /* 0 */
		F1C100S_Interrupt_Shell, /* 1 */
		F1C100S_Interrupt_Shell, /* 2 */
		F1C100S_Interrupt_Shell, /* 3 */
		F1C100S_Interrupt_Shell, /* 4 */
		F1C100S_Interrupt_Shell, /* 5 */
		F1C100S_Interrupt_Shell, /* 6 */
		F1C100S_Interrupt_Shell, /* 7 */
		F1C100S_Interrupt_Shell, /* 8 */
		F1C100S_Interrupt_Shell, /* 9 */
		F1C100S_Interrupt_Shell, /* 10 */
		F1C100S_Interrupt_Shell, /* 11 */
		F1C100S_Interrupt_Shell, /* 12 */
};

sys_pvFunPtr irqGpioFHandlerTable[] = {
		F1C100S_Interrupt_Shell, /* 0 */
		F1C100S_Interrupt_Shell, /* 1 */
		F1C100S_Interrupt_Shell, /* 2 */
		F1C100S_Interrupt_Shell, /* 3 */
		F1C100S_Interrupt_Shell, /* 4 */
		F1C100S_Interrupt_Shell, /* 5 */
};

void irq_gpio_gpiod_interrupt_handle()
{
	uint32_t val = irq_gpio_status(GPIOD_INT);
	uint32_t pos = 1;
	uint16_t i;
	for (i = 0; i < GPIOD_MAX; i++)
	{
		if (val & (pos << i))
		{
			(*irqGpioDHandlerTable[i])();
		}
	}
}

void irq_gpio_gpioe_interrupt_handle()
{
	uint32_t val = irq_gpio_status(GPIOE_INT);
	uint32_t pos = 1;
	uint16_t i;
	for (i = 0; i < GPIOE_MAX; i++)
	{
		if (val & (pos << i))
		{
			(*irqGpioEHandlerTable[i])();
		}
	}
}

void irq_gpio_gpiof_interrupt_handle()
{
	uint32_t val = irq_gpio_status(GPIOF_INT);
	uint32_t pos = 1;
	uint16_t i;
	for (i = 0; i < GPIOF_MAX; i++)
	{
		if (val & (pos << i))
		{
			(*irqGpioFHandlerTable[i])();
		}
	}
}

void irq_gpio_enable(GPIO_INT_Type *gpio_int, uint16_t offset)
{
	S_Bit(gpio_int->PIO_INT_CTRL, offset);
}

void irq_gpio_disable(GPIO_INT_Type *gpio_int, uint16_t offset)
{
	C_Bit(gpio_int->PIO_INT_CTRL, offset);
}

void irq_gpio_settype(GPIO_INT_Type *gpio_int, uint16_t offset, enum irq_type_t type, sys_pvFunPtr pvNewISR)
{
	if (offset <= 7)
	{
		gpio_int->PIO_INT_CFG0 &= ~(0xf << (offset << 2));
		gpio_int->PIO_INT_CFG0 |= type << (offset << 2);
	}
	else if (offset <= 15)
	{
		offset -= 8;
		gpio_int->PIO_INT_CFG1 &= ~(0xf << (offset << 2));
		gpio_int->PIO_INT_CFG1 |= type << (offset << 2);
	}
	else if (offset <= 23)
	{
		offset -= 16;
		gpio_int->PIO_INT_CFG2 &= ~(0xf << (offset << 2));
		gpio_int->PIO_INT_CFG2 |= type << (offset << 2);
	}
	else if (offset <= 31)
	{
		offset -= 24;
		gpio_int->PIO_INT_CFG3 &= ~(0xf << (offset << 2));
		gpio_int->PIO_INT_CFG3 |= type << (offset << 2);
	}

	if (gpio_int == GPIOD_INT)
	{
		if (offset < GPIOD_MAX)
		{
			irqGpioDHandlerTable[offset] = pvNewISR;
			irq_register(IRQ_LEVEL_1, F1C100S_IRQ_GPIOD, irq_gpio_gpiod_interrupt_handle, 3);
		}
	}
	else if (gpio_int == GPIOE_INT)
	{
		if (offset < GPIOE_MAX)
		{
			irqGpioEHandlerTable[offset] = pvNewISR;
			irq_register(IRQ_LEVEL_1, F1C100S_IRQ_GPIOE, irq_gpio_gpioe_interrupt_handle, 3);
		}
	}
	else if (gpio_int == GPIOF_INT)
	{
		if (offset < GPIOF_MAX)
		{
			irqGpioFHandlerTable[offset] = pvNewISR;
			irq_register(IRQ_LEVEL_1, F1C100S_IRQ_GPIOF, irq_gpio_gpiof_interrupt_handle, 3);
		}
	}
}

uint32_t irq_gpio_status(GPIO_INT_Type *gpio_int)
{
	uint32_t val = gpio_int->PIO_INT_STA;
	gpio_int->PIO_INT_STA = val;
	return val;
}
