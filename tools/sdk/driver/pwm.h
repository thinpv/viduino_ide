#ifndef __PWM_H__
#define __PWM_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <types.h>
#include <F1C100S.h>

	typedef struct
	{
		virtual_addr_t virt;
		char *clk;
		int32_t duty;
		int32_t period;
		int32_t channel;
		int32_t polarity;
		GPIO_Type *pwm_port;
		int32_t pwm_pin;
		int32_t pwm_pin_cfg;
	} pwm_t;

	int32_t pwm_init(pwm_t *pwm);
	void pwm_config(pwm_t *pwm, int32_t duty, int32_t period, int32_t polarity);
	void pwm_set_duty(pwm_t *pwm, int32_t duty);
	inline int32_t pwm_get_duty(pwm_t *pwm) { return pwm->duty; }
	void pwm_enable(pwm_t *pwm);
	void pwm_disable(pwm_t *pwm);

#ifdef __cplusplus
}
#endif

#endif /* __PWM__F1C100S_H__ */
