
#include <nes_main.h>
#include <rom/zdcr.h> //dat bom
#include "FrameBuffer.h"
#include <audio.h>

static FrameBuffer framebuffer;

void fb_run(int x, int y, int w, int h, pixel_format *data)
{
	framebuffer.areaPresent(x, y, w, h, data);
}

void setup()
{
	Serial.begin(115200);
	framebuffer.begin(480, 272, 100);
	Serial.println("audio_play_wav_init");
	audio_play_wav_init();
	set_video_callback(fb_run);
	set_audio_callback(audio_play_wav);
	nes_load(_aczdcr, sizeof(_aczdcr));
}

void loop()
{
	nes_loop();
	delay(1);
}