#include "Arduino.h"

extern "C" int maincpp( void );

void init(){}

int maincpp( void )
{
	init();		
	
	#if defined(__M451__) | defined(__NUC240__) |defined(__NANO100__)
	USBDevice.attach();
	#endif
			
	setup();
	
	for (;;)
	{
		loop();		
		// if(serialEventRun) serialEventRun();
	}

	return 0;
}
