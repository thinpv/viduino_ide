#ifndef _SOFT_PWM__
#define _SOFT_PWM__

#ifdef __cplusplus
extern "C"
{
#endif

#include <F1C100S.h>

typedef struct _softpwm_node_t
{
	struct _softpwm_node_t *next;
	GPIO_Type *gpio;
	uint16_t pin;
	// uint32_t period; 255
	uint32_t duty;
} softpwm_node_t;

void softpwm_add(GPIO_Type *gpio, uint16_t pin, uint32_t duty);
bool softpwm_remove(GPIO_Type *gpio, uint16_t pin);
void softpwm_update(void);

#ifdef __cplusplus
}
#endif

#endif