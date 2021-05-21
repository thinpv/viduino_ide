#include "Arduino.h"
#include "FreeRTOS.h"
#include "task.h"

#if 1

#include <lvgl.h>
#include <../lv_conf.h>
#include <touchscreen.h>

static TouchScreen touchscreen;
static framebuffer_t fb_f1c100s;
static lv_disp_buf_t disp_buf;
static lv_color_t buf[LV_HOR_RES_MAX * 10];
static int temp_touch;

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
	fb_area_present(&fb_f1c100s, area->x1, area->x2, area->y1, area->y2, (uint32_t*)color_p);
    lv_disp_flush_ready(disp);
}

// /* Reading input device (simulated encoder here) */
static bool read_encoder(lv_indev_drv_t * drv, lv_indev_data_t*data)
{
	touchscreen.read();
	temp_touch = touchscreen.getStatus();
	data->point.x = touchscreen.getX();
	data->point.y = touchscreen.getY();
	if(temp_touch == TOUCHSCREEN_STATUS_RELEASE)
		data->state = LV_INDEV_STATE_REL;
	else
		data->state = LV_INDEV_STATE_PR;
	printf("%d -- %d --> %d\r\n", data->point.x, data->point.y, temp_touch);
    return false; /*No buffering now so no more data read*/
}

void setup()
{
	printf("setup start\r\n");

    lv_init();

	fb_init(&fb_f1c100s, LV_HOR_RES_MAX, LV_VER_RES_MAX);
	fb_setbl(&fb_f1c100s, 100);

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
    indev_drv.read_cb = read_encoder;
    lv_indev_drv_register(&indev_drv);

	// lv_demo_benchmark();
	// lv_demo_printer();
	// lv_demo_widgets();

    /* Create simple label */
    lv_obj_t *label = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(label, "Hello Arduino! (V7.0.X)");
    lv_obj_align(label, NULL, LV_ALIGN_CENTER, 0, 0);

    /* Create a slider in the center of the display */
    lv_obj_t * slider = lv_slider_create(lv_scr_act(), NULL);
    lv_obj_set_width(slider, 750);                        /*Set the width*/
    lv_obj_set_height(slider, 50);
    lv_obj_align(slider, NULL, LV_ALIGN_CENTER, 0, 0);    /*Align to the center of the parent (screen)*/
    // lv_obj_set_event_cb(slider, slider_event_cb);         /*Assign an event function*/

    /* Create a label below the slider */
    lv_obj_t *slider_label = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(slider_label, "0");
    lv_obj_set_auto_realign(slider, true);
    lv_obj_align(slider_label, slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

	touchscreen.begin();
    touchscreen.calibrate(LV_HOR_RES_MAX, LV_VER_RES_MAX);

	printf("setup done\r\n");
}

void loop()
{
	printf("loop\r\n");
	 /* let the GUI do its work */
    lv_task_handler();
}

#else
// #include <nofrendo.h>

#include <usb.h>
#include <usb_keyboard.h>

void setup()
{
    printf("setup\r\n");

    // nofrendo_main(0, NULL);

    // set_usb_descriptor_request_func(usb_hid_descriptor_request);
    // usb_device_init(USB_TYPE_USB_HID);
}

void loop()
{
    printf("loop\r\n");
    vTaskDelay( 1000/portTICK_PERIOD_MS );
}

#endif

