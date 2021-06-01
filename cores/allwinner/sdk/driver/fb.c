/*
 * driver/fb.c
 *
 * Copyright(c) 2007-2018 Jianjun Jiang <8192542@qq.com>
 * Official site: http://xboot.org
 * Mobile phone: +86-18665388956
 * QQ: 8192542
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <dma.h>
#include <ccu.h>
#include <gpio.h>
#include <pwm.h>
#include <fb.h>
#include <f1c100s/reg-tcon.h>
#include <f1c100s/reg-defe.h>
#include <f1c100s/reg-debe.h>
#include <f1c100s/reg-ccu.h>
#include <io.h>
#include <sizes.h>

#define phys_to_virt(x) (x)

typedef struct
{
	virtual_addr_t virtdefe;
	virtual_addr_t virtdebe;
	virtual_addr_t virttcon;

	char *clkdefe;
	char *clkdebe;
	char *clktcon;
	int32_t rstdefe;
	int32_t rstdebe;
	int32_t rsttcon;
	int32_t width;
	int32_t height;
	int32_t pwidth;
	int32_t pheight;
	int32_t bits_per_pixel;
	int32_t bytes_per_pixel;
	int32_t index;
	void *vram[2];

	struct
	{
		int32_t pixel_clock_hz;
		int32_t h_front_porch;
		int32_t h_back_porch;
		int32_t h_sync_len;
		int32_t v_front_porch;
		int32_t v_back_porch;
		int32_t v_sync_len;
		int32_t h_sync_active;
		int32_t v_sync_active;
		int32_t den_active;
		int32_t clk_active;
	} timing;

	pwm_t *backlight;
	int32_t brightness;
} fb_pdata_t;

static pwm_t led_pwm_bl =
		{
				.virt = 0x01c21000,
				.duty = 100,
				.period = 1000000,
				.channel = 1,
				.polarity = true,
				.pwm_port = GPIOE,
				.pwm_pin = 6,
				.pwm_pin_cfg = 3,
};

// fb_pdata_t fb_pData;
//framebuffer_t fb_f1c100s;

inline static void f1c100s_debe_set_mode(fb_pdata_t *pdat)
{
	struct f1c100s_debe_reg_t *debe = (struct f1c100s_debe_reg_t *)(pdat->virtdebe);
	uint32_t val;

	val = read32((virtual_addr_t)&debe->mode);
	val |= (1 << 0);
	write32((virtual_addr_t)&debe->mode, val);

	write32((virtual_addr_t)&debe->disp_size, (((pdat->height) - 1) << 16) | (((pdat->width) - 1) << 0));
	write32((virtual_addr_t)&debe->layer0_size, (((pdat->height) - 1) << 16) | (((pdat->width) - 1) << 0));
	write32((virtual_addr_t)&debe->layer0_stride, ((pdat->width) << 5));
	write32((virtual_addr_t)&debe->layer0_addr_low32b, (uint32_t)(pdat->vram[pdat->index]) << 3);
	write32((virtual_addr_t)&debe->layer0_addr_high4b, (uint32_t)(pdat->vram[pdat->index]) >> 29);
	write32((virtual_addr_t)&debe->layer0_attr1_ctrl, 0x09 << 8);

	val = read32((virtual_addr_t)&debe->mode);
	val |= (1 << 8);
	write32((virtual_addr_t)&debe->mode, val);

	val = read32((virtual_addr_t)&debe->reg_ctrl);
	val |= (1 << 0);
	write32((virtual_addr_t)&debe->reg_ctrl, val);

	val = read32((virtual_addr_t)&debe->mode);
	val |= (1 << 1);
	write32((virtual_addr_t)&debe->mode, val);
}

inline static void f1c100s_debe_set_address(fb_pdata_t *pdat, void *vram)
{
	struct f1c100s_debe_reg_t *debe = (struct f1c100s_debe_reg_t *)(pdat->virtdebe);

	write32((virtual_addr_t)&debe->layer0_addr_low32b, (uint32_t)vram << 3);
	write32((virtual_addr_t)&debe->layer0_addr_high4b, (uint32_t)vram >> 29);
}

inline static void f1c100s_tcon_enable(fb_pdata_t *pdat)
{
	struct f1c100s_tcon_reg_t *tcon = (struct f1c100s_tcon_reg_t *)pdat->virttcon;
	uint32_t val;

	val = read32((virtual_addr_t)&tcon->ctrl);
	val |= (1 << 31);
	write32((virtual_addr_t)&tcon->ctrl, val);
}

inline static void f1c100s_tcon_disable(fb_pdata_t *pdat)
{
	struct f1c100s_tcon_reg_t *tcon = (struct f1c100s_tcon_reg_t *)pdat->virttcon;
	uint32_t val;

	write32((virtual_addr_t)&tcon->ctrl, 0);
	write32((virtual_addr_t)&tcon->int0, 0);

	val = read32((virtual_addr_t)&tcon->tcon0_dclk);
	val &= ~(0xf << 28);
	write32((virtual_addr_t)&tcon->tcon0_dclk, val);

	write32((virtual_addr_t)&tcon->tcon0_io_tristate, 0xffffffff);
	write32((virtual_addr_t)&tcon->tcon1_io_tristate, 0xffffffff);
}

inline static uint64_t f1c100s_tcon_clk_get_rate()
{
	return ccu_get_rate_video(24000000);
}

static void clk_mux_set_parent(virtual_addr_t virt, uint32_t width, uint32_t shift, uint32_t parent_val)
{
	uint32_t val;
	val = read32(virt);
	val &= ~(((1 << width) - 1) << shift);
	val |= parent_val << shift;
	write32(virt, val);
}

#define gate_clk_enable(virt, shift, invert) write32(virt, (read32(virt) & ~(0x1 << shift)) | ((invert ? 0x0 : 0x1) << shift))
#define gate_clk_disable(virt, shift, invert) write32(virt, (read32(virt) & ~(0x1 << shift)) | ((invert ? 0x1 : 0x0) << shift))

static void f1c100s_clk_tcon_init(void)
{
	clk_mux_set_parent(0x01c20118, 3, 24, 0);
}

inline static void f1c100s_clk_tcon_enable()
{
	S_Bit(CCU->TCON_CLK_REG, 31);
	ccu_reset(RESET_LCD, true);
}

inline static void f1c100s_clk_tcon_disable()
{
	C_Bit(CCU->TCON_CLK_REG, 31);
	ccu_reset(RESET_LCD, false);
}

static void clk_divider_set_rate(virtual_addr_t virt, uint32_t width, uint32_t shift, bool onebased, uint64_t prate, uint64_t rate)
{
	uint32_t mask = ((1 << (width)) - 1);
	uint32_t div;
	uint32_t val;

	if (rate == 0)
		rate = prate;

	div = prate / rate;
	if (onebased)
		div--;
	if (div > mask)
		div = mask;

	val = read32(virt);
	val &= ~(mask << shift);
	val |= div << shift;
	write32(virt, val);
}

static void f1c100s_clk_defe_init(void)
{
	clk_mux_set_parent(0x01c2010c, 3, 24, 0);
	clk_divider_set_rate(0x01c2010c, 4, 0, true, f1c100s_tcon_clk_get_rate(), 198000000);
}
inline static void f1c100s_clk_defe_enable()
{
	S_Bit(CCU->FE_CLK_REG, 31);
	ccu_reset(RESET_DEFE, true);
}

inline static void f1c100s_clk_defe_disable()
{
	C_Bit(CCU->FE_CLK_REG, 31);
	ccu_reset(RESET_DEFE, false);
}

static void f1c100s_clk_debe_init(void)
{
	clk_mux_set_parent(0x01c20104, 3, 24, 0);
	clk_divider_set_rate(0x01c20104, 4, 0, true, f1c100s_tcon_clk_get_rate(), 198000000);
}

inline static void f1c100s_clk_debe_enable()
{
	S_Bit(CCU->BE_CLK_REG, 31);
	ccu_reset(RESET_DEBE, true);
}

inline static void f1c100s_clk_debe_disable()
{
	C_Bit(CCU->BE_CLK_REG, 31);
	ccu_reset(RESET_DEBE, false);
}

inline static void f1c100s_tcon_set_mode(fb_pdata_t *pdat)
{
	struct f1c100s_tcon_reg_t *tcon = (struct f1c100s_tcon_reg_t *)pdat->virttcon;
	int32_t bp, total;
	uint32_t val;

	val = read32((virtual_addr_t)&tcon->ctrl);
	val &= ~(0x1 << 0);
	write32((virtual_addr_t)&tcon->ctrl, val);

	val = (pdat->timing.v_front_porch + pdat->timing.v_back_porch + pdat->timing.v_sync_len);
	write32((virtual_addr_t)&tcon->tcon0_ctrl, (1 << 31) | ((val & 0x1f) << 4));
	//val = clk_get_rate(pdat->clktcon) / pdat->timing.pixel_clock_hz;
	val = f1c100s_tcon_clk_get_rate() / pdat->timing.pixel_clock_hz;
	write32((virtual_addr_t)&tcon->tcon0_dclk, (0xf << 28) | (val << 0));
	write32((virtual_addr_t)&tcon->tcon0_timing_active, ((pdat->width - 1) << 16) | ((pdat->height - 1) << 0));

	bp = pdat->timing.h_sync_len + pdat->timing.h_back_porch;
	total = pdat->width + pdat->timing.h_front_porch + bp;
	write32((virtual_addr_t)&tcon->tcon0_timing_h, ((total - 1) << 16) | ((bp - 1) << 0));
	bp = pdat->timing.v_sync_len + pdat->timing.v_back_porch;
	total = pdat->height + pdat->timing.v_front_porch + bp;
	write32((virtual_addr_t)&tcon->tcon0_timing_v, ((total * 2) << 16) | ((bp - 1) << 0));
	write32((virtual_addr_t)&tcon->tcon0_timing_sync, ((pdat->timing.h_sync_len - 1) << 16) | ((pdat->timing.v_sync_len - 1) << 0));

	write32((virtual_addr_t)&tcon->tcon0_hv_intf, 0);
	write32((virtual_addr_t)&tcon->tcon0_cpu_intf, 0);

	if (pdat->bits_per_pixel == 18 || pdat->bits_per_pixel == 16)
	{
		write32((virtual_addr_t)&tcon->tcon0_frm_seed[0], 0x11111111);
		write32((virtual_addr_t)&tcon->tcon0_frm_seed[1], 0x11111111);
		write32((virtual_addr_t)&tcon->tcon0_frm_seed[2], 0x11111111);
		write32((virtual_addr_t)&tcon->tcon0_frm_seed[3], 0x11111111);
		write32((virtual_addr_t)&tcon->tcon0_frm_seed[4], 0x11111111);
		write32((virtual_addr_t)&tcon->tcon0_frm_seed[5], 0x11111111);
		write32((virtual_addr_t)&tcon->tcon0_frm_table[0], 0x01010000);
		write32((virtual_addr_t)&tcon->tcon0_frm_table[1], 0x15151111);
		write32((virtual_addr_t)&tcon->tcon0_frm_table[2], 0x57575555);
		write32((virtual_addr_t)&tcon->tcon0_frm_table[3], 0x7f7f7777);
		write32((virtual_addr_t)&tcon->tcon0_frm_ctrl, (pdat->bits_per_pixel == 18) ? ((1 << 31) | (0 << 4)) : ((1 << 31) | (5 << 4)));
	}

	val = (1 << 28);
	if (!pdat->timing.h_sync_active)
		val |= (1 << 25);
	if (!pdat->timing.h_sync_active)
		val |= (1 << 24);
	write32((virtual_addr_t)&tcon->tcon0_io_polarity, val);
	write32((virtual_addr_t)&tcon->tcon0_io_tristate, 0);
}

inline static void fb_cfg_gpios(const GPIO_Type *port, int32_t pin_start, int32_t pin_end, int32_t cfg, gpio_pull_t pull, gpio_drv_t drv)
{
	int32_t pin;
	for (pin = pin_start; pin < pin_end; pin++)
	{
		gpio_set_cfg(port, pin, cfg);
		gpio_set_pull(port, pin, pull);
		gpio_set_drv(port, pin, drv);
	}
}

static void fb_init_hw(fb_pdata_t *pdat)
{
	fb_cfg_gpios(GPIOD, 0, 22, 0x2, GPIO_PULL_NONE, GPIO_DRV_STRONG);

	f1c100s_tcon_disable(pdat);
	f1c100s_debe_set_mode(pdat);
	f1c100s_tcon_set_mode(pdat);
	f1c100s_tcon_enable(pdat);

	pwm_init(pdat->backlight);
}

void fb_setbl(framebuffer_t *fb, int32_t brightness)
{
	fb_pdata_t *pdat = (fb_pdata_t *)fb->priv;
	//led_set_brightness(pdat->backlight, brightness);
	pwm_set_duty(pdat->backlight, brightness);
}

int32_t fb_getbl(framebuffer_t *fb)
{
	fb_pdata_t *pdat = (fb_pdata_t *)fb->priv;
	// return led_get_brightness(pdat->backlight);
	return pwm_get_duty(pdat->backlight);
}

render_t *fb_create(framebuffer_t *fb)
{
	fb_pdata_t *pdat = (fb_pdata_t *)fb->priv;
	render_t *render;
	void *pixels;
	size_t pixlen;

	pixlen = pdat->width * pdat->height * pdat->bytes_per_pixel;
	pixels = memalign(4, pixlen);
	if (!pixels)
		return NULL;

	render = malloc(sizeof(render_t));
	if (!render)
	{
		free(pixels);
		return NULL;
	}

	render->width = pdat->width;
	render->height = pdat->height;
	render->pitch = (pdat->width * pdat->bytes_per_pixel + 0x3) & ~0x3;
	render->format = PIXEL_FORMAT_RGB16_565;
	render->pixels = pixels;
	render->pixlen = pixlen;
	render->priv = NULL;

	return render;
}

void fb_destroy(framebuffer_t *fb, render_t *render)
{
	if (render)
	{
		free(render->pixels);
		free(render);
	}
}

void fb_present(framebuffer_t *fb, render_t *render)
{
	fb_pdata_t *pdat = (fb_pdata_t *)fb->priv;

	if (render && render->pixels)
	{
		pdat->index = (pdat->index + 1) & 0x1;
		memcpy(pdat->vram[pdat->index], render->pixels, render->pixlen);
		// dma_cache_sync(pdat->vram[pdat->index], render->pixlen, DMA_TO_DEVICE);
		// f1c100s_debe_set_address(pdat, pdat->vram[pdat->index]);
	}
}

void fb_area_present(framebuffer_t *fb, uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2, uint32_t *data)
{
	uint16_t x = x2 - x1 + 1;
	fb_pdata_t *pdat = (fb_pdata_t *)fb->priv;
	uint16_t size_of_row = x * sizeof(uint32_t);
	pdat->index = 0;
	for (int i = y1; i < y2 + 1; i++)
	{
		memcpy(&((uint32_t *)pdat->vram[0])[i * pdat->width + x1], data, size_of_row);
		data += x;
	}
	// dma_cache_sync(pdat->vram[0], pdat->width * pdat->height, DMA_TO_DEVICE);
	// f1c100s_debe_set_address(pdat, pdat->vram[0]);
}

void fb_pixel_present(framebuffer_t *fb, uint16_t x, uint16_t y, uint32_t data)
{
	fb_pdata_t *pdat = (fb_pdata_t *)fb->priv;
	((uint32_t *)pdat->vram[0])[x * pdat->width + y] = data;
	// dma_cache_sync(pdat->vram[0], pdat->width * pdat->height, DMA_TO_DEVICE);
	// f1c100s_debe_set_address(pdat, pdat->vram[0]);
}

// "fb-f1c100s@0": {
// 	"clock-name-defe": "link-defe",
// 	"clock-name-debe": "link-debe",
// 	"clock-name-tcon": "link-tcon",
// 	"reset-defe": 46,
// 	"reset-debe": 44,
// 	"reset-tcon": 36,
// 	"width": 800,
// 	"height": 480,
// 	"physical-width": 216,
// 	"physical-height": 135,
// 	"bits-per-pixel": 18,
// 	"bytes-per-pixel": 4,
// 	"clock-frequency": 33000000,
// 	"hfront-porch": 40,
// 	"hback-porch": 87,
// 	"hsync-len": 1,
// 	"vfront-porch": 13,
// 	"vback-porch": 31,
// 	"vsync-len": 1,
// 	"hsync-active": false,
// 	"vsync-active": false,
// 	"den-active": false,
// 	"clk-active": false,
// 	"backlight": "led-pwm-bl.0"
// },
void fb_init(framebuffer_t *fb, int32_t width, int32_t height)
{
	int32_t i;
	fb->priv = malloc(sizeof(fb_pdata_t));
	fb_pdata_t *pdat = fb->priv;

	pdat->virtdefe = phys_to_virt(F1C100S_DEFE_BASE);
	pdat->virtdebe = phys_to_virt(F1C100S_DEBE_BASE);
	pdat->virttcon = phys_to_virt(F1C100S_TCON_BASE);
	pdat->clkdefe = "clkdefe";
	pdat->clkdebe = "clkdebe";
	pdat->clktcon = "clktcon";
	pdat->rstdefe = 46;
	pdat->rstdebe = 44;
	pdat->rsttcon = 36;
	pdat->width = width;
	pdat->height = height;
	pdat->pwidth = 216;
	pdat->pheight = 135;
	pdat->bits_per_pixel = 18;
	pdat->bytes_per_pixel = 4;
	pdat->index = 0;
	// if(buff1)
	// 	pdat->vram[0] = buff1;
	// else
	pdat->vram[0] = memalign(SZ_4K, pdat->width * pdat->height * pdat->bytes_per_pixel);
	// if(buff2)
	// 	pdat->vram[1] = buff2;
	// else
	// 	pdat->vram[1] = memalign(SZ_4K, pdat->width * pdat->height * pdat->bytes_per_pixel);

	pdat->timing.pixel_clock_hz = 33000000;
	pdat->timing.h_front_porch = 40;
	pdat->timing.h_back_porch = 87;
	pdat->timing.h_sync_len = 1;
	pdat->timing.v_front_porch = 13;
	pdat->timing.v_back_porch = 31;
	pdat->timing.v_sync_len = 1;
	pdat->timing.h_sync_active = false;
	pdat->timing.v_sync_active = false;
	pdat->timing.den_active = false;
	pdat->timing.clk_active = false;
	pdat->backlight = &led_pwm_bl;

	fb->name = "fb-f1c100s";
	fb->width = pdat->width;
	fb->height = pdat->height;
	fb->pwidth = pdat->pwidth;
	fb->pheight = pdat->pheight;
	fb->bpp = pdat->bytes_per_pixel * 8;
	fb->priv = pdat;

	ccu_set_enable_video(true);
	f1c100s_clk_debe_init();
	f1c100s_clk_defe_init();
	f1c100s_clk_tcon_init();
	f1c100s_clk_debe_enable();
	f1c100s_clk_defe_enable();
	f1c100s_clk_tcon_enable();
	for (i = 0x0800; i < 0x1000; i += 4)
		write32(pdat->virtdebe + i, 0);
	fb_init_hw(pdat);
}

void fb_remove(framebuffer_t *fb)
{
	fb_pdata_t *pdat = (fb_pdata_t *)fb->priv;
	if (pdat)
	{
		f1c100s_clk_defe_disable();
		f1c100s_clk_debe_disable();
		f1c100s_clk_tcon_disable();
		// free(pdat->clkdefe);
		// free(pdat->clkdebe);
		// free(pdat->clktcon);
		free(pdat->vram[0]);
		// free(pdat->vram[1]);
		free(pdat);

		// free_device_name(fb->name);
		// free(fb->priv);
		// free(fb);
	}
}

void fb_suspend(framebuffer_t *fb)
{
	fb_pdata_t *pdat = (fb_pdata_t *)fb->priv;

	//pdat->brightness = led_get_brightness(pdat->backlight);
	pdat->brightness = pwm_get_duty(pdat->backlight);
	//led_set_brightness(pdat->backlight, 0);
	pwm_set_duty(pdat->backlight, 0);
}

void fb_resume(framebuffer_t *fb)
{
	fb_pdata_t *pdat = (fb_pdata_t *)fb->priv;

	//led_set_brightness(pdat->backlight, pdat->brightness);
	pwm_set_duty(pdat->backlight, pdat->brightness);
}
