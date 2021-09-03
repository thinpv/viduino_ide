#ifndef __FRAME_BUFFER_H_
#define __FRAME_BUFFER_H_

// #include "fb.h"
#include <sys_lcd.h>

#ifdef __cplusplus
class FrameBuffer
{
public:
	FrameBuffer();
	// ~FrameBuffer();
	int begin(int width = 480, int height = 272, int bright = 100);
	int begin(pixel_format *data, int width = 480, int height = 272, int bright = 100);
	void setBright(int bright);
	void areaPresent(int x, int y, int w, int h, pixel_format *data);
	void areaPresentX(int x, int y, int w, int h, pixel_format *data);
	void areaPresentY(int x, int y, int w, int h, pixel_format *data);
	void areaPresentXY(int x, int y, int w, int h, pixel_format *data);
	void pixelPresent(int x, int y, pixel_format data);
	pixel_format * getBuffer();
};
#endif

#endif
