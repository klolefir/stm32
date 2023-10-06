#include <stdio.h>
#include <stdint.h>
#include <stm32f4xx.h>

#include "gpio.h"
#include "rcc.h"
#include "general.h"
#include "systick.h"

void enable_irq();

volatile static uint32_t ticks = 0;

static gpio_t led1_pin = { 
	.port = gpio_port_a, 
	.mask = 6, 
	.mode = gpio_mode_output, 
	.otype = gpio_otype_pp,
	.ospeed = gpio_ospeed_low,
	.pupd = gpio_pupd_pu,
	.alt = gpio_alt_system
};

void enable_irq()
{
	__enable_irq();
}
