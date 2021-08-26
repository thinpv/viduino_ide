#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <io.h>
#include <reg-ccu.h>
#include <timer.h>
#include <irq.h>

#ifdef USE_FREERTOS
#include "FreeRTOS.h"
#include "task.h"
#endif

#define TICK_PER_SECOND 1000
#define TICK_PER_MILLISECOND (TICK_PER_SECOND / 1000)
#define MICROSECOND_PER_TICK (1000000 / TICK_PER_SECOND)

static volatile uint64_t ticker = 0;

void timer0_interrupt_handle(int arg)
{
	S_Bit(TIMER->TMR_IRQ_STA_REG, 0);
	++ticker;
}

uint64_t timer_get_ticker()
{
#ifdef USE_FREERTOS
	return xTaskGetTickCount();
#else
	return ticker;
#endif
}

unsigned long millis(void)
{
#ifdef USE_FREERTOS
	return xTaskGetTickCount();
#else
	return ticker / TICK_PER_MILLISECOND;
#endif
}

unsigned long micros(void)
{
#ifdef USE_FREERTOS
	return xTaskGetTickCount() * MICROSECOND_PER_TICK + (0xB71B00 - TIMER->TMR0_CUR_VALUE_REG) / 12;
#else
	return ticker;
#endif
}

void delay(unsigned long ms)
{
#ifdef USE_FREERTOS
	vTaskDelay(ms / portTICK_PERIOD_MS);
#else
	uint64_t time_to_delay = ticker + ms * TICK_PER_MILLISECOND;
	while (ticker < time_to_delay)
	{
	}
#endif
}

void delayMicroseconds(unsigned int usec)
{
	// uint64_t time_to_delay = micros() + usec;
	// while (micros() < time_to_delay)
	// 	;
	volatile unsigned int cnt, i, s;
	s = (unsigned int)((float)usec * 13.6351); //��cache
											   //	s=us;
	for (cnt = 0; cnt < s; cnt++)
	{
		for (i = 0; i < 1; i++)
			;
	}
}

void timer0_set()
{
	TIMER->TMR0_INTV_VALUE_REG = 0xB71B00 / TICK_PER_SECOND;
	TIMER->TMR0_CTRL_REG &= 0xffffff00;
	TIMER->TMR0_CTRL_REG |= (1 << 4) | (1 << 2);
	TIMER->TMR0_CTRL_REG |= 1 << 0;
	TIMER->TMR_IRQ_EN_REG |= 1 << 0;
	irq_register(IRQ_LEVEL_1, F1C100S_IRQ_TIMER0, timer0_interrupt_handle, 3);
}
