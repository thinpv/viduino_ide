#include "sys-delay.h"
#include <F1C100S.h>
#include <timer.h>
// #include "soft-pwm.h"

#define TICK_PER_SECOND 1000
#define TICK_PER_MILLISECOND (TICK_PER_SECOND / 1000)
#define MICROSECOND_PER_TICK (1000000 / TICK_PER_SECOND)

static volatile uint64_t ticker = 0;

/*
408M ��ʱ1ms
0���Ż�
*/
void sys_delay_ms(int ms)
{
	volatile unsigned int cnt, i, s;
	// s=ms*66; //����cache
	s = ms * 1851; //��cache
	for (cnt = 0; cnt < s; cnt++)
	{
		for (i = 0; i < 20; i++)
			;
	}
}
/*
408M ��ʱ1us
0���Ż�
*/
void sys_delay_us(int us)
{
	volatile unsigned int cnt, i, s;
	s = (unsigned int)((float)us * 13.6351); //��cache
																					 //	s=us;
	for (cnt = 0; cnt < s; cnt++)
	{
		for (i = 0; i < 1; i++)
			;
	}
}

void timer0_interrupt_handle(int arg)
{
	timer_irq_clear(TIMER0);
	++ticker;
	// softpwm_update();
}

uint64_t timer_get_ticker()
{
	return ticker;
}

unsigned long millis(void)
{
	return ticker / TICK_PER_MILLISECOND;
}

unsigned long micros(void)
{
	return ticker;
}

void delay(unsigned long ms)
{
	uint64_t time_to_delay = ticker + ms * TICK_PER_MILLISECOND;
	while (ticker < time_to_delay)
	{
	}
}

void delayMicroseconds(unsigned int usec)
{
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
	timer_set_prescale(TIMER0, TIMER_PRESCALE_2);
	timer_set_interval(TIMER0, 12000000 / TICK_PER_SECOND);
	timer_irq_enbale(TIMER0);
	irq_register(IRQ_LEVEL_1, F1C100S_IRQ_TIMER0, timer0_interrupt_handle, 3);
}
