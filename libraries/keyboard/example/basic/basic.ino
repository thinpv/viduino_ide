#include "KeyBoard.h"

static KeyBoard keyboard;

void setup()
{
	keyboard.begin();
}

void loop()
{
  keyboard.write(0x04); //'a'
  delay(5000);
}
