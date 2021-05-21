#include "Arduino.h"

#include <lvgl.h>
#include <../lv_conf.h>
#include "lv_demo_benchmark.h"
#include "FrameBuffer.h"

static FrameBuffer framebuffer;
static lv_disp_buf_t disp_buf;
static lv_color_t buf[LV_HOR_RES_MAX * 10];
static int temp_touch;

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    framebuffer.displayFlush(area->x1, area->x2, area->y1, area->y2, (int*)color_p);
    lv_disp_flush_ready(disp);
}

void setup()
{
	printf("setup start\r\n");

    lv_init();

    framebuffer.begin(LV_HOR_RES_MAX, LV_VER_RES_MAX, 100);

    lv_disp_buf_init(&disp_buf, buf, NULL, LV_HOR_RES_MAX * 10);

    /*Initialize the display*/
    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = LV_HOR_RES_MAX;
    disp_drv.ver_res = LV_VER_RES_MAX;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.buffer = &disp_buf;
    lv_disp_drv_register(&disp_drv);

	lv_demo_benchmark();
	printf("setup done\r\n");
}

void loop()
{
	 /* let the GUI do its work */
    lv_task_handler();
}
