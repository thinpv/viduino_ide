#ifndef __FB_H__
#define __FB_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <types.h>

	typedef enum
	{
		PIXEL_FORMAT_ARGB32 = 0,
		PIXEL_FORMAT_RGB24 = 1,
		PIXEL_FORMAT_A8 = 2,
		PIXEL_FORMAT_A1 = 3,
		PIXEL_FORMAT_RGB16_565 = 4,
		PIXEL_FORMAT_RGB30 = 5,
	} pixel_format_t;

	typedef struct
	{
		uint8_t b;
		uint8_t g;
		uint8_t r;
		uint8_t a;
	} color_format_t;

	typedef struct render_st
	{
		/* The width of render */
		uint32_t width;

		/* The height of render */
		uint32_t height;

		/* The pitch of one scan line */
		uint32_t pitch;

		/* Pixel format */
		pixel_format_t format;

		/* Pixel data */
		void *pixels;

		/* Pixel data length */
		size_t pixlen;

		/* Private data */
		void *priv;
	} render_t;

	typedef struct framebuffer_st
	{
		/* Framebuffer name */
		char *name;

		/* The width and height in pixel */
		int32_t width, height;

		/* The physical size in millimeter */
		int32_t pwidth, pheight;

		/* The bit per pixel */
		int32_t bpp;

		/* Alone render - create by register */
		render_t *alone;

		/* Private data */
		void *priv;
	} framebuffer_t;

	void fb_setbl(framebuffer_t *fb, int32_t brightness);
	int32_t fb_getbl(framebuffer_t *fb);

	render_t *fb_create(framebuffer_t *fb);

	void fb_destroy(framebuffer_t *fb, render_t *render);

	void fb_present(framebuffer_t *fb, render_t *render);
	void fb_area_present(framebuffer_t *fb, uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2, uint32_t *data);
	void fb_pixel_present(framebuffer_t *fb, uint16_t x, uint16_t y, uint32_t data);

	void fb_init(framebuffer_t *fb, int32_t width, int32_t height);

	void fb_remove(framebuffer_t *fb);

	void fb_suspend(framebuffer_t *fb);

	void fb_resume(framebuffer_t *fb);

#ifdef __cplusplus
}
#endif

#endif /* __FB_H__ */
