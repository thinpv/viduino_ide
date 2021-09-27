#ifndef __TIMER_H__
#define __TIMER_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <types.h>
#include <F1C100S.h>

#define TIMER_CONTINUOUS_MODE 0
#define TIMER_SINGLE_MODE 1

#define TIMER_PRESCALE_1 0
#define TIMER_PRESCALE_2 1
#define TIMER_PRESCALE_4 2
#define TIMER_PRESCALE_8 3
#define TIMER_PRESCALE_16 4
#define TIMER_PRESCALE_32 5
#define TIMER_PRESCALE_64 6
#define TIMER_PRESCALE_128 7

	void timer_irq_enbale(Timer_Type *timer);
	void timer_irq_clear(Timer_Type *timer);
	void timer_set_mode(Timer_Type *timer, uint8_t mode);
	void timer_set_prescale(Timer_Type *timer, uint8_t mode);
	void timer_reload(Timer_Type *timer);
	void timer_enable(Timer_Type *timer);
	void timer_disable(Timer_Type *timer);
	void timer_set_interval(Timer_Type *timer, uint32_t interval);
	uint32_t timer_get_current_value(Timer_Type *timer);

#ifdef __cplusplus
}
#endif

#endif
