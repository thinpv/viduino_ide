#include "FrameBuffer.h"
#include "Arduino.h"
// extern "C"
// {
// 	void memcpy(void *, void *, int);
// }

FrameBuffer::FrameBuffer()
{
}

// ~FrameBuffer();
int FrameBuffer::begin(int width, int height, int bright)
{
	// fb_init(&framebuffer, width, height);
	// fb_setbl(&framebuffer, bright);
	// BT1 = (unsigned short *)LCDbuff;
	bt = (lcd_buff_pt)LCDbuff;
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
	uint16_t data_in_a_row = x2 - x1 + 1;
	uint16_t size_of_row = data_in_a_row * sizeof(uint16_t);
	for (int y = y1; y <= y2; y++)
	{
		memcpy(&(*bt)[y][x1], data, size_of_row);
		data += data_in_a_row;
	}
}

void FrameBuffer::areaPresentX(int x1, int x2, int y1, int y2, unsigned short *data)
{
	uint16_t data_in_a_row = x2 - x1 + 1;
	uint16_t size_of_row = data_in_a_row * sizeof(uint16_t);
	for (int y = y2; y >= y1; y--)
	{
		memcpy(&(*bt)[y][x1], data, size_of_row);
		data += data_in_a_row;
		// for (int x = x1; x <= x2; x++)
		// {
		// 	(*bt)[y][x] = *data;
		// 	data += 1;
		// }
	}
}

void FrameBuffer::areaPresentY(int x1, int x2, int y1, int y2, unsigned short *data)
{
	for (int y = y1; y <= y2; y++)
	{
		for (int x = x2; x >= x1; x--)
		{
			(*bt)[y][x] = *data;
			data += 1;
		}
	}
}

void FrameBuffer::areaPresentXY(int x1, int x2, int y1, int y2, unsigned short *data)
{
	for (int y = y2; y >= y1; y--)
	{
		for (int x = x2; x >= x1; x--)
		{
			(*bt)[y][x] = *data;
			data += 1;
		}
	}
}

// void FrameBuffer::pixelPresent(int x, int y, uint32_t data)
// {
// 	return fb_pixel_present(&framebuffer, x, y, data);
// }

unsigned short *FrameBuffer::getBuffer()
{
	return (unsigned short *)LCDbuff;
}