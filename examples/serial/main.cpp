#include <Arduino.h>

void setup()
{
  Serial.begin(115200);
  Serial.printf("serial test\r\n");
}

void loop()
{
  Serial.printf("loop\r\n");
  delay(1000);
}