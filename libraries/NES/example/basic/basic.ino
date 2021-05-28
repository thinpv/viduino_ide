
#include <nes_main.h>
#include <rom/zdcr.h> //dat bom
#include "FrameBuffer.h"
#include <audio.h>

static FrameBuffer framebuffer;

void setup()
{
  Serial.begin(115200);
  framebuffer.begin(800, 480, 100);
  Serial.println("audio_play_wav_init");
  audio_play_wav_init();
  nes_load_for_ram(_aczdcr,sizeof(_aczdcr));
}

void loop()
{
  delay(5000);
}