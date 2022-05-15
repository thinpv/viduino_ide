#include <fb.h>
#include <lvgl.h>
#include "src/lv_demo_music.h"
#include <sys-print.h>
#include "st7701.h"

static lv_disp_buf_t disp_buf;
static lv_color_t buf[LV_HOR_RES_MAX * 10];
static lv_obj_t *slider_label;

void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
	fb_area_present(area->x1, area->y1, area->x2 - area->x1 + 1, area->y2 - area->y1 + 1, (unsigned short *)color_p);
	lv_disp_flush_ready(disp);
}

static void slider_event_cb(lv_obj_t *slider, lv_event_t event)
{
	if (event == LV_EVENT_VALUE_CHANGED)
	{
		static char buf[4]; /* max 3 bytes for number plus 1 null terminating byte */
		snprintf(buf, 4, "%u", lv_slider_get_value(slider));
		lv_label_set_text(slider_label, buf);
	}
}

void lvgl_test()
{
	fb_init(LCD_TYPE_RGB_480_272, NULL);

	lv_init();
	lv_disp_buf_init(&disp_buf, buf, NULL, LV_HOR_RES_MAX * 10);

	/*Initialize the display*/
	lv_disp_drv_t disp_drv;
	lv_disp_drv_init(&disp_drv);
	disp_drv.hor_res = LV_HOR_RES_MAX;
	disp_drv.ver_res = LV_VER_RES_MAX;
	disp_drv.flush_cb = my_disp_flush;
	disp_drv.buffer = &disp_buf;
	lv_disp_drv_register(&disp_drv);

	/*Initialize the (dummy) input device driver*/
	lv_indev_drv_t indev_drv;
	lv_indev_drv_init(&indev_drv);
	indev_drv.type = LV_INDEV_TYPE_POINTER;
	// indev_drv.read_cb = read_encoder;
	lv_indev_drv_register(&indev_drv);

	lv_demo_music();
}

void setup()
{
	sys_print_init();

	printf("st7701_init start\r\n");
	st7701_init();
	printf("st7701_init done\r\n");

	lvgl_test();
}

void loop()
{
  lv_task_handler();
}