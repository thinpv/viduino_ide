#include <gpio.h>
#include <io.h>

#define DEBUG( ... ) // printf(##__VA_ARGS__)

void gpio_set_cfg(GPIO_Type *gpio, int pin, uint16_t cfg)
{
	if (pin >= 32)
		return;
	uint32_t addr = (uint32_t)(&gpio->CFG0 + (pin >> 3));
	*(uint32_t *)addr &= ~(0x7 << ((pin & 0x7) << 2));
	*(uint32_t *)addr |= ((cfg & 0x7) << ((pin & 0x7) << 2));
}

uint16_t gpio_get_cfg(GPIO_Type *gpio, int pin)
{
	if (pin >= 32)
		return 0xFF;
	return (gpio->CFG0 >> (pin << 2)) & 0x7;
}

void gpio_set_pull(GPIO_Type *gpio, int pin, gpio_pull_t pull)
{
	if (pin >= 32)
		return;
	uint32_t addr = (uint32_t)(&gpio->PUL0 + (pin >> 4));
	*(uint32_t *)addr &= ~(0x3 << ((pin & 0xf) << 1));
	*(uint32_t *)addr |= ((pull & 0x3) << ((pin & 0xf) << 1));
}

gpio_pull_t gpio_get_pull(GPIO_Type *gpio, int pin)
{
	if (pin >= 32)
		return GPIO_PULL_NONE;
	return (gpio->PUL0 >> (pin << 1)) & 0x3;
}

void gpio_set_drv(GPIO_Type *gpio, int pin, gpio_drv_t drv)
{
	if (pin >= 32)
		return;
	uint32_t addr = (uint32_t)(&gpio->DRV0 + (pin >> 4));
	*(uint32_t *)addr &= ~(0x3 << ((pin & 0xf) << 1));
	*(uint32_t *)addr |= ((drv & 0x3) << ((pin & 0xf) << 1));
}

gpio_drv_t gpio_get_drv(GPIO_Type *gpio, int pin)
{
	if (pin >= 32)
		return GPIO_DRV_NONE;
	return (gpio->DRV0 >> (pin << 1)) & 0x3;
}

void gpio_set_rate(GPIO_Type *gpio, int pin, gpio_rate_t rate)
{
}

gpio_rate_t gpio_get_rate(GPIO_Type *gpio, int pin)
{
	return GPIO_RATE_SLOW;
}

void gpio_set_dir(GPIO_Type *gpio, int pin, gpio_direction_t dir)
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

gpio_direction_t gpio_get_dir(GPIO_Type *gpio, int pin)
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

void gpio_set_value(GPIO_Type *gpio, int pin, uint8_t value)
{
	if (pin >= 32)
		return;
	gpio->DATA &= ~(1 << pin);
	gpio->DATA |= (value & 1) << pin;
}

uint8_t gpio_get_value(GPIO_Type *gpio, int pin)
{
	if (pin >= 32)
		return 0xFF;
	return (gpio->DATA >> pin) & 0x1;
}

uint16_t gpio_to_irq(GPIO_Type *gpio, int pin)
{
	if (pin >= 32)
		return 0xFF;
	// return port->oirq + pin;
	return 0xFF;
}

uint8_t pin_to_port(int pin_num, GPIO_Type **gpio, uint16_t *pin)
{
	switch (pin_num >> 5)
	{
	case 0:
		*gpio = GPIOA;
		*pin = pin_num & 0x1F;
		return 0;
	
	case 1:
		*gpio = GPIOB;
		*pin = pin_num & 0x1F;
		return 0;
	
	case 2:
		*gpio = GPIOC;
		*pin = pin_num & 0x1F;
		return 0;
	
	case 3:
		*gpio = GPIOD;
		*pin = pin_num & 0x1F;
		return 0;
	
	case 4:
		*gpio = GPIOE;
		*pin = pin_num & 0x1F;
		return 0;
	
	case 5:
		*gpio = GPIOF;
		*pin = pin_num & 0x1F;
		return 0;

	default:
		return 1;
	}
	return 1;
}

void gpio_set_cfg_pn(int pin_num, uint16_t cfg)
{
	GPIO_Type *gpio;
	int pin;
	if(pin_to_port(pin_num, &gpio, &pin) == 0) {
		return gpio_set_cfg(gpio, pin, cfg);
	}
}

uint16_t gpio_get_cfg_pn(int pin_num)
{
	GPIO_Type *gpio;
	int pin;
	if(pin_to_port(pin_num, &gpio, &pin) == 0) {
		return gpio_get_cfg(gpio, pin);
	}
	return 0xFF;
}

void gpio_set_pull_pn(int pin_num, gpio_pull_t pull)
{
	GPIO_Type *gpio;
	int pin;
	if(pin_to_port(pin_num, &gpio, &pin) == 0) {
		return gpio_set_pull(gpio, pin, pull);
	}
}

gpio_pull_t gpio_get_pull_pn(int pin_num)
{
	GPIO_Type *gpio;
	int pin;
	if(pin_to_port(pin_num, &gpio, &pin) == 0) {
		return gpio_get_pull(gpio, pin);
	}
	return GPIO_PULL_NONE;
}

void gpio_set_drv_pn(int pin_num, gpio_drv_t drv)
{
	GPIO_Type *gpio;
	int pin;
	if(pin_to_port(pin_num, &gpio, &pin) == 0) {
		return gpio_set_drv(gpio, pin, drv);
	}
}

gpio_drv_t gpio_get_drv_pn(int pin_num)
{
	GPIO_Type *gpio;
	int pin;
	if(pin_to_port(pin_num, &gpio, &pin) == 0) {
		return gpio_get_drv(gpio, pin);
	}
	return GPIO_DRV_NONE;
}

void gpio_set_rate_pn(int pin_num, gpio_rate_t rate)
{
	GPIO_Type *gpio;
	int pin;
	if(pin_to_port(pin_num, &gpio, &pin) == 0) {
		return gpio_set_rate(gpio, pin, rate);
	}
}

gpio_rate_t gpio_get_rate_pn(int pin_num)
{
	GPIO_Type *gpio;
	int pin;
	if(pin_to_port(pin_num, &gpio, &pin) == 0) {
		return gpio_get_rate(gpio, pin);
	}
	return GPIO_RATE_SLOW;
}

void gpio_set_dir_pn(int pin_num, gpio_direction_t dir)
{
	GPIO_Type *gpio;
	int pin;
	if(pin_to_port(pin_num, &gpio, &pin) == 0) {
		return gpio_set_dir(gpio, pin, dir);
	}
}

gpio_direction_t gpio_get_dir_pn(int pin_num)
{
	GPIO_Type *gpio;
	int pin;
	if(pin_to_port(pin_num, &gpio, &pin) == 0) {
		return gpio_get_dir(gpio, pin);
	}
	return GPIO_DIRECTION_INPUT;
}

void gpio_set_value_pn(int pin_num, uint8_t value)
{
	GPIO_Type *gpio;
	int pin;
	if(pin_to_port(pin_num, &gpio, &pin) == 0) {
		return gpio_set_value(gpio, pin, value);
	}
}

uint8_t gpio_get_value_pn(int pin_num)
{
	GPIO_Type *gpio;
	int pin;
	if(pin_to_port(pin_num, &gpio, &pin) == 0) {
		return gpio_get_value(gpio, pin);
	}
	return 0xFF;
}
