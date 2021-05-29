#include <lvgl.h>
#include <../lv_conf.h>
#include <touchscreen.h>
#include "FrameBuffer.h"

static FrameBuffer framebuffer;
static TouchScreen touchscreen;
static lv_disp_buf_t disp_buf;
static lv_color_t buf[LV_HOR_RES_MAX * 10];
static int temp_touch;

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    framebuffer.areaPresent(area->x1, area->x2, area->y1, area->y2, (int *)color_p);
    lv_disp_flush_ready(disp);
}

// /* Reading input device (simulated encoder here) */
static bool read_encoder(lv_indev_drv_t *drv, lv_indev_data_t *data)
{
    touchscreen.read();
    temp_touch = touchscreen.getStatus();
    data->point.x = touchscreen.getX();
    data->point.y = touchscreen.getY();
    if (temp_touch == TOUCHSCREEN_STATUS_RELEASE)
        data->state = LV_INDEV_STATE_REL;
    else
        data->state = LV_INDEV_STATE_PR;
    Serial.print(data->point.x);
    Serial.print(" - ");
    Serial.print(data->point.y);
    Serial.print(" --> ");
    Serial.println(temp_touch);
    return false; /*No buffering now so no more data read*/
}

static lv_obj_t *slider_label;
static void slider_event_cb(lv_obj_t *slider, lv_event_t event)
{
    if (event == LV_EVENT_VALUE_CHANGED)
    {
        static char buf[4]; /* max 3 bytes for number plus 1 null terminating byte */
        snprintf(buf, 4, "%u", lv_slider_get_value(slider));
        lv_label_set_text(slider_label, buf);
    }
}

void setup()
{
    Serial.begin(115200);
    Serial.println("setup start");

    framebuffer.begin(LV_HOR_RES_MAX, LV_VER_RES_MAX, 100);

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
    indev_drv.read_cb = read_encoder;
    lv_indev_drv_register(&indev_drv);

    /* Create simple label */
    lv_obj_t *label = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(label, "Hello Arduino! (V7.11.0)");
    lv_obj_align(label, NULL, LV_ALIGN_CENTER, 0, 0);

    /* Create a slider in the center of the display */
    lv_obj_t *slider = lv_slider_create(lv_scr_act(), NULL);
    lv_obj_set_width(slider, LV_HOR_RES_MAX - 50); /*Set the width*/
    lv_obj_set_height(slider, 30);
    lv_obj_align(slider, NULL, LV_ALIGN_CENTER, 0, 0); /*Align to the center of the parent (screen)*/
    lv_obj_set_event_cb(slider, slider_event_cb);      /*Assign an event function*/

    /* Create a label below the slider */
    slider_label = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(slider_label, "0");
    lv_obj_set_auto_realign(slider, true);
    lv_obj_align(slider_label, slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

    touchscreen.begin();
    touchscreen.calibrate(LV_HOR_RES_MAX, LV_VER_RES_MAX);

    Serial.println("setup done");
}

void loop()
{
    /* let the GUI do its work */
    lv_task_handler();
}