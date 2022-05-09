#include <fb.h>
#include <sys-delay.h>
#include <jpeg.h>
#include <printf.h>
#include "flower.h"
#include "girl.h"
#include <F1C100S.h>
#include <ccu.h>
#include <io.h>

struct jpeg_t jpeg;

void setup()
{
	sys_print_init();

  printf("setup\r\n");
  fb_init(LCD_TYPE_RGB_800_480, NULL);


  // ccu_set_enable_ve();
  CCU->PLL_VE_CTRL_REG = (1u << 31) | (1 << 24) | (12 << 8); // 312MHz
  while (!(CCU->PLL_VE_CTRL_REG & (1 << 28)))
    ;
	S_Bit(CCU->VE_CLK_REG, 31);
	ccu_reset(RESET_VE, true);
  printf("VE Version 0x%X 0x%X\n", VE->VERSION >> 16, VE->CTRL);
  VE->CTRL = 7;
  delay(100);
}

void loop()
{
  printf("loop\r\n");
  if (!parse_jpeg(&jpeg, flower_jpg, sizeof(flower_jpg)))
  {
    printf("Can't parse JPEG");
    while (1)
      ;
  }
  dump_jpeg(&jpeg);
  printf("decode_jpeg\r\n");
  decode_jpeg(&jpeg, fb_get_buffer());
  printf("decode_jpeg done\r\n");
  // fr_enable(fr2);
  delay(1000);
  if (!parse_jpeg(&jpeg, girl_jpg, sizeof(girl_jpg)))
  {
    printf("Can't parse JPEG");
    while (1)
      ;
  }
  decode_jpeg(&jpeg, fb_get_buffer());
  // fr_enable(fr1);
  delay(1000);
}