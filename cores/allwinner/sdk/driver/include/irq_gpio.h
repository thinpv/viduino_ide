#ifndef __IRQ_GPIO_H__
#define __IRQ_GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <types.h>
#include <irq.h>
#include <F1C100S.h>

enum irq_type_t {
	IRQ_TYPE_EDGE_RISING	= 0,
	IRQ_TYPE_EDGE_FALLING	= 1,
	IRQ_TYPE_LEVEL_HIGH		= 2,
	IRQ_TYPE_LEVEL_LOW		= 3,
	IRQ_TYPE_EDGE_BOTH		= 4,
	IRQ_TYPE_NONE			= 5,
};

void irq_gpio_enable(GPIO_INT_Type * gpio_int, uint16_t offset);
void irq_gpio_disable(GPIO_INT_Type * gpio_int, uint16_t offset);
void irq_gpio_settype(GPIO_INT_Type * gpio_int, uint16_t offset, enum irq_type_t type);
void irq_gpio_dispatch(GPIO_INT_Type * gpio_int, uint16_t offset);

#ifdef __cplusplus
}
#endif

#endif /* __IRQ_GPIO_H__ */
