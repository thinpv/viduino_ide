#include <gpio.h>
#include <nes_key.h>

#define STROBE_PIN 2
#define CLOCK_PIN 3
#define DATA1_PIN 4
#define DATA2_PIN 5

void nes_key_init(void)
{
  gpio_set_dir(GPIOE, STROBE_PIN, GPIO_DIRECTION_OUTPUT);
  gpio_set_dir(GPIOE, CLOCK_PIN, GPIO_DIRECTION_OUTPUT);
  gpio_set_dir(GPIOE, DATA1_PIN, GPIO_DIRECTION_INPUT);
  gpio_set_pull(GPIOE, DATA1_PIN, GPIO_PULL_UP);
  gpio_set_dir(GPIOE, DATA2_PIN, GPIO_DIRECTION_INPUT);
  gpio_set_pull(GPIOE, DATA2_PIN, GPIO_PULL_UP);
}

void nes_key_strobe(void)
{
  gpio_set_value(GPIOE, STROBE_PIN, 1);
  delayMicroseconds(12);
  gpio_set_value(GPIOE, STROBE_PIN, 0);
}

uint8_t nes_key_shiftin(uint8_t *shift1, uint8_t *shift2)
{
  *shift1 = gpio_get_value(GPIOE, DATA1_PIN);
  *shift2 = gpio_get_value(GPIOE, DATA2_PIN);
  delayMicroseconds(12);
  gpio_set_value(GPIOE, CLOCK_PIN, 1);
  delayMicroseconds(12);
  gpio_set_value(GPIOE, CLOCK_PIN, 0);
  return 0;
}

uint8_t nes_get_key(uint8_t *key1, uint8_t *key2)
{
  uint16_t i;
  uint8_t shift1;
  uint8_t shift2;
  uint8_t ret1 = 0;
  uint8_t ret2 = 0;
  nes_key_strobe();
  for (i = 0; i < 8; i++)
  {
    nes_key_shiftin(&shift1, &shift2);
    ret1 |= shift1 << i;
    ret2 |= shift2 << i;
  }
  *key1 = ~ret1;
  *key2 = ~ret2;
  return 0;
}