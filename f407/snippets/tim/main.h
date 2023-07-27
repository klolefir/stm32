#include <stdio.h>
#include <stdint.h>
#include <stm32f4xx.h>

#include "gpio.h"
#include "rcc.h"
#include "general.h"
#include "tim.h"

void enable_irq();

static gpio_t led1_pin = { 
	.port = gpio_port_a, 
	.mask = 6, 
	.mode = gpio_mode_output, 
	.otype = gpio_otype_pp,
	.ospeed = gpio_ospeed_low,
	.pupd = gpio_pupd_pu,
	.alt = gpio_alt_system
};

static tim_t tim6 = {
	.tim_num 	= tim_6,
	.psc 		= 21000,
	.arr 		= 1000,
	.irq_st 	= tim_irq_on,
	.pwr_st 	= tim_pwr_on
};

void enable_irq()
{
	__enable_irq();
}
