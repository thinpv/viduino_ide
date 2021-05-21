#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <arm32.h>
#include <io.h>
#include <reg-ccu.h>
#include <irq_gpio.h>

void irq_gpio_enable(GPIO_INT_Type * gpio_int, uint16_t offset)
{
	gpio_int->PIO_INT_CTRL |= (0x1 << offset);
}

void irq_gpio_disable(GPIO_INT_Type * gpio_int, uint16_t offset)
{
	gpio_int->PIO_INT_CTRL &= ~(0x1 << offset);
}

void irq_gpio_settype(GPIO_INT_Type * gpio_int, uint16_t offset, enum irq_type_t type)
{
	if(type < IRQ_TYPE_EDGE_RISING || type > IRQ_TYPE_EDGE_BOTH)
		return;
	vuint32_t addr = (vuint32_t)&gpio_int->PIO_INT_CFG0 + (offset >> 3);
	*(vuint32_t*)addr &= ~(0xf << ((offset & 0x7) << 2));
	*(vuint32_t*)addr |= ((type & 0x7) << ((offset & 0x7) << 2));
}

void irq_gpio_dispatch(GPIO_INT_Type * gpio_int, uint16_t offset)
{
	gpio_int->PIO_INT_STA = 0;
}
