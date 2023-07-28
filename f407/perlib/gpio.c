#include "gpio.h"

static GPIO_TypeDef *switch_port(gpio_t *gpio);

void gpio_init(gpio_t *pin)
{
	GPIO_TypeDef *port;
	uint8_t mask = pin->mask;
	rcc_periph_id_t gpio_id;

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

void gpio_set(gpio_t *pin)
{
	GPIO_TypeDef *gpio = switch_port(pin);
	uint8_t mask = pin->mask;
	gpio_mode_t mode = pin->mode;

	if(mode == gpio_mode_output)
		gpio->BSRR |= (1 << mask);
}

void gpio_clr(gpio_t *pin)
{
	GPIO_TypeDef *gpio = switch_port(pin);
	uint8_t mask = pin->mask;
	gpio_mode_t mode = pin->mode;

	if(mode == gpio_mode_output)
		gpio->BSRR |= (1 << (mask * 2));
}

void gpio_tgl(gpio_t *pin)
{
	GPIO_TypeDef *gpio = switch_port(pin);
	uint8_t mask = pin->mask;
	gpio_mode_t mode = pin->mode;

	if(mode == gpio_mode_output)
		gpio->ODR ^= (1 << mask);
}

GPIO_TypeDef *switch_port(gpio_t *gpio)
{
	gpio_port_t port = gpio->port;

	switch(port) {
	case gpio_port_a:	return GPIOA;	

	case gpio_port_b:	return GPIOB;

	case gpio_port_c:	return GPIOC;

	case gpio_port_d:	return GPIOD;
	
	default:			return 0;
	}
}

