#include <Arduino.h>

static void blinkThread(void *pvParameters)
{
	(void)pvParameters;
	pinMode(LED_BUILTIN, OUTPUT);
	for (;;)
	{
		digitalWrite(LED_BUILTIN, HIGH);
		delay(100);
		digitalWrite(LED_BUILTIN, LOW);
		delay(100);
	}
}

static void serialThread(void *pvParameters)
{
	(void)pvParameters;
	Serial.begin(115200);
	for (;;)
	{
		Serial.printf("loop\r\n");
		delay(1000);
	}
}

void setup()
{
	xTaskCreate(blinkThread, "blinkThread", 1024, NULL, 3, NULL);
	xTaskCreate(serialThread, "serialThread", 1024, NULL, 3, NULL);
}

void loop()
{
	delay(1000);
}