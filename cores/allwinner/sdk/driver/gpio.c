#include <gpio.h>
#include <io.h>

void gpio_set_cfg(GPIO_Type *gpio, uint16_t pin, uint16_t cfg)
{
	if (pin >= 32)
		return;
	uint32_t addr = (uint32_t)(&gpio->CFG0 + (pin >> 3));
	*(uint32_t *)addr &= ~(0x7 << ((pin & 0x7) << 2));
	*(uint32_t *)addr |= ((cfg & 0x7) << ((pin & 0x7) << 2));
}

uint16_t gpio_get_cfg(GPIO_Type *gpio, uint16_t pin)
{
	if (pin >= 32)
		return 0xFF;
	return (gpio->CFG0 >> (pin << 2)) & 0x7;
}

void gpio_set_pull(GPIO_Type *gpio, uint16_t pin, gpio_pull_t pull)
{
	if (pin >= 32)
		return;
	uint32_t addr = (uint32_t)(&gpio->PUL0 + (pin >> 4));
	*(uint32_t *)addr &= ~(0x3 << ((pin & 0xf) << 1));
	*(uint32_t *)addr |= ((pull & 0x3) << ((pin & 0xf) << 1));
}

gpio_pull_t gpio_get_pull(GPIO_Type *gpio, uint16_t pin)
{
	if (pin >= 32)
		return GPIO_PULL_NONE;
	return (gpio->PUL0 >> (pin << 1)) & 0x3;
}

void gpio_set_drv(GPIO_Type *gpio, uint16_t pin, gpio_drv_t drv)
{
	if (pin >= 32)
		return;
	uint32_t addr = (uint32_t)(&gpio->DRV0 + (pin >> 4));
	*(uint32_t *)addr &= ~(0x3 << ((pin & 0xf) << 1));
	*(uint32_t *)addr |= ((drv & 0x3) << ((pin & 0xf) << 1));
}

gpio_drv_t gpio_get_drv(GPIO_Type *gpio, uint16_t pin)
{
	if (pin >= 32)
		return GPIO_DRV_NONE;
	return (gpio->DRV0 >> (pin << 1)) & 0x3;
}

void gpio_set_rate(GPIO_Type *gpio, uint16_t pin, gpio_rate_t rate)
{
}

gpio_rate_t gpio_get_rate(GPIO_Type *gpio, uint16_t pin)
{
	return GPIO_RATE_SLOW;
}

void gpio_set_dir(GPIO_Type *gpio, uint16_t pin, gpio_direction_t dir)
{
	if (pin >= 32)
		return;

	switch (dir)
	{
	case GPIO_DIRECTION_INPUT:
		gpio_set_cfg(gpio, pin, 0);
		break;

	case GPIO_DIRECTION_OUTPUT:
		gpio_set_cfg(gpio, pin, 1);
		break;

	default:
		break;
	}
}

gpio_direction_t gpio_get_dir(GPIO_Type *gpio, uint16_t pin)
{
	if (pin >= 32)
		return GPIO_DIRECTION_INPUT;

	switch (gpio_get_cfg(gpio, pin))
	{
	case 0:
		return GPIO_DIRECTION_INPUT;
	case 1:
		return GPIO_DIRECTION_OUTPUT;
	default:
		break;
	}
	return GPIO_DIRECTION_INPUT;
}

void gpio_set_value(GPIO_Type *gpio, uint16_t pin, uint8_t value)
{
	if (pin >= 32)
		return;
	gpio->DATA &= ~(1 << pin);
	gpio->DATA |= (value & 1) << pin;
}

uint8_t gpio_get_value(GPIO_Type *gpio, uint16_t pin)
{
	if (pin >= 32)
		return 0xFF;
	return (gpio->DATA >> pin) & 0x1;
}

uint16_t gpio_to_irq(GPIO_Type *gpio, uint16_t pin)
{
	if (pin >= 32)
		return 0xFF;
	// return port->oirq + pin;
	return 0xFF;
}
