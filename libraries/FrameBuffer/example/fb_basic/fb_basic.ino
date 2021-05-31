#include "FrameBuffer.h"

#define LCD_WIDTH 800
#define LCD_HIGH 480

static FrameBuffer framebuffer;
static int i, j, k;
static color_format_t color;

void setup()
{
  Serial.begin(115200);
  framebuffer.begin(LCD_WIDTH, LCD_HIGH, 100);
  Serial.println("setup done");
}

void loop()
{
  Serial.println("a");
  for (k = 0; k < 32; k++)
  {
    for (i = 0; i < LCD_WIDTH; i++)
    {
      for (j = 0; j < LCD_HIGH; j++)
      {
        framebuffer.pixelPresent(i, j, *((uint32_t*)&color));
      }
    }
    color.a += 8;
    delay(20);
  }
  Serial.println("r");
  for (k = 0; k < 32; k++)
  {
    for (i = 0; i < LCD_WIDTH; i++)
    {
      for (j = 0; j < LCD_HIGH; j++)
      {
        framebuffer.pixelPresent(i, j, *((uint32_t*)&color));
      }
    }
    color.r += 8;
    delay(20);
  }
  Serial.println("g");
  for (k = 0; k < 32; k++)
  {
    for (i = 0; i < LCD_WIDTH; i++)
    {
      for (j = 0; j < LCD_HIGH; j++)
      {
        framebuffer.pixelPresent(i, j, *((uint32_t*)&color));
      }
    }
    color.g += 8;
    delay(20);
  }
  Serial.println("b");
  for (k = 0; k < 32; k++)
  {
    for (i = 0; i < LCD_WIDTH; i++)
    {
      for (j = 0; j < LCD_HIGH; j++)
      {
        framebuffer.pixelPresent(i, j, *((uint32_t*)&color));
      }
    }
    color.b += 8;
    delay(20);
  }
}