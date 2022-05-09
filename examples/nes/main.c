#include <fb.h>
#include <io.h>
#include <audio.h>
#include <../../libraries/NES/src/nes_main.h>
#include <../../libraries/NES/src/rom/zdcr.h> //dat bom

void fb_run(int x, int y, int w, int h, unsigned int *data)
{
	Defe_conversion_buff((u32)data, 0);
}

void setup()
{
	sys_print_init();

	fb_init(LCD_TYPE_RGB_480_272, NULL);
	// fb_init(480, 272);
	swap_r_b(0);
	audio_play_wav_init();
	set_video_callback(fb_run);
	set_audio_callback(audio_play_wav);
	Defe_Init();
	Defe_Config_argb8888_to_argb8888(NES_DW, NES_DH, 480, 272, (int)fb_get_buffer());
	Defe_Start();
	S_Bit(DEBE->DEBE_LAY0_ATT_CTRL_REG0, 1);
	nes_load(_aczdcr, sizeof(_aczdcr));
}

void loop()
{
  nes_loop();
}