#include "gpio.h"

static GPIO_TypeDef *switch_port(const gpio_t *gpio_set);
static rcc_periph_id_t switch_rcc_id(const gpio_t *gpio_set);
static void gpio_rcc_init(const gpio_t *gpio_set);

void gpio_init(const gpio_t *gpio_set)
{
	uint8_t mask = gpio_set->mask;
	GPIO_TypeDef *port = switch_port(gpio_set);;
	if(!port)
		return;

	gpio_rcc_init(gpio_set);
	port->MODER |= (gpio_set->mode << (mask * 2));
	if(gpio_set->mode == gpio_mode_output) {
		port->OTYPER |= (gpio_set->otype << mask);
		port->OSPEEDR |= (gpio_set->ospeed << (mask * 2));
	}
	port->PUPDR |= (gpio_set->pupd << (mask * 2));

	uint8_t boundary = 8;
	if(mask < boundary)
		port->AFR[0] |= (gpio_set->alt << (mask * 4));
	else
		port->AFR[1] |= (gpio_set->alt << ((mask - 8) * 4));
}

void gpio_set(const gpio_t *gpio_set)
{
	GPIO_TypeDef *gpio = switch_port(gpio_set);
	if(!gpio)
		return;
	if(gpio_set->mode == gpio_mode_output)
		gpio->BSRR |= (1 << gpio_set->mask);
}

void gpio_clr(const gpio_t *gpio_set)
{
	GPIO_TypeDef *gpio = switch_port(gpio_set);
	if(!gpio)
		return;
	if(gpio_set->mode == gpio_mode_output)
		gpio->BSRR |= (1 << (gpio_set->mask * 2));
}

void gpio_tgl(const gpio_t *gpio_set)
{
	GPIO_TypeDef *gpio = switch_port(gpio_set);
	if(!gpio)
		return;
	if(gpio_set->mode == gpio_mode_output)
		gpio->ODR ^= (1 << gpio_set->mask);
}

GPIO_TypeDef *switch_port(const gpio_t *gpio_set)
{
	switch(gpio_set->port) {
	case gpio_port_a:	return GPIOA;	
	case gpio_port_b:	return GPIOB;
	case gpio_port_c:	return GPIOC;
	case gpio_port_d:	return GPIOD;
	default:			return 0;
	}
}

rcc_periph_id_t switch_rcc_id(const gpio_t *gpio_set)
{
	switch(gpio_set->port) {
	case gpio_port_a:		return gpioa_id;
	case gpio_port_b: 		return gpiob_id;
	case gpio_port_c: 		return gpioc_id;
	case gpio_port_d: 		return gpiod_id;
	default:				return none_id;
	}
}

void gpio_rcc_init(const gpio_t *gpio_set)
{
	rcc_periph_id_t gpio_id = switch_rcc_id(gpio_set);
	rcc_enable(gpio_id);
}
