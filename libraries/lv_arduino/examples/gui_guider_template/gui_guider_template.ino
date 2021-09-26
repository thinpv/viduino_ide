
#include "lvgl.h"
extern "C"
{
#include "src/custom/custom.h"
#include "src/generated/events_init.h"
}
#include "src/generated/gui_guider.h"

#include "Arduino.h"
#include "FrameBuffer.h"

#define CAPACITOR_TOUCH
//#define RESISTIVE_TOUCH

#ifdef CAPACITOR_TOUCH
#define INT_PIN 6
#define RST_PIN 13

#include <Wire.h>
#include "Goodix.h"
#endif

#ifdef RESISTIVE_TOUCH
#include <touchscreen.h>
#endif

#ifdef CAPACITOR_TOUCH
Goodix touch = Goodix();
char buffer[200];
GTPoint gtPoint;
bool isUpdate = false;
#endif

#ifdef RESISTIVE_TOUCH
static TouchScreen touchscreen;
static int temp_touch;
#endif
static FrameBuffer framebuffer;
static lv_disp_buf_t disp_buf;
static lv_color_t buf[LV_HOR_RES_MAX * 10];
lv_ui guider_ui;

void plc_send(uint8_t *buff, uint8_t len)
{
	Serial1.write(buff, len);
}

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
	framebuffer.areaPresent(area->x1, area->y1, area->x2 - area->x1 + 1, area->y2 - area->y1 + 1, (pixel_format *)color_p);
	lv_disp_flush_ready(disp);
}

/* Reading input device (simulated encoder here) */
static bool read_encoder(lv_indev_drv_t *drv, lv_indev_data_t *data)
{
#ifdef CAPACITOR_TOUCH
	int rs = touch.OnePiontScan(&gtPoint);
	if (rs > 0)
	{
		data->point.x = gtPoint.x;
		data->point.y = gtPoint.y;
		data->state = LV_INDEV_STATE_PR;
		Serial.print(data->point.x);
		Serial.print(" - ");
		Serial.println(data->point.y);
	}
	else
	{
		data->state = LV_INDEV_STATE_REL;
	}
#endif

#ifdef RESISTIVE_TOUCH
	touchscreen.read();
	temp_touch = touchscreen.getStatus();
	data->point.x = touchscreen.getX();
	data->point.y = touchscreen.getY();
	if (temp_touch == TOUCHSCREEN_STATUS_RELEASE)
		data->state = LV_INDEV_STATE_REL;
	else
		data->state = LV_INDEV_STATE_PR;
	if (temp_touch > 1)
	{
		Serial.print(data->point.x);
		Serial.print(" - ");
		Serial.print(data->point.y);
		Serial.print(" --> ");
		Serial.println(temp_touch);
	}
#endif
	return false;
}

void setup()
{
	Serial.begin(115200);
	Serial.println("setup start");

	lv_init();

	framebuffer.begin();

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

	setup_ui(&guider_ui);
	events_init(&guider_ui);
	custom_init(&guider_ui);

#ifdef CAPACITOR_TOUCH
	Wire.begin(400000);
	if (touch.begin(INT_PIN, RST_PIN) != true)
	{
		Serial.println("Touch init failed");
	}
	Wire.beginTransmission(touch.i2cAddr);
	int error = Wire.endTransmission();
	if (error != 0)
	{
		Serial.print(": ERROR #");
		Serial.println(error);
	}
	touch.fwResolution(LV_HOR_RES_MAX, LV_VER_RES_MAX);
#endif

#ifdef RESISTIVE_TOUCH
	touchscreen.begin();
	touchscreen.calibrate(LV_HOR_RES_MAX, LV_VER_RES_MAX);
#endif

	Serial.println("setup done");
}

void loop()
{
	/* let the GUI do its work */
	lv_task_handler();
}
