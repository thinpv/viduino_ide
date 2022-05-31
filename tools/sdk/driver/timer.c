#include <timer.h>
#include <io.h>

#define DEBUG( ... ) // printf(##__VA_ARGS__)

void timer_irq_enbale(Timer_Type *timer)
{
	switch ((uint32_t)timer)
	{
	case (uint32_t)TIMER0:
		S_Bit(TIMER->TMR_IRQ_EN_REG, 0);
		break;
	case (uint32_t)TIMER1:
		S_Bit(TIMER->TMR_IRQ_EN_REG, 1);
		break;
	case (uint32_t)TIMER2:
		S_Bit(TIMER->TMR_IRQ_EN_REG, 2);
		break;
	default:
		break;
	}
}

void timer_irq_clear(Timer_Type *timer)
{
	switch ((uint32_t)timer)
	{
	case (uint32_t)TIMER0:
		S_Bit(TIMER->TMR_IRQ_STA_REG, 0);
		break;
	case (uint32_t)TIMER1:
		S_Bit(TIMER->TMR_IRQ_STA_REG, 1);
		break;
	case (uint32_t)TIMER2:
		S_Bit(TIMER->TMR_IRQ_STA_REG, 2);
		break;
	default:
		break;
	}
}

void timer_set_mode(Timer_Type *timer, uint8_t mode)
{
	if (mode == TIMER_CONTINUOUS_MODE)
		C_Bit(timer->TIME_CTRL_REG, 7);
	else if (mode == TIMER_SINGLE_MODE)
		S_Bit(timer->TIME_CTRL_REG, 7);
}

void timer_set_prescale(Timer_Type *timer, uint8_t mode)
{
	C_Value(timer->TIME_CTRL_REG, 4, 0x7);
	S_Value(timer->TIME_CTRL_REG, 4, mode);
}

void timer_reload(Timer_Type *timer)
{
	S_Bit(timer->TIME_CTRL_REG, 1);
}

void timer_enable(Timer_Type *timer)
{
	S_Bit(timer->TIME_CTRL_REG, 0);
}

void timer_disable(Timer_Type *timer)
{
	C_Bit(timer->TIME_CTRL_REG, 0);
}

void timer_set_interval(Timer_Type *timer, uint32_t interval)
{
	timer->TIME_INTV_VALUE_REG = interval;
}

uint32_t timer_get_current_value(Timer_Type *timer)
{
	return timer->TIME_CUR_VALUE_REG;
}
