#ifndef __SYS_DELAY_H__
#define __SYS_DELAY_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

void sys_delay_us(int us);
void sys_delay_ms(int ms);

uint64_t timer_get_ticker();
unsigned long millis(void);
// unsigned long micros(void);
// void delay(unsigned long);
// void delayMicroseconds(unsigned int usec);

#ifdef __cplusplus
}
#endif

#endif
