#include <irq.h>
#include <sys_print.h>
#include <sys_init.h>
#include <printf.h>
#include <malloc.h>

#include "FreeRTOS.h"
#include "task.h"

extern void setup();
extern void loop();

static void arduinoThread(void *pvParameters)
{
	( void ) pvParameters;
	setup();
	for (;;)
	{
		loop();
	}
}

int boot_main(int argc, char **argv)
{
	do_init_mem_pool();
	sys_init();
	irq_init();

	xTaskCreate(arduinoThread, NULL, 1024, NULL, tskIDLE_PRIORITY + 2, NULL);
	vTaskStartScheduler();
	return 0;
}

void __fatal_error(const char *msg)
{
	// printf(msg);
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

void __aeabi_dcmpun() {}
void __aeabi_fcmpun() {}
