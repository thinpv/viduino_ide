#include "sys-delay.h"
#include <F1C100S.h>
#include <timer.h>

#include "FreeRTOS.h"
#include "task.h"

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
    return xTaskGetTickCount() * 1000 + (0xB71B00 - TIMER->TIME0.TIME_CUR_VALUE_REG) / 12;
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
