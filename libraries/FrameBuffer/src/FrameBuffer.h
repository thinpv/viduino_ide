#ifndef __FRAME_BUFFER_H_
#define __FRAME_BUFFER_H_

// #include "fb.h"
#include <sys_lcd.h>

typedef unsigned short (*lcd_buff_pt)[YSIZE_PHYS][XSIZE_PHYS];

#ifdef __cplusplus
class FrameBuffer
{
public:
	FrameBuffer();
	// ~FrameBuffer();
	int begin(int width = 800, int height = 480, int bright = 100);
	void setBright(int bright);
	void areaPresent(int x1, int x2, int y1, int y2, unsigned short *color_p);
	void areaPresentX(int x1, int x2, int y1, int y2, unsigned short *color_p);
	void areaPresentY(int x1, int x2, int y1, int y2, unsigned short *color_p);
	void areaPresentXY(int x1, int x2, int y1, int y2, unsigned short *color_p);
	// void pixelPresent(int x, int y, uint32_t data);
	unsigned short *getBuffer();

private:
	int width;
	int height;
	lcd_buff_pt bt;
	__attribute__((aligned(1024))) unsigned short LCDbuff[XSIZE_PHYS * YSIZE_PHYS];
};
#endif

#endif
