#include "st7701.h"
#include <gpio.h>

static inline void cs_high(void)
{
  gpio_set_value(CS_PORT, CS_PIN, 1);
}

static inline void cs_low(void)
{
  gpio_set_value(CS_PORT, CS_PIN, 0);
}

static inline void sck_high(void)
{
  gpio_set_value(SCK_PORT, SCK_PIN, 1);
}

static inline void sck_low(void)
{
  gpio_set_value(SCK_PORT, SCK_PIN, 0);
}

static inline void sda_high(void)
{
  gpio_set_value(SDA_PORT, SDA_PIN, 1);
}

static inline void sda_low(void)
{
  gpio_set_value(SDA_PORT, SDA_PIN, 0);
}

static void send_command(uint8_t c)
{
  cs_low();

  // D/C bit, command
  sda_low();
  sck_high();
  sck_low();

  uint8_t bit = 0x80;
  while (bit)
  {
    if (c & bit)
    {
      sda_high();
    }
    else
    {
      sda_low();
    }
    sck_high();
    bit >>= 1;
    sck_low();
  }

  cs_high();
}

static void send_data(uint8_t d)
{
  cs_low();

  // D/C bit, data
  sda_high();
  sck_high();
  sck_low();

  uint8_t bit = 0x80;
  while (bit)
  {
    if (d & bit)
    {
      sda_high();
    }
    else
    {
      sda_low();
    }
    sck_high();
    bit >>= 1;
    sck_low();
  }

  cs_high();
}

void st7701_init(void)
{
  gpio_set_dir(CS_PORT, CS_PIN, GPIO_DIRECTION_OUTPUT);
  gpio_set_drv(CS_PORT, CS_PIN, GPIO_DRV_STRONG);
  cs_high(); // Deselect
  gpio_set_dir(SCK_PORT, SCK_PIN, GPIO_DIRECTION_OUTPUT);
  gpio_set_drv(SCK_PORT, SCK_PIN, GPIO_DRV_STRONG);
  sck_low();
  gpio_set_dir(SDA_PORT, SDA_PIN, GPIO_DIRECTION_OUTPUT);
  gpio_set_drv(SDA_PORT, SDA_PIN, GPIO_DRV_STRONG);
  sda_low();

#if 1
  send_command(0xFF);
  send_data(0x77);
  send_data(0x01);
  send_data(0x00);
  send_data(0x00);
  send_data(0x10);

  send_command(0xC0);
  send_data(0x3B);
  send_data(0x00);

  send_command(0xC1);
  send_data(0x0D);
  send_data(0x02);

  send_command(0xC2);
  send_data(0x31);
  send_data(0x05);

  send_command(0xCd);
  send_data(0x08);

  send_command(0xB0);
  send_data(0x00); // Positive Voltage Gamma Control
  send_data(0x11);
  send_data(0x18);
  send_data(0x0E);
  send_data(0x11);
  send_data(0x06);
  send_data(0x07);
  send_data(0x08);
  send_data(0x07);
  send_data(0x22);
  send_data(0x04);
  send_data(0x12);
  send_data(0x0F);
  send_data(0xAA);
  send_data(0x31);
  send_data(0x18);

  send_command(0xB1);
  send_data(0x00); // Negative Voltage Gamma Control
  send_data(0x11);
  send_data(0x19);
  send_data(0x0E);
  send_data(0x12);
  send_data(0x07);
  send_data(0x08);
  send_data(0x08);
  send_data(0x08);
  send_data(0x22);
  send_data(0x04);
  send_data(0x11);
  send_data(0x11);
  send_data(0xA9);
  send_data(0x32);
  send_data(0x18);

  // PAGE1
  send_command(0xFF);
  send_data(0x77);
  send_data(0x01);
  send_data(0x00);
  send_data(0x00);
  send_data(0x11);

  send_command(0xB0);
  send_data(0x60); // Vop=4.7375v
  send_command(0xB1);
  send_data(0x32); // VCOM=32
  send_command(0xB2);
  send_data(0x07); // VGH=15v
  send_command(0xB3);
  send_data(0x80);
  send_command(0xB5);
  send_data(0x49); // VGL=-10.17v
  send_command(0xB7);
  send_data(0x85);
  send_command(0xB8);
  send_data(0x21); // AVDD=6.6 & AVCL=-4.6
  send_command(0xC1);
  send_data(0x78);
  send_command(0xC2);
  send_data(0x78);

  send_command(0xE0);
  send_data(0x00);
  send_data(0x1B);
  send_data(0x02);

  send_command(0xE1);
  send_data(0x08);
  send_data(0xA0);
  send_data(0x00);
  send_data(0x00);
  send_data(0x07);
  send_data(0xA0);
  send_data(0x00);
  send_data(0x00);
  send_data(0x00);
  send_data(0x44);
  send_data(0x44);

  send_command(0xE2);
  send_data(0x11);
  send_data(0x11);
  send_data(0x44);
  send_data(0x44);
  send_data(0xED);
  send_data(0xA0);
  send_data(0x00);
  send_data(0x00);
  send_data(0xEC);
  send_data(0xA0);
  send_data(0x00);
  send_data(0x00);

  send_command(0xE3);
  send_data(0x00);
  send_data(0x00);
  send_data(0x11);
  send_data(0x11);

  send_command(0xE4);
  send_data(0x44);
  send_data(0x44);

  send_command(0xE5);
  send_data(0x0A);
  send_data(0xE9);
  send_data(0xD8);
  send_data(0xA0);
  send_data(0x0C);
  send_data(0xEB);
  send_data(0xD8);
  send_data(0xA0);
  send_data(0x0E);
  send_data(0xED);
  send_data(0xD8);
  send_data(0xA0);
  send_data(0x10);
  send_data(0xEF);
  send_data(0xD8);
  send_data(0xA0);

  send_command(0xE6);
  send_data(0x00);
  send_data(0x00);
  send_data(0x11);
  send_data(0x11);

  send_command(0xE7);
  send_data(0x44);
  send_data(0x44);

  send_command(0xE8);
  send_data(0x09);
  send_data(0xE8);
  send_data(0xD8);
  send_data(0xA0);
  send_data(0x0B);
  send_data(0xEA);
  send_data(0xD8);
  send_data(0xA0);
  send_data(0x0D);
  send_data(0xEC);
  send_data(0xD8);
  send_data(0xA0);
  send_data(0x0F);
  send_data(0xEE);
  send_data(0xD8);
  send_data(0xA0);

  send_command(0xEB);
  send_data(0x02);
  send_data(0x00);
  send_data(0xE4);
  send_data(0xE4);
  send_data(0x88);
  send_data(0x00);
  send_data(0x40);

  send_command(0xEC);
  send_data(0x3C);
  send_data(0x00);

  send_command(0xED);
  send_data(0xAB);
  send_data(0x89);
  send_data(0x76);
  send_data(0x54);
  send_data(0x02);
  send_data(0xFF);
  send_data(0xFF);
  send_data(0xFF);
  send_data(0xFF);
  send_data(0xFF);
  send_data(0xFF);
  send_data(0x20);
  send_data(0x45);
  send_data(0x67);
  send_data(0x98);
  send_data(0xBA);

  send_command(0x36);
  send_data(0x00);

  //-----------VAP & VAN---------------
  send_command(0xFF);
  send_data(0x77);
  send_data(0x01);
  send_data(0x00);
  send_data(0x00);
  send_data(0x13);

  send_command(0xE5);
  send_data(0xE4);

  send_command(0xFF);
  send_data(0x77);
  send_data(0x01);
  send_data(0x00);
  send_data(0x00);
  send_data(0x00);

  send_command(0x3A); // 0x70 RGB888, 0x60 RGB666, 0x50 RGB565
  send_data(0x50);

  send_command(0x21); // Display Inversion On

  send_command(0x11); // Sleep Out
  delay(100);

  send_command(0x29); // Display On
  delay(50);

#if 1 //test
  send_command(0xFF);
  send_data(0x77);
  send_data(0x01);
  send_data(0x00);
  send_data(0x00);
  send_data(0x12);
  send_command(0xD1);
  send_data(0x81);
  send_data(0x08);
  send_data(0x03);
  send_data(0x20);
  send_data(0x08);
  send_data(0x01);
  send_data(0xA0);
  send_data(0x01);
  send_data(0xE0);
  send_data(0xA0);
  send_data(0x01);
  send_data(0xE0);
  send_data(0x03);
  send_data(0x20);
  send_command(0xD2);
  send_data(0x08);
#endif
#endif
}