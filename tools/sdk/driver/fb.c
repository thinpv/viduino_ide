#include "fb.h"
#include <stdlib.h>

#include <irq.h>
#include <io.h>
#include <ccu.h>
#include <debe.h>

// Global structure
struct fb_f1c100s_pdata_t f1;
pixel_format *fb_buffer;
int fb_width;
int fb_height;

/*---------------------------------------------------
TCON interrupt
----------------------------------------------------*/
volatile int LCD_TE = 0;
void TCON_ISR(void)
{
	// C_BIT(TCON_Base_Address + 0x04, 13); //Clera TCON0 line trigger interrupt
	C_Bit(TCON->TCON_INT_REG0, 13);
	LCD_TE = 1;
}
/*---------------------------------------------------
Wait for TCON line trigger interrupt
----------------------------------------------------*/
void Wait_tcon_te(void)
{
	int tcon_timeout = 0; // Exit after timeout
	LCD_TE = 0;
	while (1)
	{
		sys_delay_ms(1); // Delay can do other tasks
		tcon_timeout++;
		if ((LCD_TE == 1) || (tcon_timeout > 30))
			break;
	}
}

void fb_tcon_enable()
{
	S_Bit(TCON->TCON_CTRL_REG, 31);
}

void fb_tcon_disable()
{
	TCON->TCON_CTRL_REG = 0;
	TCON->TCON_INT_REG0 = 0;
	C_Value(TCON->TCON_CLK_CTRL_REG, 28, 0xf);
	TCON->TCON0_IO_CTRL_REG1 = 0xffffffff;
	TCON->TCON1_IO_CTRL_REG1 = 0xffffffff;
}

// LCD IO initialization
void fb_io_init(void)
{
	int i = 0;
	for (i = 1; i <= 11; i++)
	{
		gpio_set_cfg(GPIOD, i, GPIO_FUNC_010);
		gpio_set_pull(GPIOD, i, GPIO_PULL_NONE);
		gpio_set_drv(GPIOD, i, GPIO_DRV_STRONG);
	}
	for (i = 13; i <= 21; i++)
	{
		gpio_set_cfg(GPIOD, i, GPIO_FUNC_010);
		gpio_set_pull(GPIOD, i, GPIO_PULL_NONE);
		gpio_set_drv(GPIOD, i, GPIO_DRV_STRONG);
	}
}

void fb_tcon_init(uint8_t f)
{
	uint32_t val, bp, total;
	struct fb_f1c100s_pdata_t *pdat = &f1;
	// Enable TCON clock
	S_Bit(CCU->TCON_CLK_REG, 31);
	ccu_reset(RESET_LCD, true);
	sys_delay_ms(1);

	//-----------------------------------------TCON related settings-------------------------------------------------
	// Set to TCON0
	C_Bit(TCON->TCON_CTRL_REG, 0);

	// Enable TCON 31
	// Swap RED and BLUE data at FIFO1
	val = (pdat->timing.v_front_porch + pdat->timing.v_back_porch + pdat->timing.v_sync_len);
	TCON->TCON0_CTRL_REG = ((u64_t)0x1 << 31) | (u64_t)0x1 << 23 | (val & 0x1f) << 4;
	// Set the clock
	TCON->TCON_CLK_CTRL_REG = ((u64_t)0xf << 28) | (f << 0);

	// Set width and height
	TCON->TCON0_BASIC_TIMING_REG0 = ((pdat->width - 1) << 16) | ((pdat->height - 1) << 0);

	// Set HT+HBP
	bp = pdat->timing.h_sync_len + pdat->timing.h_back_porch;
	total = pdat->width + pdat->timing.h_front_porch + bp;
	TCON->TCON0_BASIC_TIMING_REG1 = ((total - 1) << 16) | ((bp - 1) << 0);
	// Set VT+VBp
	bp = pdat->timing.v_sync_len + pdat->timing.v_back_porch;
	total = pdat->height + pdat->timing.v_front_porch + bp;
	TCON->TCON0_BASIC_TIMING_REG2 = ((total * 2) << 16) | ((bp - 1) << 0);
	// Set clock width
	TCON->TCON0_BASIC_TIMING_REG3 = ((pdat->timing.h_sync_len - 1) << 16) | ((pdat->timing.v_sync_len - 1) << 0);

	// Set mode
	if (pdat->mode == 1) //rgb
	{
		TCON->TCON0_HV_TIMING_REG = 0;
		TCON->TCON0_CPU_IF_REG = (u64_t)(4) << 29;
	}
	else //cpu
	{
		// Set to 8080 mode
		S_Bit(TCON->TCON0_CTRL_REG, 24);
		// Set input source
		C_Value(TCON->TCON0_CTRL_REG, 0, 0x3);
		// Set to 16-bit mode + automatic
		TCON->TCON0_CPU_IF_REG = ((u64_t)(4) << 29) | ((u64_t)(1) << 28);
	}
	//FRM
	if (pdat->bits_per_pixel == 18 || pdat->bits_per_pixel == 16)
	{
		TCON->TCON_FRM_SEED0_R_REG = 0x11111111;
		TCON->TCON_FRM_SEED0_G_REG = 0x11111111;
		TCON->TCON_FRM_SEED0_B_REG = 0x11111111;
		TCON->TCON_FRM_SEED1_R_REG = 0x11111111;
		TCON->TCON_FRM_SEED1_G_REG = 0x11111111;
		TCON->TCON_FRM_SEED1_B_REG = 0x11111111;
		TCON->TCON_FRM_TBL_REG0 = 0x01010000;
		TCON->TCON_FRM_TBL_REG1 = 0x15151111;
		TCON->TCON_FRM_TBL_REG2 = 0x57575555;
		TCON->TCON_FRM_TBL_REG3 = 0x7f7f7777;
		TCON->TCON_FRM_CTRL_REG = (pdat->bits_per_pixel == 18) ? (0 << 4) : (5 << 4);
		S_Bit(TCON->TCON_FRM_CTRL_REG, 31);
	}
	// Polarity control
	TCON->TCON0_IO_CTRL_REG0 = (1 << 28) | (1 << 24) | (1 << 25) | (1 << 26); // | (1 << 27);
	// Trigger control off
	TCON->TCON0_IO_CTRL_REG1 = 0;
	//
	irq_register(IRQ_LEVEL_1, F1C100S_IRQ_LCD, TCON_ISR, 3);
	//set line
	TCON->TCON_INT_REG1 = pdat->height << 16;
	// S_BIT(TCON_Base_Address + 0x04, 29); //TCON0 line trigger interrupt enable
	S_Bit(TCON->TCON_INT_REG0, 29);
}

void fb_init(lcd_type_t lcd_type, void *buffer)
{
	u8 F;
	struct fb_f1c100s_pdata_t *lcd_pdat = &f1;
	if (lcd_type == LCD_TYPE_RGB_480_272)
	{
		// LCD mode 0-cpu 1-rgb
		lcd_pdat->mode = 1;
		// LCD width and height
		lcd_pdat->width = 480;
		lcd_pdat->height = 272;
		// Pixel width
		lcd_pdat->bits_per_pixel = 16;

		// Timing
		lcd_pdat->timing.h_front_porch = 40; //
		lcd_pdat->timing.h_back_porch = 87;	 //
		lcd_pdat->timing.h_sync_len = 1;		 //HSPW
		lcd_pdat->timing.v_front_porch = 13; //
		lcd_pdat->timing.v_back_porch = 31;	 //
		lcd_pdat->timing.v_sync_len = 1;		 //VSPW
		// Polarity setting 0 inverted
		lcd_pdat->timing.h_sync_active = 0;
		lcd_pdat->timing.v_sync_active = 0;
		lcd_pdat->timing.den_active = 1; // Inverted
		lcd_pdat->timing.clk_active = 0;
		/*********************************DEBE SET**************************/
		lcd_pdat->index = 0;					 // FB index
		lcd_pdat->bytes_per_pixel = 2; // buff color bit width

		// Set the video clock to 390MHZ
		u8 N = 65, M = 4;
		F = 25; //(24MHz*N)/M/F=15.6MHZ
		C_Bit(TCON->TCON_FRM_CTRL_REG, 31);
		TCON->TCON_FRM_CTRL_REG = ((N - 1) << 8) | ((M - 1) << 0) | (3 << 24);
		S_Bit(TCON->TCON_FRM_CTRL_REG, 31);
		sys_delay_ms(1);
	}
	else if (lcd_type == LCD_TYPE_RGB_800_480)
	{
		// LCD mode 0-cpu 1-rgb
		lcd_pdat->mode = 1;
		// LCD width and height
		lcd_pdat->width = 800;
		lcd_pdat->height = 480;
		// Pixel width
		lcd_pdat->bits_per_pixel = 16;

		// Timing
		lcd_pdat->timing.h_front_porch = 40; //
		lcd_pdat->timing.h_back_porch = 87;	 //
		lcd_pdat->timing.h_sync_len = 1;		 //HSPW
		lcd_pdat->timing.v_front_porch = 13; //
		lcd_pdat->timing.v_back_porch = 31;	 //
		lcd_pdat->timing.v_sync_len = 1;		 //VSPW
		// Polarity setting 0 inverted
		lcd_pdat->timing.h_sync_active = 0;
		lcd_pdat->timing.v_sync_active = 0;
		lcd_pdat->timing.den_active = 1; // Inverted
		lcd_pdat->timing.clk_active = 0;
		/*********************************DEBE SET**************************/
		lcd_pdat->index = 0;					 // FB index
		lcd_pdat->bytes_per_pixel = 2; // buff color bit width

		// Set the video clock to 390MHZ
		u8 N = 65, M = 4;
		F = 9; //(24MHz*N)/M/F=43.33MHZ
		C_Bit(TCON->TCON_FRM_CTRL_REG, 31);
		TCON->TCON_FRM_CTRL_REG = ((N - 1) << 8) | ((M - 1) << 0) | (3 << 24);
		S_Bit(TCON->TCON_FRM_CTRL_REG, 31);
		sys_delay_ms(1);
	}
	else if (lcd_type == LCD_TYPE_VGA_1024_768)
	{
		// LCD mode 0-cpu 1-rgb
		lcd_pdat->mode = 1;
		// LCD width and height
		lcd_pdat->width = 1024;
		lcd_pdat->height = 768;
		// Pixel width
		lcd_pdat->bits_per_pixel = 16;

		// Timing
		lcd_pdat->timing.h_front_porch = 24; //
		lcd_pdat->timing.h_back_porch = 160; //
		lcd_pdat->timing.h_sync_len = 136;	 //HSPW
		lcd_pdat->timing.v_front_porch = 3;	 //
		lcd_pdat->timing.v_back_porch = 29;	 //
		lcd_pdat->timing.v_sync_len = 6;		 //VSPW
		// Polarity setting 0 inverted
		lcd_pdat->timing.h_sync_active = 0;
		lcd_pdat->timing.v_sync_active = 0;
		lcd_pdat->timing.den_active = 1; // Inverted
		lcd_pdat->timing.clk_active = 0;
		/*********************************DEBE SET**************************/
		lcd_pdat->index = 0;					 // FB index
		lcd_pdat->bytes_per_pixel = 2; // buff color bit width

		u8 N = 65, M = 4;
		F = 6; //(24MHz*N)/M/F=65MHZ
		C_Bit(TCON->TCON_FRM_CTRL_REG, 31);
		TCON->TCON_FRM_CTRL_REG = ((N - 1) << 8) | ((M - 1) << 0) | (3 << 24);
		S_Bit(TCON->TCON_FRM_CTRL_REG, 31);
		sys_delay_ms(1);
	}
	else if (lcd_type == LCD_TYPE_VGA_640_480_60HZ)
	{
		// LCD mode 0-cpu 1-rgb
		lcd_pdat->mode = 1;
		// LCD width and height
		lcd_pdat->width = 640;
		lcd_pdat->height = 480;
		// Pixel width
		lcd_pdat->bits_per_pixel = 16;

		// Timing
		lcd_pdat->timing.h_front_porch = 16; //
		lcd_pdat->timing.h_back_porch = 48;	 //
		lcd_pdat->timing.h_sync_len = 96;		 //HSPW
		lcd_pdat->timing.v_front_porch = 10; //
		lcd_pdat->timing.v_back_porch = 33;	 //
		lcd_pdat->timing.v_sync_len = 2;		 //VSPW
		// Polarity setting 0 inverted
		lcd_pdat->timing.h_sync_active = 0;
		lcd_pdat->timing.v_sync_active = 0;
		lcd_pdat->timing.den_active = 1; // Inverted
		lcd_pdat->timing.clk_active = 0;
		/*********************************DEBE SET**************************/
		lcd_pdat->index = 0;					 // FB index
		lcd_pdat->bytes_per_pixel = 2; // buff color bit width

		u8 N = 23, M = 2;
		F = 11; //LCD=276000000/11=25090909
		C_Bit(TCON->TCON_FRM_CTRL_REG, 31);
		TCON->TCON_FRM_CTRL_REG = ((N - 1) << 8) | ((M - 1) << 0) | (3 << 24);
		S_Bit(TCON->TCON_FRM_CTRL_REG, 31);
		sys_delay_ms(1);
	}
	else if (lcd_type == LCD_TYPE_VGA_640_480_75HZ)
	{
		// LCD mode 0-cpu 1-rgb
		lcd_pdat->mode = 1;
		// LCD width and height
		lcd_pdat->width = 640;
		lcd_pdat->height = 480;
		// Pixel width
		lcd_pdat->bits_per_pixel = 16;

		// Timing
		lcd_pdat->timing.h_front_porch = 16; //
		lcd_pdat->timing.h_back_porch = 120; //
		lcd_pdat->timing.h_sync_len = 64;		 //HSPW
		lcd_pdat->timing.v_front_porch = 1;	 //
		lcd_pdat->timing.v_back_porch = 16;	 //
		lcd_pdat->timing.v_sync_len = 3;		 //VSPW
		// Polarity setting 0 inverted
		lcd_pdat->timing.h_sync_active = 0;
		lcd_pdat->timing.v_sync_active = 0;
		lcd_pdat->timing.den_active = 1; // Inverted
		lcd_pdat->timing.clk_active = 0;
		/*********************************DEBE SET**************************/
		lcd_pdat->index = 0;					 // FB index
		lcd_pdat->bytes_per_pixel = 2; // buff color bit width

		u8 N = 63, M = 8;
		F = 6;
		C_Bit(TCON->TCON_FRM_CTRL_REG, 31);
		TCON->TCON_FRM_CTRL_REG = ((N - 1) << 8) | ((M - 1) << 0) | (3 << 24);
		S_Bit(TCON->TCON_FRM_CTRL_REG, 31);
		sys_delay_ms(1);
	}
	else if (lcd_type == LCD_TYPE_TV_PAL_720_576)
	{
		// LCD mode 0-cpu 1-rgb
		lcd_pdat->mode = 1;
		// LCD width and height
		lcd_pdat->width = 720;
		lcd_pdat->height = 576;
		// Pixel width
		lcd_pdat->bits_per_pixel = 16;

		// Timing
		lcd_pdat->timing.h_front_porch = 40; //
		lcd_pdat->timing.h_back_porch = 87;	 //
		lcd_pdat->timing.h_sync_len = 1;		 //HSPW
		lcd_pdat->timing.v_front_porch = 13; //
		lcd_pdat->timing.v_back_porch = 31;	 //
		lcd_pdat->timing.v_sync_len = 1;		 //VSPW
		// Polarity setting 0 inverted
		lcd_pdat->timing.h_sync_active = 0;
		lcd_pdat->timing.v_sync_active = 0;
		lcd_pdat->timing.den_active = 1; // Inverted
		lcd_pdat->timing.clk_active = 0;
		/*********************************DEBE SET**************************/
		lcd_pdat->index = 0;					 // FB index
		lcd_pdat->bytes_per_pixel = 2; // buff color bit width

		// Set the video clock to 390MHZ
		u8 N = 65, M = 4;
		F = 25; //(24MHz*N)/M/F=15.6MHZ
		C_Bit(TCON->TCON_FRM_CTRL_REG, 31);
		TCON->TCON_FRM_CTRL_REG = ((N - 1) << 8) | ((M - 1) << 0) | (3 << 24);
		S_Bit(TCON->TCON_FRM_CTRL_REG, 31);
		sys_delay_ms(1);
	}
	else if (lcd_type == LCD_TYPE_TV_NTSC_720_480)
	{
		// LCD mode 0-cpu 1-rgb
		lcd_pdat->mode = 1;
		// LCD width and height
		lcd_pdat->width = 720;
		lcd_pdat->height = 480;
		// Pixel width
		lcd_pdat->bits_per_pixel = 16;

		// Timing
		lcd_pdat->timing.h_front_porch = 40; //
		lcd_pdat->timing.h_back_porch = 87;	 //
		lcd_pdat->timing.h_sync_len = 1;		 //HSPW
		lcd_pdat->timing.v_front_porch = 13; //
		lcd_pdat->timing.v_back_porch = 31;	 //
		lcd_pdat->timing.v_sync_len = 1;		 //VSPW
		// Polarity setting 0 inverted
		lcd_pdat->timing.h_sync_active = 0;
		lcd_pdat->timing.v_sync_active = 0;
		lcd_pdat->timing.den_active = 1; // Inverted
		lcd_pdat->timing.clk_active = 0;
		/*********************************DEBE SET**************************/
		lcd_pdat->index = 0;					 // FB index
		lcd_pdat->bytes_per_pixel = 2; // buff color bit width

		// Set the video clock to 390MHZ
		u8 N = 65, M = 4;
		F = 25; //(24MHz*N)/M/F=15.6MHZ
		C_Bit(TCON->TCON_FRM_CTRL_REG, 31);
		TCON->TCON_FRM_CTRL_REG = ((N - 1) << 8) | ((M - 1) << 0) | (3 << 24);
		S_Bit(TCON->TCON_FRM_CTRL_REG, 31);
		sys_delay_ms(1);
	}
	else
	{
		return 1;
	}

	if (buffer)
	{
		lcd_pdat->vram[0] = (pixel_format *)buffer;
	}
	else
	{
		lcd_pdat->vram[0] = (pixel_format *)malloc(lcd_pdat->width * lcd_pdat->height * sizeof(pixel_format));
	}
	memset(lcd_pdat->vram[0], 0xFF, lcd_pdat->width * lcd_pdat->height * sizeof(pixel_format));
	fb_buffer = lcd_pdat->vram[0];
	fb_width = lcd_pdat->width;
	fb_height = lcd_pdat->height;

	fb_io_init();
	fb_tcon_disable();
	debe_disable();
	debe_init(lcd_pdat->width, lcd_pdat->height, lcd_pdat->vram[0]);
	debe_enable();
	fb_tcon_init(F);
	if (LCD_TYPE_TV_PAL_720_576 == lcd_type || LCD_TYPE_TV_NTSC_720_480 == lcd_type)
	{
		TVE_Init(lcd_type);
		debe_yuv_out_enable(0);
	}
	fb_tcon_enable();
}

void fb_clear(pixel_format data)
{
	for (int i = 0; i < fb_width * fb_height; i++)
	{
		fb_buffer[i] = data;
	}
}

void fb_area_present(int x, int y, int w, int h, pixel_format *data)
{
	uint16_t size_of_row = w * sizeof(pixel_format);
	for (int i = y; i < y + h; i++)
	{
		memcpy(&fb_buffer[i * fb_width + x], data, size_of_row);
		data += w;
	}
}

void fb_area_present_x(int x, int y, int w, int h, pixel_format *data)
{
	uint16_t size_of_row = w * sizeof(pixel_format);
	for (int i = fb_height - y - 1; i >= fb_height - y - h; i--)
	{
		memcpy(&fb_buffer[i * fb_width + x], data, size_of_row);
		data += w;
	}
}

void fb_area_present_y(int x, int y, int w, int h, pixel_format *data)
{
	for (int i = y; i < y + h; i++)
	{
		for (int j = fb_width - x - 1; j >= fb_width - x - w; j--)
		{
			fb_buffer[i * fb_width + j] = *data;
			data += 1;
		}
	}
}

void fb_area_present_xy(int x, int y, int w, int h, pixel_format *data)
{
	for (int i = fb_height - y - 1; i >= fb_height - y - h; i--)
	{
		for (int j = fb_width - x - 1; j >= fb_width - x - w; j--)
		{
			fb_buffer[i * fb_width + j] = *data;
			data += 1;
		}
	}
}

void fb_pixel_present(int x, int y, pixel_format data)
{
	fb_buffer[y * fb_width + x] = data;
}

pixel_format *fb_get_buffer()
{
	return fb_buffer;
}

void swap_r_b(uint8_t enable)
{
	if (enable)
		S_Bit(TCON->TCON0_CTRL_REG, 23);
	else
		C_Bit(TCON->TCON0_CTRL_REG, 23);
}

static uint16_t convert_888_to_565(uint32_t data_888)
{
	uint16_t data_565;
	data_565 = (data_888 >> 3) & 0x001F;
	data_565 |= (data_888 >> 5) & 0x07E0;
	data_565 |= (data_888 >> 8) & 0xF800;
	return data_565;
}

void fb_area_present_888(int x, int y, int w, int h, uint32_t *data)
{
	for (int i = y; i < y + h; i++)
	{
		for (int j = x; j < x + w; j++)
		{
			fb_buffer[i * fb_width + j] = convert_888_to_565(*data);
			data += 1;
		}
	}
}