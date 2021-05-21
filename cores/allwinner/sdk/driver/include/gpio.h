#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <types.h>
#include <F1C100S.h>

#define F1C100S_GPIOA0 (0)
#define F1C100S_GPIOA1 (1)
#define F1C100S_GPIOA2 (2)
#define F1C100S_GPIOA3 (3)

#define F1C100S_GPIOC0 (64)
#define F1C100S_GPIOC1 (65)
#define F1C100S_GPIOC2 (66)
#define F1C100S_GPIOC3 (67)

#define F1C100S_GPIOD0 (96)
#define F1C100S_GPIOD1 (97)
#define F1C100S_GPIOD2 (98)
#define F1C100S_GPIOD3 (99)
#define F1C100S_GPIOD4 (100)
#define F1C100S_GPIOD5 (101)
#define F1C100S_GPIOD6 (102)
#define F1C100S_GPIOD7 (103)
#define F1C100S_GPIOD8 (104)
#define F1C100S_GPIOD9 (105)
#define F1C100S_GPIOD10 (106)
#define F1C100S_GPIOD11 (107)
#define F1C100S_GPIOD12 (108)
#define F1C100S_GPIOD13 (109)
#define F1C100S_GPIOD14 (110)
#define F1C100S_GPIOD15 (111)
#define F1C100S_GPIOD16 (112)
#define F1C100S_GPIOD17 (113)
#define F1C100S_GPIOD18 (114)
#define F1C100S_GPIOD19 (115)
#define F1C100S_GPIOD20 (116)
#define F1C100S_GPIOD21 (117)

#define F1C100S_GPIOE0 (128)
#define F1C100S_GPIOE1 (129)
#define F1C100S_GPIOE2 (130)
#define F1C100S_GPIOE3 (131)
#define F1C100S_GPIOE4 (132)
#define F1C100S_GPIOE5 (133)
#define F1C100S_GPIOE6 (134)
#define F1C100S_GPIOE7 (135)
#define F1C100S_GPIOE8 (136)
#define F1C100S_GPIOE9 (137)
#define F1C100S_GPIOE10 (138)
#define F1C100S_GPIOE11 (139)
#define F1C100S_GPIOE12 (140)

#define F1C100S_GPIOF0 (160)
#define F1C100S_GPIOF1 (161)
#define F1C100S_GPIOF2 (162)
#define F1C100S_GPIOF3 (163)
#define F1C100S_GPIOF4 (164)
#define F1C100S_GPIOF5 (165)

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

	void gpio_set_cfg(GPIO_Type *gpio, uint16_t pin, uint16_t cfg);
	uint16_t gpio_get_cfg(GPIO_Type *gpio, uint16_t pin);
	void gpio_set_pull(GPIO_Type *gpio, uint16_t pin, gpio_pull_t pull);
	gpio_pull_t gpio_get_pull(GPIO_Type *gpio, uint16_t pin);
	void gpio_set_drv(GPIO_Type *gpio, uint16_t pin, gpio_drv_t drv);
	gpio_drv_t gpio_get_drv(GPIO_Type *gpio, uint16_t pin);
	void gpio_set_rate(GPIO_Type *gpio, uint16_t pin, gpio_rate_t rate);
	gpio_rate_t gpio_get_rate(GPIO_Type *gpio, uint16_t pin);
	void gpio_set_dir(GPIO_Type *gpio, uint16_t pin, gpio_direction_t dir);
	gpio_direction_t gpio_get_dir(GPIO_Type *gpio, uint16_t pin);
	void gpio_set_value(GPIO_Type *gpio, uint16_t pin, uint8_t value);
	uint8_t gpio_get_value(GPIO_Type *gpio, uint16_t pin);
	uint16_t gpio_to_irq(GPIO_Type *gpio, uint16_t pin);

#ifdef __cplusplus
}
#endif

#endif
