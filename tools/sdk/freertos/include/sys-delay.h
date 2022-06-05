#ifndef __SYS_DELAY_H__
#define __SYS_DELAY_H__

#include <stdint.h>

void sys_delay_us(int us);
void sys_delay_ms(int ms);

uint64_t timer_get_ticker();
void timer0_set();
unsigned long millis(void);
unsigned long micros(void);
void delay(unsigned long);
void delayMicroseconds(unsigned int usec);

#endif
