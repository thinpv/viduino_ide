#ifndef __FB_H__
#define __FB_H__

#include <types.h>

#ifdef __cplusplus
extern "C"
{
#endif

/*******************************常用色定义****************/
#define WHITE 0xFFFF
#define BLACK 0x0000
#define BLUE 0x001F
#define BRED 0XF81F
#define GRED 0XFFE0
#define GBLUE 0X07FF
#define RED 0xF800
#define MAGENTA 0xF81F
#define GREEN 0x07E0
#define CYAN 0x7FFF
#define YELLOW 0xFFE0
#define BROWN 0XBC40
#define BRRED 0XFC07
#define GRAY 0X8430
#define DARKBLUE 0X01CF
#define LIGHTBLUE 0X7D7C
#define GRAYBLUE 0X5458

	typedef unsigned short pixel_format;

	struct fb_f1c100s_pdata_t
	{
		virtual_addr_t virtdefe;
		virtual_addr_t virtdebe;
		virtual_addr_t virttcon;

		int mode;
		int width;
		int height;
		int pwidth;
		int pheight;
		int bits_per_pixel;
		int bytes_per_pixel;
		int index;
		void *vram[2];

		struct
		{
			int pixel_clock_hz;
			int h_front_porch;
			int h_back_porch;
			int h_sync_len;
			int v_front_porch;
			int v_back_porch;
			int v_sync_len;
			int h_sync_active;
			int v_sync_active;
			int den_active;
			int clk_active;
			int dat_active;
		} timing;
	};

	typedef enum
	{
		LCD_TYPE_RGB_480_272 = 0,
		LCD_TYPE_RGB_480_480,
		LCD_TYPE_RGB_800_480,
		LCD_TYPE_VGA_1024_768,
		LCD_TYPE_VGA_640_480_60HZ,
		LCD_TYPE_VGA_640_480_75HZ,
		LCD_TYPE_TV_PAL_720_576,
		LCD_TYPE_TV_NTSC_720_480
	} lcd_type_t;

	// extern struct fb_f1c100s_pdata_t *lcd_pdat;

	// extern pixel_format *fb_buffer;
	// extern int fb_width;
	// extern int fb_height;

	void Wait_tcon_te(void);

	void fb_init(lcd_type_t lcd_type, void *buffer);
	void fb_clear(pixel_format data);
	void fb_area_present(int x, int y, int w, int h, pixel_format *data);
	void fb_area_present_x(int x, int y, int w, int h, pixel_format *data);
	void fb_area_present_y(int x, int y, int w, int h, pixel_format *data);
	void fb_area_present_xy(int x, int y, int w, int h, pixel_format *data);
	void fb_pixel_present(int x, int y, pixel_format data);
	pixel_format *fb_get_buffer();
	void fb_area_present_888(int x, int y, int w, int h, uint32_t *data); //untest

	void swap_r_b(uint8_t enable);

#ifdef __cplusplus
}
#endif

#endif
