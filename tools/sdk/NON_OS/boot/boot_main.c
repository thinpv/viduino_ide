#include <malloc.h>
#include <sys-init.h>
#include <sys-delay.h>
#include <irq.h>

void init(){}
extern void setup();
extern void loop();

int boot_main(int argc, char **argv)
{
	// sys_clock_init();
	// sys_dram_init();
	// sys_print_init();

	do_init_mem_pool();
	// do_init_dma_pool();

	// sys_mmu_init();

	sys_init();

	irq_init();
	timer0_set();

	init();		
	
	// USBDevice.attach();
			
	setup();
	
	for (;;)
	{
		loop();		
		// if(serialEventRun) serialEventRun();
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