#include "Mp3Player.h"
#include "audio_file.h"

Mp3Player mp3Player;

void setup()
{
	mp3Player.begin();
  mp3Player.playBuffer(_acmp3_01, sizeof(_acmp3_01));
}

void loop()
{
  delay(1000);
}
