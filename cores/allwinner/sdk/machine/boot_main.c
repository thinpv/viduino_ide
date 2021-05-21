#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <dma.h>
#include <gpio.h>
#include <uart.h>
#include <timer.h>
#include <irq.h>
#include <irq_gpio.h>
#include <fb.h>
#include <pwm.h>
#include <usb.h>

int maincpp(void);

#include "FreeRTOS.h"
#include "task.h"

static void arduino(void *pvParameters)
{
    maincpp();
}

int boot_main(int argc, char **argv)
{
    // sys_clock_init();
    // sys_dram_init();
    // sys_print_init();

    do_init_mem_pool();
    do_init_dma_pool();

    // sys_mmu_init();

    sys_init();

    irq_init();
    xTaskCreate(arduino, "arduino", 1024, NULL, tskIDLE_PRIORITY + 2, NULL);
    vTaskStartScheduler();
    for (;;)
    {
        printf("loop\r\n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    return 0;
}

void __fatal_error(const char *msg)
{
    while (1)
        ;
}

#ifndef NDEBUG
void __assert_func(const char *file, int line, const char *func, const char *expr)
{
    //printf("Assertion '%s' failed, at file %s:%d\n", expr, file, line);
    __fatal_error("Assertion failed");
}
#endif
