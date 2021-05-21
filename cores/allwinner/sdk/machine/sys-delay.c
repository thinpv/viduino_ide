#include "sys-delay.h"

/*
408M ��ʱ1ms
0���Ż�
*/
void delay_ms(int ms)
{
 volatile unsigned int cnt,i,s;
// s=ms*66; //����cache
 s=ms*1851; //��cache	
	for(cnt=0;cnt<s;cnt++)
	{
		for(i=0;i<20;i++);
	}
}
/*
408M ��ʱ1us
0���Ż�
*/
void delay_us(int us)
{
 volatile unsigned int cnt,i,s;
  s=(unsigned int)((float)us*13.6351); //��cache	
//	s=us;
	for(cnt=0;cnt<s;cnt++)
	{
		for(i=0;i<1;i++);
	}

}
