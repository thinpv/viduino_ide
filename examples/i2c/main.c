#include <i2c.h>
#include <sys-print.h>

void setup()
{
	sys_print_init();
	printf("setup\r\n");

	gpio_set_cfg(GPIOD, 0, GPIO_FUNC_011);
	gpio_set_pull(GPIOD, 0, GPIO_PULL_UP);
	gpio_set_cfg(GPIOD, 12, GPIO_FUNC_011);
	gpio_set_pull(GPIOD, 12, GPIO_PULL_UP);
	// gpio_set_cfg(GPIOE, 11, GPIO_FUNC_011);
	// gpio_set_pull(GPIOE, 11, GPIO_PULL_UP);
	// gpio_set_cfg(GPIOE, 12, GPIO_FUNC_011);
	// gpio_set_pull(GPIOE, 12, GPIO_PULL_UP);
	i2c_init(I2C0);
	i2c_set_frequency(I2C0, 400000);
	uint8_t data;
	printf("i2c start scan\r\n");
	for (int i = 0; i < 127; i++)
	{
		if (i2c_read(I2C0, i, &data, 1, 1) == 1)
		{
			printf("found i2c addr 0x%02X\r\n", i);
		}
	}
	gpio_set_dir(GPIOA, 0, GPIO_DIRECTION_OUTPUT);
	gpio_set_value(GPIOA, 0, ~gpio_get_value(GPIOA, 0));
}

void loop()
{
}