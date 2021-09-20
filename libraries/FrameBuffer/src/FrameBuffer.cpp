#include "FrameBuffer.h"
#include "Arduino.h"

FrameBuffer::FrameBuffer()
{
}

// ~FrameBuffer();
int FrameBuffer::begin(lcd_type_t lcd_type, int bright)
{
	// fb_init(&framebuffer, width, height);
	// fb_setbl(&framebuffer, bright);
	// fb_init(width, height);
	fb_init(lcd_type, NULL);
	return 0;
}

int begin(pixel_format *data, lcd_type_t lcd_type, int bright = 100)
{
	fb_init(lcd_type, data);
	return 0;
}

void FrameBuffer::setBright(int bright)
{
	// return fb_setbl(&framebuffer, bright);
}

void FrameBuffer::areaPresent(int x, int y, int w, int h, pixel_format *data)
{
	fb_area_present(x, y, w, h, data);
}

void FrameBuffer::areaPresentX(int x, int y, int w, int h, pixel_format *data)
{
	fb_area_present_x(x, y, w, h, data);
}

void FrameBuffer::areaPresentY(int x, int y, int w, int h, pixel_format *data)
{
	fb_area_present_y(x, y, w, h, data);
}

void FrameBuffer::areaPresentXY(int x, int y, int w, int h, pixel_format *data)
{
	fb_area_present_xy(x, y, w, h, data);
}

void FrameBuffer::pixelPresent(int x, int y, pixel_format data)
{
	fb_pixel_present(x, y, data);
}

pixel_format *FrameBuffer::getBuffer()
{
	return fb_get_buffer();
}