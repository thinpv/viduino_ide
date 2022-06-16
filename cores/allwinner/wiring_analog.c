/*
 Copyright (c) 2011 Arduino.  All right reserved.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 See the GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "wiring_analog.h"

#ifdef __cplusplus
extern "C"
{
#endif

#include <pwm.h>
#include <soft_pwm.h>

	int analogRead(pin_size_t pinNumber)
	{
		return 0;
	}

	void analogReference(uint8_t mode)
	{
	}

	void analogWrite(pin_size_t pinNumber, int value)
	{
		// GPIOPinDescription *gpio = &GPIO_Desc[pinNumber];
		// gpio_set_dir(gpio->port, gpio->pin, GPIO_DIRECTION_OUTPUT);
		// softpwm_add(gpio->port, gpio->pin, value/25);
		// pwm_t led_pwm_bl =
		// 		{
		// 				.virt = 0x01c21000,
		// 				.duty = value,
		// 				.period = 1000000,
		// 				.channel = 1,
		// 				.polarity = true,
		// 				.pwm_port = gpio->port,
		// 				.pwm_pin = gpio->pin,
		// 				.pwm_pin_cfg = 3,
		// 		};
		// pwm_init(&led_pwm_bl);
	}

#ifdef __cplusplus
}
#endif
