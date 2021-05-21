
#include <nes_main.h>
#include <rom/zdcr.h> //dat bom
#include "FrameBuffer.h"
#include <audio.h>

static FrameBuffer framebuffer;

void setup()
{
  framebuffer.begin(800, 480, 100);
  printf("audio_play_wav_init\r\n");
  audio_play_wav_init();
  nes_load_for_ram(_aczdcr,sizeof(_aczdcr));
}

void loop()
{
  delay(5000);
}