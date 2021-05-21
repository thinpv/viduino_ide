#include "Arduino.h"
#include "InfoNES.h"

extern const unsigned char nes_rom[];

void setup()
{
	InfoNES_Load((unsigned char*)nes_rom);
  InfoNES_Main();
}

void loop()
{
}
