#ifndef __FRAME_BUFFER_H_
#define __FRAME_BUFFER_H_

#include "fb.h"

#ifdef __cplusplus
class FrameBuffer
{
public:
	FrameBuffer();
	// ~FrameBuffer();
	int begin(int width = 800, int height = 480, int bright = 100);
	void setBright(int bright);
	void areaPresent(int x1, int x2, int y1, int y2, int *color_p);
	void pixelPresent(int x, int y, uint32_t data);

private:
	framebuffer_t framebuffer;
};
#endif

#endif
