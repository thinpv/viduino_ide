#include <irq.h>
#include <sys-print.h>
#include <printf.h>
#include <malloc.h>

#include "FreeRTOS.h"
#include "task.h"

extern void sys_init(void);
extern void setup();
extern void loop();

// void init() {}

static void arduinoThread(void *pvParameters)
{
	( void ) pvParameters;
	// init();
	setup();
	for (;;)
	{
		loop();
	}
}

int boot_main(int argc, char **argv)
{
	sys_print_init();
	// printf("boot_main\r\n");
	do_init_mem_pool();
	sys_init();
	irq_init();

	// printf("xTaskCreate\r\n");
	xTaskCreate(arduinoThread, "arduino", 1024, NULL, tskIDLE_PRIORITY + 2, NULL);
	xTaskCreate(arduinoThread, "arduino", 1024, NULL, tskIDLE_PRIORITY + 2, NULL);
	vTaskStartScheduler();
	return 0;
}

void __fatal_error(const char *msg)
{
	printf(msg);
	while (1)
	{
	}
}

#ifndef NDEBUG
void __assert_func(const char *file, int line, const char *func, const char *expr)
{
	// printf("Assertion '%s' failed, at file %s:%d\n", expr, file, line);
	__fatal_error("Assertion failed\r\n");
}
#endif

void __aeabi_dcmpun()
{
}
void __aeabi_fcmpun() {}