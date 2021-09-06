#ifndef __SYS_LCD_H__
#define __SYS_LCD_H__

// #include "sys_types.h"
#include <types.h>

//����LCD
#define LCD_TYPE_RGB43_480_272
//#define LCD_TYPE_RGB43_800_480
//#define LCD_TYPE_Vga_1024_768
//#define LCD_TYPE_Vga_640_480_60HZ
//#define  LCD_TYPE_Vga_640_480_75HZ
//#define  LCD_TYPE_TV_PAL_720_576

typedef unsigned short pixel_format;

//-------------------------------------------
#ifdef LCD_TYPE_TV_PAL_720_576
#define XSIZE_PHYS (unsigned int)720
#define YSIZE_PHYS (unsigned int)576
#endif
//-------------------------------------------
#ifdef LCD_TYPE_RGB43_480_272
#define XSIZE_PHYS (unsigned int)480
#define YSIZE_PHYS (unsigned int)272
#endif
//-------------------------------------------
#ifdef LCD_TYPE_RGB43_800_480
#define XSIZE_PHYS (unsigned int)800
#define YSIZE_PHYS (unsigned int)480
#endif
//-------------------------------------------
#ifdef LCD_TYPE_Vga_1024_768
#define XSIZE_PHYS (unsigned int)1024
#define YSIZE_PHYS (unsigned int)768
#endif
//-------------------------------------------
#ifdef LCD_TYPE_Vga_640_480_75HZ
#define XSIZE_PHYS (unsigned int)640
#define YSIZE_PHYS (unsigned int)480
#endif
//-------------------------------------------
#ifdef LCD_TYPE_Vga_640_480_60HZ
#define XSIZE_PHYS (unsigned int)640
#define YSIZE_PHYS (unsigned int)480
#endif
//-------------------------------------------

#ifdef __cplusplus
extern "C"
{
#endif

/*******************************常用色定义****************/
#define WHITE 0xFFFF
#define BLACK 0x0000
#define BLUE 0x001F //蓝
#define BRED 0XF81F
#define GRED 0XFFE0
#define GBLUE 0X07FF
#define RED 0xF800
#define MAGENTA 0xF81F
#define GREEN 0x07E0
#define CYAN 0x7FFF
#define YELLOW 0xFFE0
#define BROWN 0XBC40		 //棕色
#define BRRED 0XFC07		 //棕红色
#define GRAY 0X8430			 //灰色
#define DARKBLUE 0X01CF	 //深蓝色
#define LIGHTBLUE 0X7D7C //浅蓝色
#define GRAYBLUE 0X5458	 //灰蓝色
												 /********************************************************/

#define TCON_Base_Address (u32_t)0x01c0c000
#define CCU_Base_Address (u32_t)0x01C20000
#define CCU_TCON_CLK_REG (u32_t) CCU_Base_Address + 0x0118
#define CCU_BUS_SOFT_RST_REG0 (u32_t) CCU_Base_Address + 0x02C0
#define CCU_BUS_SOFT_RST_REG1 (u32_t) CCU_Base_Address + 0x02C4
#define CCU_BUS_CLK_GATING_REG0 (u32_t) CCU_Base_Address + 0x0060
#define CCU_BUS_CLK_GATING_REG1 (u32_t) CCU_Base_Address + 0x0064
#define DMA_Base_Address (u32_t)0x01C02000
#define DEBE_Base_Address (u32_t)0x01E60000
#define DEFE_Base_Address (u32_t)0x01E00000

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
		} timing;
	};

	extern struct fb_f1c100s_pdata_t *lcd_pdat;

	extern pixel_format *fb_buffer;
	extern int fb_width;
	extern int fb_height;

	// void Sys_LCD_Init(int width, int height, unsigned int *buff1, unsigned int *buff2);
	void F1C100S_LCD_Init(int width, int height, unsigned int *buff1, unsigned int *buff2);
	void YUV_OUT_Enable(char mode);
	void Wait_tcon_te(void);

	void fb_init(int width, int height);
	void fb_init_buffer(pixel_format *buffer, int width, int height);
	void fb_clear(pixel_format data);
	void fb_area_present(int x, int y, int w, int h, pixel_format *data);
	void fb_area_present_x(int x, int y, int w, int h, pixel_format *data);
	void fb_area_present_y(int x, int y, int w, int h, pixel_format *data);
	void fb_area_present_xy(int x, int y, int w, int h, pixel_format *data);
	void fb_pixel_present(int x, int y, pixel_format data);
	pixel_format *fb_get_buffer();
	void fb_area_present_888(int x, int y, int w, int h, uint32_t *data);//untest

	void swap_r_b(uint8_t enable);

#ifdef __cplusplus
}
#endif

#endif
