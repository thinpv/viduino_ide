#include "FrameBuffer.h"
#include "Arduino.h"

FrameBuffer::FrameBuffer()
{
}

// ~FrameBuffer();
int FrameBuffer::begin(int width, int height, int bright)
{
	// fb_init(&framebuffer, width, height);
	// fb_setbl(&framebuffer, bright);
	LCDbuff = (unsigned short *)malloc(width * height * sizeof(uint16_t));
	Sys_LCD_Init(width, height, (unsigned int *)LCDbuff, NULL);
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
		memcpy(&LCDbuff[y * width + x1], data, size_of_row);
		data += data_in_a_row;
	}
}

void FrameBuffer::areaPresentX(int x1, int x2, int y1, int y2, unsigned short *data)
{
	uint16_t data_in_a_row = x2 - x1 + 1;
	uint16_t size_of_row = data_in_a_row * sizeof(uint16_t);
	int y11 = height - y2 - 1;
	int y21 = height - y1 - 1;
	for (int y = y21; y >= y11; y--)
	{
		memcpy(&LCDbuff[y * width + x1], data, size_of_row);
		data += data_in_a_row;
	}
}

void FrameBuffer::areaPresentY(int x1, int x2, int y1, int y2, unsigned short *data)
{
	int x11 = width - x2 - 1;
	int x21 = width - x1 - 1;
	for (int y = y1; y <= y2; y++)
	{
		for (int x = x21; x >= x11; x--)
		{
			LCDbuff[y * width + x] = *data;
			data += 1;
		}
	}
}

void FrameBuffer::areaPresentXY(int x1, int x2, int y1, int y2, unsigned short *data)
{
	int x11 = width - x2 - 1;
	int x21 = width - x1 - 1;
	int y11 = height - y2 - 1;
	int y21 = height - y1 - 1;
	for (int y = y21; y >= y11; y--)
	{
		for (int x = x21; x >= x11; x--)
		{
			LCDbuff[y * width + x] = *data;
			data += 1;
		}
	}
}

void FrameBuffer::pixelPresent(int x, int y, unsigned short data)
{
	LCDbuff[y * width + x] = data;
}

unsigned short * FrameBuffer::getBuffer()
{
	return LCDbuff;
}