#include "Wire.h"

void setup()
{
  Serial.begin(115200);
  Serial.println("setup");
	Wire.begin();
	Wire.scan(Serial);
}

void loop()
{

}