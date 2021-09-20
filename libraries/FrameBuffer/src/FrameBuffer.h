#ifndef __FRAME_BUFFER_H_
#define __FRAME_BUFFER_H_

#include <fb.h>

// typedef enum
// {
// 	LCD_TYPE_RGB_480_272 = 0,
// 	LCD_TYPE_RGB_800_480,
// 	LCD_TYPE_VGA_1024_768,
// 	LCD_TYPE_VGA_640_480_60HZ,
// 	LCD_TYPE_VGA_640_480_75HZ,
// 	LCD_TYPE_TV_PAL_720_576
// } lcd_type_t;

#ifdef __cplusplus
class FrameBuffer
{
public:
	FrameBuffer();
	// ~FrameBuffer();
	int begin(lcd_type_t lcd_type = LCD_TYPE_RGB_480_272, int bright = 100);
	int begin(pixel_format *data, lcd_type_t lcd_type = LCD_TYPE_RGB_480_272, int bright = 100);
	void setBright(int bright);
	void areaPresent(int x, int y, int w, int h, pixel_format *data);
	void areaPresentX(int x, int y, int w, int h, pixel_format *data);
	void areaPresentY(int x, int y, int w, int h, pixel_format *data);
	void areaPresentXY(int x, int y, int w, int h, pixel_format *data);
	void pixelPresent(int x, int y, pixel_format data);
	pixel_format *getBuffer();
};
#endif

#endif
