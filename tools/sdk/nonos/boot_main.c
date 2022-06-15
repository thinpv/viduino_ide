#include <malloc.h>
#include <sys_init.h>
#include <sys_delay.h>
#include <irq.h>
#include <arm32.h>

void init(){}
extern void setup();
extern void loop();

int boot_main(int argc, char **argv)
{
	do_init_mem_pool();
	sys_init();
	irq_init();
	timer0_set();
	arm32_interrupt_enable();

	init();
	setup();
	for (;;)
	{
		loop();
	}
}

void __fatal_error(const char *msg)
{
	while (1)
	{
	}
}

#ifndef NDEBUG
void __assert_func(const char *file, int line, const char *func, const char *expr)
{
	//printf("Assertion '%s' failed, at file %s:%d\n", expr, file, line);
	__fatal_error("Assertion failed");
}
#endif

void __aeabi_dcmpun()
{
}
void __aeabi_fcmpun() {}
