#include "FrameBuffer.h"

FrameBuffer::FrameBuffer()
{
}

// ~FrameBuffer();
int FrameBuffer::begin(int width, int height, int bright)
{
	fb_init(&framebuffer, width, height);
	fb_setbl(&framebuffer, bright);
}

void FrameBuffer::setBright(int bright)
{
	return fb_setbl(&framebuffer, bright);
}

void FrameBuffer::areaPresent(int x1, int x2, int y1, int y2, int *color_p)
{
	return fb_area_present(&framebuffer, x1, x2, y1, y2, (uint32_t *)color_p);
}

void FrameBuffer::pixelPresent(int x, int y, uint32_t data)
{
	return fb_pixel_present(&framebuffer, x, y, data);
}
