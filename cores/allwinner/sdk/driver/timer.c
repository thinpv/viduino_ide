#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <io.h>
#include <reg-ccu.h>
#include <timer.h>
#include <irq.h>

#include "FreeRTOS.h"
#include "task.h"

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
	return ticker;
}

unsigned long millis(void)
{
	return xTaskGetTickCount();
	// return ticker/TICK_PER_MILLISECOND;
}

unsigned long micros(void)
{
	return ticker * MICROSECOND_PER_TICK + (0xB71B00 - TIMER->TMR0_CUR_VALUE_REG) / 12;
}

void delay(unsigned long ms)
{
	vTaskDelay(ms / portTICK_PERIOD_MS);
	// uint64_t time_to_delay = ticker + ms*TICK_PER_MILLISECOND;
	// while(ticker < time_to_delay);
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
