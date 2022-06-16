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

#ifdef __cplusplus
extern "C"
{
#endif

#include "wiring_digital.h"

    extern void pinMode(pin_size_t pinNumber, uint8_t pinMode)
    {
        switch (pinMode)
        {
        case INPUT:
            gpio_set_dir_pn(pinNumber, GPIO_DIRECTION_INPUT);
            gpio_set_pull_pn(pinNumber, GPIO_PULL_NONE);
            break;

        case OUTPUT:
            gpio_set_dir_pn(pinNumber, GPIO_DIRECTION_OUTPUT);
            gpio_set_pull_pn(pinNumber, GPIO_PULL_NONE);
            break;

        case INPUT_PULLUP:
            gpio_set_dir_pn(pinNumber, GPIO_DIRECTION_INPUT);
            gpio_set_pull_pn(pinNumber, GPIO_PULL_UP);
            break;

        case INPUT_PULLDOWN:
            gpio_set_dir_pn(pinNumber, GPIO_DIRECTION_INPUT);
            gpio_set_pull_pn(pinNumber, GPIO_PULL_DOWN);
            break;

        default:
            break;
        }
    }

    extern void digitalWrite(pin_size_t pinNumber, uint8_t status)
    {
        switch (status)
        {
        case LOW:
            gpio_set_value_pn(pinNumber, 0);
            break;

        case HIGH:
            gpio_set_value_pn(pinNumber, 1);
            break;

        default:
            break;
        }
    }

    extern int digitalRead(pin_size_t pinNumber)
    {
        return gpio_get_value_pn(pinNumber);
    }

    void attachInterrupt(pin_size_t pinNumber, voidFuncPtr callback, uint8_t mode)
    {
        // GPIOPinDescription *gpio = &GPIO_Desc[interruptNumber];
        GPIO_Type *gpio;
	    uint16_t pin;
        if(pin_to_port(pinNumber, &gpio, &pin) == 0) {
            if (gpio == GPIOD)
            {
                gpio_set_cfg_pn(pinNumber, GPIO_FUNC_110);
                irq_gpio_settype(GPIOD_INT, pin, mode, callback);
                irq_gpio_enable(GPIOD_INT, pin);
            }
            else if (gpio == GPIOE)
            {
                gpio_set_cfg_pn(pinNumber, GPIO_FUNC_110);
                irq_gpio_settype(GPIOE_INT, pin, mode, callback);
                irq_gpio_enable(GPIOE_INT, pin);
            }
            else if (gpio == GPIOF)
            {
                gpio_set_cfg_pn(pinNumber, GPIO_FUNC_110);
                irq_gpio_settype(GPIOF_INT, pin, mode, callback);
                irq_gpio_enable(GPIOF_INT, pin);
            }
        }
    }

#ifdef __cplusplus
}
#endif
