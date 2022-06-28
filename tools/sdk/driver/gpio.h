#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <types.h>
#include <F1C100S.h>

#define GPIOA0 (0)
#define GPIOA1 (1)
#define GPIOA2 (2)
#define GPIOA3 (3)

#define GPIOC0 (64)
#define GPIOC1 (65)
#define GPIOC2 (66)
#define GPIOC3 (67)

#define GPIOD0 (96)
#define GPIOD1 (97)
#define GPIOD2 (98)
#define GPIOD3 (99)
#define GPIOD4 (100)
#define GPIOD5 (101)
#define GPIOD6 (102)
#define GPIOD7 (103)
#define GPIOD8 (104)
#define GPIOD9 (105)
#define GPIOD10 (106)
#define GPIOD11 (107)
#define GPIOD12 (108)
#define GPIOD13 (109)
#define GPIOD14 (110)
#define GPIOD15 (111)
#define GPIOD16 (112)
#define GPIOD17 (113)
#define GPIOD18 (114)
#define GPIOD19 (115)
#define GPIOD20 (116)
#define GPIOD21 (117)

#define GPIOE0 (128)
#define GPIOE1 (129)
#define GPIOE2 (130)
#define GPIOE3 (131)
#define GPIOE4 (132)
#define GPIOE5 (133)
#define GPIOE6 (134)
#define GPIOE7 (135)
#define GPIOE8 (136)
#define GPIOE9 (137)
#define GPIOE10 (138)
#define GPIOE11 (139)
#define GPIOE12 (140)

#define GPIOF0 (160)
#define GPIOF1 (161)
#define GPIOF2 (162)
#define GPIOF3 (163)
#define GPIOF4 (164)
#define GPIOF5 (165)

	typedef enum
	{
		GPIO_FUNC_000 = 0,
		GPIO_FUNC_001,
		GPIO_FUNC_010,
		GPIO_FUNC_011,
		GPIO_FUNC_100,
		GPIO_FUNC_101,
		GPIO_FUNC_110,
		GPIO_FUNC_111
	} gpio_func_t;

	typedef enum
	{
		GPIO_PULL_NONE = 0,
		GPIO_PULL_UP = 1,
		GPIO_PULL_DOWN = 2,
		GPIO_PULL_DIS = 3,
	} gpio_pull_t;

	typedef enum
	{
		GPIO_DRV_WEAK = 0,
		GPIO_DRV_WEAKER = 1,
		GPIO_DRV_STRONGER = 2,
		GPIO_DRV_STRONG = 3,
		GPIO_DRV_NONE = 4,
	} gpio_drv_t;

	typedef enum
	{
		GPIO_RATE_SLOW = 0,
		GPIO_RATE_FAST = 1,
	} gpio_rate_t;

	typedef enum
	{
		GPIO_DIRECTION_INPUT = 0,
		GPIO_DIRECTION_OUTPUT = 1,
		GPIO_DIRECTION_NONE = 2,
	} gpio_direction_t;

	void gpio_set_cfg(GPIO_Type *gpio, int pin, uint16_t cfg);
	uint16_t gpio_get_cfg(GPIO_Type *gpio, int pin);
	void gpio_set_pull(GPIO_Type *gpio, int pin, gpio_pull_t pull);
	gpio_pull_t gpio_get_pull(GPIO_Type *gpio, int pin);
	void gpio_set_drv(GPIO_Type *gpio, int pin, gpio_drv_t drv);
	gpio_drv_t gpio_get_drv(GPIO_Type *gpio, int pin);
	void gpio_set_rate(GPIO_Type *gpio, int pin, gpio_rate_t rate);
	gpio_rate_t gpio_get_rate(GPIO_Type *gpio, int pin);
	void gpio_set_dir(GPIO_Type *gpio, int pin, gpio_direction_t dir);
	gpio_direction_t gpio_get_dir(GPIO_Type *gpio, int pin);
	void gpio_set_value(GPIO_Type *gpio, int pin, uint8_t value);
	uint8_t gpio_get_value(GPIO_Type *gpio, int pin);
	uint16_t gpio_to_irq(GPIO_Type *gpio, int pin);

	void gpio_set_cfg_pn(int pin, uint16_t cfg);
	uint16_t gpio_get_cfg_pn(int pin);
	void gpio_set_pull_pn(int pin, gpio_pull_t pull);
	gpio_pull_t gpio_get_pull_pn(int pin);
	void gpio_set_drv_pn(int pin, gpio_drv_t drv);
	gpio_drv_t gpio_get_drv_pn(int pin);
	void gpio_set_rate_pn(int pin, gpio_rate_t rate);
	gpio_rate_t gpio_get_rate_pn(int pin);
	void gpio_set_dir_pn(int pin, gpio_direction_t dir);
	gpio_direction_t gpio_get_dir_pn(int pin);
	void gpio_set_value_pn(int pin, uint8_t value);
	uint8_t gpio_get_value_pn(int pin);

#ifdef __cplusplus
}
#endif

#endif
