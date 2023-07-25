#include <stdio.h>
#include <stdint.h>
#include <stm32f4xx.h>

#include "gpio.h"
#include "rcc.h"
#include "usart.h"
#include "general.h"
#include "nvic.h"
#include "systick.h"
#include "tim.h"

#define VECTOR_TAB_SRAM
#define LED1_PIN 6
#define LED2_PIN 7

void enable_irq();

static volatile uint32_t ticks = 0;
static volatile uint32_t usart_ticks = 0;

static usart_t usart1 = { 
	.usart = USART1, 
	.baud = usart_baud_115200, 
	.rx_st = usart_rx_on,
	.tx_st = usart_tx_on,
	.tx_int_st = usart_tx_int_off,
	.rx_int_st = usart_rx_int_off
};

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
	.arr 		= 100,
	.irq_st 	= tim_irq_on,
	.pwr_st 	= tim_pwr_on
};

void enable_irq()
{
	__enable_irq();
}
