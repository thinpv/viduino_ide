#include <gpio.h>
#include <pwm.h>
#include <io.h>

#define DEBUG( ... ) // printf(##__VA_ARGS__)

#define PWM_CTRL (0x00)
#define PWM_BIT(x, bit) ((1 << bit) << ((x)*15))
#define PWM_PERIOD(x) (((x)*0x4) + 0x4)

const uint32_t prescaler_table[] = {
		120,
		180,
		240,
		360,
		480,
		0,
		0,
		0,
		12000,
		24000,
		36000,
		48000,
		72000,
		0,
		0,
		0,
};

void pwm_config(pwm_t *pwm, int32_t duty, int32_t period, int32_t polarity)
{
	uint64_t rate = 24000000; //hosc =24MHz //clk_get_rate(pwm->clk);
	uint64_t div;
	uint32_t prd, dty, gate;
	uint32_t prescaler = 0;
	uint32_t ctrl;

	//if(pwm->duty != duty || pwm->period != period)
	{
		prescaler = 0xf;
		div = rate * period + 1000000000 / 2;
		div = div / 1000000000;
		if (div - 1 > 0xffff)
			prescaler = 0;

		if (prescaler == 0)
		{
			for (prescaler = 0; prescaler < 0xf; prescaler++)
			{
				if (!prescaler_table[prescaler])
					continue;
				div = rate;
				div = div / prescaler_table[prescaler];
				div = div * period;
				div = div / 1000000000;
				if (div - 1 <= 0xffff)
					break;
			}

			if (div - 1 > 0xffff)
			{
				return;
			}
		}

		prd = div;
		div *= duty;
		div = div / 256; //div / period;
		dty = div;

		gate = (read32(pwm->virt + PWM_CTRL) & PWM_BIT(pwm->channel, 6)) ? 1 : 0;
		if (gate)
		{
			ctrl = read32(pwm->virt + PWM_CTRL);
			ctrl &= ~PWM_BIT(pwm->channel, 6);
			write32(pwm->virt + PWM_CTRL, ctrl);
		}
		ctrl = read32(pwm->virt + PWM_CTRL);
		ctrl &= ~(0xf << (pwm->channel * 15));
		ctrl |= (prescaler << (pwm->channel * 15));
		write32(pwm->virt + PWM_CTRL, ctrl);
		write32(pwm->virt + PWM_PERIOD(pwm->channel), ((prd - 1) << 16) | (dty & 0xffff));
		if (gate)
		{
			ctrl = read32(pwm->virt + PWM_CTRL);
			ctrl |= PWM_BIT(pwm->channel, 6);
			write32(pwm->virt + PWM_CTRL, ctrl);
		}
		pwm->duty = duty;
		pwm->period = period;
	}

	//if(pwm->polarity != polarity)
	{
		ctrl = read32(pwm->virt + PWM_CTRL);
		if (polarity)
			ctrl |= PWM_BIT(pwm->channel, 5);
		else
			ctrl &= ~PWM_BIT(pwm->channel, 5);
		write32(pwm->virt + PWM_CTRL, ctrl);

		pwm->polarity = polarity;
	}
}

void pwm_set_duty(pwm_t *pwm, int32_t duty)
{
	pwm_config(pwm, duty, pwm->period, pwm->polarity);
}

void pwm_enable(pwm_t *pwm)
{
	uint32_t ctrl;

	if ((pwm->pwm_pin >= 0) && (pwm->pwm_pin_cfg >= 0))
		gpio_set_cfg(pwm->pwm_port, pwm->pwm_pin, pwm->pwm_pin_cfg);
	//clk_enable(pwm->clk);

	ctrl = read32(pwm->virt + PWM_CTRL);
	ctrl |= PWM_BIT(pwm->channel, 4);
	ctrl |= PWM_BIT(pwm->channel, 6);
	write32(pwm->virt + PWM_CTRL, ctrl);
}

void pwm_disable(pwm_t *pwm)
{
	uint32_t ctrl;

	ctrl = read32(pwm->virt + PWM_CTRL);
	ctrl &= ~PWM_BIT(pwm->channel, 4);
	ctrl &= ~PWM_BIT(pwm->channel, 6);
	write32(pwm->virt + PWM_CTRL, ctrl);
	//clk_disable(pwm->clk);
}

int32_t pwm_init(pwm_t *pwm)
{
	write32(pwm->virt + PWM_CTRL, read32(pwm->virt + PWM_CTRL) & ~(0x3fff << (pwm->channel * 15)));
	write32(pwm->virt + PWM_PERIOD(pwm->channel), 0);

	pwm_config(pwm, pwm->duty, pwm->period, pwm->polarity);
	pwm_enable(pwm);
	return 0;
}