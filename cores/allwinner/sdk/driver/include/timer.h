#ifndef __TIMER_H__
#define __TIMER_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <types.h>
#include <F1C100S.h>

#define delay_ms(x) delay(x)
#define delay_us(x) delayMicroseconds(x)

	uint64_t timer_get_ticker();
	void timer_interrupt_handle();
	void timer0_set();
	unsigned long millis(void);
	unsigned long micros(void);
	void delay(unsigned long);
	void delayMicroseconds(unsigned int usec);

#ifdef __cplusplus
}
#endif

#endif
