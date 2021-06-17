#include "FrameBuffer.h"

extern "C"
{
	void memcpy(void *, void *, int);
}

FrameBuffer::FrameBuffer()
{
}

// ~FrameBuffer();
int FrameBuffer::begin(int width, int height, int bright)
{
	// fb_init(&framebuffer, width, height);
	// fb_setbl(&framebuffer, bright);
	BT1 = (unsigned short *)LCDbuff;
	Sys_LCD_Init(XSIZE_PHYS, YSIZE_PHYS, (unsigned int *)LCDbuff, NULL);
	this->width = width;
	this->height = height;
}

void FrameBuffer::setBright(int bright)
{
	// return fb_setbl(&framebuffer, bright);
}

void FrameBuffer::areaPresent(int x1, int x2, int y1, int y2, unsigned short *data)
{
	uint16_t x = x2 - x1 + 1;
	uint16_t size_of_row = x * sizeof(uint16_t);
	for (int i = y1; i < y2 + 1; i++)
	{
		memcpy(&BT1[i * width + x1], data, size_of_row);
		data += x;
	}
	// return fb_area_present(&framebuffer, x1, x2, y1, y2, (uint32_t *)color_p);
}

// void FrameBuffer::pixelPresent(int x, int y, uint32_t data)
// {
// 	return fb_pixel_present(&framebuffer, x, y, data);
// }

unsigned short *FrameBuffer::getBuffer()
{
	return (unsigned short *)LCDbuff;
}