#include <sys_print.h>
#include <gpio.h>
#include <sys_delay.h>

void setup()
{
	sys_print_init();
  printf("setup\r\n");
	gpio_set_dir_pn(GPIOA0, GPIO_DIRECTION_OUTPUT);
}

void loop()
{
  printf("loop\r\n");
  gpio_set_value_pn(GPIOA0, 1);
  delay(100);
  gpio_set_value_pn(GPIOA0, 0);
  delay(100);
}