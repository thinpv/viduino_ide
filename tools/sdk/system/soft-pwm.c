#include "soft-pwm.h"
#include <malloc.h>

static softpwm_node_t *first = NULL;
static softpwm_node_t *last = NULL;
static uint32_t current_count = 0;

void softpwm_add(GPIO_Type *gpio, uint16_t pin, uint32_t duty)
{
	softpwm_node_t *current = first;
	while (current != NULL)
	{
		if (current->gpio == gpio && current->pin == pin)
		{
			current->duty = duty;
			return;
		}
		current = current->next;
	}
	softpwm_node_t *softpwm_node = malloc(sizeof(softpwm_node_t));
	softpwm_node->gpio = gpio;
	softpwm_node->pin = pin;
	softpwm_node->duty = duty;
	if (last == NULL)
	{
		last = softpwm_node;
		first = softpwm_node;
	}
	else
	{
		last->next = softpwm_node;
		last = softpwm_node;
	}
}

bool softpwm_remove(GPIO_Type *gpio, uint16_t pin)
{
	softpwm_node_t *current = first;
	softpwm_node_t *previous = NULL;

	while (current != NULL)
	{
		if (current->gpio == gpio && current->pin == pin)
		{
			if (previous != NULL)
			{
				previous->next = current->next;
			}

			if (current == last)
			{
				last = previous;
			}

			if (current == first)
			{
				first = current->next;
			}

			free(current);
			return true;
		}
		previous = current;
		current = current->next;
	}
	return false;
}

void softpwm_update(void)
{
	current_count++;
	if (current_count >= 10)
	{
		current_count = 0;
		softpwm_node_t *current = first;
		while (current != NULL)
		{
			gpio_set_value(current->gpio, current->pin, 1);
			current = current->next;
		}
	}
	softpwm_node_t *current = first;
	while (current != NULL)
	{
		if (current_count == current->duty)
			gpio_set_value(current->gpio, current->pin, 0);
		current = current->next;
	}
}