
#include <nes_main.h>
#include <rom/zdcr.h> //dat bom
#include "FrameBuffer.h"
#include <audio.h>
#include <defe.h>
#include <io.h>

static FrameBuffer framebuffer;

void fb_run(int x, int y, int w, int h, pixel_format *data)
{
	Defe_conversion_buff((u32)data, 0);
}

void setup()
{
	Serial.begin(115200);
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
}

void loop()
{
	nes_loop();
	delay(1);
}