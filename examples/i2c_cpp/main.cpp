#include <Arduino.h>
#include <Wire.h>

void setup()
{
	Serial.begin(115200);
	Wire.begin();
	Wire.scan(Serial);
}

void loop()
{
	delay(100);
}