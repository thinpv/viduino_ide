#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <dma.h>
#include <gpio.h>
#include <uart.h>
#include <timer.h>
#include <irq.h>
#include <irq_gpio.h>
#include <fb.h>
#include <pwm.h>
// #include <usb.h>
#include <io.h>

// #define TEST_PNG_C
// #define TEST_FATFS_C
// #define TEST_LVGL_C
// #define TEST_NES_C
// #define TEST_JPEG_C
// #define TEST_I2C_C

void init(){}
extern void setup();
extern void loop();

#ifdef TEST_PNG_C
#include "diskio.h"
#include "ff.h"
#endif

#ifdef TEST_I2C_C
#include <i2c.h>
#endif

#ifdef TEST_FATFS_C
#include <sdc.h>
#define DATA_SIZE 2048
#include "diskio.h"
#include "ff.h"
u8 work[FF_MAX_SS];
int i;
UINT br;
// #define BUFF_SIZE (1024 * 1024) //读写缓存大小
u8 buff[100] = {0};

#elif defined TEST_LVGL_C
#include <lvgl.h>
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
	// Serial.begin(115200);
	// Serial.println("setup start");

	// framebuffer.begin(LV_HOR_RES_MAX, LV_VER_RES_MAX, 100);
	fb_init(LV_HOR_RES_MAX, LV_VER_RES_MAX);

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

	/* Create simple label */
	lv_obj_t *label = lv_label_create(lv_scr_act(), NULL);
	lv_label_set_text(label, "Hello Arduino! (V7.11.0)");
	lv_obj_align(label, NULL, LV_ALIGN_CENTER, 0, 0);

	/* Create a slider in the center of the display */
	lv_obj_t *slider = lv_slider_create(lv_scr_act(), NULL);
	lv_obj_set_width(slider, LV_HOR_RES_MAX - 50); /*Set the width*/
	lv_obj_set_height(slider, 30);
	lv_obj_align(slider, NULL, LV_ALIGN_CENTER, 0, 0); /*Align to the center of the parent (screen)*/
	lv_obj_set_event_cb(slider, slider_event_cb);			 /*Assign an event function*/

	/* Create a label below the slider */
	slider_label = lv_label_create(lv_scr_act(), NULL);
	lv_label_set_text(slider_label, "0");
	lv_obj_set_auto_realign(slider, true);
	lv_obj_align(slider_label, slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

	// touchscreen.begin();
	// touchscreen.calibrate(LV_HOR_RES_MAX, LV_VER_RES_MAX);

	// Serial.println("setup done");
}
#elif defined TEST_NES_C
#include <io.h>
#include <audio.h>
#include <nes_main.h>
#include <rom/zdcr.h> //dat bom
void fb_run(int x, int y, int w, int h, unsigned int *data)
{
	Defe_conversion_buff((u32)data, 0);
}
#elif defined TEST_JPEG_C
#include <jpegdec_main.h>
#include "test_jpeg1.h" /*480x272-YUV444*/
#include "test_jpeg2.h" /*480x267-YUV422*/
#include "test_jpeg3.h" /*480x272-YUV420*/
#include "test_jpeg4.h" /*1280x720-YUV420*/

static __attribute__((aligned(1024))) unsigned char buff_y[get_yc_len(1280, 720)];
static __attribute__((aligned(1024))) unsigned char buff_c[get_yc_len(1280, 720)];
static __attribute__((aligned(1024))) unsigned char out_argb[1280 * 720 * 4];

unsigned char *jpeg[4] = { //图片指针-图片使用Bin2C.exe转
		test_jpeg1, test_jpeg2, test_jpeg3, test_jpeg4};
int jpeg_len[4] = { //图片大小
		sizeof(test_jpeg1) - 1, sizeof(test_jpeg2) - 1, sizeof(test_jpeg3) - 1, sizeof(test_jpeg4) - 1};
int pos[4][2] = {
		//拼图时4个图片的坐标
		0,
		0,
		XSIZE_PHYS / 2,
		0,
		0,
		YSIZE_PHYS / 2,
		XSIZE_PHYS / 2,
		YSIZE_PHYS / 2,
};
#endif

#ifdef USE_FREERTOS
#include "FreeRTOS.h"
#include "task.h"

static void arduino(void *pvParameters)
{
	maincpp();
}
#endif

int boot_main(int argc, char **argv)
{
	// sys_clock_init();
	// sys_dram_init();
	sys_print_init();

	do_init_mem_pool();
	// do_init_dma_pool();

	// sys_mmu_init();

	sys_init();

	irq_init();
#ifdef USE_FREERTOS
	xTaskCreate(arduino, "arduino", 1024, NULL, tskIDLE_PRIORITY + 2, NULL);
	vTaskStartScheduler();
	for (;;)
	{
		printf("loop\r\n");
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
#else
	timer0_set();

#ifdef TEST_I2C_C
	// gpio_set_cfg(GPIOD, 0, GPIO_FUNC_011);
	// gpio_set_pull(GPIOD, 0, GPIO_PULL_UP);
	// gpio_set_cfg(GPIOD, 12, GPIO_FUNC_011);
	// gpio_set_pull(GPIOD, 12, GPIO_PULL_UP);
	gpio_set_cfg(GPIOE, 11, GPIO_FUNC_011);
	gpio_set_pull(GPIOE, 11, GPIO_PULL_UP);
	gpio_set_cfg(GPIOE, 12, GPIO_FUNC_011);
	gpio_set_pull(GPIOE, 12, GPIO_PULL_UP);
	i2c_init(I2C0, 400000);
	uint8_t data;
	printf("i2c start scan\r\n");
	for (int i = 0; i < 127; i++)
	{
		if (i2c_read(I2C0, i, &data, 1, 1) == 1)
		{
			printf("found i2c addr %d\r\n", i);
		}
	}
#endif

#ifdef TEST_PNG_C
	fb_init(480, 272);
	swap_r_b(0);
	Defe_Init();

	FATFS sdfs;
	DIR dp1;
	FIL fp1;
	FRESULT rs;
	UINT br;
	if (f_mount(&sdfs, "1:", 1) == FR_OK)
	{
		if (f_opendir(&dp1, "1:/z7") != FR_NO_PATH)
		{
			rs = f_open(&fp1, "1:/z7/test.lzma", FA_OPEN_ALWAYS | FA_READ);
			if (rs == FR_OK)
			{
				fileInSize = fp1.obj.objsize;
				printf("file size: %d\r\n", fileInSize);
				bufferFileIn = malloc(fileInSize);
				rs = f_read(&fp1, bufferFileIn, fileInSize, &br);
				if (rs == FR_OK)
				{
					printf("start decode\r\n");
					my_decode();
					// main_7z("1:/z7/test.7z");
				}
			}
		}
		// if (f_opendir(&dp1, "1:/png") != FR_NO_PATH)
		// {
		// 	rs = f_open(&fp1, "1:/png/fb.png", FA_OPEN_ALWAYS | FA_READ);
		// 	if (rs == FR_OK)
		// 	{
		// 		uint8_t *data = malloc(fp1.obj.objsize);
		// 		rs = f_read(&fp1, data, fp1.obj.objsize, &br);
		// 		if (rs == FR_OK)
		// 		{
		// 			printf("f_read ok, size: %d\r\n", fp1.obj.objsize);
		// 			uint8_t *buffer;
		// 			uint32_t size;
		// 			uint32_t w, h;
		// 			// Png_Decode(data, fp1.obj.objsize);
		// 			if (Png_Decode(data, fp1.obj.objsize, 0, &buffer, &size, &w, &h) == 0)
		// 			{
		// 				printf("Png_Decode ok w: %d, h: %d\r\n", w, h);
		// 				Defe_Config_argb8888_to_argb8888(w, h, 480, 272, 0);
		// 				Defe_conversion_buff(buffer, 0);
		// 				Defe_Start();
		// 				S_Bit(DEBE->DEBE_LAY0_ATT_CTRL_REG0, 1);
		// 				// fb_area_present(0, 0, w, h, buffer);
		// 				printf("Png_Decode ok2\r\n");
		// 				delay(5000);
		// 				free(buffer);
		// 				printf("Png_Decode ok3\r\n");
		// 			}
		// 		}
		// 		else
		// 		{
		// 			printf("f_read error %d\r\n", rs);
		// 		}
		// 	}
		// 	else
		// 	{
		// 		printf("f_open error %d\r\n", rs);
		// 	}
		// }
		else
		{
			printf("Path not exists...\r\n");
		}
	}
	else
	{
		printf("f_mount error\r\n");
	}

	// printf("Test_png\r\n");
	// Test_png();
	delay(2000);
#endif
#ifdef TEST_FATFS_C
	// sdc_init(SDC0);
	FATFS sdfs;
	DIR dp1;
	FIL fp1;
	if (f_mount(&sdfs, "1:", 1) == FR_OK)
	{
		printf("sdcard file system open successfully!!!\r\n");
		f_setlabel("1:F1C100S-SD");

		if (f_opendir(&dp1, "1:/test") == FR_NO_PATH)
		{
			f_closedir(&dp1);
			printf("No file path, create..\r\n");
			if (f_mkdir("1:/test") == 0)
			{
				printf("Path created successfully...\r\n");
			}
		}
		else
		{
			printf("Path exists...\r\n");
		}

		i = f_open(&fp1, "1:/test/2018.txt", FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
		if (i == 5)
			printf("Folder path does not exist..\r\n");
		else if (i == 4)
		{
			printf("No file,..\r\n");
			i = f_open(&fp1, "1:/test/2018.txt", FA_CREATE_NEW | FA_WRITE | FA_READ);
		}
		else if (i == 0)
		{
			printf("File opened successfully..\r\n");
			f_lseek(&fp1, 0);
			if (f_read(&fp1, (uint8_t *)buff, 20, &br) == 0)
			{
				buff[20] = '\0';
				printf("buff: %s\r\n", buff);
			}
			printf("------------------------------\r\n");
			f_close(&fp1);
		}
		else
		{
			printf("ERR: f_open: %d\r\n", i);
		}
	}
	else
	{
		printf("ERR:not file system Files!!!\r\n");
		printf("Formatting...\r\n");
		f_mkfs("1:", FM_FAT, 0, work, sizeof work); //need to format
		printf("Format successful\r\n");
	}
	f_setlabel("1:F1C100S-SD");
#elif defined TEST_LVGL_C
	lvgl_test();
	for (;;)
	{
		lv_task_handler();
	}
#elif defined TEST_NES_C
	fb_init(480, 272);
	swap_r_b(0);
	audio_play_wav_init();
	set_video_callback(fb_run);
	set_audio_callback(audio_play_wav);
	Defe_Init();
	Defe_Config_argb8888_to_argb8888(NES_DW, NES_DH, 480, 272, (int)fb_get_buffer());
	Defe_Start();
	S_Bit(DEBE->DEBE_LAY0_ATT_CTRL_REG0, 1);
	nes_load(_aczdcr, sizeof(_aczdcr));
	for (;;)
	{
		nes_loop();
	}

#elif defined TEST_JPEG_C
	int res = 0;
	int Time = 0, i, l;
	int jpeg_w;
	int jpeg_h;
	int argb_w = XSIZE_PHYS;
	int argb_h = YSIZE_PHYS;
	printf("Jpeg_decoder_2\r\n");
	MACC_Init(MACC_MODULE_MPEG_DEC);
	Defe_Init();
	Time = millis();
	res = Jpeg_decoder_2(jpeg[0], jpeg_len[0], &jpeg_w, &jpeg_h, //JPEG输入设置
											 out_argb, argb_w, argb_h,							 //ARGB输出设置
											 buff_y, buff_c, 500);
	printf("jpeg_w: %d, jpeg_h: %d, time: %d\r\n", jpeg_w, jpeg_h, millis() - Time);
	fb_init(480, 272);
	swap_r_b(0);
	Defe_Config_argb8888_to_argb8888(argb_w == 0 ? jpeg_w : argb_w, argb_h == 0 ? jpeg_h : argb_h, 480, 272, 0);
	Defe_conversion_buff(out_argb, 0);
	Defe_Start();
	S_Bit(DEBE->DEBE_LAY0_ATT_CTRL_REG0, 1);
	// LCD_Draw_Picture(0, 0, argb_w == 0 ? jpeg_w : argb_w, argb_h == 0 ? jpeg_h : argb_h, out_argb);
#else
	// maincpp();
	init();		
	
	// USBDevice.attach();
			
	setup();
	
	for (;;)
	{
		loop();		
		// if(serialEventRun) serialEventRun();
	}
#endif
#endif
	while (1)
	{
	}
	return 0;
}

void __fatal_error(const char *msg)
{
	while (1)
	{
	}
}

#ifndef NDEBUG
void __assert_func(const char *file, int line, const char *func, const char *expr)
{
	//printf("Assertion '%s' failed, at file %s:%d\n", expr, file, line);
	__fatal_error("Assertion failed");
}
#endif

void __aeabi_dcmpun()
{
}
void __aeabi_fcmpun() {}