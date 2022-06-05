#include "sys-delay.h"
#include <F1C100S.h>
#include <timer.h>
// #include "soft-pwm.h"

#include "FreeRTOS.h"
#include "task.h"

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
                                                                                     //    s=us;
    for (cnt = 0; cnt < s; cnt++)
    {
        for (i = 0; i < 1; i++)
            ;
    }
}

uint64_t timer_get_ticker()
{
    return xTaskGetTickCount();
}

unsigned long millis(void)
{
    return xTaskGetTickCount();
}

unsigned long micros(void)
{
    return xTaskGetTickCount() * MICROSECOND_PER_TICK + (0xB71B00 - TIMER->TIME0.TIME_CUR_VALUE_REG) / 12;
}

void delay(unsigned long ms)
{
    vTaskDelay(ms / portTICK_PERIOD_MS);
}

void delayMicroseconds(unsigned int usec)
{
    volatile unsigned int cnt, i, s;
    s = (unsigned int)((float)usec * 13.6351); //��cache
                                               //    s=us;
    for (cnt = 0; cnt < s; cnt++)
    {
        for (i = 0; i < 1; i++)
            ;
    }
}