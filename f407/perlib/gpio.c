#include "gpio.h"

void gpio_init(gpio_t *pin)
{
	GPIO_TypeDef *port;
	switch(pin->port) {
	case gpio_port_a:	port = GPIOA;	
						break;

	case gpio_port_b:	port = GPIOB;
						break;

	case gpio_port_c:	port = GPIOC;
						break;

	case gpio_port_d:	port = GPIOD;
						break;
	
	default:			return;
	}
	uint8_t mask = pin->mask;

	port->MODER |= (pin->mode << (mask * 2));
	if(pin->mode == gpio_mode_output) {
		port->OTYPER |= (pin->otype << mask);
		port->OSPEEDR |= (pin->ospeed << (mask * 2));
	}
	port->PUPDR |= (pin->pupd << (mask * 2));

	uint8_t boundary = 8;
	if(mask < boundary)
		port->AFR[0] |= (pin->alt << (mask * 4));
	else
		port->AFR[1] |= (pin->alt << (mask * 4));
}

void gpio_setout(gpio_t *pin)
{

}

void gpio_clrout(gpio_t *pin)
{

}

void gpio_tglout(gpio_t *pin)
{

}
