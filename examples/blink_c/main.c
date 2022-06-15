#include <sys-print.h>
#include <gpio.h>
#include <sys-delay.h>

void setup()
{
	sys_print_init();
  printf("setup\r\n");
	gpio_set_dir(GPIOA, 0, GPIO_DIRECTION_OUTPUT);
}

void loop()
{
  printf("loop\r\n");
  gpio_set_value(GPIOA, 0, 1);
  delay(100);
  gpio_set_value(GPIOA, 0, 0);
  delay(100);
}