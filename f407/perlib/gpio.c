#include "gpio.h"

void gpio_init(gpio_t *pin)
{
	GPIO_TypeDef *port;
	uint8_t mask = pin->mask;
	periph_id_t gpio_id;

	switch(pin->port) {
	case gpio_port_a:	gpio_id = gpioa_id;
						port = GPIOA;	
						break;

	case gpio_port_b:	gpio_id = gpiob_id;
						port = GPIOB;
						break;

	case gpio_port_c:	gpio_id = gpioc_id;
						port = GPIOC;
						break;

	case gpio_port_d:	gpio_id = gpiod_id;
						port = GPIOD;
						break;
	
	default:			return;
	}

	rcc_enable(gpio_id);
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
		port->AFR[1] |= (pin->alt << ((mask - 8) * 4));
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
